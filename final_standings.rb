#!/usr/bin/env ruby
#
# Reads filled in schedule.csv file and calculates
# final regular season standings taking in account
# tie breaking criteria
location = File.dirname __FILE__

$: << "#{location}"
require 'tmpdir'
require 'FileParser'
require 'ProgRunner'
require 'ScheduleParser'
require 'TeamRecords'


def add_game( team_records, game )
  return if !game.played

  # road team
  if team_records.has_key? game.road_team
    team_record = team_records[game.road_team]
  else
    team_record = TeamRecords.new game.road_team
  end

  team_record.update game, false
  team_records[game.road_team] = team_record

  # home team
  if team_records.has_key? game.home_team
    team_record = team_records[game.home_team]
  else
    team_record = TeamRecords.new game.home_team
  end

  team_record.update game, true
  team_records[game.home_team] = team_record
end

def compare_team_records( a, b )
  if a.overall.wins != b.overall.wins
    return a.overall.wins <=> b.overall.wins
  end

  if a.division.wins != b.division.wins
    return a.division.wins <=> b.division.wins
  end

  a_score_diff = a.points_scored - a.points_allowed
  b_score_diff = b.points_scored - b.points_allowed

  unless a_score_diff == b_score_diff
    return a_score_diff <=> b_score_diff
  end

  return a.points_scored <=> b.points_scored
end

def compare_head2head( teams )
  workingset = teams.map.with_index { |t,i| {index: i, name: t.team, wins: t.overall.wins, dwins: t.division.wins, opponents: t.opponents} }

  swaps = []

  workingset.each do |team|
    subset = workingset.select { |t| t[:wins] == team[:wins] && t[:dwins] == team[:dwins] }

    if subset.length == 2
      if subset[1][:opponents][subset[0][:name]].wins > subset[0][:opponents][subset[1][:name]].wins
        swap = [subset[0][:index], subset[1][:index]]

        unless swaps.include? swap
          swaps.push swap
        end
      end
    end
  end

  swaps.each do |swap|
    teams[swap[0]], teams[swap[1]] = teams[swap[1]], teams[swap[0]]
  end
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

reg_season = {}
divisions  = []

schedule.days.each do |day|
  day.games.each do |game|
    next if day.day > 152

    add_game reg_season, game
  end
end

reg_season.each do |team, team_record|
  unless divisions[TeamRecords::TeamDivisions[team]]
    divisions[TeamRecords::TeamDivisions[team]] = []
  end

  divisions[TeamRecords::TeamDivisions[team]].push team_record
end

divisions.each do |division|
  division.sort! { |a,b| compare_team_records a, b }
  division.reverse!

  compare_head2head division
end

puts "World League                                      Global League\n"
puts "-                                                 -\n"
puts "Atlantic       W   L     North          W   L     South          W   L     Pacific        W   L\n"
puts "-              -   -     -              -   -     -              -   -     -              -   -\n"

(0..7).each do |idx|
  (0..3).each do |div|
    printf "%-12s %3d %3d     ", TeamRecords::TeamLocations[divisions[div][idx].team], divisions[div][idx].overall.wins, divisions[div][idx].overall.losses
  end

  puts "\n"
end
