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


if ARGV.length == 1
  @player_id = ARGV[0]
elsif ARGV.length == 2
  @firstname = ARGV[0]
  @lastname  = ARGV[1]
else
  raise "Player ID or Player Name is required."
end


@db = SQLite3::Database.new "#{location}/mba.db"

@db.results_as_hash  = true
@db.type_translation = true


def calc_era( stats )
  if stats[:earned_runs] == 0; then return 0.0 end

  stats[:earned_runs].to_f / (stats[:outs].to_f / 3.0 + stats[:innings].to_f) * 9.0
end

def calc_avg( stats )
  if stats[:at_bats] == 0; then return 0.0 end

  stats[:hits].to_f / stats[:at_bats].to_f
end

def calc_ip( stats )
  stats[:innings].to_f + (stats[:outs].to_f / 10.0)
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

def get_player_by_name( firstname, lastname )
  args = { first_name: firstname, last_name: lastname }
  query = "select * from players_t where first_name = :first_name and last_name = :last_name"

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

def get_best_pitcher_stats_overall()
  query = 'select max(wins) wins, max(games) games, max(saves) saves, max(strike_outs) strike_outs from pitcher_stats_t'
  result = Utils::transform_hash @db.execute query

  query2  = 'select innings, outs, earned_runs, min(cast(earned_runs as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) era from pitcher_stats_t where innings >= 185'
  result2 = Utils::transform_hash @db.execute query2

  query3  = 'select innings, outs, max(cast(innings as float) + (cast(outs as float) / 10.0)) ip from pitcher_stats_t'
  result3 = Utils::transform_hash @db.execute query3

  result[0][:era] = result2[0]
  result[0][:ip]  = result3[0]

  return result[0]
end

def get_best_pitcher_stats_by_season( season )
  query = "select max(wins) wins, max(games) games, max(saves) saves, max(strike_outs) strike_outs from pitcher_stats_t where season = #{season}"
  result = Utils::transform_hash @db.execute query

  query2  = "select innings, outs, earned_runs, min(cast(earned_runs as float) / (cast(outs as float) / 3.0 + cast(innings as float)) * 9.0) era from pitcher_stats_t where innings >= 185 and season = #{season}"
  result2 = Utils::transform_hash @db.execute query2

  query3  = "select innings, outs, max(cast(innings as float) + (cast(outs as float) / 10.0)) ip from pitcher_stats_t where season = #{season}"
  result3 = Utils::transform_hash @db.execute query3

  result[0][:era] = result2[0]
  result[0][:ip]  = result3[0]

  return result[0]
end

def get_best_batter_stats_overall()
  query = 'select max(at_bats) at_bats, max(runs) runs, max(hits) hits, max(doubles) doubles, max(triples) triples, max(home_runs) home_runs, max(runs_batted_in) runs_batted_in, max(steals) steals, max(walks) walks from batter_stats_t'
  result = Utils::transform_hash @db.execute query

  query2  = 'select at_bats, hits, max(cast(hits as float) / at_bats) avg from batter_stats_t where at_bats >= 300'
  result2 = Utils::transform_hash @db.execute query2

  result[0][:average] = result2[0]

  return result[0]
end

def get_best_batter_stats_by_season( season )
  query = "select max(at_bats) at_bats, max(runs) runs, max(hits) hits, max(doubles) doubles, max(triples) triples, max(home_runs) home_runs, max(runs_batted_in) runs_batted_in, max(steals) steals, max(walks) walks from batter_stats_t where season = #{season}"
  result = Utils::transform_hash @db.execute query

  query2  = "select at_bats, hits, max(cast(hits as float) / at_bats) avg from batter_stats_t where at_bats >= 300 and season = #{season}"
  result2 = Utils::transform_hash @db.execute query2

  result[0][:average] = result2[0]

  return result[0]
end

def hilite( value )
  if value == 2; then return "\e[33m\e[1m" end # bold yellow
  if value == 1; then return "\e[1m"       end # bold white
  return                     "\e[0m"           # no hilite
end

def unhilite()
  return "\e[0m"
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

  if type == :regular
    overall_best = get_best_pitcher_stats_overall()
  end

  puts "Year Team          ERA     W   L   IP     G SV    H   ER   BB   SO"

  pitcher[:stats][type].each do |stat|
    era = calc_era stat
    ip  = calc_ip  stat

    categories = {}

    if type == :regular and stat[:innings] >= 185
      season_best = get_best_pitcher_stats_by_season stat[:season]

      stat.each_pair do |k, v|
        next if overall_best[k].nil?

        if    v >= overall_best[k]; then categories[k] = 2
        elsif v >= season_best[k];  then categories[k] = 1
        else                             categories[k] = 0 end
      end

      overall_best_era = calc_era overall_best[:era]
      season_best_era  = calc_era season_best[:era]

      if    era <= overall_best_era; then categories[:era] = 2
      elsif era <= season_best_era;  then categories[:era] = 1
      else                                categories[:era] = 0 end

      overall_best_ip = calc_ip overall_best[:ip]
      season_best_ip  = calc_ip season_best[:ip]

      if    ip >= overall_best_ip; then categories[:ip] = 2
      elsif ip >= season_best_ip;  then categories[:ip] = 1
      else                              categories[:ip] = 0 end
    end

    printf "S%02d  %-10s  ", stat[ :season      ], stat[ :name        ]
    printf "%s%6.2f%s  ", hilite( categories[ :era         ]),        era,                                 unhilite()
    printf "%s%3d%s ",    hilite( categories[ :wins        ]), stat[ :wins        ],                       unhilite()
    printf "%3d ",                                             stat[ :losses      ]
    printf "%s%4d.%d%s ", hilite( categories[ :ip          ]), stat[ :innings     ], stat[ :outs        ], unhilite()
    printf "%s%3d%s ",    hilite( categories[ :games       ]), stat[ :games       ],                       unhilite()
    printf "%s%2d%s ",    hilite( categories[ :saves       ]), stat[ :saves       ],                       unhilite()
    printf "%4d ",                                             stat[ :hits        ]
    printf "%4d ",                                             stat[ :earned_runs ]
    printf "%4d ",                                             stat[ :walks       ]
    printf "%s%4d%s ",    hilite( categories[ :strike_outs ]), stat[ :strike_outs ],                       unhilite()
    printf "\n"

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

  if type == :regular
    overall_best = get_best_batter_stats_overall()
  end

  puts "Year Team          BA     G   AB    R    H  2B  3B   HR  RBI  SB   BB   SO"

  batter[:stats][type].each do |stat|
    avg = calc_avg stat

    dsp_avg = sprintf( "%5.3f", avg ).gsub '0.', ' .'

    categories = {}

    if type == :regular and stat[:at_bats] >= 300
      season_best = get_best_batter_stats_by_season stat[:season]

      stat.each_pair do |k, v|
        next if overall_best[k].nil?

        if    v >= overall_best[k]; then categories[k] = 2
        elsif v >= season_best[k];  then categories[k] = 1
        else                             categories[k] = 0 end
      end

      overall_best_avg = calc_avg overall_best[:average]
      season_best_avg  = calc_avg season_best[:average]

      if    avg >= overall_best_avg; then categories[:avg] = 2
      elsif avg >= season_best_avg;  then categories[:avg] = 1
      else                                categories[:avg] = 0 end
    end

    printf "S%02d  %-10s  ", stat[ :season ], stat[ :name   ]
    printf "%s%s%s ",  hilite( categories[ :avg            ]),        dsp_avg,          unhilite()
    printf "%4d ",                                             stat[ :games          ]
    printf "%s%4d%s ", hilite( categories[ :at_bats        ]), stat[ :at_bats        ], unhilite()
    printf "%s%4d%s ", hilite( categories[ :runs           ]), stat[ :runs           ], unhilite()
    printf "%s%4d%s ", hilite( categories[ :hits           ]), stat[ :hits           ], unhilite()
    printf "%s%3d%s ", hilite( categories[ :doubles        ]), stat[ :doubles        ], unhilite()
    printf "%s%3d%s ", hilite( categories[ :triples        ]), stat[ :triples        ], unhilite()
    printf "%s%4d%s ", hilite( categories[ :home_runs      ]), stat[ :home_runs      ], unhilite()
    printf "%s%4d%s ", hilite( categories[ :runs_batted_in ]), stat[ :runs_batted_in ], unhilite()
    printf "%s%3d%s ", hilite( categories[ :steals         ]), stat[ :steals         ], unhilite()
    printf "%s%4d%s ", hilite( categories[ :walks          ]), stat[ :walks          ], unhilite()
    printf "%4d ",                                             stat[ :strike_outs    ]
    printf "\n"

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

player = nil

if ARGV.length == 1
  player = get_player @player_id
elsif ARGV.length == 2
  player = get_player_by_name @firstname, @lastname
end

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

