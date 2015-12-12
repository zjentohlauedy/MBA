require 'utils'

class LeagueRepository

  def initialize( db )
    @db = db
  end

  def get_league_stats_by_highest( stat, season = nil, phase = nil )
    args = {}

    query = "select max(#{stat}) value from league_stats_t"

    unless season.nil?; query = "#{query} where season     = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase = :phase";  args[:phase ] = phase  end

    result = @db.get_first_row query, args

    args[:value] = result['value']

    query = "select * from league_stats_t where #{stat} = :value"

    unless season.nil?; query = "#{query} and season       = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase = :phase";  args[:phase ] = phase  end

    return Utils::transform_hash @db.execute query, args
  end

  def get_league_accolades( league_id, season = nil )
    args = { league_id: league_id }

    query = 'select * from league_accolades_t where league_id = :league_id'

    unless season.nil?; query = "#{query} and season = :season"; args[:season] = season end

    return Utils::transform_hash @db.execute query, args
  end

  def save_league_accolade( league_accolade )
    result = nil
    args   = {}

    query = 'insert into league_accolades_t values ( :league_id, :season, :accolade )'

    args[ :league_id ] = league_accolade[ :league_id ]
    args[ :season    ] = league_accolade[ :season    ]
    args[ :accolade  ] = league_accolade[ :accolade  ]

    begin
      result = @db.execute query, args
    rescue SQLite3::ConstraintException
    end
  end
end
