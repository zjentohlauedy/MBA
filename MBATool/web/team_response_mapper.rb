#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

class TeamResponseMapper

  def map_team_list( teams )
    return nil unless teams.is_a? Array

    result = []

    teams.each do |team|
      return nil unless team.is_a? Hash

      result.push team.select { |k,v| [:team_id, :name].include? k }
    end

    return result
  end

end
