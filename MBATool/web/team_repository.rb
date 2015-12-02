require 'utils'

class TeamRepository

  def initialize( db )
    @db = db
  end

  def get_team( team_id )
    args = { team_id: team_id }

    query = 'select * from teams_t where team_id = :team_id'

    return Utils::transform_hash @db.get_first_row query, args
  end

  def get_team_stats( team_id, season = nil, phase = nil )
    args = { team_id: team_id }

    query = 'select * from team_stats_t where team_id = :team_id'

    unless season.nil?; query = "#{query} and season        = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase  = :phase";  args[:phase ] = phase  end

    unless season.nil? or phase.nil?
      return Utils::transform_hash @db.get_first_row query, args
    end

    return Utils::transform_hash @db.execute query, args
  end

  def get_team_versus_stats( team_id, season, season_phase, opponent )
    args = { team_id: team_id, season: season, season_phase: season_phase, opponent: opponent }

    query = 'select * from team_versus_stats_t where team_id = :team_id and season = :season and season_phase = :season_phase and opponent = :opponent'

    return Utils::transform_hash @db.get_first_row query, args
  end

  def get_division_teams
    query = 'select * from division_teams_t dt join teams_t t on dt.team_id = t.team_id'

    return Utils::transform_hash @db.execute query
  end

  def get_division_teams_with_stats( season, phase )
    args = { season: season, phase: phase }

    query = '''select * from division_teams_t dt
                          join teams_t           t on dt.team_id =  t.team_id
                          join team_stats_t     ts on dt.team_id = ts.team_id
                 where ts.season       = :season
                 and   ts.season_phase = :phase
              '''

    return Utils::transform_hash @db.execute query, args
  end

  def get_team_accolades( team_id, season = nil )
    args = { team_id: team_id }

    query = 'select * from team_accolades_t where team_id = :team_id'

    unless season.nil?; query = "#{query} and season = :season"; args[:season] = season end

    return Utils::transform_hash @db.execute query, args
  end

  def save_team_accolade( team_accolade )
    result = nil
    args   = {}

    query = 'insert into team_accolades_t values ( :team_id, :season, :accolade )'

    args[ :team_id  ] = team_accolade[ :team_id  ]
    args[ :season   ] = team_accolade[ :season   ]
    args[ :accolade ] = team_accolade[ :accolade ]

    begin
      result = @db.execute query, args
    rescue SQLite3::ConstraintException
    end
  end
end
