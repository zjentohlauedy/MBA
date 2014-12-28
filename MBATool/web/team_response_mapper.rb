#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

class TeamResponseMapper

  def display_season_phase( season_phase )
    case season_phase
    when 1; return 'Regular'
    when 2; return 'Playoff'
    when 3; return 'Allstar'
    end

    return 'Unknown'
  end

  def map_team_list( teams )
    return nil unless teams.is_a? Array

    result = []

    teams.each do |team|
      return nil unless team.is_a? Hash

      result.push team.select { |k,v| [:team_id, :name].include? k }
    end

    return result
  end

  def map_team_stats( team_stats )
    return nil unless team_stats.is_a? Hash

    team_stats[:season_phase] = display_season_phase team_stats[:season_phase]

    return team_stats
  end
end
