#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

require 'accolades'

class TeamResponseMapper

  def display_accolade( type, value )
    result = Accolades::get_accolade_name type, value

    if result.nil?
      raise InternalServerError.new "Invalid accolade of type #{type} with value #{value}."
    end

    return result
  end

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

  def map_team_accolade( team_accolade )
    return nil unless team_accolade.is_a? Hash

    team_accolade[:accolade] = display_accolade 'team', team_accolade[:accolade]

    return team_accolade
  end
end
