class AbsolutePlayerLeadersFilter
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
    return pitchers.select { |p| p.innings.to_f >= 185.0 }
  end

  def filter_batters( batters )
    return batters.select { |b| b.at_bats >= 300 }
  end

  def filter_closers( closers )
    return closers.select { |p| p.innings.to_f >= 85.0 }
  end
end
