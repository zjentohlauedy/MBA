#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"

class ResponseMapper

  def initialize( decorator )
    @decorator = decorator
  end

  def display_season_phase( season_phase )
    case season_phase
    when 1; return 'Regular'
    when 2; return 'Playoff'
    when 3; return 'Allstar'
    end

    return 'Unknown'
  end

  def display_player_type( player_type )
    case player_type
    when 1; return 'Pitcher'
    when 2; return 'Batter'
    end
  end

  def display_handedness( handedness )
    case handedness
    when 1; return 'R'
    when 2; return 'L'
    when 3; return 'S'
    end

    return 'X'
  end

  def display_skin_tone( skin_tone )
    case skin_tone
    when 1; return 'Light'
    when 2; return 'Dark'
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

    if ! player.has_key? 'Season' then return rating end

    adjustment = (player['Season'] - player['Rookie_Season']) - (player['Longevity'] + 3)

    if adjustment < 0 then adjustment = 0 end

    rating -= adjustment

    return (rating > 1) ? rating : 1
  end

  def adjust_fatigue( rating, player )
    rating = adjust_rating rating, player

    (rating > 4) ? rating : 4
  end

  def map_teams_response( teams )
    result = []

    teams.each do |team|
      result.push map_team_response( team, false )
    end

    return result
  end

  def map_team_response( team, detailed = true )
    mapped_team = {}

    mapped_team[ :team_id ] = team[ 'Team_Id' ]
    mapped_team[ :name    ] = team[ 'Name'    ]

    if detailed
      mapped_team[ :location        ] = team[ 'Location'        ]
      mapped_team[ :primary_color   ] = team[ 'Primary_Color'   ]
      mapped_team[ :secondary_color ] = team[ 'Secondary_Color' ]
    end

    return @decorator.decorate_team mapped_team
  end

  def map_team_stats_response( team_stats_list )
    result = []

    team_stats_list.each do |team_stats|
      mapped_team_stats = {}

      mapped_team_stats[ :team_id         ] = team_stats[ 'Team_Id'         ]
      mapped_team_stats[ :season          ] = team_stats[ 'Season'          ]
      mapped_team_stats[ :season_phase    ] = team_stats[ 'Season_Phase'    ]
      mapped_team_stats[ :wins            ] = team_stats[ 'Wins'            ]
      mapped_team_stats[ :losses          ] = team_stats[ 'Losses'          ]
      mapped_team_stats[ :home_wins       ] = team_stats[ 'Home_Wins'       ]
      mapped_team_stats[ :home_losses     ] = team_stats[ 'Home_Losses'     ]
      mapped_team_stats[ :road_wins       ] = team_stats[ 'Road_Wins'       ]
      mapped_team_stats[ :road_losses     ] = team_stats[ 'Road_Losses'     ]
      mapped_team_stats[ :division_wins   ] = team_stats[ 'Division_Wins'   ]
      mapped_team_stats[ :division_losses ] = team_stats[ 'Division_Losses' ]
      mapped_team_stats[ :league_wins     ] = team_stats[ 'League_Wins'     ]
      mapped_team_stats[ :league_losses   ] = team_stats[ 'League_Losses'   ]
      mapped_team_stats[ :runs_scored     ] = team_stats[ 'Runs_Scored'     ]
      mapped_team_stats[ :runs_allowed    ] = team_stats[ 'Runs_Allowed'    ]

      result.push @decorator.decorate_team_stats mapped_team_stats
    end

    return result
  end

  def map_team_player_response( team_player )
    mapped_team_player = {}

    mapped_team_player[ :team_id   ] = team_player[ 'Team_Id'   ]
    mapped_team_player[ :player_id ] = team_player[ 'Player_Id' ]
    mapped_team_player[ :season    ] = team_player[ 'Season'    ]

    return @decorator.decorate_team_player mapped_team_player
  end

  def map_players_response( players )
    result = []

    players.each do |player|
      result.push map_player_response( player, false )
    end

    return result
  end

  def map_player_response( player, detailed = true )
    mapped_player = {}

    if player.has_key? 'Team_Id'; mapped_player[ :team_id ] = player[ 'Team_Id' ] end
    if player.has_key? 'Season';  mapped_player[ :season  ] = player[ 'Season'  ] end

    mapped_player[ :player_id ] = player[ 'Player_Id'  ]
    mapped_player[ :name      ] = "#{player['First_Name']} #{player['Last_Name']}"

    if detailed
      mapped_player[ :first_phoenetic ] =                     player[ 'First_Phoenetic' ]
      mapped_player[ :last_phoenetic  ] =                     player[ 'Last_Phoenetic'  ]
      mapped_player[ :skin_tone       ] = display_skin_tone   player[ 'Skin_Tone'       ]
      mapped_player[ :handedness      ] = display_handedness  player[ 'Handedness'      ]
      mapped_player[ :player_type     ] = display_player_type player[ 'Player_Type'     ]
      mapped_player[ :rookie_season   ] =                     player[ 'Rookie_Season'   ]

      if player.has_key? 'Details'
        details = player[ 'Details' ]

        if player['Player_Type'] == 1
          mapped_player[ :speed   ] = adjust_rating  details[ 'Speed'   ], player
          mapped_player[ :control ] = adjust_rating  details[ 'Control' ], player
          mapped_player[ :bunt    ] = adjust_rating  details[ 'Bunt'    ], player
          mapped_player[ :fatigue ] = adjust_fatigue details[ 'Fatigue' ], player
        end

        if player['Player_Type'] == 2
          mapped_player[ :primary_position   ] = display_position details[ 'Primary_Position'   ]
          mapped_player[ :secondary_position ] = display_position details[ 'Secondary_Position' ]
          mapped_player[ :power              ] = adjust_rating    details[ 'Power'              ], player
          mapped_player[ :hit_n_run          ] = adjust_rating    details[ 'Hit_N_Run'          ], player
          mapped_player[ :bunt               ] = adjust_rating    details[ 'Bunt'               ], player
          mapped_player[ :running            ] = adjust_rating    details[ 'Running'            ], player
          mapped_player[ :range              ] = adjust_rating    details[ 'Range'              ], player
          mapped_player[ :arm                ] = adjust_rating    details[ 'Arm'                ], player
        end
      end
    end

    return @decorator.decorate_player mapped_player
  end

  def map_pitcher_stats_response( pitcher_stats_list )
    result = []

    pitcher_stats_list.each do |pitcher_stats|
      mapped_pitcher_stats = {}

      mapped_pitcher_stats[ :player_id    ] = pitcher_stats[ 'Player_Id'    ]
      mapped_pitcher_stats[ :season       ] = pitcher_stats[ 'Season'       ]
      mapped_pitcher_stats[ :season_phase ] = pitcher_stats[ 'Season_Phase' ]
      mapped_pitcher_stats[ :wins         ] = pitcher_stats[ 'Wins'         ]
      mapped_pitcher_stats[ :losses       ] = pitcher_stats[ 'Losses'       ]
      mapped_pitcher_stats[ :games        ] = pitcher_stats[ 'Games'        ]
      mapped_pitcher_stats[ :saves        ] = pitcher_stats[ 'Saves'        ]
      mapped_pitcher_stats[ :innings      ] = pitcher_stats[ 'Innings'      ]
      mapped_pitcher_stats[ :outs         ] = pitcher_stats[ 'Outs'         ]
      mapped_pitcher_stats[ :hits         ] = pitcher_stats[ 'Hits'         ]
      mapped_pitcher_stats[ :earned_runs  ] = pitcher_stats[ 'Earned_Runs'  ]
      mapped_pitcher_stats[ :home_runs    ] = pitcher_stats[ 'Home_Runs'    ]
      mapped_pitcher_stats[ :walks        ] = pitcher_stats[ 'Walks'        ]
      mapped_pitcher_stats[ :strike_outs  ] = pitcher_stats[ 'Strike_Outs'  ]

      result.push @decorator.decorate_pitcher_stats mapped_pitcher_stats
    end

    return result
  end

  def map_batter_stats_response( batter_stats_list )
    result = []

    batter_stats_list.each do |batter_stats|
      mapped_batter_stats = {}

      mapped_batter_stats[ :player_id      ] = batter_stats[ 'Player_Id'      ]
      mapped_batter_stats[ :season         ] = batter_stats[ 'Season'         ]
      mapped_batter_stats[ :season_phase   ] = batter_stats[ 'Season_Phase'   ]
      mapped_batter_stats[ :games          ] = batter_stats[ 'Games'          ]
      mapped_batter_stats[ :at_bats        ] = batter_stats[ 'At_Bats'        ]
      mapped_batter_stats[ :runs           ] = batter_stats[ 'Runs'           ]
      mapped_batter_stats[ :hits           ] = batter_stats[ 'Hits'           ]
      mapped_batter_stats[ :doubles        ] = batter_stats[ 'Doubles'        ]
      mapped_batter_stats[ :triples        ] = batter_stats[ 'Triples'        ]
      mapped_batter_stats[ :home_runs      ] = batter_stats[ 'Home_Runs'      ]
      mapped_batter_stats[ :runs_batted_in ] = batter_stats[ 'Runs_Batted_In' ]
      mapped_batter_stats[ :walks          ] = batter_stats[ 'Walks'          ]
      mapped_batter_stats[ :strike_outs    ] = batter_stats[ 'Strike_Outs'    ]
      mapped_batter_stats[ :steals         ] = batter_stats[ 'Steals'         ]
      mapped_batter_stats[ :errors         ] = batter_stats[ 'Errors'         ]

      result.push @decorator.decorate_batter_stats mapped_batter_stats
    end

    return result
  end

end
