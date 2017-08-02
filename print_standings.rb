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

def print_teams( teams )
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

    win_pct = record[:wins].to_f  / (record[:wins]  + record[:losses] ).to_f
    div_pct = record[:dwins].to_f / (record[:dwins] + record[:dlosses]).to_f

    games_back = (leader_wins - record[:wins]).to_s
    games_back = (games_back == '0') ? '-' : games_back

    printf "%-10s %3d %3d  %5s  %2s    %2d %2d  %5s   %2d %2d  %2d%c  %4d\n",
           record[:name],
           record[:wins],
           record[:losses],
           display_avg( win_pct ),
           games_back,
           record[:dwins],
           record[:dlosses],
           display_avg( div_pct ),
           last_ten[:wins],
           last_ten[:losses],
           streak[:count],
           streak[:type],
           record[:runs_scored] - record[:runs_allowed]
  end
end

Heading = '-           Record    Pct  GB  Divison    Pct  Last10  Str  Diff'
#          Settlers   107  45   .704   -    34 22   .607    6  4   4W   268

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
  raise 'Must run from directory contain a schedule file!'
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

puts 'World League'
puts '-'
puts 'Atlantic'
puts Heading
#     Settlers   107- 45   34- 22    6-  4   4W   268
print_teams( teams.values.select {|team| team[:division] == 0}.sort { |a, b| compare a, b })
puts ''
puts 'North'
puts Heading
print_teams( teams.values.select {|team| team[:division] == 1}.sort { |a, b| compare a, b })
puts ''
puts 'Global League'
puts 'South'
puts Heading
print_teams( teams.values.select {|team| team[:division] == 2}.sort { |a, b| compare a, b })
puts ''
puts 'Pacific'
puts Heading
print_teams( teams.values.select {|team| team[:division] == 3}.sort { |a, b| compare a, b })
