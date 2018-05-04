require          'stats'
require_relative 'MBATool/web/positions'

class StatRankings
  attr_reader :pitchers, :batters

  def initialize( printer, filter, compiler )
    @players  = Array.new
    @printer  = printer
    @filter   = filter
    @compiler = compiler
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

    @compiler.compile_stats @players, object, type
  end

  def select_top_players( players, min=15 )
    top_players = []
    last_player = nil
    i           = 0

    players.each do |player|
      if i < min
        top_players.push player
      elsif player.get_sort_value == last_player.get_sort_value
        top_players.push player
      else
        break
      end

      i += 1
      last_player = player
    end

    return top_players
  end

  def summarize_ties( players, max=20 )
    if players.length > max
      tied = players.select { |p| p.get_sort_value == players[-1].get_sort_value }

      players.reject! { |p| tied.include? p }

      return TieSummary.new tied.length, tied[-1].get_sort_value, tied[-1].avgs, tied[-1].get_sort_key
    end

    return nil
  end

  def print_top_players( stat, dir=:ascending, format='%d' )
    @players.each do |player|
      player.set_sort_key stat
      player.set_direction dir
      player.set_format format
    end

    players = @players.sort

    # CLOSERS DONT GET ENOUGH INNINGS BOO HOO
    if stat != :saves
      players = @filter.apply players, stat
    end

#    players.sort!

    top_players = select_top_players players
    tie_summary = summarize_ties top_players

    last_player = nil
    last_idx    = -1

    top_players.each_with_index do |player, idx|
      tied = (!last_player.nil? and player.get_sort_value == last_player.get_sort_value)

      @printer.print player, format, idx, tied

      last_player = player
      last_idx    = idx
    end

    unless tie_summary.nil?
      @printer.print_tie_message tie_summary, format, last_idx + 1
    end
  end

end

@categories = {
  'pitching'      => {  'label' => "Pitching Leaders",     'type' => 'pitcher',     'class'  => Pitcher,
    'stats'       => [{ 'label' => "Wins",                 'stat' => :wins,         'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Win/Loss Ratio",       'stat' => :wl_ratio,     'format' => '%s',     'direction' => :descending },
                      { 'label' => "Win Percentage",       'stat' => :win_pct,      'format' => '%s',     'direction' => :descending },
                      { 'label' => "Earned Run Average",   'stat' => :era,          'format' => '%5.2f',  'direction' => :ascending  },
                      { 'label' => "vs. Batting Average",  'stat' => :vsba,         'format' => '%s',     'direction' => :ascending  },
                      { 'label' => "Innings Per Game",     'stat' => :inn_per_game, 'format' => '%5.2f',  'direction' => :descending },
                      { 'label' => "Strike Outs",          'stat' => :strike_outs,  'format' => '%4d',    'direction' => :descending },
                      { 'label' => "Strikeouts Per 9 Inn", 'stat' => :so_per_9,     'format' => '%5.2f',  'direction' => :descending },
                      { 'label' => "Walks & Hits Per Inn", 'stat' => :whip,         'format' => '%6.3f',  'direction' => :ascending  },
                      { 'label' => "Home Runs Per 9 Inn",  'stat' => :hr_per_9,     'format' => '%5.2f',  'direction' => :ascending  },
                      { 'label' => "Efficiency",           'stat' => :eff_per_9,    'format' => '%+5.2f', 'direction' => :descending }]},

  'closing '      => {  'label' => "Closing Leaders",      'type' => 'closer',      'class'  => Closer,
    'stats'       => [{ 'label' => "Saves",                'stat' => :saves,        'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Games",                'stat' => :games,        'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Innings Pitched",      'stat' => :innings,      'format' => '%s',     'direction' => :descending },
                      { 'label' => "Earned Run Average",   'stat' => :era,          'format' => '%5.2f',  'direction' => :ascending  },
                      { 'label' => "vs. Batting Average",  'stat' => :vsba,         'format' => '%s',     'direction' => :ascending  },
                      { 'label' => "Strike Outs",          'stat' => :strike_outs,  'format' => '%4d',    'direction' => :descending },
                      { 'label' => "Strikeouts Per 9 Inn", 'stat' => :so_per_9,     'format' => '%5.2f',  'direction' => :descending },
                      { 'label' => "Walks & Hits Per Inn", 'stat' => :whip,         'format' => '%6.3f',  'direction' => :ascending  },
                      { 'label' => "Home Runs Per 9 Inn",  'stat' => :hr_per_9,     'format' => '%5.2f',  'direction' => :ascending  },
                      { 'label' => "Efficiency",           'stat' => :eff_per_9,    'format' => '%+5.2f', 'direction' => :descending }]},

  'batting '      => {  'label' => "Hitting Leaders",      'type' => 'batter',      'class'  => Batter,
    'stats'       => [{ 'label' => "Batting Average",      'stat' => :average,      'format' => '%s',     'direction' => :descending },
                      { 'label' => "Hits",                 'stat' => :hits,         'format' => '%4d',    'direction' => :descending },
                      { 'label' => "Doubles",              'stat' => :doubles,      'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Triples",              'stat' => :triples,      'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Home Runs",            'stat' => :home_runs,    'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Total Bases",          'stat' => :total_bases,  'format' => '%3d',    'direction' => :descending },
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


@team_categories = {
  'records'       => {  'label' => "Record Leaders",       'type' => 'records',             'class'  => Records,
    'stats'       => [{ 'label' => "Wins",                 'stat' => :wins,                 'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Division Wins",        'stat' => :division_wins,        'format' => '%2d',    'direction' => :descending },
                      { 'label' => "League Wins",          'stat' => :league_wins,          'format' => '%2d',    'direction' => :descending },
                      { 'label' => "Home Wins",            'stat' => :home_wins,            'format' => '%2d',    'direction' => :descending },
                      { 'label' => "Road Wins",            'stat' => :road_wins,            'format' => '%2d',    'direction' => :descending },
                      { 'label' => "Runs Scored",          'stat' => :runs_scored,          'format' => '%4d',    'direction' => :descending },
                      { 'label' => "Runs Allowed",         'stat' => :runs_allowed,         'format' => '%4d',    'direction' => :ascending  },
                      { 'label' => "Scoring Differential", 'stat' => :scoring_differential, 'format' => '%4d',    'direction' => :descending }]},

  'pitching'      => {  'label' => "Pitching Leaders",     'type' => 'pitching',            'class'  => Pitching,
    'stats'       => [{ 'label' => "Wins",                 'stat' => :wins,                 'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Win/Loss Ratio",       'stat' => :wl_ratio,             'format' => '%s',     'direction' => :descending },
                      { 'label' => "Win Percentage",       'stat' => :win_pct,              'format' => '%s',     'direction' => :descending },
                      { 'label' => "Earned Run Average",   'stat' => :era,                  'format' => '%5.2f',  'direction' => :ascending  },
                      { 'label' => "vs. Batting Average",  'stat' => :vsba,                 'format' => '%s',     'direction' => :ascending  },
                      { 'label' => "Saves",                'stat' => :saves,                'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Innings Per Game",     'stat' => :inn_per_game,         'format' => '%5.2f',  'direction' => :descending },
                      { 'label' => "Strike Outs",          'stat' => :strike_outs,          'format' => '%4d',    'direction' => :descending },
                      { 'label' => "Strikeouts Per 9 Inn", 'stat' => :so_per_9,             'format' => '%5.2f',  'direction' => :descending },
                      { 'label' => "Walks & Hits Per Inn", 'stat' => :whip,                 'format' => '%6.3f',  'direction' => :ascending  },
                      { 'label' => "Home Runs Per 9 Inn",  'stat' => :hr_per_9,             'format' => '%5.2f',  'direction' => :ascending  },
                      { 'label' => "Efficiency",           'stat' => :eff_per_9,            'format' => '%+5.2f', 'direction' => :descending }]},

  'batting '      => {  'label' => "Hitting Leaders",      'type' => 'batting',             'class'  => Batting,
    'stats'       => [{ 'label' => "Batting Average",      'stat' => :average,              'format' => '%s',     'direction' => :descending },
                      { 'label' => "Doubles",              'stat' => :doubles,              'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Triples",              'stat' => :triples,              'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Home Runs",            'stat' => :home_runs,            'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Runs",                 'stat' => :runs,                 'format' => '%4d',    'direction' => :descending },
                      { 'label' => "Runs Batted In",       'stat' => :rbi,                  'format' => '%4d',    'direction' => :descending },
                      { 'label' => "Stolen Bases",         'stat' => :steals,               'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Walks",                'stat' => :walks,                'format' => '%3d',    'direction' => :descending },
                      { 'label' => "Errors",               'stat' => :errors,               'format' => '%3d',    'direction' => :ascending  },
                      { 'label' => "Slugging Average",     'stat' => :slugging,             'format' => '%s',     'direction' => :descending },
                      { 'label' => "On Base Percentage",   'stat' => :obp,                  'format' => '%s',     'direction' => :descending },
                      { 'label' => "Strike Out Average",   'stat' => :soa,                  'format' => '%s',     'direction' => :ascending  },
                      { 'label' => "Runs Per Game",        'stat' => :rpg,                  'format' => '%4.2f',  'direction' => :descending }]}
}
