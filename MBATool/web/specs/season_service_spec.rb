location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'fileutils'
require 'player_types'
require 'org_repository'
require 'org_decorator'
require 'player_repository'
require 'name_manager'
require 'player_generator'
require 'season_service'

describe SeasonService do
  before :each do
    @db = SQLite3::Database.new "test_mba.db"

    @db.results_as_hash  = true
    @db.type_translation = true

    @or = double OrgRepository
    @od = double OrgDecorator
    @pr = double PlayerRepository
    @nm = double NameManager
    @pg = double PlayerGenerator

    @season_service = SeasonService.new @db, @or, @od, @pr, @nm, @pg

    allow( @nm ).to receive :load_names
    allow( @nm ).to receive :save_names
    allow( @pg ).to receive( :generate_pitcher ).and_return Hash.new
    allow( @pg ).to receive( :generate_batter ).and_return Hash.new
    allow( @pr ).to receive :save_pitcher
    allow( @pr ).to receive :save_batter
    allow( @pr ).to receive :save_player
    allow( @or ).to receive( :get_org ).and_return( { organization_id: 1, season: 4 } )
    allow( @or ).to receive :save_org
    allow( @od ).to receive :decorate_org

    @db.transaction
  end

  after :each do
    @db.rollback
  end

  describe '#start_new_season' do
    it 'should copy team player records from the current season to the new season' do
      @db.execute 'insert into division_teams_t values (1, 1)'
      @db.execute 'insert into team_players_t values (1, 4, 1)'

      @season_service.start_new_season

      results = @db.get_first_row 'select * from team_players_t where season = 5'

      expect( results ).to_not be_nil
      expect( results ).to     be_a   Hash

      expect( results['Team_Id']   ).to eq 1
      expect( results['Season']    ).to eq 5
      expect( results['Player_Id'] ).to eq 1
    end

    it 'should not copy team player records from previous seasons' do
      @db.execute 'insert into division_teams_t values (1, 1)'
      @db.execute 'insert into team_players_t values (1, 3, 7)'
      @db.execute 'insert into team_players_t values (1, 4, 1)'

      @season_service.start_new_season

      results = @db.execute 'select * from team_players_t where season = 5'

      expect( results ).to_not be_nil
      expect( results ).to     be_a   Array

      expect( results.length ).to eq 1

      expect( results[0]['Team_Id']   ).to eq 1
      expect( results[0]['Season']    ).to eq 5
      expect( results[0]['Player_Id'] ).to eq 1
    end

    it 'should not copy team player records for allstar teams' do
      @db.execute 'insert into division_teams_t values (1, 1)'
      @db.execute 'insert into team_players_t values (2, 4, 7)'
      @db.execute 'insert into team_players_t values (1, 4, 1)'

      @season_service.start_new_season

      results = @db.execute 'select * from team_players_t where season = 5'

      expect( results ).to_not be_nil
      expect( results ).to     be_a   Array

      expect( results.length ).to eq 1

      expect( results[0]['Team_Id']   ).to eq 1
      expect( results[0]['Season']    ).to eq 5
      expect( results[0]['Player_Id'] ).to eq 1
    end

    it 'should load the player names' do
      @db.execute 'insert into team_players_t values (1, 4, 1)'

      expect( @nm ).to receive :load_names

      @season_service.start_new_season
    end

    it 'should generate 32 pitchers' do
      @db.execute 'insert into team_players_t values (1, 4, 1)'

      expect( @pg ).to receive( :generate_pitcher ).with( 5 ).exactly( 32 ).times.and_return Hash.new

      @season_service.start_new_season
    end

    it 'should generate 32 batters' do
      @db.execute 'insert into team_players_t values (1, 4, 1)'

      expect( @pg ).to receive( :generate_batter ).with( 5 ).exactly( 32 ).times

      @season_service.start_new_season
    end

    it 'should save 32 pitchers' do
      pitcher = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, details: {player_id: 1, speed: 4, control: 6, bunt: 2, fatigue: 7}}

      allow( @pg ).to receive( :generate_pitcher ).and_return pitcher

      @db.execute 'insert into team_players_t values (1, 4, 1)'

      expect( @pr ).to receive( :save_pitcher ).with( pitcher[:details] ).exactly( 32 ).times
      expect( @pr ).to receive( :save_player  ).with( pitcher           ).exactly( 32 ).times

      @season_service.start_new_season
    end

    it 'should save 32 batters' do
      batter = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, details: {player_id: 1, speed: 4, control: 6, bunt: 2, fatigue: 7}}

      allow( @pg ).to receive( :generate_batter  ).and_return batter

      @db.execute 'insert into team_players_t values (1, 4, 1)'

      expect( @pr ).to receive( :save_batter ).with( batter[:details] ).exactly( 32 ).times
      expect( @pr ).to receive( :save_player ).with( batter           ).exactly( 32 ).times

      @season_service.start_new_season
    end

    it 'should save the player names' do
      @db.execute 'insert into team_players_t values (1, 4, 1)'

      expect( @nm ).to receive :save_names

      @season_service.start_new_season
    end

    it 'should save the new season on the organization' do
      expect( @or ).to receive( :save_org ).with(1, hash_including(:season => 5))

      @season_service.start_new_season
    end

    it 'should return a hash containing the updated organization' do
      decorated_org = { _links: {}}

      @db.execute 'insert into team_players_t values (1, 4, 1)'

      expect( @od ).to receive( :decorate_org ).with(hash_including(:season => 5)).and_return decorated_org

      result = @season_service.start_new_season

      expect( result ).to be decorated_org
    end

    it 'should decorate the organization that is being returned' do
      @db.execute 'insert into team_players_t values (1, 4, 1)'

      expect( @od ).to receive( :decorate_org ).and_return({ _links: {}})

      result = @season_service.start_new_season

      expect( result          ).to_not be_nil
      expect( result          ).to     be_a   Hash
      expect( result[:_links] ).to_not be_nil
    end
  end

  xdescribe '#export_season' do
    before :each do
      @test_dir = '/tmp/export_season'
      @dos_dir  = '/tmp/dosewb'

      Dir.mkdir @test_dir
      Dir.mkdir @dos_dir
    end

    after :each do
      FileUtils.rm_rf @test_dir
      FileUtils.rm_rf @dos_dir
    end

    it 'should return an appropriate success message' do
      response = @season_service.export_season @test_dir, @dos_dir

      expect( response ).to be_a String
      expect( response ).to match /[Ss]uccess/
    end

    it 'should create the season directory' do
      @season_service.export_season @test_dir, @dos_dir

      expect( File.directory? "#{@test_dir}/S04" ).to be_truthy
    end

    it 'should export the season into the season directory' do
      @season_service.export_season @test_dir, @dos_dir

      expect( File.exists? "#{@test_dir}/S04/LeagName.Dat" ).to be_truthy
      expect( File.exists? "#{@test_dir}/S04/Players.S"    ).to be_truthy
    end

    it 'should put the parks file into the season directory' do
      @season_service.export_season @test_dir, @dos_dir

      expect( File.exists? "#{@test_dir}/S04/parks.dat" ).to be_truthy
    end

    it 'should put the import dos script into the season directory' do
      @season_service.export_season @test_dir, @dos_dir

      expect( File.exists?     "#{@test_dir}/S04/import_dos.sh" ).to be_truthy
      expect( File.executable? "#{@test_dir}/S04/import_dos.sh" ).to be_truthy
    end

    it 'should put the import amiga script in the base directory' do
      @season_service.export_season @test_dir, @dos_dir

      expect( File.exists?     "#{@test_dir}/import_amiga.sh" ).to be_truthy
      expect( File.executable? "#{@test_dir}/import_amiga.sh" ).to be_truthy
    end

    it 'should put the pregame script into the season directory' do
      @season_service.export_season @test_dir, @dos_dir

      expect( File.exists?     "#{@test_dir}/S04/pregame" ).to be_truthy
      expect( File.executable? "#{@test_dir}/S04/pregame" ).to be_truthy
    end

    it 'should put the postgame script into the season directory' do
      @season_service.export_season @test_dir, @dos_dir

      expect( File.exists?     "#{@test_dir}/S04/postgame" ).to be_truthy
      expect( File.executable? "#{@test_dir}/S04/postgame" ).to be_truthy
    end

    it 'should put the schedule into the season directory' do
      @season_service.export_season @test_dir, @dos_dir

      expect( File.exists? "#{@test_dir}/S04/schedule.csv" ).to be_truthy
    end

    it 'should put the TV games list into the season directory' do
      @season_service.export_season @test_dir, @dos_dir

      expect( File.exists? "#{@test_dir}/S04/tv_games.txt" ).to be_truthy
    end

    it 'should be idempotent' do
      @season_service.export_season @test_dir, @dos_dir
      @season_service.export_season @test_dir, @dos_dir
    end
  end

  describe '#import_season' do
    before :each do
      @import_test_dir = "#{location}/files/import"

      @import_db = SQLite3::Database.new "#{@import_test_dir}/mba.db"

      @import_db.results_as_hash  = true
      @import_db.type_translation = true
    end

    after :each do
      @import_db.execute 'delete from batter_stats_t'
      @import_db.execute 'delete from pitcher_stats_t'
      @import_db.execute 'delete from team_stats_t'
      @import_db.execute 'delete from team_versus_stats_t'
      @import_db.execute 'delete from team_batting_stats_t'
      @import_db.execute 'delete from team_pitching_stats_t'
      @import_db.execute 'delete from division_stats_t'
      @import_db.execute 'delete from league_stats_t'
    end

    it 'should import regular season stats' do
      @season_service.import_season @import_test_dir

      results = @import_db.execute 'select count(1) from batter_stats_t where season_phase = 1'

      expect( results ).to_not be_nil
      expect( results ).to     be_a   Array

      expect( results.length ).to eq 1

      expect( results[0]['count(1)'] ).to be > 0
    end

    it 'should import playoff stats' do
      @season_service.import_season @import_test_dir

      results = @import_db.execute 'select count(1) from batter_stats_t where season_phase = 2'

      expect( results ).to_not be_nil
      expect( results ).to     be_a   Array

      expect( results.length ).to eq 1

      expect( results[0]['count(1)'] ).to be > 0
    end

    it 'should import allstar stats' do
      @season_service.import_season @import_test_dir

      results = @import_db.execute 'select count(1) from batter_stats_t where season_phase = 3'

      expect( results ).to_not be_nil
      expect( results ).to     be_a   Array

      expect( results.length ).to eq 1

      expect( results[0]['count(1)'] ).to be > 0
    end
  end
end
