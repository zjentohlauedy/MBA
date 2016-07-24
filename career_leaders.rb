#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'sqlite3'
require 'TopPlayers'


@db = SQLite3::Database.new "#{location}/mba.db"

@db.results_as_hash  = true
@db.type_translation = true


def transform_hash db_hash
  result = []

  db_hash.each do |element|
    hash = {}

    element.each do|key, value|
      hash.store key.downcase.to_sym, value
    end

    result.push hash
  end

  return result
end


def get_organization( organization_id )
  result = transform_hash @db.execute "select * from organizations_t where organization_id = #{organization_id}"
  result[0]
end

def get_team_by_name( name )
  transform_hash @db.execute( "select * from teams_t where name = :name collate nocase", { name: name } )
end

def get_players
  transform_hash @db.execute "select * from players_t"
end

def get_players_for_team( team_id )
  transform_hash @db.execute "select * from players_t where player_id in (select distinct player_id from team_players_t where team_id = #{team_id})"
end

def get_team_player_seasons( team_id, player_id )
  transform_hash @db.execute "select season from team_players_t where team_id = #{team_id} and player_id = #{player_id}"
end

def get_pitcher_by_player_id( player_id )
  result = transform_hash @db.execute "select * from pitchers_t where player_id = #{player_id}"
  result[0]
end

def get_batter_by_player_id( player_id )
  result = transform_hash @db.execute "select * from batters_t where player_id = #{player_id}"
  result[0]
end

def get_pitcher_stats_by_player_id( player_id )
  result = transform_hash @db.execute "select player_id, count(1) seasons, sum(wins) wins, sum(losses) losses, sum(games) games, sum(saves) saves, sum(innings) innings, sum(outs) outs, sum(hits) hits, sum(earned_runs) earned_runs, sum(home_runs) home_runs, sum(walks) walks, sum(strike_outs) strike_outs from pitcher_stats_t where player_id = #{player_id} and season_phase = 1 group by player_id"
  result[0]
end

def get_pitcher_stats_by_player_id_and_season( player_id, seasons )
  result = transform_hash @db.execute "select player_id, count(1) seasons, sum(wins) wins, sum(losses) losses, sum(games) games, sum(saves) saves, sum(innings) innings, sum(outs) outs, sum(hits) hits, sum(earned_runs) earned_runs, sum(home_runs) home_runs, sum(walks) walks, sum(strike_outs) strike_outs from pitcher_stats_t where player_id = #{player_id} and season in #{seasons.to_s.gsub('[','(').gsub(']',')')} and season_phase = 1 group by player_id"
  result[0]
end

def get_batter_stats_by_player_id( player_id )
  result = transform_hash @db.execute "select player_id, count(1) seasons, sum(games) games, sum(at_bats) at_bats, sum(runs) runs, sum(hits) hits, sum(doubles) doubles, sum(triples) triples, sum(home_runs) home_runs, sum(runs_batted_in) runs_batted_in, sum(walks) walks, sum(strike_outs) strike_outs, sum(steals) steals, sum(errors) errors from batter_stats_t where player_id = #{player_id} and season_phase = 1 group by player_id"
  result[0]
end

def get_batter_stats_by_player_id_and_season( player_id, seasons )
  result = transform_hash @db.execute "select player_id, count(1) seasons, sum(games) games, sum(at_bats) at_bats, sum(runs) runs, sum(hits) hits, sum(doubles) doubles, sum(triples) triples, sum(home_runs) home_runs, sum(runs_batted_in) runs_batted_in, sum(walks) walks, sum(strike_outs) strike_outs, sum(steals) steals, sum(errors) errors from batter_stats_t where player_id = #{player_id} and season in #{seasons.to_s.gsub('[','(').gsub(']',')')} and season_phase = 1 group by player_id"
  result[0]
end

def enrich_pitcher( pitcher, seasons = nil )
  pitcher[:type] = 'pitcher'
  pitcher[:position] = 1
  pitcher[:stats]    = {}

  pitcher[:ratings] = get_pitcher_by_player_id pitcher[:player_id]

  if seasons
    pitcher[:stats][:simulated] = get_pitcher_stats_by_player_id_and_season pitcher[:player_id], seasons.map {|entry| entry[:season]}
  else
    pitcher[:stats][:simulated] = get_pitcher_stats_by_player_id pitcher[:player_id]
  end
end

def enrich_batter( batter, seasons = nil )
  ratings = get_batter_by_player_id batter[:player_id]

  batter[:type] = 'batter'
  batter[:primary_position] = ratings[:primary_position]
  batter[:secondary_position] = ratings[:secondary_position]
  batter[:ratings] = ratings
  batter[:stats] = {}

  if seasons
    batter[:stats][:simulated] = get_batter_stats_by_player_id_and_season batter[:player_id], seasons.map {|entry| entry[:season]}
  else
    batter[:stats][:simulated] = get_batter_stats_by_player_id batter[:player_id]
  end
end


if ARGV.length > 0
  chosen_team = ARGV[0]
end


org = get_organization 1

org[:leagues] = [{league_id: 0, name: 'career league'}]

org[:leagues].each do |league|
  league[:divisions] = [{division_id: 0, name: 'career division'}]

  league[:divisions].each do |division|
    if chosen_team.nil?
      division[:teams] = [{team_id: 0, name: '', location: ''}]

      division[:teams].each do |team|
        team[:players] = get_players

        team[:players].each do |player|
          if player[:player_type] == 1
            enrich_pitcher player
          else
            enrich_batter player
          end
        end
      end
    else
      division[:teams] = get_team_by_name chosen_team

      if division[:teams].length == 0
        raise "Team not found."
      end

      division[:teams].each do |team|
        team[:players] = get_players_for_team team[:team_id]

        team[:players].each do |player|
          seasons = get_team_player_seasons team[:team_id], player[:player_id]

          if player[:player_type] == 1
            enrich_pitcher player, seasons
          else
            enrich_batter player, seasons
          end
        end
      end
    end
  end
end

sr = StatRankings.new org

sr.process_categories @categories
