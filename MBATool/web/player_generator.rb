#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

require 'player_types'
require 'positions'
require 'handedness'
require 'skin_tones'

class PlayerGenerator

  def initialize( db, name_manager )
    @random       = Random.new
    @db           = db
    @name_manager = name_manager

    #                     0    1   2     3     4     5     6     7     8     9   10
    #     percentage:     0  .05  .9  3.75  12.5    50    25  6.25  1.25   0.2  0.1
    @rating_pbs       = [ 0,   4, 72,  300, 1000, 4000, 2000,  500,  100,   16,   8 ]
    @rating_pbs_total = @rating_pbs.reduce(:+)

    #                      0    1   2     3     4     5     6     7     8     9   10
    #      percentage:     0    0   0     0  1.25  12.5    50    25    10  0.95  0.3
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
                      [   20,    0,    0,   10,    0,   70 ]]  # RF
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
    pitcher[ :longevity       ] = roll

    details = {}

    details[ :player_id ] = pitcher[ :player_id ]
    details[ :speed     ] = roll
    details[ :control   ] = roll
    details[ :bunt      ] = pitcher[ :longevity ]
    details[ :fatigue   ] = roll_fatigue

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
    batter[ :longevity       ] = roll

    details = {}

    details[ :player_id          ] = batter[ :player_id ]
    details[ :primary_position   ] = pick_primary_position
    details[ :secondary_position ] = pick_secondary_position details[ :primary_position ]
    details[ :power              ] = roll
    details[ :hit_n_run          ] = roll
    details[ :bunt               ] = roll
    details[ :running            ] = roll
    details[ :range              ] = roll
    details[ :arm                ] = roll

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

  def roll
    x = @random.rand(@rating_pbs_total) + 1

    @rating_pbs.each_with_index do |p,i|
      if (x -= p) <= 0
        return i
      end
    end
  end

  def roll_fatigue
    x = @random.rand(@fatigue_pbs_total) + 1

    @fatigue_pbs.each_with_index do |p,i|
      if (x -= p) <= 0
        return i
      end
    end
  end

end
