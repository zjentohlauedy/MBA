#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'sqlite3'

require_relative 'MBATool/web/positions'
require_relative 'MBATool/web/phases'
require_relative 'MBATool/web/utils'


if ARGV.length == 0
  raise "Player ID is required as arg."
end

@player_id = ARGV[0]

@db = SQLite3::Database.new "#{location}/mba.db"

@db.results_as_hash  = true
@db.type_translation = true


def calc_era( stat )
  if stat[:earned_runs] == 0; then return 0.0 end

  stat[:earned_runs].to_f / (stat[:outs].to_f / 3.0 + stat[:innings].to_f) * 9.0
end

def calc_avg( stat )
  if stat[:at_bats] == 0; then return 0.0 end

  stat[:hits].to_f / stat[:at_bats].to_f
end

def get_organization( organization_id )
  result = Utils::transform_hash @db.execute "select * from organizations_t where organization_id = #{organization_id}"
  result[0]
end

def get_player( player_id )
  args = { player_id: player_id }
  query = "select * from players_t where player_id = :player_id"

  result = Utils::transform_hash @db.execute query, args
  result[0]
end

def get_current_team( player_id, season )
  args = { player_id: player_id, season: season }
  query = "select * from team_players_t tp join teams_t t on tp.team_id = t.team_id where tp.player_id = :player_id and tp.season = :season"

  result = Utils::transform_hash @db.execute query, args
  result[0]
end

def get_pitcher_details( player_id )
  result = Utils::transform_hash @db.execute "select * from pitchers_t where player_id = #{player_id}"
  result[0]
end

def get_batter_details( player_id )
  result = Utils::transform_hash @db.execute "select * from batters_t where player_id = #{player_id}"
  result[0]
end

def get_pitcher_stats( player_id, phase = Phases::RegularSeason )
  query = "select * from pitcher_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season join teams_t t on tp.team_id = t.team_id where ps.player_id = #{player_id} and ps.season_phase = #{phase} and tp.team_id <= 32 order by ps.season"

  result = Utils::transform_hash @db.execute query
end

def get_batter_stats( player_id, phase = Phases::RegularSeason )
  query = "select * from batter_stats_t ps join team_players_t tp on ps.player_id = tp.player_id and ps.season = tp.season join teams_t t on tp.team_id = t.team_id where ps.player_id = #{player_id} and ps.season_phase = #{phase} and tp.team_id <= 32 order by ps.season"

  result = Utils::transform_hash @db.execute query
end

def print_pitcher_stats( pitcher, type )
  totals = { wins: 0, losses: 0, innings: 0, outs: 0, games: 0, saves: 0, hits: 0, earned_runs: 0, walks: 0, strike_outs: 0 }

  if pitcher[:stats][type].length == 0; then return; end

  puts ""

  case type
  when :regular; puts 'Regular Season:'
  when :playoff; puts 'Playoffs:'
  when :allstar; puts 'Allstar:'
  end

  puts "Year Team          ERA     W   L   IP     G SV    H   ER   BB   SO"

  pitcher[:stats][type].each do |stat|
    era = calc_era stat

    printf "S%02d  %-10s  %6.2f  %3d %3d %4d.%d %3d %2d %4d %4d %4d %4d\n",
           stat[ :season      ],
           stat[ :name        ],
           era,
           stat[ :wins        ],
           stat[ :losses      ],
           stat[ :innings     ],
           stat[ :outs        ],
           stat[ :games       ],
           stat[ :saves       ],
           stat[ :hits        ],
           stat[ :earned_runs ],
           stat[ :walks       ],
           stat[ :strike_outs ]

    totals[ :wins        ] += stat[ :wins        ]
    totals[ :losses      ] += stat[ :losses      ]
    totals[ :innings     ] += stat[ :innings     ]
    totals[ :outs        ] += stat[ :outs        ]
    totals[ :games       ] += stat[ :games       ]
    totals[ :saves       ] += stat[ :saves       ]
    totals[ :hits        ] += stat[ :hits        ]
    totals[ :earned_runs ] += stat[ :earned_runs ]
    totals[ :walks       ] += stat[ :walks       ]
    totals[ :strike_outs ] += stat[ :strike_outs ]
  end

  totals[ :innings ] += totals[ :outs ] / 3
  totals[ :outs    ]  = totals[ :outs ] % 3

  overall_era = calc_era totals

  printf "%-15s  %6.2f  %3d %3d %4d.%d %3d %2d %4d %4d %4d %4d\n",
         'TOTAL',
         overall_era,
         totals[ :wins        ],
         totals[ :losses      ],
         totals[ :innings     ],
         totals[ :outs        ],
         totals[ :games       ],
         totals[ :saves       ],
         totals[ :hits        ],
         totals[ :earned_runs ],
         totals[ :walks       ],
         totals[ :strike_outs ]
end

def print_batter_stats( batter, type )
  totals = { games: 0, at_bats: 0, runs: 0, hits: 0, doubles: 0, triples: 0, home_runs: 0, runs_batted_in: 0, steals: 0, walks: 0, strike_outs: 0 }

  if batter[:stats][type].length == 0; then return; end

  puts ""

  case type
  when :regular; puts 'Regular Season:'
  when :playoff; puts 'Playoffs:'
  when :allstar; puts 'Allstar:'
  end

  puts "Year Team          BA     G   AB    R    H  2B  3B   HR  RBI  SB   BB   SO"

  batter[:stats][type].each do |stat|
    avg = calc_avg stat

    dsp_avg = sprintf( "%5.3f", avg ).gsub '0.', ' .'

    printf "S%02d  %-10s  %s %4d %4d %4d %4d %3d %3d %4d %4d %3d %4d %4d\n",
           stat[ :season ],
           stat[ :name   ],
           dsp_avg,
           stat[ :games          ],
           stat[ :at_bats        ],
           stat[ :runs           ],
           stat[ :hits           ],
           stat[ :doubles        ],
           stat[ :triples        ],
           stat[ :home_runs      ],
           stat[ :runs_batted_in ],
           stat[ :steals         ],
           stat[ :walks          ],
           stat[ :strike_outs    ]

    totals[ :games         ] += stat[ :games         ]
    totals[ :at_bats       ] += stat[ :at_bats       ]
    totals[ :runs          ] += stat[ :runs          ]
    totals[ :hits          ] += stat[ :hits          ]
    totals[ :doubles       ] += stat[ :doubles       ]
    totals[ :triples       ] += stat[ :triples       ]
    totals[ :home_runs     ] += stat[ :home_runs     ]
    totals[ :runs_batted_in] += stat[ :runs_batted_in]
    totals[ :steals        ] += stat[ :steals        ]
    totals[ :walks         ] += stat[ :walks         ]
    totals[ :strike_outs   ] += stat[ :strike_outs   ]
  end

  overall_avg = calc_avg totals

  dsp_overall_avg = sprintf( "%5.3f", overall_avg ).gsub '0.', ' .'

  printf "%-15s  %s %4d %4d %4d %4d %3d %3d %4d %4d %3d %4d %4d\n",
         'TOTAL',
         dsp_overall_avg,
         totals[ :games          ],
         totals[ :at_bats        ],
         totals[ :runs           ],
         totals[ :hits           ],
         totals[ :doubles        ],
         totals[ :triples        ],
         totals[ :home_runs      ],
         totals[ :runs_batted_in ],
         totals[ :steals         ],
         totals[ :walks          ],
         totals[ :strike_outs    ]
end


def print_pitcher_card( pitcher )
  printf "P #{pitcher[:first_name]} #{pitcher[:last_name]}"

  puts pitcher[:team].nil? ? "" : " - #{pitcher[:team][:name]}"

  print_pitcher_stats pitcher, :regular
  print_pitcher_stats pitcher, :playoff
  print_pitcher_stats pitcher, :allstar
end

def print_batter_card( batter )
  printf "#{Positions::string_value batter[:details][:primary_position]} #{batter[:first_name]} #{batter[:last_name]}"

  puts batter[:team].nil? ? "" : " - #{batter[:team][:name]}"

  print_batter_stats batter, :regular
  print_batter_stats batter, :playoff
  print_batter_stats batter, :allstar
end


org = get_organization 1

player = get_player @player_id

if player.nil?
  raise "Player not found."
end

player[:team] = get_current_team player[:player_id], org[:season]

if player[:player_type] == 1
  player[:details]         = get_pitcher_details player[:player_id]
  player[:stats]           = {}
  player[:stats][:regular] = get_pitcher_stats   player[:player_id]
  player[:stats][:playoff] = get_pitcher_stats   player[:player_id], Phases::Playoff
  player[:stats][:allstar] = get_pitcher_stats   player[:player_id], Phases::Allstar

  print_pitcher_card player
else
  player[:details]         = get_batter_details player[:player_id]
  player[:stats]           = {}
  player[:stats][:regular] = get_batter_stats   player[:player_id]
  player[:stats][:playoff] = get_batter_stats   player[:player_id], Phases::Playoff
  player[:stats][:allstar] = get_batter_stats   player[:player_id], Phases::Allstar

  print_batter_card player
end

