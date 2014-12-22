require 'utils'

class TeamPlayerRepository

  def initialize( db )
    @db = db
  end

  def get_team_player( team_id, season, player_id )
    args = { team_id: team_id, season: season, player_id: player_id }

    query = 'select * from team_players_t where team_id = :team_id and season = :season and player_id = :player_id'

    return Utils::transform_hash @db.get_first_row query, args
  end

  def save_team_player( team_id, season, player_id )
    args = { team_id: team_id, season: season, player_id: player_id }

    query = 'insert into team_players_t values ( :team_id, :season, :player_id )'

    return @db.execute query, args
  end

  def delete_team_player( team_id, season, player_id )
    args = { team_id: team_id, season: season, player_id: player_id }

    query = 'delete from team_players_t where team_id = :team_id and season = :season and player_id = :player_id'

    return @db.execute query, args
  end
end
