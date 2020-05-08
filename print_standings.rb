#!/usr/bin/env ruby
#
# Reads filled in schedule.csv file and calculates
# standings with percentage, last 10 games,
# games back, and streaks.
location = File.dirname __FILE__

$: << "#{location}"
require 'tmpdir'
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

def print_team( team )
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


teams = {}

schedule.days.each do |day|
  day.games.each do |game|
    break if day.day > 152
    break if !game.played

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
  print_team atlantic[i]
  printf '   '
  print_team south[i]
  printf "\n"
end

puts ''
puts 'North                                                        Pacific'

puts "#{Heading}   #{Heading}"

(0..7).each do |i|
  print_team north[i]
  printf '   '
  print_team pacific[i]
  printf "\n"
end
