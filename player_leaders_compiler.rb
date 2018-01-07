class PlayerLeadersCompiler
  def initialize( org )
    @org = org
  end

  def compile_stats( list, target_class, target_type )
    @org[:leagues].each do |league|
      league[:divisions].each do |division|
        division[:teams].each do |team|
          next if team[:players].nil?

          team[:players].each do |player|
            next if  player[:stats][:simulated].nil?

            if player_qualifies? player, target_type
              list.push( target_class.new team[:name], player, :simulated )
            end
          end
        end
      end
    end
  end

  def player_qualifies?( player, target_type )
    if player[:type] == target_type
      if target_type == 'pitcher' and player[:ratings][:fatigue] == 1
        return false
      end

      return true
    end

    if target_type == 'closer' and player[:type] == 'pitcher' and player[:ratings][:fatigue] == 1
      return true
    end

    return false
  end
end
