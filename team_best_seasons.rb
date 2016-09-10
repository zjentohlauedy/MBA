#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'sqlite3'
require 'TopTeams'

require_relative 'MBATool/web/utils'


@db = SQLite3::Database.new "#{location}/mba.db"

@db.results_as_hash  = true
@db.type_translation = true


def get_organization( organization_id )
  result = Utils::transform_hash @db.execute "select * from organizations_t where organization_id = #{organization_id}"
  result[0]
end

def get_leagues_by_org( organization )
  Utils::transform_hash @db.execute "select l.* from organization_leagues_t ol join leagues_t l on ol.league_id = l.league_id where ol.organization_id = #{organization[:organization_id]}"
end

def get_divisions_by_league( league )
  Utils::transform_hash @db.execute "select d.* from league_divisions_t ld join divisions_t d on ld.division_id = d.division_id where ld.league_id = #{league[:league_id]}"
end

def get_teams_by_division( division )
  Utils::transform_hash @db.execute "select t.*, ts.season from division_teams_t dt join teams_t t on dt.team_id = t.team_id join team_stats_t ts on t.team_id = ts.team_id where dt.division_id = #{division[:division_id]} and ts.season_phase = 1"
end

def get_team_stats_by_team_id_and_season(team_id, season)
  result = Utils::transform_hash @db.execute "select * from team_stats_t where team_id = #{team_id} and season = #{season} and season_phase = 1"
  result[0]
end

def get_team_pitching_stats_by_team_id_and_season(team_id, season)
  result = Utils::transform_hash @db.execute "select * from team_pitching_stats_t where team_id = #{team_id} and season = #{season} and season_phase = 1"
  result[0]
end

def get_team_batting_stats_by_team_id_and_season(team_id, season)
  result = Utils::transform_hash @db.execute "select * from team_batting_stats_t where team_id = #{team_id} and season = #{season} and season_phase = 1"
  result[0]
end


org = get_organization 1

org[:leagues] = get_leagues_by_org org

org[:leagues].each do |league|
  league[:divisions] = get_divisions_by_league league

  league[:divisions].each do |division|
    division[:teams] = get_teams_by_division division

    division[:teams].each do |team|
      team[ :stats          ] =              get_team_stats_by_team_id_and_season(          team[:team_id], team[:season] )
      team[ :pitching_stats ] = { simulated: get_team_pitching_stats_by_team_id_and_season( team[:team_id], team[:season] ) }
      team[ :batting_stats  ] = { simulated: get_team_batting_stats_by_team_id_and_season(  team[:team_id], team[:season] ) }
    end
  end
end

sr = StatRankings.new org

sr.process_categories @categories
