require 'utils'

class LeagueRepository

  def initialize( db )
    @db = db
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
