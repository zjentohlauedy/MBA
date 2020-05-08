#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'sqlite3'

require_relative 'MBATool/web/phases'
require_relative 'MBATool/web/utils'


if ARGV.length != 1
  raise "Team Name is required."
end

@team_name = ARGV[0]

@db = SQLite3::Database.new "#{location}/mba.db"

@db.results_as_hash  = true
@db.type_translation = true


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


def compile_totals( totals, stats )
  stats.keys.each do |key|
    if totals[key].nil?
      totals[key] = stats[key]
    else
      totals[key] += stats[key]
    end
  end
end


def print_phase( phase )
  case phase
  when Phases::RegularSeason; puts 'Regular Season:'
  when Phases::Playoff;       puts 'Playoffs:'
  when Phases::Allstar;       puts 'Allstar:'
  end
end

def print_record_stats( stats, options={} )
  if options[:total]
    printf "\e[1m"
    season = 'TOTAL'
  else
    season = sprintf "S%02d", stats[:season]
  end

  printf "%-5s  %3d %3d  %3d %3d  %3d %3d  %3d %3d  %3d %3d  %4d  %4d\n",
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

  printf "\e[0m"
end

def print_pitching_stats( stats, options={} )
  if options[:total]
    printf "\e[1m"
    season = 'TOTAL'
  else
    season = sprintf "S%02d", stats[:season]
  end

  inn = stats[:innings].to_f + (stats[:outs].to_f / 3.0)
  era = (inn > 0) ? stats[:earned_runs].to_f / inn * 9.0 : 0.0

  printf "%-5s  %5.2f  %4d  %3d  %5d.%1d  %5d  %4d  %4d  %4d  %5d\n",
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
  puts "Year     W   L   HW  HL   RW  RL   DW  DL   LW  LL    RS    RA"

  record_totals = {}

  team_stats.select { |ts| ts[:season_phase] == phase }.each do |stats|
    print_record_stats stats

    compile_totals record_totals, stats
  end

  print_record_stats record_totals, :total => true
end

puts ""
puts "-- Pitching --"

pitching.map { |ts| ts[:season_phase] }.uniq.each do |phase|
  phase_pitching = pitching.select { |ts| ts[:season_phase] == phase && ts[:games] > 0 }

  next if phase_pitching.size == 0

  puts ""
  print_phase phase
  puts "Year    ERA      G    SV    IP        H    ER    HR    BB     SO"

  pitching_totals = {}

  phase_pitching.each do |stats|
    print_pitching_stats stats

    compile_totals pitching_totals, stats
  end

  print_pitching_stats pitching_totals, :total => true
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
