#!/usr/bin/env ruby
#
# Reads filled in schedule.csv file and sums the total
# number of runs scored in every game with the same
# home team
location = File.dirname __FILE__

$: << "#{location}"
require 'FileParser'
require 'ScheduleParser'


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

runs_by_team = Hash.new

schedule.days.each do |day|
  day.games.each do |game|
    next if ! game.played
    if runs_by_team.has_key? game.home_team
      runs_by_team[game.home_team] += (game.home_score + game.road_score)
    else
      runs_by_team[game.home_team]  = (game.home_score + game.road_score)
    end
  end
end

runs_by_team.sort.each do |team, runs|
  printf "%9s %4d\n", team, runs
end
