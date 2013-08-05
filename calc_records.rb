#!/usr/bin/env ruby
#
# Reads filled in schedule.csv file and calculates
# Overall, Division and Head-To-Head records as well
# as scoring totals.
location = File.dirname __FILE__

$: << "#{location}"
require 'FileParser'
require 'ScheduleParser'


class TeamRecords

  class Record
    attr_accessor :wins, :losses

    def initialize
      @wins   = 0
      @losses = 0
    end

    def update( score, opp_score )
      if score > opp_score
        @wins += 1
      else
        @losses += 1
      end
    end

    def to_s( precision = 2 )
      return sprintf "%#{precision}d-%#{precision}d", @wins, @losses
    end
  end


  def initialize
    @team_divisions = {
      'Sabres'    => 1, 'Scorpions' => 2, 'Aces'      => 3, 'Eclipse'   => 4,
      'Portsmen'  => 1, 'Lightning' => 2, 'Cyclone'   => 3, 'Legends'   => 4,
      'Settlers'  => 1, 'Goblins'   => 2, 'Stormers'  => 3, 'Waves'     => 4,
      'Kings'     => 1, 'Photons'   => 2, 'Express'   => 3, 'Horizon'   => 4,
      'Voyagers'  => 1, 'Dragons'   => 2, 'Warriors'  => 3, 'Sharks'    => 4,
      'Rockets'   => 1, 'Hammers'   => 2, 'Wanderers' => 3, 'Flames'    => 4,
      'Knights'   => 1, 'Expos'     => 2, 'Thunder'   => 3, 'Techs'     => 4,
      'Drizzle'   => 1, 'Dynamo'    => 2, 'Glory'     => 3, 'Quasars'   => 4 }

    @overall  = Record.new
    @division = Record.new
    @home     = Record.new
    @road     = Record.new

    @points_scored  = 0
    @points_allowed = 0

    @opponents = Hash.new
  end

  def print_records
    printf "%s  %s  %s  %s  %4d  %4d\n", @overall.to_s(3), @division.to_s, @home.to_s, @road.to_s, @points_scored, @points_allowed

    count = 0
    @opponents.sort.each do |opponent, record|
      printf "   %9s %s", opponent, record.to_s(1)

      count += 1

      if count == 4
        print "\n"
        count = 0
      end
    end

    print "\n\n"
  end

  def update( game, is_home )
    score     = is_home ? game.home_score : game.road_score
    opp_score = is_home ? game.road_score : game.home_score
    opponent  = is_home ? game.road_team  : game.home_team

    @points_scored += score
    @points_allowed += opp_score
    @overall.update score, opp_score

    if in_division? game.home_team, game.road_team
      @division.update score, opp_score
    end

    if is_home
      @home.update score, opp_score
    else
      @road.update score, opp_score
    end

    if @opponents.has_key? opponent
      record = @opponents.fetch opponent
    else
      record = Record.new
    end

    record.update score, opp_score
    @opponents.store opponent, record
  end

  def in_division?( home_team, road_team )
    return (@team_divisions.fetch( home_team ) == @team_divisions.fetch( road_team ))
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

schedule = sp.schedule

team_records = Hash.new

schedule.days.each do |day|
  day.games.each do |game|
    next if ! game.played

    # road team
    if team_records.has_key? game.road_team
      team_record = team_records.fetch game.road_team
    else
      team_record = TeamRecords.new
    end

    team_record.update game, false
    team_records.store game.road_team, team_record

    # home team
    if team_records.has_key? game.home_team
      team_record = team_records.fetch game.home_team
    else
      team_record = TeamRecords.new
    end

    team_record.update game, true
    team_records.store game.home_team, team_record
  end
end

team_records.sort.each do |team, team_record|
  printf "%-9s ", team
  team_record.print_records
end
