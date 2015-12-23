require 'utils'

class PlayerRepository

  def initialize( db )
    @db = db
  end

  def get_players_by_team( team_id, season = nil )
    args = { team_id: team_id }

    query = 'select * from team_players_t as tp natural join players_t as p where tp.team_id = :team_id'

    unless season.nil?
      query = "#{query} and tp.season = :season"; args[:season] = season
    end

    return Utils::transform_hash @db.execute query, args
  end

  def get_players
    return Utils::transform_hash @db.execute 'select * from players_t'
  end

  def get_players_by_rookie_season( season )
    args = { season: season }

    query = 'select * from players_t where rookie_season = :season and player_id not in (select player_id from team_players_t where season = :season)'

    return Utils::transform_hash @db.execute query, args
  end

  def get_free_agents_by_season( season )
    args = { season: season }

    query = 'select * from players_t where player_id not in (select player_id from team_players_t where season = :season)'

    return Utils::transform_hash @db.execute query, args
  end

  def get_player( player_id )
    args = { player_id: player_id}

    query = 'select * from players_t where player_id = :player_id'

    return Utils::transform_hash @db.get_first_row query, args
  end

  def get_pitcher_details( player_id )
    args = { player_id: player_id}

    query = 'select * from pitchers_t where player_id = :player_id'

    return Utils::transform_hash @db.get_first_row query, args
  end

  def get_batter_details( player_id )
    args = { player_id: player_id}

    query = 'select * from batters_t where player_id = :player_id'

    return Utils::transform_hash @db.get_first_row query, args
  end

  def get_pitcher_stats( player_id, season = nil, phase = nil )
    args = { player_id: player_id}

    query = 'select * from pitcher_stats_t where player_id = :player_id'

    unless season.nil?; query = "#{query} and season        = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase  = :phase";  args[:phase ] = phase  end

    unless season.nil? or phase.nil?
      return Utils::transform_hash @db.get_first_row query, args
    end

    return Utils::transform_hash @db.execute query, args
  end

  def get_pitcher_stats_by_highest( stat, qualifying_stat, qualifying_value, season = nil, phase = nil )
    args = {qualifying_value: qualifying_value}

    query = "select max(#{stat}) value from pitcher_stats_v where #{qualifying_stat} >= :qualifying_value"

    unless season.nil?; query = "#{query} and season       = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase = :phase";  args[:phase ] = phase  end

    result = @db.get_first_row query, args

    args = {value: result['value']}

    query = "select * from pitcher_stats_v where #{stat} = :value"

    unless season.nil?; query = "#{query} and season       = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase = :phase";  args[:phase ] = phase  end

    return Utils::transform_hash @db.execute query, args
  end

  def get_pitcher_stats_by_lowest( stat, qualifying_stat, qualifying_value, season = nil, phase = nil )
    args = {qualifying_value: qualifying_value}

    query = "select min(#{stat}) value from pitcher_stats_v where #{qualifying_stat} >= :qualifying_value"

    unless season.nil?; query = "#{query} and season       = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase = :phase";  args[:phase ] = phase  end

    result = @db.get_first_row query, args

    args = {value: result['value']}

    query = "select * from pitcher_stats_v where #{stat} = :value"

    unless season.nil?; query = "#{query} and season       = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase = :phase";  args[:phase ] = phase  end

    return Utils::transform_hash @db.execute query, args
  end

  def get_batter_stats( player_id, season = nil, phase = nil )
    args = { player_id: player_id}

    query = 'select * from batter_stats_t where player_id = :player_id'

    unless season.nil?; query = "#{query} and season        = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase  = :phase";  args[:phase ] = phase  end

    unless season.nil? or phase.nil?
      return Utils::transform_hash @db.get_first_row query, args
    end

    return Utils::transform_hash @db.execute query, args
  end

  def get_batter_stats_by_highest( stat, qualifying_stat, qualifying_value, season = nil, phase = nil )
    args = {qualifying_value: qualifying_value}

    query = "select max(#{stat}) value from batter_stats_v where #{qualifying_stat} >= :qualifying_value"

    unless season.nil?; query = "#{query} and season       = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase = :phase";  args[:phase ] = phase  end

    result = @db.get_first_row query, args

    args = {value: result['value']}

    query = "select * from batter_stats_v where #{stat} = :value"

    unless season.nil?; query = "#{query} and season       = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase = :phase";  args[:phase ] = phase  end

    return Utils::transform_hash @db.execute query, args
  end

  def get_batter_stats_by_lowest( stat, qualifying_stat, qualifying_value, season = nil, phase = nil )
    args = {qualifying_value: qualifying_value}

    query = "select min(#{stat}) value from batter_stats_v where #{qualifying_stat} >= :qualifying_value"

    unless season.nil?; query = "#{query} and season       = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase = :phase";  args[:phase ] = phase  end

    result = @db.get_first_row query, args

    args = {value: result['value']}

    query = "select * from batter_stats_v where #{stat} = :value"

    unless season.nil?; query = "#{query} and season       = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase = :phase";  args[:phase ] = phase  end

    return Utils::transform_hash @db.execute query, args
  end

  def get_player_accolades( player_id, season = nil )
    args = { player_id: player_id }

    query = 'select * from player_accolades_t where player_id = :player_id'

    unless season.nil?; query = "#{query} and season = :season"; args[:season] = season end

    return Utils::transform_hash @db.execute query, args
  end

  def get_player_accolade( player_id, season, accolade )
    args = { player_id: player_id, season: season, accolade: accolade }

    query = 'select * from player_accolades_t where player_id = :player_id and season = :season and accolade = :accolade'

    return Utils::transform_hash @db.get_first_row query, args
  end

  def get_pitcher_accolade( player_id, season, accolade )
    args = { player_id: player_id, season: season, accolade: accolade }

    query = 'select * from pitcher_accolades_t where player_id = :player_id and season = :season and accolade = :accolade'

    return Utils::transform_hash @db.get_first_row query, args
  end

  def get_pitcher_accolades( player_id, season = nil )
    args = { player_id: player_id }

    query = 'select * from pitcher_accolades_t where player_id = :player_id'

    unless season.nil?; query = "#{query} and season = :season"; args[:season] = season end

    return Utils::transform_hash @db.execute query, args
  end

  def get_batter_accolade( player_id, season, accolade )
    args = { player_id: player_id, season: season, accolade: accolade }

    query = 'select * from batter_accolades_t where player_id = :player_id and season = :season and accolade = :accolade'

    return Utils::transform_hash @db.get_first_row query, args
  end

  def get_batter_accolades( player_id, season = nil )
    args = { player_id: player_id }

    query = 'select * from batter_accolades_t where player_id = :player_id'

    unless season.nil?; query = "#{query} and season = :season"; args[:season] = season end

    return Utils::transform_hash @db.execute query, args
  end

  def save_player( player )
    args = {}

    query = 'insert into players_t values ( :player_id, :first_name, :last_name, :first_phoenetic, :last_phoenetic, :skin_tone, :handedness, :player_type, :rookie_season, :longevity )'

    args[ :player_id       ] = player[ :player_id       ]
    args[ :first_name      ] = player[ :first_name      ]
    args[ :last_name       ] = player[ :last_name       ]
    args[ :first_phoenetic ] = player[ :first_phoenetic ]
    args[ :last_phoenetic  ] = player[ :last_phoenetic  ]
    args[ :skin_tone       ] = player[ :skin_tone       ]
    args[ :handedness      ] = player[ :handedness      ]
    args[ :player_type     ] = player[ :player_type     ]
    args[ :rookie_season   ] = player[ :rookie_season   ]
    args[ :longevity       ] = player[ :longevity       ]

    @db.execute query, args
  end

  def save_pitcher( pitcher )
    args = {}

    query = 'insert into pitchers_t values ( :player_id, :speed, :control, :bunt, :fatigue )'

    args[ :player_id ] = pitcher[ :player_id ]
    args[ :speed     ] = pitcher[ :speed     ]
    args[ :control   ] = pitcher[ :control   ]
    args[ :bunt      ] = pitcher[ :bunt      ]
    args[ :fatigue   ] = pitcher[ :fatigue   ]

    @db.execute query, args
  end

  def save_batter( batter )
    args = {}

    query = 'insert into batters_t values ( :player_id, :power, :hit_n_run, :bunt, :running, :range, :arm, :primary_position, :secondary_position )'

    args[ :player_id          ] = batter[ :player_id          ]
    args[ :primary_position   ] = batter[ :primary_position   ]
    args[ :secondary_position ] = batter[ :secondary_position ]
    args[ :power              ] = batter[ :power              ]
    args[ :hit_n_run          ] = batter[ :hit_n_run          ]
    args[ :bunt               ] = batter[ :bunt               ]
    args[ :running            ] = batter[ :running            ]
    args[ :range              ] = batter[ :range              ]
    args[ :arm                ] = batter[ :arm                ]

    @db.execute query, args
  end

  def save_player_accolade( player_accolade )
    result = nil
    args   = {}

    query = 'insert into player_accolades_t values ( :player_id, :season, :accolade )'

    args[ :player_id ] = player_accolade[ :player_id ]
    args[ :season    ] = player_accolade[ :season    ]
    args[ :accolade  ] = player_accolade[ :accolade  ]

    begin
      result = @db.execute query, args
    rescue SQLite3::ConstraintException
    end

    return result
  end

  def save_pitcher_accolade( pitcher_accolade )
    result = nil
    args   = {}

    query = 'insert into pitcher_accolades_t values ( :player_id, :season, :accolade )'

    args[ :player_id ] = pitcher_accolade[ :player_id ]
    args[ :season    ] = pitcher_accolade[ :season    ]
    args[ :accolade  ] = pitcher_accolade[ :accolade  ]

    begin
      result = @db.execute query, args
    rescue SQLite3::ConstraintException
    end

    return result
  end

  def save_batter_accolade( batter_accolade )
    result = nil
    args   = {}

    query = 'insert into batter_accolades_t values ( :player_id, :season, :accolade )'

    args[ :player_id ] = batter_accolade[ :player_id ]
    args[ :season    ] = batter_accolade[ :season    ]
    args[ :accolade  ] = batter_accolade[ :accolade  ]

    begin
      result = @db.execute query, args
    rescue SQLite3::ConstraintException
    end

    return result
  end
end
