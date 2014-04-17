#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'sqlite3'

class Repository

    def initialize( mapper )
      @db     = nil
      @mapper = mapper
      connect
    end

    def connect
#      @db = SQLite3::Database.new "/home/zjentohlauedy/Amiga/HD/MBA/mba.db"
      @db = SQLite3::Database.new "mba.db"

      @db.results_as_hash  = true
      @db.type_translation = true
    end

    def disconnect
      @db.close
    end

    def start_transaction
      @db.transaction
    end

    def commit
      @db.commit
    end

    def rollback
      @db.rollback
    end

    def get_teams
      return @mapper.map_teams_response @db.execute 'select * from teams_t where team_id in (select distinct team_id from division_teams_t)'
    end

    def get_teams_with_stats( params )
      args = {}

      query = '''select * from division_teams_t dt
                          join teams_t           t on dt.team_id =  t.team_id
                          join team_stats_t     ts on dt.team_id = ts.team_id
                 where ts.season       = :season
                 and   ts.season_phase = :phase
              '''

      args[:season] = params[:season]
      args[:phase ] = params[:phase ]

      return @db.execute query, args
    end

    def get_team( params )
      args = {}

      query = 'select * from teams_t where team_id = :team_id'; args[:team_id] = params[:team_id]

      return @mapper.map_team_response (@db.execute query, args)[0]
    end

    def get_team_stats( params )
      args = {}

      query = 'select * from team_stats_t where team_id = :team_id'; args[:team_id] = params[:team_id]

      if params.has_key? "season"; query = "#{query} and season        = :season"; args[:season] = params[:season] end
      if params.has_key? "phase";  query = "#{query} and season_phase  = :phase";  args[:phase ] = params[:phase ] end

      return @mapper.map_team_stats_response @db.execute query, args
    end

    def get_team_players( params )
      args = {}

      query = 'select * from team_players_t as tp natural join players_t as p where tp.team_id = :team_id'; args[:team_id] = params[:team_id]

      if params.has_key? "season"
        query = "#{query} and tp.season = :season"; args[:season] = params[:season]
      end

      return @mapper.map_players_response @db.execute query, args
    end

    def get_team_player( params )
      args = {}

      query = 'select * from team_players_t where team_id = :team_id and player_id = :player_id and season = :season'

      args[ :team_id   ] = params[ :team_id   ]
      args[ :player_id ] = params[ :player_id ]
      args[ :season    ] = params[ :season    ]

      return @mapper.map_team_player_response @db.get_first_row query, args
    end

    def save_team_player( params )
      args = {}

      query = 'insert into team_players_t values ( :team_id, :season, :player_id )'

      args[ :team_id   ] = params[ :team_id   ]
      args[ :player_id ] = params[ :player_id ]
      args[ :season    ] = params[ :season    ]

      return @db.execute query, args
    end

    def delete_team_player( params )
      args = {}

      query = 'delete from team_players_t where team_id = :team_id and player_id = :player_id and season = :season'

      args[ :team_id   ] = params[ :team_id   ]
      args[ :player_id ] = params[ :player_id ]
      args[ :season    ] = params[ :season    ]

      return @db.execute query, args
    end

    def get_players
      return @mapper.map_players_response @db.execute 'select * from players_t'
    end

    def get_rookies( params )
      args = {}

      query = 'select * from players_t where rookie_season = :season'; args[:season] = params[:season]

      return @mapper.map_players_response @db.execute query, args
    end

    def get_free_agents( params )
      args = {}

      query = 'select * from players_t where player_id not in (select player_id from team_players_t where season = :season)'; args[:season] = params[:season]

      return @mapper.map_players_response @db.execute query, args
    end

    def get_player( params )
      args = {}

      query = 'select * from players_t where player_id = :player_id'; args[:player_id] = params[:player_id]

      player = (@db.execute query, args)[0]

      player['Details'] = get_player_details player

      if ! params[:season].nil?
        player['Season'] = params[:season].to_i
      end

      return @mapper.map_player_response player
    end

    def get_player_details( player )
      args = {}

      if player['Player_Type'] == 1
        query = 'select * from pitchers_t where player_id = :player_id'; args[:player_id] = player['Player_Id']
      end

      if player['Player_Type'] == 2
        query = 'select * from batters_t where player_id = :player_id'; args[:player_id] = player['Player_Id']
      end

      return (@db.execute query, args)[0]
    end

    def get_player_stats( params )
      args = {}

      query = 'select player_type from players_t where player_id = :player_id'; args[:player_id] = params[:player_id]

      player = (@db.execute query, args)[0]

      if player['Player_Type'] == 1
        return get_pitcher_stats params
      end

      if player['Player_Type'] == 2
        return get_batter_stats params
      end

      return []
    end

    def get_pitcher_stats( params )
      args = {}

      query = 'select * from pitcher_stats_t where player_id = :player_id'; args[:player_id] = params[:player_id]

      if params.has_key? "season"; query = "#{query} and season        = :season"; args[:season] = params[:season] end
      if params.has_key? "phase";  query = "#{query} and season_phase  = :phase";  args[:phase ] = params[:phase ] end

      return @mapper.map_pitcher_stats_response @db.execute query, args
    end

    def get_batter_stats( params )
      args = {}

      query = 'select * from batter_stats_t where player_id = :player_id'; args[:player_id] = params[:player_id]

      if params.has_key? "season"; query = "#{query} and season        = :season"; args[:season] = params[:season] end
      if params.has_key? "phase";  query = "#{query} and season_phase  = :phase";  args[:phase ] = params[:phase ] end

      return @mapper.map_batter_stats_response @db.execute query, args
    end

    def get_max_player_id
      args = {}

      query = 'select max(player_id) Player_Id from players_t'

      return (@db.execute query, args)[0]
    end

    def get_current_season
      args = {}

      query = 'select max(season) Season from team_players_t'

      return (@db.execute query, args)[0]
    end

    def copy_team_players_for_new_season( current, new )
      args = {}

      query = '''insert into team_players_t (team_id, season, player_id)
                   select team_id, :new, player_id from team_players_t where season = :current
                     and team_id in (select distinct team_id from division_teams_t)
              '''

      args[:current] = current
      args[:new    ] = new

      @db.execute query, args
    end

    def save_player( params )
      args = {}

      query = 'insert into players_t values ( :player_id, :first_name, :last_name, :first_phoenetic, :last_phoenetic, :skin_tone, :handedness, :player_type, :rookie_season, :longevity )'

      args[ :player_id       ] = params[ :player_id       ]
      args[ :first_name      ] = params[ :first_name      ]
      args[ :last_name       ] = params[ :last_name       ]
      args[ :first_phoenetic ] = params[ :first_phoenetic ]
      args[ :last_phoenetic  ] = params[ :last_phoenetic  ]
      args[ :skin_tone       ] = params[ :skin_tone       ]
      args[ :handedness      ] = params[ :handedness      ]
      args[ :player_type     ] = params[ :player_type     ]
      args[ :rookie_season   ] = params[ :rookie_season   ]
      args[ :longevity       ] = params[ :longevity       ]

      @db.execute query, args
    end

    def save_pitcher( params )
      args = {}

      query = 'insert into pitchers_t values ( :player_id, :speed, :control, :bunt, :fatigue )'

      args[ :player_id ] = params[ :player_id ]
      args[ :speed     ] = params[ :speed     ]
      args[ :control   ] = params[ :control   ]
      args[ :bunt      ] = params[ :bunt      ]
      args[ :fatigue   ] = params[ :fatigue   ]

      @db.execute query, args
    end

    def save_batter( params )
      args = {}

      query = 'insert into batters_t values ( :player_id, :power, :hit_n_run, :bunt, :running, :range, :arm, :primary_position, :secondary_position )'

      args[ :player_id          ] = params[ :player_id          ]
      args[ :primary_position   ] = params[ :primary_position   ]
      args[ :secondary_position ] = params[ :secondary_position ]
      args[ :power              ] = params[ :power              ]
      args[ :hit_n_run          ] = params[ :hit_n_run          ]
      args[ :bunt               ] = params[ :bunt               ]
      args[ :running            ] = params[ :running            ]
      args[ :range              ] = params[ :range              ]
      args[ :arm                ] = params[ :arm                ]

      @db.execute query, args
    end

end
