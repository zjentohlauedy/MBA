location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'team_player_repository'

describe TeamPlayerRepository do
  before :each do
    @db = SQLite3::Database.new "test_mba.db"

    @db.results_as_hash  = true
    @db.type_translation = true

    @team_player_repository = TeamPlayerRepository.new @db

    @db.transaction
  end

  after :each do
    @db.rollback
  end

  describe '#get_team_player' do
    it 'should return a hash containing team player information' do
      @db.execute 'insert into team_players_t values ( 99, 5, 123 )'

      result = @team_player_repository.get_team_player 99, 5, 123

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:team_id  ] ).to     eq     99
      expect( result[:season   ] ).to     eq     5
      expect( result[:player_id] ).to     eq     123
    end

    it 'should return nil if record is not found' do
      result = @team_player_repository.get_team_player 99, 5, 123

      expect( result ).to be_nil
    end
  end

  describe '#save_team_player' do
    it 'should return an empty array on success' do
      result = @team_player_repository.save_team_player 99, 5, 123

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should insert a team_players_t record' do
      @team_player_repository.save_team_player 99, 5, 123

      result = @db.get_first_row 'select * from team_players_t where team_id = 99'

      expect( result              ).to_not be_nil
      expect( result              ).to     be_a   Hash
      expect( result['Team_Id'  ] ).to     eq     99
      expect( result['Season'   ] ).to     eq     5
      expect( result['Player_Id'] ).to     eq     123
    end

    it 'should throw an exception on failure' do
      @team_player_repository.save_team_player 99, 5, 123

      expect { @team_player_repository.save_team_player 99, 5, 123 }.to raise_error SQLite3::ConstraintException
    end
  end

  describe '#delete_team_player' do
    it 'should return an empty array on success' do
      @db.execute 'insert into team_players_t values ( 99, 5, 123 )'

      result = @team_player_repository.delete_team_player 99, 5, 123

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should remove the team_players_t record from the database' do
      @db.execute 'insert into team_players_t values ( 99, 5, 123 )'

      @team_player_repository.delete_team_player 99, 5, 123

      result = @db.get_first_row 'select * from team_players_t where team_id = 99'

      expect( result ).to be_nil
    end
  end
end
