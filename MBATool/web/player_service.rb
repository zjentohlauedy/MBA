#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

require 'player_types'
require 'positions'
require 'handedness'
require 'skin_tones'
require 'internal_server_error'
require 'resource_not_found_error'

class PlayerService

  def initialize( repository, mapper, decorator )
    @repository = repository
    @mapper     = mapper
    @decorator  = decorator
  end

  def process_player_list( player_list, season, phase )
    player_list = @mapper.map_player_list player_list

    player_list.each do |player|
      @decorator.decorate_player player, season, phase
    end
  end

  def get_team_players( team_id, season = nil, phase = nil )
    player_list = @repository.get_players_by_team team_id, season

    process_player_list player_list, season, phase
  end

  def get_players( season = nil, phase = nil )
    player_list = @repository.get_players

    process_player_list player_list, season, phase
  end

  def get_rookies( season, phase = nil )
    player_list = @repository.get_players_by_rookie_season season

    process_player_list player_list, season, phase
  end

  def get_free_agents( season, phase = nil )
    player_list = @repository.get_free_agents_by_season season

    process_player_list player_list, season, phase
  end

  def process_player( player, season, phase )
    player = @mapper.map_player player

    @decorator.decorate_player player, season, phase
  end

  def process_pitcher( player, season, phase )
    pitcher = @repository.get_pitcher_details player[:player_id]

    if pitcher.nil?
      raise InternalServerError.new "There was an error retrieving Pitcher with Player ID #{player[:player_id]}."
    end

    player = @mapper.map_pitcher player, pitcher, season

    @decorator.decorate_player player, season, phase
  end

  def process_batter( player, season, phase )
    batter = @repository.get_batter_details player[:player_id]

    if batter.nil?
      raise InternalServerError.new "There was an error retrieving Batter with Player ID #{player[:player_id]}."
    end

    player = @mapper.map_batter player, batter, season

    @decorator.decorate_player player, season, phase
  end

  def get_player( player_id, season = nil, phase = nil )
    player = @repository.get_player player_id

    if player.nil?
      raise ResourceNotFoundError.new "Player with Player ID #{player_id} cannot be located."
    end

    if    player[:player_type] == PlayerTypes::Pitcher; process_pitcher player, season, phase
    elsif player[:player_type] == PlayerTypes::Batter;  process_batter  player, season, phase
    else                                                process_player  player, season, phase
    end
  end

  def get_pitcher_stats( player_id, season, phase )
    pitcher_stats = @repository.get_pitcher_stats player_id, season, phase

    if pitcher_stats.nil?
      raise ResourceNotFoundError.new "Player Stats for Player ID #{player_id}, Season #{season} and Phase #{phase} cannot be located."
    end

    if pitcher_stats.is_a? Hash
      pitcher_stats = @mapper.map_pitcher_stats pitcher_stats

      return @decorator.decorate_player_stats pitcher_stats, phase
    end

    results = []

    pitcher_stats.each do |stats|
      phase = stats[:season_phase]

      mapped_stats = @mapper.map_pitcher_stats stats

      @decorator.decorate_player_stats mapped_stats, phase

      results.push mapped_stats
    end

    return results
  end

  def get_batter_stats( player_id, season, phase )
    batter_stats = @repository.get_batter_stats player_id, season, phase

    if batter_stats.nil?
      raise ResourceNotFoundError.new "Player Stats for Player ID #{player_id}, Season #{season} and Phase #{phase} cannot be located."
    end

    if batter_stats.is_a? Hash
      batter_stats = @mapper.map_batter_stats batter_stats

      return @decorator.decorate_player_stats batter_stats, phase
    end

    results = []

    batter_stats.each do |stats|
      phase = stats[:season_phase]

      mapped_stats = @mapper.map_batter_stats stats

      @decorator.decorate_player_stats mapped_stats, phase

      results.push mapped_stats
    end

    return results
  end

  def get_player_stats( player_id, season = nil, phase = nil )
    player = @repository.get_player player_id

    if player.nil?
      raise ResourceNotFoundError.new "Player with Player ID #{player_id} cannot be located."
    end

    if    player[:player_type] == PlayerTypes::Pitcher; return get_pitcher_stats player_id, season, phase
    elsif player[:player_type] == PlayerTypes::Batter;  return get_batter_stats  player_id, season, phase
    end

    unless season.nil? or phase.nil?
      raise ResourceNotFoundError.new "Player Stats for Player ID #{player_id}, Season #{season} and Phase #{phase} cannot be located."
    end

    return Array.new
  end

  private :process_player_list, :process_player, :process_pitcher, :process_batter, :get_pitcher_stats, :get_batter_stats
end
