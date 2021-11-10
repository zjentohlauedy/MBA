#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'optparse'
require 'sqlite3'
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

    first_season = player.rookie_season > 0 ? player.rookie_season : 1
    last_season  = first_season + player.seasons - 1

    printf "%-2s %-20s %2d S%02d-S%02d   #{format}\n", player.pos, player.name, player.seasons, first_season, last_season, value
  end

  def print_tie_message( summary, format, index )
    printf "%2d.    %-30s    #{format}\n", index + 1, "#{summary.count} Players Tied At", summary.value
  end

  def display_avg(average)
    avg = sprintf "%5.3f", average
    avg.gsub /^0\./, ' .'
  end
end

class LeadersFilter
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
    return pitchers.select { |p| p.innings.to_f >= 1295.0 }
  end

  def filter_batters( batters )
    return batters.select { |b| b.at_bats >= 2100 }
  end

  def filter_closers( closers )
    return closers.select { |p| p.innings.to_f >= 595.0 }
  end
end


def get_organization( organization_id )
  result = Utils::transform_hash @db.execute "select * from organizations_t where organization_id = #{organization_id}"
  result[0]
end

def get_team_by_name( name )
  Utils::transform_hash @db.execute( "select * from teams_t where name = :name collate nocase", { name: name } )
end

def get_players
  Utils::transform_hash @db.execute "select * from players_t"
end

def get_players_for_team( team_id )
  Utils::transform_hash @db.execute "select * from players_t where player_id in (select distinct player_id from team_players_t where team_id = #{team_id})"
end

def get_team_player_seasons( team_id, player_id )
  Utils::transform_hash @db.execute "select season from team_players_t where team_id = #{team_id} and player_id = #{player_id}"
end

def get_pitcher_by_player_id( player_id )
  result = Utils::transform_hash @db.execute "select * from pitchers_t where player_id = #{player_id}"
  result[0]
end

def get_batter_by_player_id( player_id )
  result = Utils::transform_hash @db.execute "select * from batters_t where player_id = #{player_id}"
  result[0]
end

def get_pitcher_stats_by_player_id( player_id )
  result = Utils::transform_hash @db.execute "select player_id, count(1) seasons, sum(wins) wins, sum(losses) losses, sum(games) games, sum(saves) saves, sum(innings) innings, sum(outs) outs, sum(hits) hits, sum(earned_runs) earned_runs, sum(home_runs) home_runs, sum(walks) walks, sum(strike_outs) strike_outs from pitcher_stats_t where player_id = #{player_id} and season_phase = 1 group by player_id"
  result[0]
end

def get_pitcher_stats_by_player_id_and_season( player_id, seasons )
  result = Utils::transform_hash @db.execute "select player_id, count(1) seasons, sum(wins) wins, sum(losses) losses, sum(games) games, sum(saves) saves, sum(innings) innings, sum(outs) outs, sum(hits) hits, sum(earned_runs) earned_runs, sum(home_runs) home_runs, sum(walks) walks, sum(strike_outs) strike_outs from pitcher_stats_t where player_id = #{player_id} and season in #{seasons.to_s.gsub('[','(').gsub(']',')')} and season_phase = 1 group by player_id"
  result[0]
end

def get_batter_stats_by_player_id( player_id )
  result = Utils::transform_hash @db.execute "select player_id, count(1) seasons, sum(games) games, sum(at_bats) at_bats, sum(runs) runs, sum(hits) hits, sum(doubles) doubles, sum(triples) triples, sum(home_runs) home_runs, sum(runs_batted_in) runs_batted_in, sum(walks) walks, sum(strike_outs) strike_outs, sum(steals) steals, sum(errors) errors from batter_stats_t where player_id = #{player_id} and season_phase = 1 group by player_id"
  result[0]
end

def get_batter_stats_by_player_id_and_season( player_id, seasons )
  result = Utils::transform_hash @db.execute "select player_id, count(1) seasons, sum(games) games, sum(at_bats) at_bats, sum(runs) runs, sum(hits) hits, sum(doubles) doubles, sum(triples) triples, sum(home_runs) home_runs, sum(runs_batted_in) runs_batted_in, sum(walks) walks, sum(strike_outs) strike_outs, sum(steals) steals, sum(errors) errors from batter_stats_t where player_id = #{player_id} and season in #{seasons.to_s.gsub('[','(').gsub(']',')')} and season_phase = 1 group by player_id"
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


@options = {}

OptionParser.new do |opt|
  opt.on( '--db', '--database DATABASE_FILE' ) { |o| @options[ :database ] = o }
end.parse!

@db = SQLite3::Database.new ( @options[:database] || "#{location}/mba.db")

@db.results_as_hash  = true
@db.type_translation = true


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

printer  = LeadersPrinter.new
filter   = LeadersFilter.new
compiler = PlayerLeadersCompiler.new org

sr = StatRankings.new printer, filter, compiler

sr.process_categories @categories
