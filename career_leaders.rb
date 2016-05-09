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

def get_players
  transform_hash @db.execute "select * from players_t"
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

def get_batter_stats_by_player_id( player_id )
  result = transform_hash @db.execute "select player_id, count(1) seasons, sum(games) games, sum(at_bats) at_bats, sum(runs) runs, sum(hits) hits, sum(doubles) doubles, sum(triples) triples, sum(home_runs) home_runs, sum(runs_batted_in) runs_batted_in, sum(walks) walks, sum(strike_outs) strike_outs, sum(steals) steals, sum(errors) errors from batter_stats_t where player_id = #{player_id} and season_phase = 1 group by player_id"
  result[0]
end

def enrich_pitcher( pitcher )
  pitcher[:type] = 'pitcher'
  pitcher[:position] = 1
  pitcher[:stats]    = {}

  pitcher[:ratings] = get_pitcher_by_player_id pitcher[:player_id]
  pitcher[:stats][:simulated] = get_pitcher_stats_by_player_id pitcher[:player_id]
end

def enrich_batter( batter )
  ratings = get_batter_by_player_id batter[:player_id]

  batter[:type] = 'batter'
  batter[:primary_position] = ratings[:primary_position]
  batter[:secondary_position] = ratings[:secondary_position]
  batter[:ratings] = ratings
  batter[:stats] = {}
  batter[:stats][:simulated] = get_batter_stats_by_player_id batter[:player_id]
end


org = get_organization 1

org[:leagues] = [{league_id: 0, name: 'career league'}]

org[:leagues].each do |league|
  league[:divisions] = [{division_id: 0, name: 'career division'}]

  league[:divisions].each do |division|
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
  end
end

sr = StatRankings.new org

sr.process_categories @categories


#pitchers = get_pitchers

#pitchers.each do |pitcher|
#  pitcher[:stats] = get_pitcher_stats_by_player_id pitcher[:player_id]
#end

#pitchers.reject! { |pitcher| pitcher[:stats].nil? }

#batters = get_batters

#batters.each do |batter|
#  batter[:stats] = get_batter_stats_by_player_id batter[:player_id]
#end

#batters.reject! { |batter| batter[:stats].nil? }

#sr = StatRankings.new pitchers, batters

#sr.process_categories categories
