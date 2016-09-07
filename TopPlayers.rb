require_relative 'MBATool/web/positions'

class Stats
  include Comparable

  def initialize
    @direction = :descending
    @format = "%d"
    @sort_key = nil
  end

  def <=>( other )
    if get_sort_key == other.get_sort_key
      return @name <=> other.name
    end

    if @direction == :ascending
      return get_sort_key <=> other.get_sort_key
    else
      return other.get_sort_key <=> get_sort_key
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

  def has_season?
    @season.nil? ? false : true
  end

  def to_s
    sort_key = get_sort_key
    sort_key = @avgs.include?(@sort_key) ? display_avg(sort_key) : sort_key

    if @season.nil?
      sprintf "%-2s %-20s %-15s #{@format}", @pos, @name, @team, sort_key
    else
      sprintf "%-2s %-20s S%02d  %-15s #{@format}", @pos, @name, @season, @team, sort_key
    end
  end

  def display_avg(average)
    avg = sprintf "%5.3f", average
    avg.gsub /^0\./, ' .'
  end

end


class TieMessage

  def initialize( count, value, format, padding="" )
    @count   = count
    @value   = value
    @format  = format
    @padding = padding
  end

  def get_sort_key
    return nil
  end

  def to_s
    sprintf "   %-35s #{@padding} #{@format}", "#{@count} Players Tied At", @value
  end
end


class Pitcher < Stats
  attr_reader :name, :wins, :losses, :era, :games, :saves, :innings
  attr_reader :hits, :earned_runs, :home_runs, :walks, :strike_outs, :vsba, :win_pct
  attr_reader :inn_per_game, :whip, :so_per_9, :hr_per_9, :wl_ratio, :efficiency, :eff_per_9

  def initialize( team, pitcher, style=:simulated )
    @name         = "#{pitcher[:last_name]}, #{pitcher[:first_name]}"
    @team         = team
    @season       = pitcher[:stats][style][:season]
    @pos          = 'P'
    @wins         = pitcher[:stats][style][:wins]
    @losses       = pitcher[:stats][style][:losses]
    @games        = pitcher[:stats][style][:games]
    @saves        = pitcher[:stats][style][:saves]
    @hits         = pitcher[:stats][style][:hits]
    @earned_runs  = pitcher[:stats][style][:earned_runs]
    @home_runs    = pitcher[:stats][style][:home_runs]
    @walks        = pitcher[:stats][style][:walks]
    @strike_outs  = pitcher[:stats][style][:strike_outs]

    adj_innings = pitcher[:stats][style][:innings] + (pitcher[:stats][style][:outs] / 3)
    adj_outs    = pitcher[:stats][style][:outs] % 3
    est_at_bats = (adj_innings * 3) + adj_outs + @hits

    f_innings = adj_innings.to_f + (adj_outs.to_f / 3.0)

    @innings      = "#{adj_innings}.#{adj_outs}"
    @wl_ratio     = (@wins + @losses) > 0 ? @wins.to_f / (@wins + @losses) : 0
    @win_pct      = (@games > 0) ? @wins.to_f / @games : 0
    @era          = (f_innings > 0) ? @earned_runs.to_f / f_innings * 9.0 : 0
    @vsba         = (est_at_bats > 0) ? @hits.to_f / est_at_bats.to_f : 0
    @inn_per_game = (@games > 0) ? f_innings / @games.to_f : 0
    @whip         = (f_innings > 0) ? (@walks.to_f + @hits.to_f) / f_innings : 0
    @so_per_9     = (f_innings > 0) ? @strike_outs.to_f / f_innings * 9.0 : 0
    @hr_per_9     = (f_innings > 0) ? @home_runs.to_f / f_innings * 9.0 : 0
    @efficiency   = (adj_innings - @hits) + (@strike_outs - @hits)
    @eff_per_9    = (f_innings > 0) ? @efficiency.to_f / f_innings * 9.0 : 0

    @avgs = [:wl_ratio, :win_pct, :vsba]
  end

end

class Batter < Stats
  attr_reader :name, :team, :pos, :average, :games, :at_bats, :runs, :hits
  attr_reader :doubles, :triples, :home_runs, :rbi, :walks, :strike_outs
  attr_reader :steals, :errors, :slugging, :obp, :soa, :rpg

  def initialize( team, batter, style=:simulated )
    @name        = "#{batter[:last_name]}, #{batter[:first_name]}"
    @team        = team
    @season      = batter[:stats][style][:season]
    @pos         = Positions::string_value batter[:primary_position]
    @games       = batter[:stats][style][:games]
    @at_bats     = batter[:stats][style][:at_bats]
    @runs        = batter[:stats][style][:runs]
    @hits        = batter[:stats][style][:hits]
    @doubles     = batter[:stats][style][:doubles]
    @triples     = batter[:stats][style][:triples]
    @home_runs   = batter[:stats][style][:home_runs]
    @rbi         = batter[:stats][style][:runs_batted_in]
    @walks       = batter[:stats][style][:walks]
    @strike_outs = batter[:stats][style][:strike_outs]
    @steals      = batter[:stats][style][:steals]
    @errors      = batter[:stats][style][:errors]

    appearances = @at_bats + @walks
    singles     = @hits - (@doubles + @triples + @home_runs)
    bases       = singles + (@doubles * 2) + (@triples * 3) + (@home_runs * 4)
    production  = @runs + @rbi - @home_runs

    @average     = (@at_bats > 0) ? @hits.to_f / @at_bats.to_f : 0
    @slugging    = (@at_bats > 0) ? bases.to_f / @at_bats.to_f : 0
    @obp         = (@at_bats > 0) ? (@hits.to_f + @walks.to_f) / appearances.to_f : 0
    @soa         = (@at_bats > 0) ? @strike_outs.to_f / @at_bats.to_f : 0
    @rpg         = (@games > 0) ? production / @games.to_f : 0

    @avgs = [:average, :obp, :soa, :slugging]
  end

end


class StatRankings
  attr_reader :pitchers, :batters

  def initialize( org, style=:simulated, absolute=false )
    @org      = org
    @style    = style
    @absolute = absolute
    @players  = Array.new
  end

  def process_categories( categories )
    categories.each do |key, value|
      compile_stats value.fetch( 'class' ), value.fetch( 'type' )
      print "#{value.fetch 'label'}:\n"
      print "\n"
      value.fetch( 'stats' ).each do |stat|
        print "#{stat.fetch 'label'}\n"
        print_top_players stat.fetch( 'stat' ), stat.fetch( 'direction' ), stat.fetch( 'format' )
        print "\n"
      end
    end
  end

  def compile_stats( object, type )
    @players = Array.new

    @org[:leagues].each do |league|
      league[:divisions].each do |division|
        division[:teams].each do |team|
          next if team[:players].nil?

          team[:players].each do |player|
            next if  player[:stats][@style].nil?

            if player[:type] == type
              @players.push( object.new team[:name], player, @style )
            end
          end
        end
      end
    end
  end

  def filter_players( players )
    if players[0].class == Pitcher
      return filter_pitchers players
    elsif players[0].class == Batter
      return filter_batters players
    end

    return players
  end

  def filter_pitchers( pitchers )
    if @absolute
      return pitchers.select { |p| p.innings.to_f >= 185.0 }
    end

    pitchers.sort! { |a,b| b.innings.to_f <=> a.innings.to_f }

    max = pitchers[0].innings.to_f

    return pitchers.select { |p| p.innings.to_f >= (max * 0.4) }
  end

  def filter_batters( batters )
    if @absolute
      return batters.select { |b| b.at_bats >= 300 }
    end

    batters.sort! { |a,b| b.at_bats <=> a.at_bats }

    max = batters[0].at_bats

    return batters.select { |b| b.at_bats >= (max * 0.4) }
  end

  def select_top_players( players, min=15 )
    top_players = []
    last_player = nil
    i           = 0

    players.each do |player|
      if i < min
        top_players.push player
      elsif player.get_sort_key == last_player.get_sort_key
        top_players.push player
      else
        break
      end

      i += 1
      last_player = player
    end

    return top_players
  end

  def summarize_ties( players, format, max=20 )
    if players.length > max
      tied = players.select { |p| p.get_sort_key == players[-1].get_sort_key }

      players = players.reject { |p| tied.include? p }

      players.push TieMessage.new tied.length, tied[-1].get_sort_key, format, tied[-1].has_season? ? "     " : ""
    end

    return players
  end

  def print_top_players( stat, dir=:ascending, format='%d' )
    # CLOSERS DONT GET ENOUGH INNINGS BOO HOO
    if stat == :saves
      players = @players
    else
      players = filter_players @players
    end

    players.each do |player|
      player.set_sort_key stat
      player.set_direction dir
      player.set_format format
    end

    players.sort!

    top_players = select_top_players players

    top_players = summarize_ties top_players, format

    last_player = nil

    top_players.each_with_index do |player, idx|
      if last_player.nil? or player.get_sort_key != last_player.get_sort_key
        printf "%2d. %s\n", idx + 1, player.to_s
      else
        printf " -  %s\n", player.to_s
      end

      last_player = player
    end
  end

end

@categories = {
  'pitching'      => {  'label' => "Pitching Leaders",     'type' => 'pitcher',     'class'  => Pitcher,
    'stats'       => [{ 'label' => "Wins",                 'stat' => :wins,         'format' => '%2d',    'direction' => :descending },
                      { 'label' => "Win/Loss Ratio",       'stat' => :wl_ratio,     'format' => '%s',     'direction' => :descending },
                      { 'label' => "Win Percentage",       'stat' => :win_pct,      'format' => '%s',     'direction' => :descending },
                      { 'label' => "Earned Run Average",   'stat' => :era,          'format' => '%5.2f',  'direction' => :ascending  },
                      { 'label' => "vs. Batting Average",  'stat' => :vsba,         'format' => '%s',     'direction' => :ascending  },
                      { 'label' => "Saves",                'stat' => :saves,        'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Innings Per Game",     'stat' => :inn_per_game, 'format' => '%5.2f',  'direction' => :descending },
                      { 'label' => "Strike Outs",          'stat' => :strike_outs,  'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Strikeouts Per 9 Inn", 'stat' => :so_per_9,     'format' => '%5.2f',  'direction' => :descending },
                      { 'label' => "Walks & Hits Per Inn", 'stat' => :whip,         'format' => '%6.3f',  'direction' => :ascending  },
                      { 'label' => "Home Runs Per 9 Inn",  'stat' => :hr_per_9,     'format' => '%5.2f',  'direction' => :ascending  },
                      { 'label' => "Efficiency",           'stat' => :eff_per_9,    'format' => '%+5.2f', 'direction' => :descending }]},

  'batting '      => {  'label' => "Hitting Leaders",      'type' => 'batter',      'class'  => Batter,
    'stats'       => [{ 'label' => "Batting Average",      'stat' => :average,      'format' => '%s',     'direction' => :descending },
                      { 'label' => "Doubles",              'stat' => :doubles,      'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Triples",              'stat' => :triples,      'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Home Runs",            'stat' => :home_runs,    'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Runs",                 'stat' => :runs,         'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Runs Batted In",       'stat' => :rbi,          'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Stolen Bases",         'stat' => :steals,       'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Walks",                'stat' => :walks,        'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Errors",               'stat' => :errors,       'format' => '%3d',    'direction' => :ascending  },
                      { 'label' => "Slugging Average",     'stat' => :slugging,     'format' => '%s',     'direction' => :descending },
                      { 'label' => "On Base Percentage",   'stat' => :obp,          'format' => '%s',     'direction' => :descending },
                      { 'label' => "Strike Out Average",   'stat' => :soa,          'format' => '%s',     'direction' => :ascending  },
                      { 'label' => "Runs Per Game",        'stat' => :rpg,          'format' => '%4.2f',  'direction' => :descending }]}
}
