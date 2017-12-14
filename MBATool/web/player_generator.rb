#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

require 'player_types'
require 'pitcher_style'
require 'positions'
require 'handedness'
require 'skin_tones'

class PlayerGenerator

  def initialize( db, name_manager )
    @random       = Random.new
    @db           = db
    @name_manager = name_manager

    #                      0    1   2     3     4     5     6     7     8     9   10
    #     percentage:      0  .05  .9  3.75  12.5    50    25  6.25  1.25    .2   .1
    @rating_pbs       =  [ 0,   4, 72,  300, 1000, 4000, 2000,  500,  100,   16,   8 ]
    @rating_pbs_total =  @rating_pbs.reduce(:+)

    #                      0    1   2     3     4     5     6     7     8     9   10
    #     percentage:      0    0   0     0     0     0     4    50    25    13    8
    @closer_pbs       =  [ 0,   0,  0,    0,    0,    0,  320, 4000, 2000, 1040, 640 ]
    @closer_pbs_total =  @closer_pbs.reduce(:+)

    #                      0    1   2     3     4     5     6     7     8     9   10
    #     percentage:      0  .05  .9  3.75  12.5    50    25  6.35  1.45     0    0
    @power_pbs       =   [ 0,   4, 72,  300, 1000, 4000, 2000,  508,  116,    0,   0 ]
    @power_pbs_total =   @power_pbs.reduce(:+)

    #                      0    1   2     3     4     5     6     7     8     9   10
    #      percentage:     0    0   0     0  1.25  12.5    50    25    10   .95   .3
    @fatigue_pbs       = [ 0,   0,  0,    0,  100, 1000, 4000, 2000,  800,   76,  24 ]
    @fatigue_pbs_total = @fatigue_pbs.reduce(:+)

    @secondary_pos = [[],
                      #    C    1B    3B    IF    RF    OF
                      [    0,   30,   30,   10,   20,   10 ], # C
                      [   20,    0,    0,   60,    0,   20 ], # 1B
                      [    5,    0,    0,   85,    0,   10 ], # 2B
                      [   20,    0,    0,   70,    0,   10 ], # 3B
                      [    5,    0,    0,   75,    0,   20 ], # SS
                      [    5,    0,    0,   10,    0,   85 ], # LF
                      [    5,    0,    0,   20,    0,   75 ], # CF
                      [   20,    0,    0,   10,    0,   70 ]] # RF
  end

  def generate_pitcher( season )
    pitcher = {}

    name = @name_manager.get_name

    pitcher[ :player_id       ] = get_next_player_id
    pitcher[ :first_name      ] = name[ :first ]
    pitcher[ :last_name       ] = name[ :last  ]
    pitcher[ :first_phoenetic ] = ''
    pitcher[ :last_phoenetic  ] = ''
    pitcher[ :skin_tone       ] = pick_skin_tone
    pitcher[ :handedness      ] = pick_handedness
    pitcher[ :player_type     ] = PlayerTypes::Pitcher
    pitcher[ :rookie_season   ] = season
    pitcher[ :longevity       ] = roll @rating_pbs, @rating_pbs_total

    details = {}

    details[ :player_id ] = pitcher[ :player_id ]
    details[ :speed     ] = roll @rating_pbs, @rating_pbs_total
    details[ :control   ] = roll @rating_pbs, @rating_pbs_total
    details[ :bunt      ] = pitcher[ :longevity ]
    details[ :fatigue   ] = roll @fatigue_pbs, @fatigue_pbs_total

    pitcher[ :details ] = details

    return pitcher
  end

  def generate_closer( season )
    pitcher = {}

    name = @name_manager.get_name

    pitcher[ :player_id       ] = get_next_player_id
    pitcher[ :first_name      ] = name[ :first ]
    pitcher[ :last_name       ] = name[ :last  ]
    pitcher[ :first_phoenetic ] = ''
    pitcher[ :last_phoenetic  ] = ''
    pitcher[ :skin_tone       ] = pick_skin_tone
    pitcher[ :handedness      ] = pick_handedness
    pitcher[ :player_type     ] = PlayerTypes::Pitcher
    pitcher[ :rookie_season   ] = season
    pitcher[ :longevity       ] = roll @rating_pbs, @rating_pbs_total

    details = {}

    details[ :player_id ] = pitcher[ :player_id ]
    details[ :speed     ] = roll @closer_pbs, @closer_pbs_total
    details[ :control   ] = roll @closer_pbs, @closer_pbs_total
    details[ :bunt      ] = pitcher[ :longevity ]
    details[ :fatigue   ] = 1

    pitcher[ :details ] = details

    return pitcher
  end

  def generate_batter( season )
    batter = {}

    name = @name_manager.get_name

    batter[ :player_id       ] = get_next_player_id
    batter[ :first_name      ] = name[ :first ]
    batter[ :last_name       ] = name[ :last  ]
    batter[ :first_phoenetic ] = ''
    batter[ :last_phoenetic  ] = ''
    batter[ :skin_tone       ] = pick_skin_tone
    batter[ :handedness      ] = pick_handedness true
    batter[ :player_type     ] = PlayerTypes::Batter
    batter[ :rookie_season   ] = season
    batter[ :longevity       ] = roll @rating_pbs, @rating_pbs_total

    details = {}

    details[ :player_id          ] = batter[ :player_id ]
    details[ :primary_position   ] = pick_primary_position
    details[ :secondary_position ] = pick_secondary_position details[ :primary_position ]
    details[ :power              ] = roll @power_pbs,  @power_pbs_total
    details[ :hit_n_run          ] = roll @rating_pbs, @rating_pbs_total
    details[ :bunt               ] = roll @rating_pbs, @rating_pbs_total
    details[ :running            ] = roll @rating_pbs, @rating_pbs_total
    details[ :range              ] = roll @rating_pbs, @rating_pbs_total
    details[ :arm                ] = roll @rating_pbs, @rating_pbs_total

    batter[ :details ] = details

    return batter
  end

  def get_next_player_id
    result = @db.get_first_row 'select max(player_id) player_id from players_t'

    return result['player_id'] + 1
  end

  def pick_skin_tone
    return (@random.rand(100) > 66) ? SkinTones::Dark : SkinTones::Light
  end

  def pick_handedness( can_be_switch = false )
    if can_be_switch
      x = @random.rand(100)

      if x > 95; return Handedness::Switch end

      return (x > 61) ? Handedness::Left : Handedness::Right
    end

    return (@random.rand(100) > 66) ? Handedness::Left : Handedness::Right
  end

  def pick_primary_position
    return @random.rand(8) + 1
  end

  def pick_pitcher_style
    return (@random.rand(100) > 90) ? PitcherStyle::Closer : PitcherStyle::Starter
  end

  def pick_secondary_position( primary_position )
    x = @random.rand(100) + 1

    @secondary_pos[primary_position].each_with_index do |p,i|
      if (x -= p) <= 0
        case i
        when 0; return Positions::Catcher
        when 1; return Positions::FirstBaseman
        when 2; return Positions::ThirdBaseman
        when 3; return Positions::Infielder
        when 4; return Positions::RightFielder
        when 5; return Positions::Outfielder
        end
      end
    end
  end

  def roll( probabilities, total )
    x = @random.rand(total) + 1

    probabilities.each_with_index do |p,i|
      if (x -= p) <= 0
        return i
      end
    end
  end
end
