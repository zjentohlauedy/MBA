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
      if @season == other.season
        return @name <=> other.name
      end

      return @season <=> other.season
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
      sprintf "%-20s %-15s #{@format}", @name, @team, sort_key
    else
      sprintf "%-20s %-15s S%02d  #{@format}", @name, @team, @season, sort_key
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
    sprintf "   %-32s #{@padding} #{@format}", "#{@count} Teams Tied At", @value
  end
end


class Records < Stats
  attr_reader :name, :wins, :home_wins, :road_wins, :division_wins, :league_wins, :runs_scored, :runs_allowed, :scoring_differential, :season

  def initialize( team )
    @name            = team[:location]
    @team            = team[:name]
    @season          = team[:stats][:season         ]
    @wins            = team[:stats][:wins           ]
    @losses          = team[:stats][:losses         ]
    @home_wins       = team[:stats][:home_wins      ]
    @home_losses     = team[:stats][:home_losses    ]
    @road_wins       = team[:stats][:road_wins      ]
    @road_losses     = team[:stats][:road_losses    ]
    @division_wins   = team[:stats][:division_wins  ]
    @division_losses = team[:stats][:division_losses]
    @league_wins     = team[:stats][:league_wins    ]
    @league_losses   = team[:stats][:league_losses  ]
    @runs_scored     = team[:stats][:runs_scored    ]
    @runs_allowed    = team[:stats][:runs_allowed   ]

    @scoring_differential = team[:stats][:runs_scored] - team[:stats][:runs_allowed]

    @avgs = []
  end
end

class Pitching < Stats
  attr_reader :name, :wins, :losses, :era, :games, :saves, :innings, :season
  attr_reader :hits, :earned_runs, :home_runs, :walks, :strike_outs, :vsba, :win_pct
  attr_reader :inn_per_game, :whip, :so_per_9, :hr_per_9, :wl_ratio, :efficiency, :eff_per_9

  def initialize( team, style=:simulated )
    @name         = team[:location]
    @team         = team[:name]
    @season       = team[:pitching_stats][style][:season]
    @wins         = team[:pitching_stats][style][:wins]
    @losses       = team[:pitching_stats][style][:losses]
    @games        = team[:pitching_stats][style][:games]
    @saves        = team[:pitching_stats][style][:saves]
    @hits         = team[:pitching_stats][style][:hits]
    @earned_runs  = team[:pitching_stats][style][:earned_runs]
    @home_runs    = team[:pitching_stats][style][:home_runs]
    @walks        = team[:pitching_stats][style][:walks]
    @strike_outs  = team[:pitching_stats][style][:strike_outs]

    adj_innings = team[:pitching_stats][style][:innings] + (team[:pitching_stats][style][:outs] / 3)
    adj_outs    = team[:pitching_stats][style][:outs] % 3
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

class Batting < Stats
  attr_reader :name, :team, :pos, :average, :games, :at_bats, :runs, :hits
  attr_reader :doubles, :triples, :home_runs, :rbi, :walks, :strike_outs
  attr_reader :steals, :errors, :slugging, :obp, :soa, :rpg, :season

  def initialize( team, style=:simulated )
    @name        = team[:location]
    @team        = team[:name]
    @season      = team[:batting_stats][style][:season]
    @games       = team[:batting_stats][style][:games]
    @at_bats     = team[:batting_stats][style][:at_bats]
    @runs        = team[:batting_stats][style][:runs]
    @hits        = team[:batting_stats][style][:hits]
    @doubles     = team[:batting_stats][style][:doubles]
    @triples     = team[:batting_stats][style][:triples]
    @home_runs   = team[:batting_stats][style][:home_runs]
    @rbi         = team[:batting_stats][style][:runs_batted_in]
    @walks       = team[:batting_stats][style][:walks]
    @strike_outs = team[:batting_stats][style][:strike_outs]
    @steals      = team[:batting_stats][style][:steals]
    @errors      = team[:batting_stats][style][:errors]

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
    @teams    = Array.new
  end

  def process_categories( categories )
    categories.each do |key, value|
      compile_stats value.fetch( 'class' ), value.fetch( 'type' )
      print "#{value.fetch 'label'}:\n"
      print "\n"
      value.fetch( 'stats' ).each do |stat|
        print "#{stat.fetch 'label'}\n"
        print_top_teams stat.fetch( 'stat' ), stat.fetch( 'direction' ), stat.fetch( 'format' )
        print "\n"
      end
    end
  end

  def compile_stats( object, type )
    @teams = Array.new

    @org[:leagues].each do |league|
      league[:divisions].each do |division|
        division[:teams].each do |team|
          if    type == 'records'  and !team[:stats].nil?
            @teams.push( object.new team )
          elsif type == 'pitching' and !team[:pitching_stats][@style].nil?
            @teams.push( object.new team, @style )
          elsif type == 'batting'  and !team[:batting_stats][@style].nil?
            @teams.push( object.new team, @style )
          end
        end
      end
    end
  end

  def select_top_teams( teams, min=15 )
    top_teams = []
    last_team = nil
    i         = 0

    teams.each do |team|
      if i < min
        top_teams.push team
      elsif team.get_sort_key == last_team.get_sort_key
        top_teams.push team
      else
        break
      end

      i += 1
      last_team = team
    end

    return top_teams
  end

  def summarize_ties( teams, format, max=20 )
    if teams.length > max
      tied = teams.select { |p| p.get_sort_key == teams[-1].get_sort_key }

      teams = teams.reject { |p| tied.include? p }

      teams.push TieMessage.new tied.length, tied[-1].get_sort_key, format, tied[-1].has_season? ? "     " : ""
    end

    return teams
  end

  def print_top_teams( stat, dir=:ascending, format='%d' )
    @teams.each do |team|
      team.set_sort_key stat
      team.set_direction dir
      team.set_format format
    end

    @teams.sort!

    if stat == :errors
      # started recording errors in S03
      top_teams = select_top_teams( @teams.select {|team| team.season.nil? || team.season >= 3} )
    else
      top_teams = select_top_teams @teams
    end

    if top_teams.size == 0
      puts " - No Teams Qualified"
      return
    end

    top_teams = summarize_ties top_teams, format

    last_team = nil

    top_teams.each_with_index do |team, idx|
      if last_team.nil? or team.get_sort_key != last_team.get_sort_key
        printf "%2d. %s\n", idx + 1, team.to_s
      else
        printf " -  %s\n", team.to_s
      end

      last_team = team
    end
  end

end

@categories = {
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
