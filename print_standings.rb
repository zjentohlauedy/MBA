#!/usr/bin/env ruby
#
# Reads filled in schedule.csv file and calculates
# standings with percentage, last 10 games,
# games back, and streaks.
location = File.dirname __FILE__

$: << "#{location}"
require 'json'

require 'tmpdir'
require 'optparse'
require 'FileParser'
require 'ProgRunner'
require 'ScheduleParser'
require 'TeamRecords'


TeamDivisions = {
  'Sabres'    => 0, 'Scorpions' => 1, 'Aces'      => 2, 'Eclipse'   => 3, 'Global' => 4,
  'Portsmen'  => 0, 'Lightning' => 1, 'Cyclone'   => 2, 'Legends'   => 3, 'World'  => 4,
  'Settlers'  => 0, 'Goblins'   => 1, 'Stormers'  => 2, 'Waves'     => 3,
  'Kings'     => 0, 'Photons'   => 1, 'Express'   => 2, 'Horizon'   => 3,
  'Voyagers'  => 0, 'Dragons'   => 1, 'Warriors'  => 2, 'Sharks'    => 3,
  'Rockets'   => 0, 'Hammers'   => 1, 'Wanderers' => 2, 'Flames'    => 3,
  'Knights'   => 0, 'Expos'     => 1, 'Thunder'   => 2, 'Techs'     => 3,
  'Drizzle'   => 0, 'Dynamo'    => 1, 'Glory'     => 2, 'Quasars'   => 3 }

PlayoffMatchTypes = {
  atl_tie_break: { divisions: [0],   order: 0, depth:  5 },
  nor_tie_break: { divisions: [1],   order: 2, depth:  5 },
  sou_tie_break: { divisions: [2],   order: 4, depth:  5 },
  pac_tie_break: { divisions: [3],   order: 6, depth:  5 },
  wlcs:          { divisions: [0,1], order: 1, depth: 25 },
  glcs:          { divisions: [2,3], order: 5, depth: 25 },
  world_series:  { divisions: [],    order: 3, depth: 57 }
}

Heading = '-          Record   Pct  GB  Dvsn.   Pct  Last10 Str  Diff'
#          Lightning 107  45  .704   -  34 22  .607   6  4   4W   268


def update_teams( teams, game )
  if teams[game.road_team].nil?
    teams[game.road_team] = { name: game.road_team, division: TeamDivisions[game.road_team], wins: 0, losses: 0, dwins: 0, dlosses: 0, runs_scored: 0, runs_allowed: 0, results: [] }
  end

  if teams[game.home_team].nil?
    teams[game.home_team] = { name: game.home_team, division: TeamDivisions[game.home_team], wins: 0, losses: 0, dwins: 0, dlosses: 0, runs_scored: 0, runs_allowed: 0, results: [] }
  end

  road = teams[game.road_team]
  home = teams[game.home_team]

  road[:runs_scored ] += game.road_score
  road[:runs_allowed] += game.home_score

  home[:runs_scored ] += game.home_score
  home[:runs_allowed] += game.road_score

  if game.road_score > game.home_score
    road[:wins  ] += 1
    home[:losses] += 1

    if TeamDivisions[game.road_team] == TeamDivisions[game.home_team]
      road[:dwins  ] += 1
      home[:dlosses] += 1
    end

    road[:results].push 'W'
    home[:results].push 'L'
  else
    road[:losses] += 1
    home[:wins  ] += 1

    if TeamDivisions[game.road_team] == TeamDivisions[game.home_team]
      road[:dlosses] += 1
      home[:dwins  ] += 1
    end

    road[:results].push 'L'
    home[:results].push 'W'
  end
end

def decorate_teams( teams )
  leader_wins = teams[0][:wins]

  teams.each do |record|
    last_ten = { wins: 0, losses: 0 }
    streak = { type: nil, count: 0, final: false }

    while result = record[:results].pop do
      break if (last_ten[:wins] + last_ten[:losses]) == 10 and streak[:final]

      if (last_ten[:wins] + last_ten[:losses]) < 10
        if result == 'W'; then last_ten[:wins  ] += 1; end
        if result == 'L'; then last_ten[:losses] += 1; end
      end

      if streak[:type].nil?
        streak[:type] = result
        streak[:count] += 1
      elsif streak[:final] == false and streak[:type] == result
        streak[:count] += 1
      elsif streak[:final] == false and streak[:type] != result
        streak[:final] = true
      end
    end

    record[:win_pct] = record[:wins].to_f  / (record[:wins]  + record[:losses] ).to_f
    record[:div_pct] = record[:dwins].to_f / (record[:dwins] + record[:dlosses]).to_f

    record[:games_back] = (leader_wins - record[:wins]).to_s
    record[:games_back] = (record[:games_back] == '0') ? '-' : record[:games_back]

    record[:last_ten] = last_ten
    record[:streak] = streak
  end
end

def print_team( team, games )
  games_remaining = 152 - games

  if team[:games_back].to_i > games_remaining
    printf "\e[90m"
  end

  printf "%-9s %3d %3d %5s  %2s  %2d %2d %5s  %2d %2d  %2d%c  %4d",
         team[:name],
         team[:wins],
         team[:losses],
         display_avg( team[:win_pct] ),
         team[:games_back],
         team[:dwins],
         team[:dlosses],
         display_avg( team[:div_pct] ),
         team[:last_ten][:wins],
         team[:last_ten][:losses],
         team[:streak][:count],
         team[:streak][:type],
         team[:runs_scored] - team[:runs_allowed]

  printf "\e[0m"
end

def compare(a, b)
  if b[:wins] == a[:wins]
    bpct = b[:dwins].to_f / (b[:dwins] + b[:dlosses]).to_f
    apct = a[:dwins].to_f / (a[:dwins] + a[:dlosses]).to_f

    if bpct == apct
      bdiff = b[:runs_scored] - b[:runs_allowed]
      adiff = a[:runs_scored] - a[:runs_allowed]

      if bdiff == adiff
        return b[:runs_scored] <=> a[:runs_scored]
      end

      return bdiff <=> adiff
    end

    return bpct <=> apct
  end

  return b[:wins] <=> a[:wins]
end

def display_avg(average)
  avg = sprintf "%5.3f", average
  avg.gsub /^0\./, ' .'
end


@options = {}

OptionParser.new do |opt|
  opt.on( '-r', '--regular-season' ) { |o| @options[ :regular_season ] = o }
  opt.on( '-p', '--playoff'        ) { |o| @options[ :playoff        ] = o }
  opt.on( '-a', '--allstar'        ) { |o| @options[ :allstar        ] = o }
end.parse!


if !File::exist? './schedule.ods'
  raise 'Must run from directory containing a schedule file!'
end

convert_schedule = ProgRunner.new '/usr/bin', 'soffice'

convert_schedule.execute "-env:UserInstallation=file://#{ENV['HOME']}/.libreoffice-alt", '--headless', '--convert-to', 'csv', '--outdir', Dir::tmpdir, './schedule.ods'

if !convert_schedule.success?
  puts "Error converting schedule: "
  puts convert_schedule.get_output
  raise "Processing Failed"
end

filename = "#{Dir::tmpdir}/schedule.csv"

sp = ScheduleParser.new
fp = FileParser.new sp

fp.process_file filename

schedule = sp.schedule


if @options.empty? || @options[:regular_season]
  teams = {}
  games_played = 0

  schedule.days.each do |day|
    day.games.each do |game|
      break if day.day > 152
      break if !game.played

      games_played = day.day
      update_teams teams, game
    end
  end


  atlantic = teams.values.select {|team| team[:division] == 0}.sort { |a, b| compare a, b }
  north    = teams.values.select {|team| team[:division] == 1}.sort { |a, b| compare a, b }
  south    = teams.values.select {|team| team[:division] == 2}.sort { |a, b| compare a, b }
  pacific  = teams.values.select {|team| team[:division] == 3}.sort { |a, b| compare a, b }

  decorate_teams atlantic
  decorate_teams north
  decorate_teams south
  decorate_teams pacific

  puts 'World League                                                 Global League'
  puts '-                                                            -'
  puts 'Atlantic                                                     South'

  puts "#{Heading}   #{Heading}"

  (0..7).each do |i|
    print_team atlantic[i], games_played
    printf '   '
    print_team south[i], games_played
    printf "\n"
  end

  puts ''
  puts 'North                                                        Pacific'

  puts "#{Heading}   #{Heading}"

  (0..7).each do |i|
    print_team north[i], games_played
    printf '   '
    print_team pacific[i], games_played
    printf "\n"
  end
end

def find_match( matches, teams )
  matches.each do |match|
    if match[:teams].sort == teams.sort
      return match
    end
  end

  return nil
end

def get_playoff_match_type( game )
  PlayoffMatchTypes.each do |k,v|
    divisions = v[:divisions]

    if divisions.include?(TeamDivisions[game.road_team]) && divisions.include?(TeamDivisions[game.home_team])
      return k
    end
  end

  return :world_series
end

def winner( match_type, team )
  if [:atl_tie_break,:nor_tie_break,:sou_tie_break,:pac_tie_break].include? match_type
    return team[:wins] == 1
  end

  if match_type == :allstar
    return team[:wins] == 3
  end

  if [:wlcs,:glcs].include? match_type
    return team[:wins] == 4
  end

  return team[:wins] == 5
end


if @options.empty? || @options[:playoff]
  if @options.empty? || @options[:regular_season]
    puts ''
    puts '---'
    puts ''
  end

  playoff_matches = []
  games_played = 0
  tie_breakers = false

  schedule.days.each do |day|
    day.games.each do |game|
      next  if day.day <= 152
      break if !game.played
      break if TeamDivisions[game.home_team] == 4

      teams = [ game.road_team, game.home_team ]

      match = find_match playoff_matches, teams

      if match.nil?

        match = {
          teams: [ game.road_team, game.home_team ],
          type: get_playoff_match_type( game ),
          results: {}
        }

        playoff_matches.push match
      end

      games_played = day.day
      update_teams match[:results], game
    end
  end

  playoff_matches.sort! { |a,b|
    PlayoffMatchTypes[ a[:type] ][:order] <=> PlayoffMatchTypes[ b[:type] ][:order]
  }

  playoff_matches.each do |match|
    road = true

    indent = sprintf "%*s", PlayoffMatchTypes[ match[:type] ][:depth], ""

    match[:teams].each do |team|
      output = sprintf '%-10s', team

      match[:results][team][:results].each do |result|
        output += sprintf ' %s', result
      end

      output += sprintf " (%d)", match[:results][team][:wins]

      if winner( match[:type], match[:results][team] )
        if match[:type] == :world_series
          puts "#{indent}\e[1;33m#{output}\e[0m"
        else
          puts "#{indent}\e[1;37m#{output}\e[0m"
        end
      else
        puts indent + output
      end

      if road
        road = false

        puts indent + ('-' * output.length)
      end
    end
  end
end


if @options.empty? || @options[:allstar]
  if @options.empty? || @options[:regular_season] || @options[:playoff]
    puts ''
    puts '---'
    puts ''
  end

  match = nil
  playoff_matches = []
  games_played = 0
  tie_breakers = false

  schedule.days.each do |day|
    day.games.each do |game|
      next  if day.day <= 152
      next if TeamDivisions[game.home_team] != 4
      break if !game.played

      if match.nil?
        match = {
          teams: [ game.road_team, game.home_team ],
          type: :allstar,
          results: {}
        }
      end

      games_played = day.day
      update_teams match[:results], game
    end
  end

  road = true
  indent = '     '

  if match
    match[:teams].each do |team|
      output = sprintf '%-10s', team

      match[:results][team][:results].each do |result|
        output += sprintf ' %s', result
      end

      output += sprintf " (%d)", match[:results][team][:wins]

      if winner( match[:type], match[:results][team] )
        puts "#{indent}\e[1;37m#{output}\e[0m"
      else
        puts indent + output
      end

      if road
        road = false

        puts indent + ('-' * output.length)
      end
    end
  end
end
