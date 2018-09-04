#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'optparse'
require 'sqlite3'

require_relative 'MBATool/web/accolades'
require_relative 'MBATool/web/positions'
require_relative 'MBATool/web/phases'
require_relative 'MBATool/web/utils'


@options = {}

OptionParser.new do |opt|
  opt.on( '-x', '--extended'            ) { |o| @options[ :extended            ] = o }
  opt.on( '-r', '--regular-season-only' ) { |o| @options[ :regular_season_only ] = o }
end.parse!


if ARGV.length == 1
  @player_id = ARGV[0]
elsif ARGV.length == 2
  @firstname = ARGV[0]
  @lastname  = ARGV[1]
else
  raise "Player ID or Player Name is required."
end


AllTimeRecord            = 5
SeasonBestWithTeamRecord = 4
SeasonBest               = 3
TeamRecord               = 2
SeasonBestForTeam        = 1
NothingSpecial           = 0

BatterComparisons = {
  games:          '>=',
  at_bats:        '>=',
  runs:           '>=',
  hits:           '>=',
  doubles:        '>=',
  triples:        '>=',
  home_runs:      '>=',
  runs_batted_in: '>=',
  steals:         '>=',
  walks:          '>=',
  strike_outs:    '<=',
  avg:            '>=',
  sa:             '>=',
  oba:            '>=',
  soa:            '<=',
  rpg:            '>='
}

PitcherComparisons = {
  wins:           '>=',
  losses:         '<=',
  innings:        '>=',
  outs:           '>=',
  games:          '>=',
  saves:          '>=',
  hits:           '<=',
  earned_runs:    '<=',
  home_runs:      '<=',
  walks:          '<=',
  strike_outs:    '>=',
  era:            '<=',
  vsba:           '<=',
  ip:             '>=',
  ipg:            '>=',
  whip:           '<=',
  sop9:           '>=',
  hrp9:           '<=',
  eff:            '>='
}


class PrintFormatter
  def initialize()
    @stats        = {}
    @categories   = {}
    @career_bests = {}
  end

  def set_stats( stats )
    @stats = stats
  end

  def set_categories( categories )
    @categories = categories
  end

  def set_career_bests( career_bests )
    @career_bests = career_bests
  end

  def print_stat( stat, format, field_length )
    if format == '#avg'
      text_value = sprintf( "%5.3f", @stats[stat] ).gsub '0.', '.'
    elsif format == '#ip'
      text_value = sprintf "%d.%d", @stats[:innings], @stats[:outs]
    else
      text_value = sprintf format, @stats[stat]
    end

    padding = (field_length - text_value.length)

    printf( "%*s%s%s%s ", padding < 0 ? 0 : padding, "", hilite( @categories[stat], @career_bests[stat]), text_value, unhilite() )
  end

  def hilite( value, career_best = false )
    hilite = "\e[0m"

    case value
    when AllTimeRecord;            hilite = "\e[1;33m"
    when SeasonBestWithTeamRecord; hilite = "\e[1;36m"
    when SeasonBest;               hilite = "\e[1;37m"
    when TeamRecord;               hilite = "\e[36m"
    when SeasonBestForTeam;        hilite = "\e[35m"
    end

    if career_best
      hilite = "#{hilite}\e[4m"
    end

    return hilite
  end

  def unhilite()
    return "\e[0m"
  end
end


@db = SQLite3::Database.new "#{location}/mba.db"

@db.results_as_hash  = true
@db.type_translation = true


def calc_eff( stats )
  if (stats[:innings] + stats[:outs]) == 0; then return 0.0 end

  finn = (stats[:outs].to_f / 3.0 + stats[:innings].to_f)
  eff  = (finn - stats[:hits]) + (stats[:strike_outs] - stats[:hits])

  eff.to_f / finn * 9.0
end

def calc_ipg( stats )
  if stats[:games] == 0; then return 0.0 end

  (stats[:outs].to_f / 3.0 + stats[:innings].to_f) / stats[:games].to_f
end

def calc_vsba( stats )
  if (stats[:innings] + stats[:outs]) == 0; then return 0.0 end

  stats[:hits].to_f / (stats[:innings] * 3 + stats[:outs] + stats[:hits]).to_f
end

def calc_era( stats )
  if stats[:earned_runs] == 0; then return 0.0 end

  stats[:earned_runs].to_f / (stats[:outs].to_f / 3.0 + stats[:innings].to_f) * 9.0
end

def calc_hrp9( stats )
  if stats[:home_runs] == 0; then return 0.0 end

  stats[:home_runs].to_f / (stats[:outs].to_f / 3.0 + stats[:innings].to_f) * 9.0
end

def calc_sop9( stats )
  if stats[:strike_outs] == 0; then return 0.0 end

  stats[:strike_outs].to_f / (stats[:outs].to_f / 3.0 + stats[:innings].to_f) * 9.0
end

def calc_whip( stats )
  if (stats[:innings] + stats[:outs]) == 0; then return 0.0 end

  (stats[:hits] + stats[:walks]).to_f / (stats[:outs].to_f / 3.0 + stats[:innings].to_f)
end

def calc_avg( stats )
  if stats[:at_bats] == 0; then return 0.0 end

  stats[:hits].to_f / stats[:at_bats].to_f
end

def calc_soa( stats )
  if stats[:at_bats] == 0; then return 0.0 end

  stats[:strike_outs].to_f / stats[:at_bats].to_f
end

def calc_oba( stats )
  if stats[:at_bats] == 0; then return 0.0 end

  (stats[:hits] + stats[:walks]).to_f / (stats[:at_bats] + stats[:walks]).to_f
end

def calc_sa( stats )
  if stats[:at_bats] == 0; then return 0.0 end

  extra_bases = stats[:doubles] + stats[:triples] + stats[:home_runs]
  total_bases = (stats[:hits] - extra_bases) + (stats[:doubles] * 2) + (stats[:triples] * 3) + (stats[:home_runs] * 4)

  total_bases.to_f / stats[:at_bats].to_f
end

def calc_rpg( stats )
  if stats[:games] == 0; then return 0.0 end

  (stats[:runs] + stats[:runs_batted_in] - stats[:home_runs]).to_f / stats[:games].to_f
end

def calc_ip( stats )
  stats[:innings].to_f + (stats[:outs].to_f / 10.0)
end

def get_organization( organization_id )
  result = Utils::transform_hash @db.execute "select * from organizations_t where organization_id = #{organization_id}"
  result[0]
end

def get_player( player_id )
  args = { player_id: player_id }
  query = "select * from players_t where player_id = :player_id"

  result = Utils::transform_hash @db.execute query, args
  result[0]
end

def get_player_by_name( firstname, lastname )
  args = { first_name: firstname, last_name: lastname }
  query = "select * from players_t where first_name = :first_name collate nocase and last_name = :last_name collate nocase"

  result = Utils::transform_hash @db.execute query, args
  result[0]
end

def get_current_team( player_id, season )
  args = { player_id: player_id, season: season }
  query = "select * from team_players_t tp join teams_t t on tp.team_id = t.team_id where tp.player_id = :player_id and tp.season = :season"

  result = Utils::transform_hash @db.execute query, args
  result[0]
end

def get_pitcher_details( player_id )
  result = Utils::transform_hash @db.execute "select * from pitchers_t where player_id = #{player_id}"
  result[0]
end

def get_batter_details( player_id )
  result = Utils::transform_hash @db.execute "select * from batters_t where player_id = #{player_id}"
  result[0]
end

def get_pitcher_stats( player_id, phase = Phases::RegularSeason )
  query = "select * from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season join teams_t t on tp.team_id = t.team_id where ps.player_id = #{player_id} and ps.season_phase = #{phase} and tp.team_id <= 32 order by ps.season"

  result = Utils::transform_hash @db.execute query
end

def get_batter_stats( player_id, phase = Phases::RegularSeason )
  query = "select * from batter_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season join teams_t t on tp.team_id = t.team_id where ps.player_id = #{player_id} and ps.season_phase = #{phase} and tp.team_id <= 32 order by ps.season"

  result = Utils::transform_hash @db.execute query
end

def db_get_one( query )
  result = Utils::transform_hash @db.execute query

  return result[0]
end

def get_best_pitcher_stats_overall()
  result        = db_get_one 'select max(wins) wins, max(games) games, max(saves) saves, max(strike_outs) strike_outs from pitcher_stats_t'
  result[:era]  = db_get_one 'select innings, outs, earned_runs, min(cast(earned_runs as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) era from pitcher_stats_t where innings >= 185'
  result[:ip]   = db_get_one 'select innings, outs, max(cast(innings as float) + (cast(outs as float) / 10.0)) ip from pitcher_stats_t'
  result[:vsba] = db_get_one 'select innings, outs, hits, min(cast(hits as float) / cast((innings * 3 + outs + hits) as float)) vsba from pitcher_stats_t where innings >= 185'
  result[:ipg]  = db_get_one 'select innings, outs, games, max((cast(outs as float) / 3.0 + cast(innings as float)) / cast(games as float)) ipg from pitcher_stats_t where innings >= 185'
  result[:whip] = db_get_one 'select innings, outs, walks, hits, min(cast((walks + hits) as float) / (cast(outs as float) / 3.0 + cast(innings as float))) whip from pitcher_stats_t where innings >= 185'
  result[:sop9] = db_get_one 'select innings, outs, strike_outs, max(cast(strike_outs as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) era from pitcher_stats_t where innings >= 185'
  result[:hrp9] = db_get_one 'select innings, outs, home_runs, min(cast(home_runs as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) era from pitcher_stats_t where innings >= 185'
  result[:eff]  = db_get_one 'select innings, outs, hits, strike_outs, max(cast(((cast(outs as float) / 3.0 + cast(innings as float)) - hits) + (strike_outs - hits) as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) eff from pitcher_stats_t where innings >= 185'

  return result
end

def get_best_pitcher_stats_by_team( team_id )
  result        = db_get_one "select max(wins) wins, max(games) games, max(saves) saves, max(strike_outs) strike_outs from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id}"
  result[:era]  = db_get_one "select innings, outs, earned_runs, min(cast(earned_runs as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) era from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and innings >= 185"
  result[:ip]   = db_get_one "select innings, outs, max(cast(innings as float) + (cast(outs as float) / 10.0)) ip from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id}"
  result[:vsba] = db_get_one "select innings, outs, hits, min(cast(hits as float) / cast((innings * 3 + outs + hits) as float)) vsba from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and innings >= 185"
  result[:ipg]  = db_get_one "select innings, outs, games, max((cast(outs as float) / 3.0 + cast(innings as float)) / cast(games as float)) ipg from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and innings >= 185"
  result[:whip] = db_get_one "select innings, outs, walks, hits, min(cast((walks + hits) as float) / (cast(outs as float) / 3.0 + cast(innings as float))) whip from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and innings >= 185"
  result[:sop9] = db_get_one "select innings, outs, strike_outs, max(cast(strike_outs as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) era from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and innings >= 185"
  result[:hrp9] = db_get_one "select innings, outs, home_runs, min(cast(home_runs as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) era from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and innings >= 185"
  result[:eff]  = db_get_one "select innings, outs, hits, strike_outs, max(cast(((cast(outs as float) / 3.0 + cast(innings as float)) - hits) + (strike_outs - hits) as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) eff from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and innings >= 185"

  return result
end

def get_best_pitcher_stats_by_season( season )
  result        = db_get_one "select max(wins) wins, max(games) games, max(saves) saves, max(strike_outs) strike_outs from pitcher_stats_t where season = #{season}"
  result[:era]  = db_get_one "select innings, outs, earned_runs, min(cast(earned_runs as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) era from pitcher_stats_t where innings >= 185 and season = #{season}"
  result[:ip]   = db_get_one "select innings, outs, max(cast(innings as float) + (cast(outs as float) / 10.0)) ip from pitcher_stats_t where season = #{season}"
  result[:vsba] = db_get_one "select innings, outs, hits, min(cast(hits as float) / cast((innings * 3 + outs + hits) as float)) vsba from pitcher_stats_t where innings >= 185 and season = #{season}"
  result[:ipg]  = db_get_one "select innings, outs, games, max((cast(outs as float) / 3.0 + cast(innings as float)) / cast(games as float)) ipg from pitcher_stats_t where innings >= 185 and season = #{season}"
  result[:whip] = db_get_one "select innings, outs, walks, hits, min(cast((walks + hits) as float) / (cast(outs as float) / 3.0 + cast(innings as float))) whip from pitcher_stats_t where innings >= 185 and season = #{season}"
  result[:sop9] = db_get_one "select innings, outs, strike_outs, max(cast(strike_outs as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) era from pitcher_stats_t where innings >= 185 and season = #{season}"
  result[:hrp9] = db_get_one "select innings, outs, home_runs, min(cast(home_runs as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) era from pitcher_stats_t where innings >= 185 and season = #{season}"
  result[:eff]  = db_get_one "select innings, outs, hits, strike_outs, max(cast(((cast(outs as float) / 3.0 + cast(innings as float)) - hits) + (strike_outs - hits) as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) eff from pitcher_stats_t where innings >= 185 and season = #{season}"

  return result
end

def get_best_pitcher_stats_by_team_and_season( team_id, season )
  result        = db_get_one "select max(wins) wins, max(games) games, max(saves) saves, max(strike_outs) strike_outs from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and ps.season = #{season}"
  result[:era]  = db_get_one "select innings, outs, earned_runs, min(cast(earned_runs as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) era from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and ps.season = #{season} and innings >= 185"
  result[:ip]   = db_get_one "select innings, outs, max(cast(innings as float) + (cast(outs as float) / 10.0)) ip from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and ps.season = #{season}"
  result[:vsba] = db_get_one "select innings, outs, hits, min(cast(hits as float) / cast((innings * 3 + outs + hits) as float)) vsba from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and ps.season = #{season} and innings >= 185"
  result[:ipg]  = db_get_one "select innings, outs, games, max((cast(outs as float) / 3.0 + cast(innings as float)) / cast(games as float)) ipg from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and ps.season = #{season} and innings >= 185"
  result[:whip] = db_get_one "select innings, outs, walks, hits, min(cast((walks + hits) as float) / (cast(outs as float) / 3.0 + cast(innings as float))) whip from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and ps.season = #{season} and innings >= 185"
  result[:sop9] = db_get_one "select innings, outs, strike_outs, max(cast(strike_outs as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) era from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and ps.season = #{season} and innings >= 185"
  result[:hrp9] = db_get_one "select innings, outs, home_runs, min(cast(home_runs as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) era from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and ps.season = #{season} and innings >= 185"
  result[:eff]  = db_get_one "select innings, outs, hits, strike_outs, max(cast(((cast(outs as float) / 3.0 + cast(innings as float)) - hits) + (strike_outs - hits) as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) eff from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season where tp.team_id = #{team_id} and ps.season = #{season} and innings >= 185"

  return result
end

BatterStatsBaseQuery    = 'select max(at_bats) at_bats, max(runs) runs, max(hits) hits, max(doubles) doubles, max(triples) triples, max(home_runs) home_runs, max(runs_batted_in) runs_batted_in, max(steals) steals, max(walks) walks from batter_stats_t'
BattingAverageBaseQuery = 'select at_bats, hits, max(cast(hits as float) / at_bats) avg from batter_stats_t'
BattingSABaseQuery      = 'select at_bats, hits, doubles, triples, home_runs, max(cast(((hits - doubles - triples - home_runs) + doubles * 2 + triples * 3 + home_runs * 4) as float) / cast(at_bats as float)) sa from batter_stats_t'
BattingOBABaseQuery     = 'select at_bats, hits, walks, max(cast((hits + walks) as float) / (at_bats + walks)) oba from batter_stats_t'
BattingSOABaseQuery     = 'select at_bats, strike_outs, min(cast(strike_outs as float) / at_bats) soa from batter_stats_t'
BattingRPGBaseQuery     = 'select games, runs, runs_batted_in, home_runs, max(cast((runs + runs_batted_in - home_runs) as float) / games) rpg from batter_stats_t'
BattingJoinTeamPlayers  = 'bs join team_players_t tp on bs.player_id = tp.player_id and bs.season = tp.season'

def get_best_batter_stats_overall()
  result       = db_get_one    BatterStatsBaseQuery
  result[:avg] = db_get_one "#{BattingAverageBaseQuery} where at_bats >= 300"
  result[:sa ] = db_get_one "#{BattingSABaseQuery     } where at_bats >= 300"
  result[:oba] = db_get_one "#{BattingOBABaseQuery    } where at_bats >= 300"
  result[:soa] = db_get_one "#{BattingSOABaseQuery    } where at_bats >= 300"
  result[:rpg] = db_get_one "#{BattingRPGBaseQuery    } where at_bats >= 300"

  return result
end

def get_best_batter_stats_by_team( team_id )
  result       = db_get_one "#{BatterStatsBaseQuery   } #{BattingJoinTeamPlayers} where tp.team_id = #{team_id}"
  result[:avg] = db_get_one "#{BattingAverageBaseQuery} #{BattingJoinTeamPlayers} where tp.team_id = #{team_id} and at_bats >= 300"
  result[:sa ] = db_get_one "#{BattingSABaseQuery     } #{BattingJoinTeamPlayers} where tp.team_id = #{team_id} and at_bats >= 300"
  result[:oba] = db_get_one "#{BattingOBABaseQuery    } #{BattingJoinTeamPlayers} where tp.team_id = #{team_id} and at_bats >= 300"
  result[:soa] = db_get_one "#{BattingSOABaseQuery    } #{BattingJoinTeamPlayers} where tp.team_id = #{team_id} and at_bats >= 300"
  result[:rpg] = db_get_one "#{BattingRPGBaseQuery    } #{BattingJoinTeamPlayers} where tp.team_id = #{team_id} and at_bats >= 300"

  return result
end

def get_best_batter_stats_by_season( season )
  result       = db_get_one "#{BatterStatsBaseQuery   } where season = #{season}"
  result[:avg] = db_get_one "#{BattingAverageBaseQuery} where season = #{season} and at_bats >= 300"
  result[:sa ] = db_get_one "#{BattingSABaseQuery     } where season = #{season} and at_bats >= 300"
  result[:oba] = db_get_one "#{BattingOBABaseQuery    } where season = #{season} and at_bats >= 300"
  result[:soa] = db_get_one "#{BattingSOABaseQuery    } where season = #{season} and at_bats >= 300"
  result[:rpg] = db_get_one "#{BattingRPGBaseQuery    } where season = #{season} and at_bats >= 300"

  return result
end

def get_best_batter_stats_by_team_and_season( team_id, season )
  result       = db_get_one "#{BatterStatsBaseQuery   } #{BattingJoinTeamPlayers} where tp.team_id = #{team_id} and bs.season = #{season}"
  result[:avg] = db_get_one "#{BattingAverageBaseQuery} #{BattingJoinTeamPlayers} where tp.team_id = #{team_id} and bs.season = #{season} and at_bats >= 300"
  result[:sa ] = db_get_one "#{BattingSABaseQuery     } #{BattingJoinTeamPlayers} where tp.team_id = #{team_id} and bs.season = #{season} and at_bats >= 300"
  result[:oba] = db_get_one "#{BattingOBABaseQuery    } #{BattingJoinTeamPlayers} where tp.team_id = #{team_id} and bs.season = #{season} and at_bats >= 300"
  result[:soa] = db_get_one "#{BattingSOABaseQuery    } #{BattingJoinTeamPlayers} where tp.team_id = #{team_id} and bs.season = #{season} and at_bats >= 300"
  result[:rpg] = db_get_one "#{BattingRPGBaseQuery    } #{BattingJoinTeamPlayers} where tp.team_id = #{team_id} and bs.season = #{season} and at_bats >= 300"

  return result
end

def get_player_accolades( player_id )
  args = { player_id:   player_id }
  query = %Q(
  SELECT *
  FROM   player_accolades_t
  WHERE  player_id = :player_id
  AND    accolade in ( #{Accolades::Player::All_Star_MVP}, #{Accolades::Player::World_Series_MVP} )
  )

  results = Utils::transform_hash @db.execute query, args

  results.each do |result|
    result[:accolade_name] = Accolades::get_accolade_name Accolades::Player::Type, result[:accolade]
  end

  return results
end

def get_batter_accolades( player_id )
  args = { player_id:   player_id }
  query = %Q(
  SELECT *
  FROM   batter_accolades_t
  WHERE  player_id = :player_id
  AND    accolade IN (
           #{Accolades::Batting::Batter_of_the_Year          },
           #{Accolades::Batting::Global_Batter_of_the_Year   },
           #{Accolades::Batting::World_Batter_of_the_Year    },
           #{Accolades::Batting::Atlantic_Batter_of_the_Year },
           #{Accolades::Batting::North_Batter_of_the_Year    },
           #{Accolades::Batting::Pacific_Batter_of_the_Year  },
           #{Accolades::Batting::South_Batter_of_the_Year    },
           #{Accolades::Batting::Batting_Rookie_of_the_Year  }
         )
  )

  results = Utils::transform_hash @db.execute query, args

  results.each do |result|
    result[:accolade_name] = Accolades::get_accolade_name Accolades::Batting::Type, result[:accolade]
  end

  return results
end

def get_pitcher_accolades( player_id )
  args = { player_id:   player_id }
  query = %Q(
  SELECT *
  FROM   pitcher_accolades_t
  WHERE  player_id = :player_id
  AND    accolade IN (
         #{Accolades::Pitching::Pitcher_of_the_Year          },
         #{Accolades::Pitching::Global_Pitcher_of_the_Year   },
         #{Accolades::Pitching::World_Pitcher_of_the_Year    },
         #{Accolades::Pitching::Atlantic_Pitcher_of_the_Year },
         #{Accolades::Pitching::North_Pitcher_of_the_Year    },
         #{Accolades::Pitching::Pacific_Pitcher_of_the_Year  },
         #{Accolades::Pitching::South_Pitcher_of_the_Year    },
         #{Accolades::Pitching::Pitching_Rookie_of_the_Year  }
         )
  )

  results = Utils::transform_hash @db.execute query, args

  results.each do |result|
    result[:accolade_name] = Accolades::get_accolade_name Accolades::Pitching::Type, result[:accolade]
  end

  return results
end

def calc_category_tag( value, op, targets = {} )
  if    value.send( op, targets[:overall]);                                  then return AllTimeRecord
  elsif value.send( op, targets[:season]) \
    and value.send( op, targets[:team]);                                     then return SeasonBestWithTeamRecord
  elsif value.send( op, targets[:season]);                                   then return SeasonBest
  elsif value.send( op, targets[:team]);                                     then return TeamRecord
  elsif value.send( op, targets[:team_season]);                              then return SeasonBestForTeam
  else                                                                            return NothingSpecial end
end

def special_category_tag( categories, calc_method, stat, value, op, best )
  overall_best     = send calc_method, best[ :overall     ][stat]
  season_best      = send calc_method, best[ :season      ][stat]
  team_best        = send calc_method, best[ :team        ][stat]
  team_season_best = send calc_method, best[ :team_season ][stat]

  categories[stat] = calc_category_tag( value, op, overall: overall_best, season: season_best, team: team_best, team_season: team_season_best )
end


def print_pitcher_stats( pitcher, type )
  totals = { wins: 0, losses: 0, innings: 0, outs: 0, games: 0, saves: 0, hits: 0, earned_runs: 0, home_runs: 0, walks: 0, strike_outs: 0 }

  if pitcher[:stats][type].length == 0; then return; end

  pf = PrintFormatter.new

  puts ""

  case type
  when :regular; puts 'Regular Season:'
  when :playoff; puts 'Playoffs:'
  when :allstar; puts 'Allstar:'
  end

  if @options[:extended]
    puts "Year Team          ERA     W   L   IP     G SV    H   ER   HR   BB   SO  VSBA INN/G  WHIP  SO/9  HR/9    EFF"
  else
    puts "Year Team          ERA     W   L   IP     G SV    H   ER   BB   SO"
  end

  pitcher[:stats][type].each do |stat|
    stat[ :era  ] = calc_era  stat
    stat[ :vsba ] = calc_vsba stat
    stat[ :ipg  ] = calc_ipg  stat
    stat[ :ip   ] = calc_ip   stat
    stat[ :whip ] = calc_whip stat
    stat[ :sop9 ] = calc_sop9 stat
    stat[ :hrp9 ] = calc_hrp9 stat
    stat[ :eff  ] = calc_eff  stat
  end

  pitcher[:stats][type].each do |stat|
    categories  = {}
    career_high = {}

    if type == :regular and stat[:innings] >= 185
      best = {
        overall:     get_best_pitcher_stats_overall(),
        season:      get_best_pitcher_stats_by_season( stat[:season] ),
        team:        get_best_pitcher_stats_by_team( stat[:team_id] ),
        team_season: get_best_pitcher_stats_by_team_and_season( stat[:team_id], stat[:season] )
      }

      stat.each_pair do |k, v|
        next if best[:overall][k].nil? or best[:overall][k].is_a? Hash

        targets = {
          overall:     best[ :overall     ][k],
          season:      best[ :season      ][k],
          team:        best[ :team        ][k],
          team_season: best[ :team_season ][k]
        }

        categories[k] = calc_category_tag( v, PitcherComparisons[k], targets )
      end

      special_category_tag categories, 'calc_era',  :era,  stat[ :era  ], PitcherComparisons[ :era  ], best
      special_category_tag categories, 'calc_vsba', :vsba, stat[ :vsba ], PitcherComparisons[ :vsba ], best
      special_category_tag categories, 'calc_ip',   :ip,   stat[ :ip   ], PitcherComparisons[ :ip   ], best
      special_category_tag categories, 'calc_ipg',  :ipg,  stat[ :ipg  ], PitcherComparisons[ :ipg  ], best
      special_category_tag categories, 'calc_whip', :whip, stat[ :whip ], PitcherComparisons[ :whip ], best
      special_category_tag categories, 'calc_sop9', :sop9, stat[ :sop9 ], PitcherComparisons[ :sop9 ], best
      special_category_tag categories, 'calc_hrp9', :hrp9, stat[ :hrp9 ], PitcherComparisons[ :hrp9 ], best
      special_category_tag categories, 'calc_eff',  :eff,  stat[ :eff  ], PitcherComparisons[ :eff  ], best

      stat.keys.each do |key|
        career_best = pitcher[:stats][type].map { |x|
          x[:innings] >= 185 ? x[key] : nil
        }.select { |x| ! x.nil? }.send PitcherComparisons[key] == '>=' ? :max : :min

        next unless [Integer, Float].include? career_best.class

        if career_best > 0
          career_high[key] = stat[key] == career_best
        end
      end
    end

    pf.set_stats        stat
    pf.set_categories   categories
    pf.set_career_bests career_high

    printf "S%02d  %-10s  ", stat[ :season      ], stat[ :name        ]

    pf.print_stat :era,           "%4.2f", 6
    pf.print_stat :wins,          "%d",    4
    pf.print_stat :losses,        "%d",    3
    pf.print_stat :ip,            "#ip",   6
    pf.print_stat :games,         "%d",    3
    pf.print_stat :saves,         "%d",    2
    pf.print_stat :hits,          "%d",    4
    pf.print_stat :earned_runs,   "%d",    4

    if @options[:extended]
      pf.print_stat :home_runs,     "%d",     4
    end

    pf.print_stat :walks,         "%d",    4
    pf.print_stat :strike_outs,   "%d",    4

    if @options[:extended]
      pf.print_stat :vsba,          "#avg",   5
      pf.print_stat :ipg,           "%4.2f",  5
      pf.print_stat :whip,          "%5.3f",  5
      pf.print_stat :sop9,          "%4.2f",  5
      pf.print_stat :hrp9,          "%4.2f",  5
      pf.print_stat :eff,           "%+5.2f", 6
    end

    printf "\n"

    totals[ :wins        ] += stat[ :wins        ]
    totals[ :losses      ] += stat[ :losses      ]
    totals[ :innings     ] += stat[ :innings     ]
    totals[ :outs        ] += stat[ :outs        ]
    totals[ :games       ] += stat[ :games       ]
    totals[ :saves       ] += stat[ :saves       ]
    totals[ :hits        ] += stat[ :hits        ]
    totals[ :earned_runs ] += stat[ :earned_runs ]
    totals[ :home_runs   ] += stat[ :home_runs   ]
    totals[ :walks       ] += stat[ :walks       ]
    totals[ :strike_outs ] += stat[ :strike_outs ]
  end

  totals[ :innings ] += totals[ :outs ] / 3
  totals[ :outs    ]  = totals[ :outs ] % 3

  overall_era  = calc_era  totals

  if @options[:extended]
    overall_vsba = calc_vsba totals
    overall_ipg  = calc_ipg  totals
    overall_whip = calc_whip totals
    overall_sop9 = calc_sop9 totals
    overall_hrp9 = calc_hrp9 totals
    overall_eff  = calc_eff  totals

    dsp_overall_avg = sprintf( "%5.3f", overall_vsba ).gsub '0.', ' .'

    printf "\e[1m%-15s  %6.2f  %3d %3d %4d.%d %3d %2d %4d %4d %4d %4d %4d %s %5.2f %5.3f %5.2f %5.2f %+6.2f\e[0m\n",
           'TOTAL',
           overall_era,
           totals[ :wins        ],
           totals[ :losses      ],
           totals[ :innings     ],
           totals[ :outs        ],
           totals[ :games       ],
           totals[ :saves       ],
           totals[ :hits        ],
           totals[ :earned_runs ],
           totals[ :home_runs   ],
           totals[ :walks       ],
           totals[ :strike_outs ],
           dsp_overall_avg,
           overall_ipg,
           overall_whip,
           overall_sop9,
           overall_hrp9,
           overall_eff
  else
    printf "\e[1m%-15s  %6.2f  %3d %3d %4d.%d %3d %2d %4d %4d %4d %4d\e[0m\n",
           'TOTAL',
           overall_era,
           totals[ :wins        ],
           totals[ :losses      ],
           totals[ :innings     ],
           totals[ :outs        ],
           totals[ :games       ],
           totals[ :saves       ],
           totals[ :hits        ],
           totals[ :earned_runs ],
           totals[ :walks       ],
           totals[ :strike_outs ]
  end
end

def print_pitcher_awards( pitcher )
  accolades = get_player_accolades( pitcher[:player_id] ).concat get_pitcher_accolades( pitcher[:player_id] )

  return if accolades.length == 0

  puts ""
  puts "Awards:"
  puts "Year  Award"

  accolades.sort {|a,b| a[:season] <=> b[:season]}.each do |accolade|
    printf "S%02d   %s\n", accolade[:season], accolade[:accolade_name]
  end
end

def print_batter_stats( batter, type )
  totals = { games: 0, at_bats: 0, runs: 0, hits: 0, doubles: 0, triples: 0, home_runs: 0, runs_batted_in: 0, steals: 0, walks: 0, strike_outs: 0, errors: 0 }

  if batter[:stats][type].length == 0; then return; end

  pf = PrintFormatter.new

  puts ""

  case type
  when :regular; puts 'Regular Season:'
  when :playoff; puts 'Playoffs:'
  when :allstar; puts 'Allstar:'
  end

  if @options[:extended]
    puts "Year Team          BA     G   AB    R    H  2B  3B   HR  RBI  SB   BB   SO   E    SA   OBA   SOA  RPG"
  else
    puts "Year Team          BA     G   AB    R    H  2B  3B   HR  RBI  SB   BB   SO"
  end

  batter[:stats][type].each do |stat|
    stat[ :avg ] = calc_avg stat
    stat[ :sa  ] = calc_sa  stat
    stat[ :oba ] = calc_oba stat
    stat[ :soa ] = calc_soa stat
    stat[ :rpg ] = calc_rpg stat
  end

  batter[:stats][type].each do |stat|
    categories  = {}
    career_high = {}

    if type == :regular and stat[:at_bats] >= 300
      best = {
        overall:     get_best_batter_stats_overall(),
        season:      get_best_batter_stats_by_season( stat[:season] ),
        team:        get_best_batter_stats_by_team( stat[:team_id] ),
        team_season: get_best_batter_stats_by_team_and_season( stat[:team_id], stat[:season] )
      }

      stat.each_pair do |k, v|
        next if best[:overall][k].nil? or best[:overall][k].is_a? Hash

        targets = {
          overall:     best[ :overall     ][k],
          season:      best[ :season      ][k],
          team:        best[ :team        ][k],
          team_season: best[ :team_season ][k]
        }

        categories[k] = calc_category_tag( v, BatterComparisons[k], targets )
      end

      special_category_tag categories, 'calc_avg',  :avg, stat[ :avg ], BatterComparisons[ :avg ], best
      special_category_tag categories, 'calc_sa',   :sa,  stat[ :sa  ], BatterComparisons[ :sa  ], best
      special_category_tag categories, 'calc_oba',  :oba, stat[ :oba ], BatterComparisons[ :oba ], best
      special_category_tag categories, 'calc_soa',  :soa, stat[ :soa ], BatterComparisons[ :soa ], best
      special_category_tag categories, 'calc_rpg',  :rpg, stat[ :rpg ], BatterComparisons[ :rpg ], best

      stat.keys.each do |key|
        career_best = batter[:stats][type].map { |x|
          x[:at_bats] >= 300 ? x[key] : nil
        }.select { |x| ! x.nil? }.send BatterComparisons[key] == '>=' ? :max : :min

        next unless [Integer, Float].include? career_best.class

        if career_best > 0
          career_high[key] = stat[key] == career_best
        end
      end
    end

    pf.set_stats        stat
    pf.set_categories   categories
    pf.set_career_bests career_high

    printf "S%02d  %-10s  ", stat[ :season ], stat[ :name   ]


    pf.print_stat :avg,            "#avg", 5
    pf.print_stat :games,          "%d",   4
    pf.print_stat :at_bats,        "%d",   4
    pf.print_stat :runs,           "%d",   4
    pf.print_stat :hits,           "%d",   4
    pf.print_stat :doubles,        "%d",   3
    pf.print_stat :triples,        "%d",   3
    pf.print_stat :home_runs,      "%d",   4
    pf.print_stat :runs_batted_in, "%d",   4
    pf.print_stat :steals,         "%d",   3
    pf.print_stat :walks,          "%d",   4
    pf.print_stat :strike_outs,    "%d",   4

    if @options[:extended]
      pf.print_stat :errors, "%d",    3
      pf.print_stat :sa,     "#avg",  5
      pf.print_stat :oba,    "#avg",  5
      pf.print_stat :soa,    "#avg",  5
      pf.print_stat :rpg,    "%4.2f", 4
    end

    printf "\n"

    totals[ :games         ] += stat[ :games         ]
    totals[ :at_bats       ] += stat[ :at_bats       ]
    totals[ :runs          ] += stat[ :runs          ]
    totals[ :hits          ] += stat[ :hits          ]
    totals[ :doubles       ] += stat[ :doubles       ]
    totals[ :triples       ] += stat[ :triples       ]
    totals[ :home_runs     ] += stat[ :home_runs     ]
    totals[ :runs_batted_in] += stat[ :runs_batted_in]
    totals[ :steals        ] += stat[ :steals        ]
    totals[ :walks         ] += stat[ :walks         ]
    totals[ :strike_outs   ] += stat[ :strike_outs   ]
    totals[ :errors        ] += stat[ :errors        ]
  end

  overall_avg = calc_avg totals

  dsp_overall_avg = sprintf( "%5.3f", overall_avg ).gsub '0.', ' .'

  if @options[:extended]
    overall_sa  = calc_sa  totals
    overall_oba = calc_oba totals
    overall_soa = calc_soa totals
    overall_rpg = calc_rpg totals

    dsp_overall_sa  = sprintf( "%5.3f", overall_sa  ).gsub '0.', ' .'
    dsp_overall_oba = sprintf( "%5.3f", overall_oba ).gsub '0.', ' .'
    dsp_overall_soa = sprintf( "%5.3f", overall_soa ).gsub '0.', ' .'

    printf "\e[1m%-15s  %s %4d %4d %4d %4d %3d %3d %4d %4d %3d %4d %4d %3d %s %s %s %4.2f\e[0m\n",
           'TOTAL',
           dsp_overall_avg,
           totals[ :games          ],
           totals[ :at_bats        ],
           totals[ :runs           ],
           totals[ :hits           ],
           totals[ :doubles        ],
           totals[ :triples        ],
           totals[ :home_runs      ],
           totals[ :runs_batted_in ],
           totals[ :steals         ],
           totals[ :walks          ],
           totals[ :strike_outs    ],
           totals[ :errors         ],
           dsp_overall_sa,
           dsp_overall_oba,
           dsp_overall_soa,
           overall_rpg
  else
    printf "\e[1m%-15s  %s %4d %4d %4d %4d %3d %3d %4d %4d %3d %4d %4d\e[0m\n",
           'TOTAL',
           dsp_overall_avg,
           totals[ :games          ],
           totals[ :at_bats        ],
           totals[ :runs           ],
           totals[ :hits           ],
           totals[ :doubles        ],
           totals[ :triples        ],
           totals[ :home_runs      ],
           totals[ :runs_batted_in ],
           totals[ :steals         ],
           totals[ :walks          ],
           totals[ :strike_outs    ]
  end
end

def print_batter_awards( batter )
  accolades = get_player_accolades( batter[:player_id] ).concat get_batter_accolades( batter[:player_id] )

  return if accolades.length == 0

  puts ""
  puts "Awards:"
  puts "Year  Award"

  accolades.sort {|a,b| a[:season] <=> b[:season]}.each do |accolade|
    printf "S%02d   %s\n", accolade[:season], accolade[:accolade_name]
  end
end

def print_pitcher_card( pitcher )
  printf "P #{pitcher[:first_name]} #{pitcher[:last_name]}"

  puts pitcher[:team].nil? ? "" : " - #{pitcher[:team][:name]}"

  print_pitcher_stats pitcher, :regular

  unless @options[:regular_season_only]
    print_pitcher_stats pitcher, :playoff
    print_pitcher_stats pitcher, :allstar
    print_pitcher_awards pitcher
  end
end

def print_batter_card( batter )
  printf "#{Positions::string_value batter[:details][:primary_position]} #{batter[:first_name]} #{batter[:last_name]}"

  puts batter[:team].nil? ? "" : " - #{batter[:team][:name]}"

  print_batter_stats batter, :regular

  unless @options[:regular_season_only]
    print_batter_stats batter, :playoff
    print_batter_stats batter, :allstar
    print_batter_awards batter
  end
end


org = get_organization 1

player = nil

if ARGV.length == 1
  player = get_player @player_id
elsif ARGV.length == 2
  player = get_player_by_name @firstname, @lastname
end

if player.nil?
  raise "Player not found."
end

player[:team] = get_current_team player[:player_id], org[:season]

if player[:player_type] == 1
  player[:details]         = get_pitcher_details player[:player_id]
  player[:stats]           = {}
  player[:stats][:regular] = get_pitcher_stats   player[:player_id]
  player[:stats][:playoff] = get_pitcher_stats   player[:player_id], Phases::Playoff
  player[:stats][:allstar] = get_pitcher_stats   player[:player_id], Phases::Allstar

  print_pitcher_card player
else
  player[:details]         = get_batter_details player[:player_id]
  player[:stats]           = {}
  player[:stats][:regular] = get_batter_stats   player[:player_id]
  player[:stats][:playoff] = get_batter_stats   player[:player_id], Phases::Playoff
  player[:stats][:allstar] = get_batter_stats   player[:player_id], Phases::Allstar

  print_batter_card player
end

