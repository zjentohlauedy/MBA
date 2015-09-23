#!/usr/bin/env ruby
#
# Reads filled in schedule.csv file and calculates
# rookie and free agent draft orders
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

def compare_team_records( a, b )
  if a.overall.wins != b.overall.wins
    return a.overall.wins <=> b.overall.wins
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
    subset = workingset.select { |t| t[:wins] == team[:wins] }

    if subset.length == 2
      if subset[1][:opponents][subset[0][:name]].wins < subset[0][:opponents][subset[1][:name]].wins
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

def determine_playoff_order( playoff_records, regular_season_records )
  playoff_teams = []

  playoff_records.each do |team, team_record|
    playoff_teams.push team_record
  end

  playoff_teams.select! { |t| t.division.losses == 0 }

  lcs_losers = playoff_teams.select { |t| t.league.losses == 4 }

  ws_losers = playoff_teams.select { |t| (t.overall.losses - t.league.losses - t.division.losses) == 5 }
  ws_champs = playoff_teams.select { |t| (t.overall.wins   - t.league.wins   - t.division.wins)   == 5 }

  results = []

  results.push regular_season_records[lcs_losers[0].team]
  results.push regular_season_records[lcs_losers[1].team]

  results.sort! { |a,b| compare_team_records a, b }

  results.push regular_season_records[ws_losers[0].team]
  results.push regular_season_records[ws_champs[0].team]

  return results
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

reg_season   = {}
playoffs     = {}
rookie_draft = []
agent_draft  = []

schedule.days.each do |day|
  day.games.each do |game|
    if day.day <= 152
      add_game reg_season, game
    elsif ! TeamRecords.is_allstar? game.home_team
      add_game playoffs, game
    end
  end
end

reg_season.each do |team, team_record|
  rookie_draft.push team_record
  agent_draft.push team_record
end

rookie_draft.sort! { |a,b| compare_team_records a, b }
compare_head2head rookie_draft

playoff_teams = determine_playoff_order playoffs, reg_season

rookie_draft.reject! { |t| playoff_teams.include? t }

rookie_draft.concat playoff_teams

agent_draft.sort! { |a,b| compare_team_records a, b }
compare_head2head agent_draft

puts "Rookie Draft:\n\n"

rookie_draft.each_with_index do |team_record, idx|
  printf "%2d. %s\n", idx + 1, TeamRecords::TeamLocations[team_record.team]
end

puts "\n\n"

puts "Free Agent Draft:\n\n"

agent_draft.each_with_index do |team_record, idx|
  printf "%2d. %s\n", idx + 1, TeamRecords::TeamLocations[team_record.team]
end
