require 'erb'
require 'fileutils'
require 'utils'
require 'prog_runner'

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

  def export_season( base_path, dos_path )
    location = File.dirname __FILE__

    organization   = @org_repository.get_org 1
    current_season = organization[:season]

    season_name = sprintf 'S%02d', current_season

    season_dir = "#{base_path}/#{season_name}"

    unless File.exists? season_dir
      begin
        Dir.mkdir season_dir
      rescue Exception => e
        puts "Error creating season directory: #{e.message}"
        raise InternalServerError.new 'Error: cannot create season directory.'
      end
    end

    unless File.exists? "#{season_dir}/LeagName.Dat"
      export_prog = ProgRunner.new "#{location}/..", "export_season"

      export_prog.execute "#{base_path}/mba.db", season_dir, current_season

      if not export_prog.success?
        puts "Error exporting season: #{export_prog.get_output}"
        raise InternalServerError.new "Error: cannot export season #{season_name}."
      end
    end

    unless File.exists? "#{season_dir}/parks.dat"
      begin
        FileUtils.cp "#{location}/files/parks.dat", "#{season_dir}/parks.dat"
      rescue Exception => e
        puts "Error creating parks file: #{e.message}"
        raise InternalServerError.new 'Error: cannot create parks file.'
      end
    end

    unless File.exists? "#{season_dir}/import_dos.sh"
      begin
        template = File.read "#{location}/templates/import_dos.sh.erb"

        result = ERB.new( template ).result binding

        File.write "#{season_dir}/import_dos.sh", result
        File.chmod 0755, "#{season_dir}/import_dos.sh"
      rescue Exception => e
        puts "Error creating DOS import script: #{e.message}"
        raise InternalServerError.new 'Error: cannot create DOS import script.'
      end
    end

    unless File.exists? "#{season_dir}/import_amiga.sh"
      begin
        template = File.read "#{location}/templates/import_amiga.sh.erb"

        result = ERB.new( template ).result binding

        File.write "#{base_path}/import_amiga.sh", result
        File.chmod 0755, "#{base_path}/import_amiga.sh"
      rescue Exception => e
        puts "Error creating Amiga import script: #{e.message}"
        raise InternalServerError.new 'Error: cannot create Amiga import script.'
      end
    end

    unless File.exists? "#{season_dir}/pregame"
      begin
        template = File.read "#{location}/templates/pregame.erb"

        result = ERB.new( template ).result binding

        File.write "#{season_dir}/pregame", result
        File.chmod 0755, "#{season_dir}/pregame"
      rescue Exception => e
        puts "Error creating pregame script: #{e.message}"
        raise InternalServerError.new 'Error: cannot create pregame script.'
      end
    end

    unless File.exists? "#{season_dir}/postgame"
      begin
        template = File.read "#{location}/templates/postgame.erb"

        result = ERB.new( template ).result binding

        File.write "#{season_dir}/postgame", result
        File.chmod 0755, "#{season_dir}/postgame"
      rescue Exception => e
        puts "Error creating postgame script: #{e.message}"
        raise InternalServerError.new 'Error: cannot create postgame script.'
      end
    end

    unless File.exists? "#{season_dir}/schedule.csv"
      schedule_prog = ProgRunner.new "#{location}/../..", "schedule"

      schedule_prog.execute

      if not schedule_prog.success?
        puts "Error generating schedule: #{schedule_prog.get_output}"
        raise InternalServerError.new "Error: cannot generate schedule."
      end

      begin
        File.write "#{season_dir}/schedule.csv", schedule_prog.get_output
      rescue Exception => e
        puts "Error writing schedule file: #{e.message}"
        raise InternalServerError.new 'Error: cannot generate schedule.'
      end
    end

    unless File.exists? "#{season_dir}/tv_games.txt"
      tv_games_prog = ProgRunner.new "#{location}/../..", "pick_tv_games.rb"

      tv_games_prog.execute "#{season_dir}/schedule.csv"

      if not tv_games_prog.success?
        puts "Error choosing TV games: #{tv_games_prog.get_output}"
        raise InternalServerError.new "Error: cannot determine TV games."
      end

      begin
        File.write "#{season_dir}/tv_games.txt", tv_games_prog.get_output
      rescue Exception => e
        puts "Error writing TV games file: #{e.message}"
        raise InternalServerError.new 'Error: cannot determine TV games.'
      end
    end

    return "Season #{season_name} was exported successfully!"
  end

  def import_season( base_path )
    location = File.dirname __FILE__

    organization   = @org_repository.get_org 1
    current_season = organization[:season]

    season_name = sprintf 'S%02d', current_season

    season_dir = "#{base_path}/#{season_name}"

    import_prog = ProgRunner.new "#{location}/../", "import_season"

    import_prog.execute "#{base_path}/mba.db", "#{season_dir}/RegularSeason", "#{season_dir}/schedule.csv", current_season.to_s, 'R'

    if not import_prog.success?
      puts "Error importing regular season: #{import_prog.get_output}"
      raise InternalServerError.new "Error: cannot import regular season."
    end

    import_prog.execute "#{base_path}/mba.db", "#{season_dir}/Playoffs", "#{season_dir}/schedule.csv", current_season.to_s, 'P'

    if not import_prog.success?
      puts "Error importing regular season: #{import_prog.get_output}"
      raise InternalServerError.new "Error: cannot import playoffs."
    end

    import_prog.execute "#{base_path}/mba.db", "#{season_dir}/Allstar", "#{season_dir}/schedule.csv", current_season.to_s, 'A'

    if not import_prog.success?
      puts "Error importing regular season: #{import_prog.get_output}"
      raise InternalServerError.new "Error: cannot import allstars."
    end
  end

  private :copy_team_players_for_new_season
end
