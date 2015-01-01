require 'utils'

class SeasonService

  def initialize( db, player_repository, name_manager, player_generator )
    @db                = db
    @player_repository = player_repository
    @name_manager      = name_manager
    @player_generator  = player_generator
  end

  def start_new_season
    current_season = get_current_season
    next_season    = current_season + 1

    copy_team_players_for_new_season current_season, next_season

    @name_manager.load_names

    (1..32).each do
      rookie_pitcher = @player_generator.generate_pitcher next_season

      @player_repository.save_pitcher rookie_pitcher[:details]
      @player_repository.save_player  rookie_pitcher
    end

    (1..32).each do
      rookie_batter = @player_generator.generate_batter next_season

      @player_repository.save_batter rookie_batter[:details]
      @player_repository.save_player rookie_batter
    end

    @name_manager.save_names

    return { season: next_season }
  end

  def copy_team_players_for_new_season( current, new )
    args = { current: current, new: new }

    query = '''insert into team_players_t (team_id, season, player_id)
                   select team_id, :new, player_id from team_players_t where season = :current
                     and team_id in (select distinct team_id from division_teams_t)
              '''

    @db.execute query, args
  end

  def get_current_season
    result = @db.get_first_row 'select max(season) season from team_players_t'

    return result['season']
  end

  private :get_current_season, :copy_team_players_for_new_season
end
