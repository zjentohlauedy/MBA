#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'sqlite3'


class Stats
  include Comparable

  def initialize
    @direction = :descending
    @format = "%d"
    @sort_key = nil
  end

  def <=>( other )
    if @direction == :ascending
      get_sort_key <=> other.get_sort_key
    else
      other.get_sort_key <=> get_sort_key
    end
  end

  def set_direction( direction )
    @direction = direction
  end

  def set_format( format )
    @format = format
  end

  def set_sort_key( key )
    @sort_key = key
  end

  def get_sort_key
    send @sort_key
  end

  def to_s
    sprintf "%-2s %-20s #{@format}", @pos, @name, get_sort_key
  end

end


class Pitcher < Stats
  attr_reader :name, :wins, :losses, :era, :games, :saves, :innings
  attr_reader :hits, :earned_runs, :home_runs, :walks, :strike_outs, :vsba
  attr_reader :inn_per_game, :whip, :so_per_9, :hr_per_9, :win_pct, :efficiency, :eff_per_9

  def initialize( pitcher )
    adj_innings = pitcher[:stats][:innings] + (pitcher[:stats][:outs] / 3)
    adj_outs    = pitcher[:stats][:outs] % 3
    est_at_bats = (adj_innings * 3) + adj_outs + pitcher[:stats][:hits] + pitcher[:stats][:walks]

    f_innings = adj_innings.to_f + (adj_outs.to_f / 3.0)

    @name         = "#{pitcher[:last_name]}, #{pitcher[:first_name]}"
    @pos          = 'P'
    @wins         = pitcher[:stats][:wins]
    @losses       = pitcher[:stats][:losses]
    @games        = pitcher[:stats][:games]
    @saves        = pitcher[:stats][:saves]
    @innings      = "#{adj_innings}.#{adj_outs}"
    @hits         = pitcher[:stats][:hits]
    @earned_runs  = pitcher[:stats][:earned_runs]
    @home_runs    = pitcher[:stats][:home_runs]
    @walks        = pitcher[:stats][:walks]
    @strike_outs  = pitcher[:stats][:strike_outs]
    @era          = (f_innings > 0) ? pitcher[:stats][:earned_runs].to_f / f_innings * 9.0 : 0
    @vsba         = (est_at_bats > 0) ? pitcher[:stats][:hits].to_f / est_at_bats.to_f : 0
    @inn_per_game = (pitcher[:stats][:games] > 0) ? f_innings / pitcher[:stats][:games].to_f : 0
    @whip         = (f_innings > 0) ? (pitcher[:stats][:walks].to_f + pitcher[:stats][:hits].to_f) / f_innings : 0
    @so_per_9     = (f_innings > 0) ? pitcher[:stats][:strike_outs].to_f / f_innings * 9.0 : 0
    @hr_per_9     = (f_innings > 0) ? pitcher[:stats][:home_runs].to_f / f_innings * 9.0 : 0
    @win_pct      = (@wins + @losses) > 0 ? @wins.to_f / (@wins + @losses) : 0
    @efficiency   = (adj_innings - @hits) + (@strike_outs - @hits)
    @eff_per_9    = (f_innings > 0) ? @efficiency.to_f / f_innings * 9.0 : 0
  end

end

class Batter < Stats
  attr_reader :name, :team, :pos, :average, :games, :at_bats, :runs, :hits
  attr_reader :doubles, :triples, :home_runs, :rbi, :walks, :strike_outs
  attr_reader :steals, :errors, :slugging, :obp, :soa, :rpg

  def initialize( batter )
    appearances = batter[:stats][:at_bats] + batter[:stats][:walks]
    singles     = batter[:stats][:hits] - (batter[:stats][:doubles] + batter[:stats][:triples] + batter[:stats][:home_runs])
    bases       = singles + (batter[:stats][:doubles] * 2) + (batter[:stats][:triples] * 3) + (batter[:stats][:home_runs] * 4)
    production  = batter[:stats][:runs] + batter[:stats][:runs_batted_in] - batter[:stats][:home_runs]

    @name        = "#{batter[:last_name]}, #{batter[:first_name]}"
    @pos         = 'B'
    @games       = batter[:stats][:games]
    @at_bats     = batter[:stats][:at_bats]
    @runs        = batter[:stats][:runs]
    @hits        = batter[:stats][:hits]
    @doubles     = batter[:stats][:doubles]
    @triples     = batter[:stats][:triples]
    @home_runs   = batter[:stats][:home_runs]
    @rbi         = batter[:stats][:runs_batted_in]
    @walks       = batter[:stats][:walks]
    @strike_outs = batter[:stats][:strike_outs]
    @steals      = batter[:stats][:steals]
    @errors      = batter[:stats][:errors]
    @average     = (batter[:stats][:at_bats] > 0) ? batter[:stats][:hits].to_f / batter[:stats][:at_bats].to_f : 0
    @slugging    = (batter[:stats][:at_bats] > 0) ? bases.to_f / batter[:stats][:at_bats].to_f : 0
    @obp         = (batter[:stats][:at_bats] > 0) ? (batter[:stats][:hits].to_f + batter[:stats][:walks].to_f) / appearances.to_f : 0
    @soa         = (batter[:stats][:at_bats] > 0) ? batter[:stats][:strike_outs].to_f / batter[:stats][:at_bats].to_f : 0
    @rpg         = (batter[:stats][:games] > 0) ? production / batter[:stats][:games].to_f : 0
  end

end


class StatRankings
  attr_reader :pitchers, :batters

  def initialize( pitchers, batters )
    @pitchers_in = pitchers
    @pitchers    = Array.new
    @batters_in  = batters
    @batters     = Array.new
    @collection  = :pitchers
  end

  def get_collection
    send @collection
  end

  def process_categories( categories )
    compile_stats
    categories.each do |key, value|
      @collection = value.fetch( 'type' )
      print "#{value.fetch 'label'}:\n"
      print "\n"
      value.fetch( 'stats' ).each do |stat|
        print "#{stat.fetch 'label'}\n"
        print_top_players stat.fetch( 'stat' ), stat.fetch( 'direction' ), stat.fetch( 'format' )
        print "\n"
      end
    end
  end

  def compile_stats
    @pitchers   = Array.new
    @batters    = Array.new

    @pitchers_in.each do |pitcher|
      @pitchers.push( Pitcher.new pitcher )
    end

    @batters_in.each do |batter|
      @batters.push( Batter.new batter )
    end
  end

  def filter_players( players )
    if @collection == :pitchers
      return filter_pitchers players
    else
      return filter_batters players
    end
  end

  def filter_pitchers( pitchers )
    pitchers.sort! { |a,b| b.innings.to_f <=> a.innings.to_f }

    max = pitchers[0].innings.to_f

    return pitchers.select { |p| p.innings.to_f >= (max * 0.4) }
  end

  def filter_batters( batters )
    batters.sort! { |a,b| b.at_bats <=> a.at_bats }

    max = batters[0].at_bats

    return batters.select { |b| b.at_bats >= (max * 0.4) }
  end

  def print_top_players( stat, dir=:ascending, format='%d', count=15 )
    # CLOSERS DONT GET ENOUGH INNINGS BOO HOO
    if stat == :saves
      players = get_collection
    else
      players = filter_players get_collection
    end

    players.each do |player|
      player.set_sort_key stat
      player.set_direction dir
      player.set_format format
    end

    players.sort!

    i = 0

    while i < [count, players.length].min
      print "#{players[i].to_s}\n"
      i += 1
    end
  end

end

categories = {
  'pitching'      => {  'label' => "Pitching Leaders",     'type' => :pitchers,
    'stats'       => [{ 'label' => "Wins",                 'stat' => :wins,         'format' => '%2d',    'direction' => :descending },
                      { 'label' => "Win Percentage",       'stat' => :win_pct,      'format' => '%5.3f',  'direction' => :descending },
                      { 'label' => "Earned Run Average",   'stat' => :era,          'format' => '%5.2f',  'direction' => :ascending  },
                      { 'label' => "vs. Batting Average",  'stat' => :vsba,         'format' => '%5.3f',  'direction' => :ascending  },
                      { 'label' => "Saves",                'stat' => :saves,        'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Strike Outs",          'stat' => :strike_outs,  'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Innings Per Game",     'stat' => :inn_per_game, 'format' => '%5.2f',  'direction' => :descending },
                      { 'label' => "Walks & Hits Per Inn", 'stat' => :whip,         'format' => '%5.2f',  'direction' => :ascending  },
                      { 'label' => "Strikeouts Per 9 Inn", 'stat' => :so_per_9,     'format' => '%5.2f',  'direction' => :descending },
                      { 'label' => "Home Runs Per 9 Inn",  'stat' => :hr_per_9,     'format' => '%5.2f',  'direction' => :ascending  },
                      { 'label' => "Efficiency",           'stat' => :efficiency,   'format' => '%+3d',   'direction' => :descending },
                      { 'label' => "Efficiency Per 9 Inn", 'stat' => :eff_per_9,    'format' => '%+5.2f', 'direction' => :descending }]},

  'batting '      => {  'label' => "Hitting Leaders",      'type' => :batters,
    'stats'       => [{ 'label' => "Batting Average",      'stat' => :average,      'format' => '%5.3f',  'direction' => :descending },
                      { 'label' => "Doubles",              'stat' => :doubles,      'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Triples",              'stat' => :triples,      'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Home Runs",            'stat' => :home_runs,    'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Runs",                 'stat' => :runs,         'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Runs Batted In",       'stat' => :rbi,          'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Stolen Bases",         'stat' => :steals,       'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Walks",                'stat' => :walks,        'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Errors",               'stat' => :errors,       'format' => '%3d',    'direction' => :ascending  },
                      { 'label' => "Slugging Average",     'stat' => :slugging,     'format' => '%5.3f',  'direction' => :descending },
                      { 'label' => "On Base Percentage",   'stat' => :obp,          'format' => '%5.3f',  'direction' => :descending },
                      { 'label' => "Strike Out Average",   'stat' => :soa,          'format' => '%5.3f',  'direction' => :ascending  },
                      { 'label' => "Runs Per Game",        'stat' => :rpg,          'format' => '%4.2f',  'direction' => :descending }]}
}


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


def get_pitchers
  transform_hash @db.execute "select * from players_t where player_type = 1"
end


def get_pitcher_stats_by_player_id( player_id )
  result = transform_hash @db.execute "select player_id, count(1) seasons, sum(wins) wins, sum(losses) losses, sum(games) games, sum(saves) saves, sum(innings) innings, sum(outs) outs, sum(hits) hits, sum(earned_runs) earned_runs, sum(home_runs) home_runs, sum(walks) walks, sum(strike_outs) strike_outs from pitcher_stats_t where player_id = #{player_id} and season_phase = 1 group by player_id"
  result[0]
end


def get_batters
  transform_hash @db.execute "select * from players_t where player_type = 2"
end


def get_batter_stats_by_player_id( player_id )
  result = transform_hash @db.execute "select player_id, count(1) seasons, sum(games) games, sum(at_bats) at_bats, sum(runs) runs, sum(hits) hits, sum(doubles) doubles, sum(triples) triples, sum(home_runs) home_runs, sum(runs_batted_in) runs_batted_in, sum(walks) walks, sum(strike_outs) strike_outs, sum(steals) steals, sum(errors) errors from batter_stats_t where player_id = #{player_id} and season_phase = 1 group by player_id"
  result[0]
end


pitchers = get_pitchers

pitchers.each do |pitcher|
  pitcher[:stats] = get_pitcher_stats_by_player_id pitcher[:player_id]
end

pitchers.reject! { |pitcher| pitcher[:stats].nil? }

batters = get_batters

batters.each do |batter|
  batter[:stats] = get_batter_stats_by_player_id batter[:player_id]
end

batters.reject! { |batter| batter[:stats].nil? }

sr = StatRankings.new pitchers, batters

sr.process_categories categories
