#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"

class Decorator

  def initialize( repository_root )
    @repository_root = repository_root
  end

  def decorate_team( team )
    links = []

    links << { :rel => 'self',    :href => "#{@repository_root}/teams/#{team[:team_id]}"         }
    links << { :rel => 'players', :href => "#{@repository_root}/teams/#{team[:team_id]}/players" }
    links << { :rel => 'stats',   :href => "#{@repository_root}/teams/#{team[:team_id]}/stats"   }

    team[:links] = links

    return team
  end

  def decorate_team_stats( team_stats )
    links = []

    links << { :rel => 'self', :href => "#{@repository_root}/teams/#{team_stats[:team_id]}/stats?season=#{team_stats[:season]}&phase=#{team_stats[:season_phase]}" }

    team_stats[:links] = links

    return team_stats
  end

  def decorate_player( player )
    links = []

    links << { :rel => 'self',  :href => "#{@repository_root}/players/#{player[:player_id]}"       }
    links << { :rel => 'stats', :href => "#{@repository_root}/players/#{player[:player_id]}/stats" }

    player[:links] = links

    return player
  end

  def decorate_pitcher_stats( pitcher_stats )
    links = []

    links << { :rel => 'self',  :href => "#{@repository_root}/players/#{pitcher_stats[:player_id]}/stats?season=#{pitcher_stats[:season]}&phase=#{pitcher_stats[:season_phase]}" }

    pitcher_stats[:links] = links

    return pitcher_stats
  end

  def decorate_batter_stats( batter_stats )
    links = []

    links << { :rel => 'self',  :href => "#{@repository_root}/players/#{batter_stats[:player_id]}/stats?season=#{batter_stats[:season]}&phase=#{batter_stats[:season_phase]}" }

    batter_stats[:links] = links

    return batter_stats
  end

end
