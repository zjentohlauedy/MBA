#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

require 'utils'
require 'accolades'
require 'phases'
require 'player_types'
require 'positions'
require 'handedness'
require 'skin_tones'

class PlayerResponseMapper

  def display_accolade( type, value )
    result = Accolades::get_accolade_name type, value

    if result.nil?
      raise InternalServerError.new "Invalid accolade of type #{type} with value #{value}."
    end

    return result
  end

  def display_season_phase( season_phase )
    case season_phase
    when Phases::RegularSeason; return 'Regular'
    when Phases::Playoff;       return 'Playoff'
    when Phases::Allstar;       return 'Allstar'
    end

    return 'Unknown'
  end

  def display_player_type( player_type )
    case player_type
    when PlayerTypes::Pitcher; return 'Pitcher'
    when PlayerTypes::Batter;  return 'Batter'
    end
  end

  def display_handedness( handedness )
    case handedness
    when Handedness::Right;  return 'R'
    when Handedness::Left;   return 'L'
    when Handedness::Switch; return 'S'
    end

    return 'X'
  end

  def display_skin_tone( skin_tone )
    case skin_tone
    when SkinTones::Light; return 'Light'
    when SkinTones::Dark;  return 'Dark'
    end

    return 'Unknown'
  end

  def display_position( position )
    case position
    when Positions::Catcher;       return  "C"
    when Positions::FirstBaseman;  return "1B"
    when Positions::SecondBaseman; return "2B"
    when Positions::ThirdBaseman;  return "3B"
    when Positions::ShortStop;     return "SS"
    when Positions::LeftFielder;   return "LF"
    when Positions::CenterFielder; return "CF"
    when Positions::RightFielder;  return "RF"
    when Positions::Infielder;     return "IF"
    when Positions::Outfielder;    return "OF"
    end

    return "DH"
  end

  def adjust_rating( rating, player, season )
    if player.nil? then return rating end
    if season.nil? then return rating end

    return Utils::adjust_rating rating, season.to_i, player[:rookie_season], player[:longevity]
  end

  def adjust_fatigue( rating, player, season )
    if player.nil? then return rating end
    if season.nil? then return rating end

    return Utils::adjust_fatigue rating, season.to_i, player[:rookie_season], player[:longevity]
  end

  def combine_names( player )
    pl = player.select { |k,v| ! [:first_name, :last_name].include? k }

    pl[:name] = "#{player[:first_name]} #{player[:last_name]}"

    return pl
  end

  def map_player_list( players )
    return nil unless players.is_a? Array

    result = []

    players.each do |player|
      return nil unless player.is_a? Hash

      player = combine_names player

      result.push player.select { |k,v| [:player_id, :name].include? k }
    end

    return result
  end

  def map_player( player )
    return nil unless player.is_a? Hash

    player = combine_names player

    player.delete :longevity

    player[:skin_tone  ] = display_skin_tone   player[:skin_tone  ]
    player[:handedness ] = display_handedness  player[:handedness ]
    player[:player_type] = display_player_type player[:player_type]

    return player
  end

  def map_pitcher( player, pitcher, season = nil )
    return nil unless player.is_a? Hash
    return nil unless pitcher.is_a? Hash

    pl = map_player player

    pl[ :speed   ] = adjust_rating  pitcher[ :speed   ], player, season
    pl[ :control ] = adjust_rating  pitcher[ :control ], player, season
    pl[ :bunt    ] = adjust_rating  pitcher[ :bunt    ], player, season
    pl[ :fatigue ] = adjust_fatigue pitcher[ :fatigue ], player, season

    return pl
  end

  def map_batter( player, batter, season = nil )
    return nil unless player.is_a? Hash
    return nil unless batter.is_a? Hash

    pl = map_player player

    pl[:power    ] = adjust_rating batter[:power    ], player, season
    pl[:hit_n_run] = adjust_rating batter[:hit_n_run], player, season
    pl[:bunt     ] = adjust_rating batter[:bunt     ], player, season
    pl[:running  ] = adjust_rating batter[:running  ], player, season
    pl[:range    ] = adjust_rating batter[:range    ], player, season
    pl[:arm      ] = adjust_rating batter[:arm      ], player, season

    pl[:primary_position  ] = display_position batter[:primary_position  ]
    pl[:secondary_position] = display_position batter[:secondary_position]

    return pl
  end

  def map_pitcher_stats( pitcher_stats )
    return nil unless pitcher_stats.is_a? Hash

    pitcher_stats[:season_phase] = display_season_phase pitcher_stats[:season_phase]

    return pitcher_stats
  end

  def map_batter_stats( batter_stats )
    return nil unless batter_stats.is_a? Hash

    batter_stats[:season_phase] = display_season_phase batter_stats[:season_phase]

    return batter_stats
  end

  def map_player_accolade( player_accolade )
    return nil unless player_accolade.is_a? Hash

    player_accolade[:accolade] = display_accolade 'player', player_accolade[:accolade]

    return player_accolade
  end

  def map_batter_accolade( batter_accolade )
    return nil unless batter_accolade.is_a? Hash

    batter_accolade[:accolade] = display_accolade 'batting', batter_accolade[:accolade]

    return batter_accolade
  end

  def map_pitcher_accolade( pitcher_accolade )
    return nil unless pitcher_accolade.is_a? Hash

    pitcher_accolade[:accolade] = display_accolade 'pitching', pitcher_accolade[:accolade]

    return pitcher_accolade
  end
end
