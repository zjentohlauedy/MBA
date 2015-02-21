require 'utils'

class SeasonService

  def initialize( db, org_repository, org_decorator, player_repository, name_manager, player_generator )
    @db                = db
    @org_repository    = org_repository
    @org_decorator     = org_decorator
    @player_repository = player_repository
    @name_manager      = name_manager
    @player_generator  = player_generator
  end

  def start_new_season
    organization   = @org_repository.get_org 1
    current_season = organization[:season]
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

    organization[:season] = next_season

    @org_repository.save_org organization[:organization_id], { season: next_season }

    return @org_decorator.decorate_org organization
  end

  def copy_team_players_for_new_season( current, new )
    args = { current: current, new: new }

    query = '''insert into team_players_t (team_id, season, player_id)
                   select team_id, :new, player_id from team_players_t where season = :current
                     and team_id in (select distinct team_id from division_teams_t)
              '''

    @db.execute query, args
  end

  private :copy_team_players_for_new_season
end
