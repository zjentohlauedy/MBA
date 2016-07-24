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

def get_leagues_by_org( organization )
  transform_hash @db.execute "select l.* from organization_leagues_t ol join leagues_t l on ol.league_id = l.league_id where ol.organization_id = #{organization[:organization_id]}"
end

def get_divisions_by_league( league )
  transform_hash @db.execute "select d.* from league_divisions_t ld join divisions_t d on ld.division_id = d.division_id where ld.league_id = #{league[:league_id]}"
end

def get_teams_by_division( division )
  transform_hash @db.execute "select t.* from division_teams_t dt join teams_t t on dt.team_id = t.team_id where dt.division_id = #{division[:division_id]}"
end

def get_players_by_team( team )
  transform_hash @db.execute "select p.*, tp.season from team_players_t tp join players_t p on tp.player_id = p.player_id where tp.team_id = #{team[:team_id]}"
end

def get_pitcher_by_player_id( player_id )
  result = transform_hash @db.execute "select * from pitchers_t where player_id = #{player_id}"
  result[0]
end

def get_batter_by_player_id( player_id )
  result = transform_hash @db.execute "select * from batters_t where player_id = #{player_id}"
  result[0]
end

def get_pitcher_stats_by_player_id_and_season( player_id, season )
  result = transform_hash @db.execute "select * from pitcher_stats_t where player_id = #{player_id} and season = #{season} and season_phase = 1"
  result[0]
end

def get_batter_stats_by_player_id_and_season( player_id, season )
  result = transform_hash @db.execute "select * from batter_stats_t where player_id = #{player_id}  and season = #{season} and season_phase = 1"
  result[0]
end

def enrich_pitcher( pitcher )
  pitcher[:type] = 'pitcher'
  pitcher[:position] = 1
  pitcher[:stats]    = {}

  pitcher[:ratings] = get_pitcher_by_player_id pitcher[:player_id]
  pitcher[:stats][:simulated] = get_pitcher_stats_by_player_id_and_season pitcher[:player_id], pitcher[:season]
end

def enrich_batter( batter )
  ratings = get_batter_by_player_id batter[:player_id]

  batter[:type] = 'batter'
  batter[:primary_position] = ratings[:primary_position]
  batter[:secondary_position] = ratings[:secondary_position]
  batter[:ratings] = ratings
  batter[:stats] = {}
  batter[:stats][:simulated] = get_batter_stats_by_player_id_and_season batter[:player_id], batter[:season]
end


if ARGV.length > 0
  chosen_team = ARGV[0]
end


org = get_organization 1

org[:leagues] = get_leagues_by_org org

org[:leagues].each do |league|
  league[:divisions] = get_divisions_by_league league

  league[:divisions].each do |division|
    division[:teams] = get_teams_by_division division

    division[:teams].each do |team|
      next if chosen_team and team[:name].downcase != chosen_team.downcase

      team[:players] = get_players_by_team team

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
