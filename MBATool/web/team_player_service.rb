#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

require 'internal_server_error'
require 'resource_not_found_error'

class TeamPlayerService

  def initialize( repository, decorator )
    @repository = repository
    @decorator  = decorator
  end

  def get_team_player( team_id, season, player_id )
    team_player = @repository.get_team_player team_id, season, player_id

    if team_player.nil?
      raise ResourceNotFoundError.new "Team Player with Team ID #{team_id}, Player ID #{player_id} and Season #{season} cannot be located."
    end

    return @decorator.decorate_team_player team_player
  end

  def save_team_player( team_id, season, player_id )
    @repository.save_team_player team_id, season, player_id

    begin
      return get_team_player team_id, season, player_id
    rescue ResourceNotFoundError => e
      raise InternalServerError.new, "Team Player with Team ID #{team_id}, Player ID #{player_id} and Season #{season} was not saved successfully."
    end
  end

  def delete_team_player( team_id, season, player_id )
    @repository.delete_team_player team_id, season, player_id

    unless @repository.get_team_player( team_id, season, player_id ).nil?
      raise InternalServerError.new, "Team Player with Team ID #{team_id}, Player ID #{player_id} and Season #{season} was not deleted successfully."
    end

    return Hash.new
  end

end
