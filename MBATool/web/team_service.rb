#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

require 'internal_server_error'
require 'resource_not_found_error'

class TeamService

  def initialize( repository, mapper, decorator )
    @repository = repository
    @mapper     = mapper
    @decorator  = decorator
  end

  def get_teams( season = nil, phase = nil )
    teams = @repository.get_division_teams

    teams = @mapper.map_team_list teams

    teams.each do |team|
      @decorator.decorate_team team, season, phase
    end

    return teams
  end

  def get_team( team_id, season = nil, phase = nil )
    team = @repository.get_team team_id

    if team.nil?
      raise ResourceNotFoundError.new "Team with Team ID #{team_id} cannot be located."
    end

    @decorator.decorate_team team, season, phase
  end

  def get_team_stats( team_id, season = nil, phase = nil )
    team_stats = @repository.get_team_stats team_id, season, phase

    if team_stats.nil?
      raise ResourceNotFoundError.new "Team Stats for Team ID #{team_id}, Season #{season} and Phase #{phase} cannot be located."
    end

    if team_stats.is_a? Hash
      team_stats = @mapper.map_team_stats team_stats

      return @decorator.decorate_team_stats team_stats, phase
    end

    results = []

    team_stats.each do |stats|
      phase = stats[:season_phase]

      mapped_stats = @mapper.map_team_stats stats

      @decorator.decorate_team_stats mapped_stats, phase

      results.push mapped_stats
    end

    return results
  end

  def get_team_accolades( team_id, season = nil )
    team_accolades = @repository.get_team_accolades team_id, season

    results = []

    team_accolades.each do |accolade|
      results.push @decorator.decorate_team_accolade @mapper.map_team_accolade accolade
    end

    return results
  end
end
