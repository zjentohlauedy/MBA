#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'ProgRunner'


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
    sprintf "%-2s %-20s %-15s #{@format}", @pos, @name, @team, get_sort_key
  end

end


class Pitcher < Stats
  attr_reader :name, :team, :wins, :losses, :era, :games, :saves, :innings
  attr_reader :hits, :earned_runs, :home_runs, :walks, :strike_outs, :vsba
  attr_reader :inn_per_game, :hits_per_9, :walks_per_9, :so_per_9, :hr_per_9
  attr_reader :win_pct, :efficiency

  def initialize( str, team )
    fields = str.split(/ +/)

    @name         = "#{fields[1]} #{fields[2]}"
    @team         = team
    @pos          = fields[ 0]
    @wins         = fields[ 3].to_i
    @losses       = fields[ 4].to_i
    @era          = fields[ 5].to_f
    @games        = fields[ 6].to_i
    @saves        = fields[ 7].to_i
    @innings      = fields[ 8].to_f
    @hits         = fields[ 9].to_i
    @earned_runs  = fields[10].to_i
    @home_runs    = fields[11].to_i
    @walks        = fields[12].to_i
    @strike_outs  = fields[13].to_i
    @vsba         = fields[14].to_f
    @inn_per_game = fields[15].to_f
    @hits_per_9   = fields[16].to_f
    @walks_per_9  = fields[17].to_f
    @so_per_9     = fields[17].to_f
    @hr_per_9     = fields[17].to_f
    @win_pct      = @wins.to_f / (@wins + @losses)
    @efficiency   = (@innings.to_i - @hits) + (@strike_outs - @hits)
  end

end

class Batter < Stats
  attr_reader :name, :team, :pos, :average, :games, :at_bats, :runs, :hits
  attr_reader :doubles, :triples, :home_runs, :rbi, :walks, :strike_outs
  attr_reader :steals, :errors, :slugging, :obp, :soa, :rpg

  def initialize( str, team )
    fields = str.split(/ +/)

    @name        = "#{fields[1]} #{fields[2]}"
    @team        = team
    @pos         = fields[ 0]
    @average     = fields[ 3].to_f
    @games       = fields[ 4].to_i
    @at_bats     = fields[ 5].to_i
    @runs        = fields[ 6].to_i
    @hits        = fields[ 7].to_i
    @doubles     = fields[ 8].to_i
    @triples     = fields[ 9].to_i
    @home_runs   = fields[10].to_i
    @rbi         = fields[11].to_i
    @walks       = fields[12].to_i
    @strike_outs = fields[13].to_i
    @steals      = fields[14].to_i
    @errors      = fields[15].to_i
    @slugging    = fields[16].to_f
    @obp         = fields[17].to_f
    @soa         = fields[18].to_f
    @rpg         = fields[19].to_f
  end

end


class StatRankings
  attr_reader :pitchers, :batters

  def initialize( stats_prog )
    @stats_prog = stats_prog
    @pitchers   = Array.new
    @batters    = Array.new
    @collection = :pitchers
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

    @stats_prog.execute 'LeagName.Dat', 'Players.S', 'P'

    if @stats_prog.success?
      team = "No Team"

      @stats_prog.get_output.split( "\n" ).each do |line|
        next if line.length == 0

        if line =~ /^[A-Za-z]+ +[0-9]+ +[-] +[0-9]+$/
          team = line.match(/^[A-Za-z]+/)[0]
          next
        end

        if line =~ /^P /
          @pitchers.push(Pitcher.new line, team )
        end

        if line =~ /^[C123LRS][ BSF]/
          @batters.push( Batter.new line, team )
        end
      end
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
    pitchers.sort! { |a,b| b.innings <=> a.innings }

    max = pitchers[0].innings

    return pitchers.select { |p| p.innings >= (max * 0.4) }
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



rosters_prog = ProgRunner.new location, "print_rosters"

categories = {
  'pitching'      => {  'label' => "Pitching Leaders",    'type' => :pitchers,
    'stats'       => [{ 'label' => "Wins",                'stat' => :wins,         'format' => '%2d',   'direction' => :descending },
                      { 'label' => "Win Percentage",      'stat' => :win_pct,      'format' => '%5.3f', 'direction' => :descending },
                      { 'label' => "Earned Run Average",  'stat' => :era,          'format' => '%5.2f', 'direction' => :ascending  },
                      { 'label' => "vs. Batting Average", 'stat' => :vsba,         'format' => '%5.3f', 'direction' => :ascending  },
                      { 'label' => "Saves",               'stat' => :saves,        'format' => '%3d',   'direction' => :descending },
                      { 'label' => "Strike Outs",         'stat' => :strike_outs,  'format' => '%3d',   'direction' => :descending },
                      { 'label' => "Innings Per Game",    'stat' => :inn_per_game, 'format' => '%5.2f', 'direction' => :descending },
                      { 'label' => "Hits Per 9 Innings",  'stat' => :hits_per_9,   'format' => '%5.2f', 'direction' => :ascending  },
                      { 'label' => "Walks Per 9 Innings", 'stat' => :walks_per_9,  'format' => '%5.2f', 'direction' => :ascending  },
                      { 'label' => "Strikeouts Per 9 Inn",'stat' => :so_per_9,     'format' => '%5.2f', 'direction' => :descending },
                      { 'label' => "Home Runs Per 9 Inn", 'stat' => :hr_per_9,     'format' => '%5.2f', 'direction' => :ascending  },
                      { 'label' => "Efficiency",          'stat' => :efficiency,   'format' => '%+3d',  'direction' => :descending }]},

  'batting '      => {  'label' => "Hitting Leaders",     'type' => :batters,
    'stats'       => [{ 'label' => "Batting Average",     'stat' => :average,      'format' => '%5.3f', 'direction' => :descending },
                      { 'label' => "Doubles",             'stat' => :doubles,      'format' => '%3d',   'direction' => :descending },
                      { 'label' => "Triples",             'stat' => :triples,      'format' => '%3d',   'direction' => :descending },
                      { 'label' => "Home Runs",           'stat' => :home_runs,    'format' => '%3d',   'direction' => :descending },
                      { 'label' => "Runs",                'stat' => :runs,         'format' => '%3d',   'direction' => :descending },
                      { 'label' => "Runs Batted In",      'stat' => :rbi,          'format' => '%3d',   'direction' => :descending },
                      { 'label' => "Stolen Bases",        'stat' => :steals,       'format' => '%3d',   'direction' => :descending },
                      { 'label' => "Walks",               'stat' => :walks,        'format' => '%3d',   'direction' => :descending },
                      { 'label' => "Errors",              'stat' => :errors,       'format' => '%3d',   'direction' => :ascending  },
                      { 'label' => "Slugging Average",    'stat' => :slugging,     'format' => '%5.3f', 'direction' => :descending },
                      { 'label' => "On Base Percentage",  'stat' => :obp,          'format' => '%5.3f', 'direction' => :descending },
                      { 'label' => "Strike Out Average",  'stat' => :soa,          'format' => '%5.3f', 'direction' => :ascending  },
                      { 'label' => "Runs Per Game",       'stat' => :rpg,          'format' => '%4.2f', 'direction' => :descending }]}
}


sr = StatRankings.new rosters_prog

sr.process_categories categories
