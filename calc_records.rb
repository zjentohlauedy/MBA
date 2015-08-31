#!/usr/bin/env ruby
#
# Reads filled in schedule.csv file and calculates
# Overall, Division and Head-To-Head records as well
# as scoring totals.
location = File.dirname __FILE__

$: << "#{location}"
require 'FileParser'
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


if ARGV.length != 1
  abort "Usage: #{__FILE__} <filename>\n"
end

filename = ARGV[0]

if ! File.exists? filename
  abort "File '#{filename}' not found.\n"
end

sp = ScheduleParser.new
fp = FileParser.new sp

fp.process_file filename

schedule = sp.schedule

reg_season = {}
playoffs   = {}
allstar    = {}

schedule.days.each do |day|
  day.games.each do |game|
    if day.day <= 152
      add_game reg_season, game
    elsif TeamRecords.is_allstar? game.home_team
      add_game allstar, game
    else
      add_game playoffs, game
    end
  end
end

puts "Regular Season:\n\n"

reg_season.sort.each do |team, team_record|
  printf "%-9s ", team
  team_record.print_records
end

puts "Playoffs:\n\n"

playoffs.sort.each do |team, team_record|
  printf "%-9s ", team
  team_record.print_records
end

puts "All Star Games:\n\n"

allstar.sort.each do |team, team_record|
  printf "%-9s ", team
  team_record.print_records
end
