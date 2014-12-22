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

  def get_division_teams
    query = 'select * from division_teams_t dt join teams_t t on dt.team_id = t.team_id'

    return Utils::transform_hash @db.execute query
  end

  def get_division_teams_with_stats( season, phase )
    args = { season: season, phase: phase}

    query = '''select * from division_teams_t dt
                          join teams_t           t on dt.team_id =  t.team_id
                          join team_stats_t     ts on dt.team_id = ts.team_id
                 where ts.season       = :season
                 and   ts.season_phase = :phase
              '''

    return Utils::transform_hash @db.execute query, args
  end
end
