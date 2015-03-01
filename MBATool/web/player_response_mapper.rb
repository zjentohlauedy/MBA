#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

require 'utils'
require 'phases'
require 'player_types'
require 'handedness'
require 'skin_tones'

class PlayerResponseMapper

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
    when  1; return  "C"
    when  2; return "1B"
    when  3; return "2B"
    when  4; return "3B"
    when  5; return "SS"
    when  6; return "LF"
    when  7; return "CF"
    when  8; return "RF"
    when  9; return "IF"
    when 10; return "OF"
    end

    return "DH"
  end

  def adjust_rating( rating, player )
    if player.nil? then return rating end

    unless player.has_key? :season then return rating end

    return Utils::adjust_rating rating, player[:season], player[:rookie_season], player[:longevity]
  end

  def adjust_fatigue( rating, player )
    if player.nil? then return rating end

    unless player.has_key? :season then return rating end

    return Utils::adjust_fatigue rating, player[:season], player[:rookie_season], player[:longevity]
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

  def map_pitcher( player, pitcher )
    return nil unless player.is_a? Hash
    return nil unless pitcher.is_a? Hash

    pl = map_player player

    pl[ :speed   ] = adjust_rating  pitcher[ :speed   ], player
    pl[ :control ] = adjust_rating  pitcher[ :control ], player
    pl[ :bunt    ] = adjust_rating  pitcher[ :bunt    ], player
    pl[ :fatigue ] = adjust_fatigue pitcher[ :fatigue ], player

    return pl
  end

  def map_batter( player, batter )
    return nil unless player.is_a? Hash
    return nil unless batter.is_a? Hash

    pl = map_player player

    pl[:power    ] = adjust_rating batter[:power    ], player
    pl[:hit_n_run] = adjust_rating batter[:hit_n_run], player
    pl[:bunt     ] = adjust_rating batter[:bunt     ], player
    pl[:running  ] = adjust_rating batter[:running  ], player
    pl[:range    ] = adjust_rating batter[:range    ], player
    pl[:arm      ] = adjust_rating batter[:arm      ], player

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
end
