#!/usr/bin/env ruby
#
# Reads filled in schedule.csv file and determines
# matches for the next game day. Outputs a shell export
# command to create an environment variable containing
# all the games for the day.
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

schedule.days.each do |day|
  unless day.completed
    printf $stderr, "Next Gameday: %03d\n", day.day

    print "export GAMES='"

    day.games.each do |game|
      print "#{game.road_team} #{game.home_team} "
    end

    print "'\n"

    break
  end
end

exit
