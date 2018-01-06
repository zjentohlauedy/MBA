class Stats
  include Comparable

  def initialize
    @direction = :descending
    @format = "%d"
    @sort_key = nil
  end

  def <=>( other )
    if get_sort_value == other.get_sort_value
      if @season == other.season
        return @identifier <=> other.identifier
      end

      return @season <=> other.season
    end

    if @direction == :ascending
      return get_sort_value.to_f <=> other.get_sort_value.to_f
    else
      return other.get_sort_value.to_f <=> get_sort_value.to_f
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
    @sort_key
  end

  def get_sort_value
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


class TieSummary
  attr_reader :count, :value

  def initialize( count, value )
    @count   = count
    @value   = value
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
  attr_reader :name, :team, :wins, :losses, :era, :games, :saves, :innings, :season, :seasons, :pos,
              :hits, :earned_runs, :home_runs, :walks, :strike_outs, :vsba, :win_pct, :avgs, :rookie_season,
              :inn_per_game, :whip, :so_per_9, :hr_per_9, :wl_ratio, :efficiency, :eff_per_9, :identifier

  def initialize( team, pitcher, style=:simulated )
    @name          = "#{pitcher[:last_name]}, #{pitcher[:first_name]}"
    @team          = team
    @rookie_season = pitcher[:rookie_season]
    @season        = pitcher[:stats][style][:season]
    @seasons       = pitcher[:stats][style][:seasons]
    @pos           = 'P'
    @wins          = pitcher[:stats][style][:wins]
    @losses        = pitcher[:stats][style][:losses]
    @games         = pitcher[:stats][style][:games]
    @saves         = pitcher[:stats][style][:saves]
    @hits          = pitcher[:stats][style][:hits]
    @earned_runs   = pitcher[:stats][style][:earned_runs]
    @home_runs     = pitcher[:stats][style][:home_runs]
    @walks         = pitcher[:stats][style][:walks]
    @strike_outs   = pitcher[:stats][style][:strike_outs]
    @identifier    = @name

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
  attr_reader :name, :team, :pos, :average, :games, :at_bats, :runs, :hits, :seasons,
              :doubles, :triples, :home_runs, :rbi, :walks, :strike_outs, :rookie_season,
              :steals, :errors, :slugging, :obp, :soa, :rpg, :season, :avgs, :identifier

  def initialize( team, batter, style=:simulated )
    @name          = "#{batter[:last_name]}, #{batter[:first_name]}"
    @team          = team
    @rookie_season = batter[:rookie_season]
    @season        = batter[:stats][style][:season]
    @seasons       = batter[:stats][style][:seasons]
    @pos           = Positions::string_value batter[:primary_position]
    @games         = batter[:stats][style][:games]
    @at_bats       = batter[:stats][style][:at_bats]
    @runs          = batter[:stats][style][:runs]
    @hits          = batter[:stats][style][:hits]
    @doubles       = batter[:stats][style][:doubles]
    @triples       = batter[:stats][style][:triples]
    @home_runs     = batter[:stats][style][:home_runs]
    @rbi           = batter[:stats][style][:runs_batted_in]
    @walks         = batter[:stats][style][:walks]
    @strike_outs   = batter[:stats][style][:strike_outs]
    @steals        = batter[:stats][style][:steals]
    @errors        = batter[:stats][style][:errors]
    @identifier    = @name

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

class Records < Stats
  attr_reader :name, :wins, :home_wins, :road_wins, :division_wins, :league_wins,
              :runs_scored, :runs_allowed, :scoring_differential, :season, :avgs,
              :team, :identifier

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
    @identifier      = @name || @team

    @scoring_differential = team[:stats][:runs_scored] - team[:stats][:runs_allowed]

    @avgs = []
  end
end

class Pitching < Stats
  attr_reader :name, :wins, :losses, :era, :games, :saves, :innings, :season, :avgs, :team,
              :hits, :earned_runs, :home_runs, :walks, :strike_outs, :vsba, :win_pct, :identifier,
              :inn_per_game, :whip, :so_per_9, :hr_per_9, :wl_ratio, :efficiency, :eff_per_9

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
    @identifier   = @name || @team

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
  attr_reader :name, :team, :pos, :average, :games, :at_bats, :runs, :hits, :team,
              :doubles, :triples, :home_runs, :rbi, :walks, :strike_outs, :identifier,
              :steals, :errors, :slugging, :obp, :soa, :rpg, :season, :avgs

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
    @identifier  = @name || @team

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
