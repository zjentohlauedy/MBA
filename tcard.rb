#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'optparse'
require 'sqlite3'

require_relative 'MBATool/web/accolades'
require_relative 'MBATool/web/phases'
require_relative 'MBATool/web/utils'


@options = {}

OptionParser.new do |opt|
  opt.on( '-x', '--extended' ) { |o| @options[ :extended ] = o }
end.parse!


if ARGV.length != 1
  raise "Team Name is required."
end

@team_name = ARGV[0]

@db = SQLite3::Database.new "#{location}/mba.db"

@db.results_as_hash  = true
@db.type_translation = true


def calc_eff( stats )
  if (stats[:innings] + stats[:outs]) == 0; then return 0.0 end

  finn = (stats[:outs].to_f / 3.0 + stats[:innings].to_f)
  eff  = (finn - stats[:hits]) + (stats[:strike_outs] - stats[:hits])

  eff.to_f / finn * 9.0
end

def calc_ipg( stats )
  if stats[:games] == 0; then return 0.0 end

  (stats[:outs].to_f / 3.0 + stats[:innings].to_f) / stats[:games].to_f
end

def calc_vsba( stats )
  if (stats[:innings] + stats[:outs]) == 0; then return 0.0 end

  stats[:hits].to_f / (stats[:innings] * 3 + stats[:outs] + stats[:hits]).to_f
end

def calc_era( stats )
  if (stats[:innings] + stats[:outs]) == 0; then return 0.0 end

  stats[:earned_runs].to_f / (stats[:outs].to_f / 3.0 + stats[:innings].to_f) * 9.0
end

def calc_hrp9( stats )
  if stats[:home_runs] == 0; then return 0.0 end

  stats[:home_runs].to_f / (stats[:outs].to_f / 3.0 + stats[:innings].to_f) * 9.0
end

def calc_sop9( stats )
  if stats[:strike_outs] == 0; then return 0.0 end

  stats[:strike_outs].to_f / (stats[:outs].to_f / 3.0 + stats[:innings].to_f) * 9.0
end

def calc_whip( stats )
  if (stats[:innings] + stats[:outs]) == 0; then return 0.0 end

  (stats[:hits] + stats[:walks]).to_f / (stats[:outs].to_f / 3.0 + stats[:innings].to_f)
end


def get_team_by_name( team_name )
  args  = { team_name: team_name }
  query = "select * from teams_t where name = :team_name collate nocase"

  result = Utils::transform_hash @db.execute query, args
  result[0]
end

def get_team_stats( team_id )
  query = "select * from team_stats_t where team_id = #{team_id}"

  return Utils::transform_hash @db.execute query
end

def get_team_pitching_stats( team_id )
  query = "select * from team_pitching_stats_t where team_id = #{team_id}"

  return Utils::transform_hash @db.execute query
end

def get_team_batting_stats( team_id )
  query = "select * from team_batting_stats_t where team_id = #{team_id}"

  return Utils::transform_hash @db.execute query
end

def get_season_finish( team_id, season )
  query = %Q(
select ts.Team_Id, ROW_NUMBER() over (order by ts.Wins desc) as rank from Teams_T t
join Division_Teams_T dt on t.Team_Id = dt.Team_Id
join Division_Teams_T dt2 on dt.Division_Id = dt2.Division_Id
join Team_Stats_T ts on dt2.Team_Id = ts.Team_Id
where t.Team_Id = #{team_id}
and ts.Season = #{season}
and ts.Season_Phase = 1
)

  result = Utils::transform_hash @db.execute query

  result.each do |record|
    if record[:team_id] == team_id
      return record[:rank]
    end
  end

  return 0
end


def add_extended_stats( stats )
  stats[:vsba] = calc_vsba stats
  stats[:ipg]  = calc_ipg  stats
  stats[:whip] = calc_whip stats
  stats[:sop9] = calc_sop9 stats
  stats[:hrp9] = calc_hrp9 stats
  stats[:eff]  = calc_eff  stats
end


def compile_totals( totals, stats, options = {} )
  stats.keys.each do |key|
    if totals[key].nil?
      totals[key] = stats[key]
    else
      totals[key] += stats[key]
    end
  end

  if totals[:seasons].nil?
    totals[:seasons] = 1
  else
    totals[:seasons] += 1
  end

  if totals[:outs] && totals[:outs] > 2
    totals[:innings] += 1
    totals[:outs] -= 3
  end

  if options[:extended]
    add_extended_stats totals
  end
end


def print_phase( phase )
  case phase
  when Phases::RegularSeason; puts 'Regular Season:'
  when Phases::Playoff;       puts 'Playoffs:'
  when Phases::Allstar;       puts 'Allstar:'
  end
end

def get_finish_name( value )
  case value
  when 1; return 'First'
  when 2; return 'Second'
  when 3; return 'Third'
  when 4; return 'Fourth'
  when 5; return 'Fifth'
  when 6; return 'Sixth'
  when 7; return 'Seventh'
  when 8; return 'Last'
  end

  return 'Unknown'
end


RecordStatsHeading = "Year     W   L   HW  HL   RW  RL   DW  DL   LW  LL    RS    RA"
RecordStatsFormat  = "%-5s  %3d %3d  %3d %3d  %3d %3d  %3d %3d  %3d %3d  %4d  %4d\n"

RecordStatsHeadingExtended = "Year     W   L   HW  HL   RW  RL   DW  DL   LW  LL   NW  NL    RS    RA  Diff  Finish"
RecordStatsFormatExtended  = "%-5s  %3d %3d  %3d %3d  %3d %3d  %3d %3d  %3d %3d  %3d %3d  %4d  %4d  %4d  %s\n"

def print_record_stats( team, stats, options={} )
  if options[:total]
    printf "\e[1m"
    season = 'TOTAL'
  else
    season = sprintf "S%02d", stats[:season]
  end

  if options[:extended]
    stats[ :non_league_wins   ] = stats[ :wins        ] - stats[ :league_wins   ]
    stats[ :non_league_losses ] = stats[ :losses      ] - stats[ :league_losses ]
    stats[ :scoring_diff      ] = stats[ :runs_scored ] - stats[ :runs_allowed  ]

    if options[:phase] == Phases::RegularSeason
      if options[:total]
        avg = stats[:finish].to_f / stats[:seasons]

        finish = sprintf "%3.1f", avg
      else
        finish = get_finish_name stats[:finish]
      end
    else
      finish = '-'
    end

    printf RecordStatsFormatExtended,
           season,
           stats[:wins],
           stats[:losses],
           stats[:home_wins],
           stats[:home_losses],
           stats[:road_wins],
           stats[:road_losses],
           stats[:division_wins],
           stats[:division_losses],
           stats[:league_wins],
           stats[:league_losses],
           stats[:non_league_wins],
           stats[:non_league_losses],
           stats[:runs_scored],
           stats[:runs_allowed],
           stats[:scoring_diff],
           finish
  else
    printf RecordStatsFormat,
           season,
           stats[:wins],
           stats[:losses],
           stats[:home_wins],
           stats[:home_losses],
           stats[:road_wins],
           stats[:road_losses],
           stats[:division_wins],
           stats[:division_losses],
           stats[:league_wins],
           stats[:league_losses],
           stats[:runs_scored],
           stats[:runs_allowed]
  end

  printf "\e[0m"
end

PitchingStatsHeading = "Year    ERA      G    SV    IP        H    ER    HR    BB     SO"
PitchingStatsFormat  = "%-5s  %5.2f  %4d  %3d  %5d.%1d  %5d  %4d  %4d  %4d  %5d\n"

PitchingStatsHeadingExtended = "Year    ERA      G    SV    IP        H    ER    HR    BB     SO  VSBA INN/G  WHIP  SO/9  HR/9    EFF"
PitchingStatsFormatExtended  = "%-5s  %5.2f  %4d  %3d  %5d.%1d  %5d  %4d  %4d  %4d  %5d  %s  %4.2f %5.3f %5.2f  %4.2f %+6.2f\n"


def print_pitching_stats( stats, options={} )
  if options[:total]
    printf "\e[1m"
    season = 'TOTAL'
  else
    season = sprintf "S%02d", stats[:season]
  end

  era = calc_era stats

  if options[:extended]
    dspavg = sprintf( "%5.3f", stats[:vsba]).gsub '0.','.'

    printf PitchingStatsFormatExtended,
           season,
           era,
           stats[:games],
           stats[:saves],
           stats[:innings],
           stats[:outs],
           stats[:hits],
           stats[:earned_runs],
           stats[:home_runs],
           stats[:walks],
           stats[:strike_outs],
           dspavg,
           stats[:ipg],
           stats[:whip],
           stats[:sop9],
           stats[:hrp9],
           stats[:eff]
  else
    printf PitchingStatsFormat,
           season,
           era,
           stats[:games],
           stats[:saves],
           stats[:innings],
           stats[:outs],
           stats[:hits],
           stats[:earned_runs],
           stats[:home_runs],
           stats[:walks],
           stats[:strike_outs]
  end

  printf "\e[0m"
end

def print_batting_stats( stats, options={} )
  if options[:total]
    printf "\e[1m"
    season = 'TOTAL'
  else
    season = sprintf "S%02d", stats[:season]
  end

  avg = (stats[:at_bats] > 0) ? stats[:hits].to_f / stats[:at_bats].to_f : 0.0
  dspavg = sprintf( "%5.3f", avg).gsub '0.',' .'

  printf "%-5s  %s  %5d  %5d  %4d  %5d  %4d  %3d  %4d  %4d  %4d  %5d  %3d  %3d\n",
         season,
         dspavg,
         stats[:games],
         stats[:at_bats],
         stats[:runs],
         stats[:hits],
         stats[:doubles],
         stats[:triples],
         stats[:home_runs],
         stats[:runs_batted_in],
         stats[:walks],
         stats[:strike_outs],
         stats[:steals],
         stats[:errors]

  printf "\e[0m"
end

def get_team_accolades( team_id )
  args = { team_id: team_id }
  query = 'SELECT * FROM team_accolades_t WHERE team_id = :team_id'

  results = Utils::transform_hash @db.execute query, args

  results.each do |result|
    result[:accolade_name] = Accolades::get_accolade_name Accolades::Team::Type, result[:accolade]
  end

  return results
end


def print_team_awards( team )
  accolades = get_team_accolades( team[:team_id] )

  return if accolades.length == 0

  puts ""
  puts "Awards:"
  puts "Year  Award"

  accolades.sort_by {|acc| [ acc[:season], acc[:accolade] ]}.each do |accolade|
    printf "S%02d   %s\n", accolade[:season], accolade[:accolade_name]
  end
end


team = get_team_by_name @team_name

if team.nil?
  raise "Team not found."
end

team_stats = get_team_stats          team[:team_id]
pitching   = get_team_pitching_stats team[:team_id]
batting    = get_team_batting_stats  team[:team_id]

puts "#{team[:location]} #{team[:name]}"

team_stats.map { |ts| ts[:season_phase] }.uniq.each do |phase|
  puts ""
  print_phase phase
  if @options[:extended]
    puts RecordStatsHeadingExtended
  else
    puts RecordStatsHeading
  end

  record_totals = {}

  team_stats.select { |ts| ts[:season_phase] == phase }.each do |stats|
    if @options[:extended]
      stats[:finish] = get_season_finish team[:team_id], stats[:season]
    end

    print_record_stats team, stats, :phase => phase, :extended => @options[:extended]

    compile_totals record_totals, stats
  end

  print_record_stats team, record_totals, :total => true, :phase => phase, :extended => @options[:extended]
end

puts ""
puts "-- Pitching --"

pitching.map { |ts| ts[:season_phase] }.uniq.each do |phase|
  phase_pitching = pitching.select { |ts| ts[:season_phase] == phase && ts[:games] > 0 }

  next if phase_pitching.size == 0

  puts ""
  print_phase phase
  if @options[:extended]
    puts PitchingStatsHeadingExtended
  else
    puts PitchingStatsHeading
  end

  pitching_totals = {}

  phase_pitching.each do |stats|
    if @options[:extended]
      add_extended_stats stats
    end

    print_pitching_stats stats, :extended => @options[:extended]

    compile_totals pitching_totals, stats, :extended => @options[:extended]
  end

  print_pitching_stats pitching_totals, :total => true, :extended => @options[:extended]
end

puts ""
puts "-- Batting --"

batting.map { |ts| ts[:season_phase] }.uniq.each do |phase|
  phase_batting = batting.select { |ts| ts[:season_phase] == phase && ts[:games] > 0 }

  next if phase_batting.size == 0

  puts ""
  print_phase phase
  puts "Year     BA       G     AB     R      H    2B   3B    HR   RBI    BB     SO   SB    E"

  batting_totals = {}

  phase_batting.each do |stats|
    print_batting_stats stats

    compile_totals batting_totals, stats
  end

  print_batting_stats batting_totals, :total => true
end

print_team_awards team
