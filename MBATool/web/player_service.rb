#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

require 'accolades'
require 'player_types'
require 'positions'
require 'handedness'
require 'skin_tones'
require 'bad_request_error'
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

  def get_player_accolades_by_season( season )
    result = []

    player_accolades = @repository.get_player_accolades_by_season season

    player_accolades.each do |player_accolade|
      result.push @decorator.decorate_player_accolade @mapper.map_player_accolade player_accolade
    end

    pitcher_accolades = @repository.get_pitcher_accolades_by_season season

    pitcher_accolades.each do |pitcher_accolade|
      result.push @decorator.decorate_player_accolade @mapper.map_pitcher_accolade pitcher_accolade
    end

    batter_accolades = @repository.get_batter_accolades_by_season season

    batter_accolades.each do |batter_accolade|
      result.push @decorator.decorate_player_accolade @mapper.map_batter_accolade batter_accolade
    end

    return result
  end

  def get_player_accolades( player_id, season = nil )
    player = @repository.get_player player_id

    if player.nil?
      raise ResourceNotFoundError.new "Player with Player ID #{player_id} cannot be located."
    end

    player_accolades = @repository.get_player_accolades player_id, season

    results = []

    player_accolades.each do |player_accolade|
      mapped_player_accolade =  @mapper.map_player_accolade player_accolade

      results.push @decorator.decorate_player_accolade mapped_player_accolade
    end

    if player[:player_type] == PlayerTypes::Pitcher
      pitcher_accolades = @repository.get_pitcher_accolades player_id, season

      pitcher_accolades.each do |pitcher_accolade|
        mapped_pitcher_accolade = @mapper.map_pitcher_accolade pitcher_accolade

        results.push @decorator.decorate_player_accolade mapped_pitcher_accolade
      end
    end

    if player[:player_type] == PlayerTypes::Batter
      batter_accolades = @repository.get_batter_accolades player_id, season

      batter_accolades.each do |batter_accolade|
        mapped_batter_accolade = @mapper.map_batter_accolade batter_accolade

        results.push @decorator.decorate_player_accolade mapped_batter_accolade
      end
    end

    return results
  end

  def save_player_accolade( request )
    unless ['batting', 'pitching', 'player'].include? request[:type]
      raise BadRequestError.new "Accolade type #{request[:type]} is not supported. Only batting and pitching accolade types are allowed."
    end

    unless Accolades::valid? request[:type], request[:accolade]
      raise BadRequestError.new "Accolade with value #{request[:accolade]} is not a valid #{request[:type]} accolade."
    end

    player = @repository.get_player request[:player_id]

    accolade = {}

    accolade[ :player_id ] = request[ :player_id ]
    accolade[ :season    ] = request[ :season    ]
    accolade[ :accolade  ] = request[ :accolade  ]

    if request[:type] == 'player'
      @repository.save_player_accolade accolade

      saved_accolade = @repository.get_player_accolade request[:player_id], request[:season], request[:accolade]

      return @decorator.decorate_player_accolade @mapper.map_player_accolade saved_accolade
    end

    if player[:player_type] == PlayerTypes::Pitcher
      unless request[:type] == 'pitching'
        raise BadRequestError.new "Request to create #{request[:type]} accolade failed because player with ID #{request[:player_id]} is a pitcher."
      end

      @repository.save_pitcher_accolade accolade

      saved_accolade = @repository.get_pitcher_accolade request[:player_id], request[:season], request[:accolade]

      return @decorator.decorate_player_accolade @mapper.map_player_accolade saved_accolade
    end

    if player[:player_type] == PlayerTypes::Batter
      unless request[:type] == 'batting'
        raise BadRequestError.new "Request to create #{request[:type]} accolade failed because player with ID #{request[:player_id]} is a batter."
      end

      @repository.save_batter_accolade accolade

      saved_accolade = @repository.get_batter_accolade request[:player_id], request[:season], request[:accolade]

      return @decorator.decorate_player_accolade @mapper.map_player_accolade saved_accolade
    end
  end

  private :process_player_list, :process_player, :process_pitcher, :process_batter, :get_pitcher_stats, :get_batter_stats
end
