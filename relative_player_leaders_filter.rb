class RelativePlayerLeadersFilter
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
    max = pitchers.sort { |a,b| b.innings.to_f <=> a.innings.to_f }[0].innings.to_f

    return pitchers.select { |p| p.innings.to_f >= (max * 0.4) }
  end

  def filter_batters( batters )
    max = batters.sort { |a,b| b.at_bats <=> a.at_bats }[0].at_bats

    return batters.select { |b| b.at_bats >= (max * 0.4) }
  end

  def filter_closers( closers )
    max = closers.sort { |a,b| b.innings.to_f <=> a.innings.to_f }[0].innings.to_f

    return closers.select { |p| p.innings.to_f >= (max * 0.4) }
  end
end
