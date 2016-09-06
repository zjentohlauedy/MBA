#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'FileParser'
require 'ProgRunner'
require 'ScheduleParser'


def find_team(org, team_name)
  org[:leagues].each do |league|
    league[:divisions].each do |division|
      division[:teams].each do |team|
        if team[:name] == team_name
          return team
        end
      end
    end
  end

  return nil
end


def find_winning_pitcher(team)
  team[:players].each do |player|
    next if player[:type] == 'batter'

    if player[:stats][:simulated][:wins] > 0
      return player
    end
  end

  return nil
end


def find_losing_pitcher(team)
  team[:players].each do |player|
    next if player[:type] == 'batter'

    if player[:stats][:simulated][:losses] > 0
      return player
    end
  end

  return nil
end


def calc_totals(team)
  pitching_totals = {wins: 0, losses: 0, games: 0, saves: 0, innings: 0, outs: 0, hits: 0, earned_runs: 0, home_runs: 0, walks: 0, strike_outs: 0}
  batting_totals  = {at_bats: 0, runs: 0, hits: 0, doubles: 0, triples: 0, home_runs: 0, runs_batted_in: 0, walks: 0, strike_outs: 0, steals: 0, errors: 0}

  team[:players].each do |player|
    if    player[:type] == 'pitcher'
      stats = player[:stats][:simulated]

      pitching_totals[ :wins        ] += stats[ :wins        ]
      pitching_totals[ :losses      ] += stats[ :losses      ]
      pitching_totals[ :games       ] += stats[ :games       ]
      pitching_totals[ :saves       ] += stats[ :saves       ]
      pitching_totals[ :innings     ] += stats[ :innings     ]
      pitching_totals[ :outs        ] += stats[ :outs        ]
      pitching_totals[ :hits        ] += stats[ :hits        ]
      pitching_totals[ :earned_runs ] += stats[ :earned_runs ]
      pitching_totals[ :home_runs   ] += stats[ :home_runs   ]
      pitching_totals[ :walks       ] += stats[ :walks       ]
      pitching_totals[ :strike_outs ] += stats[ :strike_outs ]

    elsif player[:type] == 'batter'
      stats = player[:stats][:simulated]

      batting_totals[ :at_bats        ] += stats[ :at_bats        ]
      batting_totals[ :runs           ] += stats[ :runs           ]
      batting_totals[ :hits           ] += stats[ :hits           ]
      batting_totals[ :doubles        ] += stats[ :doubles        ]
      batting_totals[ :triples        ] += stats[ :triples        ]
      batting_totals[ :home_runs      ] += stats[ :home_runs      ]
      batting_totals[ :runs_batted_in ] += stats[ :runs_batted_in ]
      batting_totals[ :walks          ] += stats[ :walks          ]
      batting_totals[ :strike_outs    ] += stats[ :strike_outs    ]
      batting_totals[ :steals         ] += stats[ :steals         ]
      batting_totals[ :errors         ] += stats[ :errors         ]
    end
  end

  pitching_totals[:innings] += (pitching_totals[:outs   ] / 3).to_i
  pitching_totals[:outs   ]  =  pitching_totals[:outs   ] % 3

  team[ :pitching_totals ] = pitching_totals
  team[ :batting_totals  ] = batting_totals
end


def get_hitters(team)
  hitters  = "                       Hitters: #{team[:name]}\n\n"
  hitters += "           AB  R   H  2b 3b HR RBI SH  BB  SO  SB CS E  PB\n"

  team[:players].each do |player|
    next if player[:type] == 'pitcher'

    stats = player[:stats][:simulated]

    next if stats[:at_bats] == 0 and stats[:walks] == 0

    hitters += sprintf "%-10s %2d %2d  %2d %2d %2d %2d  %2d  0  %2d  %2d  %2d  0 %2d   0\n",
                       player[ :last_name      ],
                       stats[  :at_bats        ],
                       stats[  :runs           ],
                       stats[  :hits           ],
                       stats[  :doubles        ],
                       stats[  :triples        ],
                       stats[  :home_runs      ],
                       stats[  :runs_batted_in ],
                       stats[  :walks          ],
                       stats[  :strike_outs    ],
                       stats[  :steals         ],
                       stats[  :errors         ]
  end

  hitters += sprintf "TOTAL      %2d %2d  %2d %2d %2d %2d  %2d  0  %2d  %2d  %2d  0 %2d   0\n",
                     team[:batting_totals][ :at_bats        ],
                     team[:batting_totals][ :runs           ],
                     team[:batting_totals][ :hits           ],
                     team[:batting_totals][ :doubles        ],
                     team[:batting_totals][ :triples        ],
                     team[:batting_totals][ :home_runs      ],
                     team[:batting_totals][ :runs_batted_in ],
                     team[:batting_totals][ :walks          ],
                     team[:batting_totals][ :strike_outs    ],
                     team[:batting_totals][ :steals         ],
                     team[:batting_totals][ :errors         ]

  return hitters
end


def get_pitchers(team)
  pitchers  = "                       Pitchers: #{team[:name]}\n\n"
  pitchers += "            IP   H  R ER HR  BB  SO  WP HB BALK\n"

  team[:players].each do |player|
    next if player[:type] == 'batter'

    stats = player[:stats][:simulated]

    next if (stats[:innings] + stats[:outs] + stats[:hits] + stats[:walks]) == 0

    pitchers += sprintf "%-10s %2d.%1d %2d %2d %2d %2d  %2d  %2d  0  0  0\n",
                        player[ :last_name      ],
                        stats[ :innings     ],
                        stats[ :outs        ],
                        stats[ :hits        ],
                        stats[ :earned_runs ],
                        stats[ :earned_runs ],
                        stats[ :home_runs   ],
                        stats[ :walks       ],
                        stats[ :strike_outs ]
  end

  pitchers += sprintf "TOTAL      %2d.%1d %2d %2d %2d %2d  %2d  %2d  0  0  0\n",
                      team[:pitching_totals][ :innings     ],
                      team[:pitching_totals][ :outs        ],
                      team[:pitching_totals][ :hits        ],
                      team[:pitching_totals][ :earned_runs ],
                      team[:pitching_totals][ :earned_runs ],
                      team[:pitching_totals][ :home_runs   ],
                      team[:pitching_totals][ :walks       ],
                      team[:pitching_totals][ :strike_outs ]
end


def gen_boxscore(road_team, home_team)
  boxscore  =  "                       #{road_team[:name]} vs #{home_team[:name]}\n\n"
  boxscore += "            1  2  3  4  5  6  7  8  9     R  H  E\n"
  boxscore += sprintf "%-9s   0  0  0  0  0  0  0  0  0    %2d %2d %2d\n",
                      road_team[:name],
                      road_team[:batting_totals][:runs],
                      road_team[:batting_totals][:hits],
                      road_team[:batting_totals][:errors]
  boxscore += sprintf "%-9s   0  0  0  0  0  0  0  0  0    %2d %2d %2d\n",
                      home_team[:name],
                      home_team[:batting_totals][:runs],
                      home_team[:batting_totals][:hits],
                      home_team[:batting_totals][:errors]

  if home_team[:batting_totals][:runs] > road_team[:batting_totals][:runs]
    winner = find_winning_pitcher home_team
    loser  = find_losing_pitcher  road_team
  else
    winner = find_winning_pitcher road_team
    loser  = find_losing_pitcher  home_team
  end

  boxscore += sprintf "W  %-15s     DP: %-9s  0\n", winner[ :last_name ], road_team[:name]
  boxscore += sprintf "L  %-15s         %-9s  0\n", loser[  :last_name ], home_team[:name]
  boxscore += sprintf "                        T: 0:00\n"
  boxscore += "GWRBI:\n"
  boxscore += "\n#{get_hitters road_team}"
  boxscore += "\n#{get_pitchers road_team}"
  boxscore += "\n#{get_hitters home_team}"
  boxscore += "\n#{get_pitchers home_team}"

  return boxscore
end


path = ARGV[0] || '.'

extract_data = ProgRunner.new location, "extract_data"

extract_data.execute "#{path}/LeagName.Dat", "#{path}/Players.S"

org = JSON.parse extract_data.get_output, {:symbolize_names => true}


schedule_file = "#{path}/schedule.csv"

sp = ScheduleParser.new
fp = FileParser.new sp

fp.process_file schedule_file

schedule = sp.schedule


schedule.days[0].games.each do |game|
  road_team = find_team org, game.road_team
  home_team = find_team org, game.home_team

  calc_totals road_team
  calc_totals home_team

  boxscore = gen_boxscore road_team, home_team

  boxscore_file = sprintf "D001G%02d.txt", game.number

  File::write boxscore_file, boxscore
end
