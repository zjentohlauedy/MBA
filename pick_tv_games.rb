#!/usr/bin/env ruby
#
# Reads empty in schedule.csv file and calculates
# which games to televise for a balanced tv season.
location = File.dirname __FILE__

$: << "#{location}"
require 'FileParser'
require 'ScheduleParser'


class TeamTelevisedGames

  class TeamTelevisedCounts
    attr_accessor :home, :road

    def initialize
      @home = 0
      @road = 0
    end

  end

  def initialize( seed = 1 )
    @generator   = Random.new seed
    @team_counts = Hash.new
  end

  def find_game_to_televise( day )
    i   = 0
    idx = @generator.rand day.games.length

    while i < day.games.length
      game = day.games[idx]

      if can_game_be_televised? game
        update_counts game.home_team, true
        update_counts game.road_team, false

        game.televised = true

        return true
      end

      i   += 1
      idx += 1

      if idx >= day.games.length
        idx = 0
      end
    end

    return false
  end

  def can_game_be_televised?( game )
    if get_team_count( game.road_team, false ) >= 4
      return false
    end

    if get_team_count( game.home_team, true  ) >= 4
      return false
    end

    return true
  end

  def get_team_count( team, is_home )
    if @team_counts.has_key? team
      counts = @team_counts.fetch team
      return is_home ? counts.home : counts.road
    else
      return 0
    end
  end

  def update_counts( team, is_home )
    if @team_counts.has_key? team
      counts = @team_counts.fetch team
    else
      counts = TeamTelevisedCounts.new
    end

    if is_home
      counts.home += 1
    else
      counts.road += 1
    end

    @team_counts.store team, counts
  end
end


def pick_tv_games( schedule )
  team_tv_games     = TeamTelevisedGames.new Time.new.usec
  team_home_openers = []
  days_televised    = 0

  schedule.days.each do |day|
    day.games.each do |game|
      if ! team_home_openers.include? game.home_team
        game.televised = true
        day.televised = true

        team_home_openers.push game.home_team
      end
    end

    if ! day.televised
      if team_tv_games.find_game_to_televise day
        day.televised   = true
        days_televised += 1
      end
    end
  end

  if days_televised < 128
    return false
  end

  return true
end

def print_tv_schedule( schedule )
  schedule.days.each do |day|
    print "Day #{day.day}: "

    day.games.each do |game|
      if game.televised
        print "#{game.road_team} @ #{game.home_team}, "
      end
    end

    print "\n"
  end
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

while ! pick_tv_games sp.schedule
  sp.schedule.reset_tv
end

print_tv_schedule sp.schedule


