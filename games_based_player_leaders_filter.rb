class GamesBasedPlayerLeadersFilter
  def initialize( games )
    @min_innings = 185 * games / 152
    @min_at_bats = 300 * games / 152
    @min_inn_cls =  85 * games / 152
  end

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
    return pitchers.select { |p| p.innings.to_f >= @min_innings.to_f }
  end

  def filter_batters( batters )
    return batters.select { |b| b.at_bats >= @min_at_bats }
  end

  def filter_closers( closers )
    return closers.select { |p| p.innings.to_f >= @min_inn_cls.to_f }
  end
end
