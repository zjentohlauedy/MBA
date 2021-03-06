#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'sqlite3'
require 'relative_player_leaders_filter'
require 'player_leaders_compiler'
require 'stat_rankings'

require_relative 'MBATool/web/utils'


class LeadersPrinter
  def print_empty_indicator()
    puts "--"
  end

  def print( player, format, index, tied )
    value = player.get_sort_value
    value = player.avgs.include?(player.get_sort_key) ? display_avg(value) : value

    if tied; then printf " -  ";
    else          printf "%2d. ", index + 1;
    end

    printf "%-2s %-20s S%02d  %-15s #{format}\n", player.pos, player.name, player.season, player.team, value
  end

  def print_tie_message( summary, format, index )
    printf "%2d.    %-40s  #{format}\n", index + 1, "#{summary.count} Players Tied At", summary.value
  end

  def display_avg(average)
    avg = sprintf "%5.3f", average
    avg.gsub /^0\./, ' .'
  end
end

class AllstarPlayersFilter
  def apply( players, stat )
    if players[0].class == Pitcher
      return filter_pitchers players
    elsif players[0].class == Batter
      return filter_batters players
    elsif players[0].class == Closer
      return filter_closers players
    end

    return players
  end

  def filter_pitchers( pitchers )
    return pitchers.select { |p| p.innings.to_f >= 7.0 }
  end

  def filter_batters( batters )
    return batters.select { |b| b.at_bats >= 9 }
  end

  def filter_closers( closers )
    return closers
  end
end


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
  Utils::transform_hash @db.execute "select t.* from division_teams_t dt join teams_t t on dt.team_id = t.team_id where dt.division_id = #{division[:division_id]}"
end

def get_players_by_team( team )
  Utils::transform_hash @db.execute "select p.*, tp.season from team_players_t tp join players_t p on tp.player_id = p.player_id where tp.team_id = #{team[:team_id]}"
end

def get_pitcher_by_player_id( player_id )
  result = Utils::transform_hash @db.execute "select * from pitchers_t where player_id = #{player_id}"
  result[0]
end

def get_batter_by_player_id( player_id )
  result = Utils::transform_hash @db.execute "select * from batters_t where player_id = #{player_id}"
  result[0]
end

def get_pitcher_stats_by_player_id_and_season( player_id, season )
  result = Utils::transform_hash @db.execute "select * from pitcher_stats_t where player_id = #{player_id} and season = #{season} and season_phase = 3"
  result[0]
end

def get_batter_stats_by_player_id_and_season( player_id, season )
  result = Utils::transform_hash @db.execute "select * from batter_stats_t where player_id = #{player_id}  and season = #{season} and season_phase = 3"
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

printer  = LeadersPrinter.new
filter   = AllstarPlayersFilter.new
compiler = PlayerLeadersCompiler.new org

sr = StatRankings.new printer, filter, compiler

sr.process_categories @categories
