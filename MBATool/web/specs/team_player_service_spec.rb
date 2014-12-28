location = File.dirname __FILE__
$: << "#{location}"

require 'team_player_decorator'
require 'team_player_repository'
require 'team_player_service'

describe TeamPlayerService do
  before :each do
    @repo = double TeamPlayerRepository
    @deco = double TeamPlayerDecorator

    @team_player_service = TeamPlayerService.new @repo, @deco
  end

  describe '#get_team_player' do
    it 'should call the repository to get the team player record from the database' do
      expect( @repo ).to receive( :get_team_player ).with( 1, 2, 3 ).and_return Hash.new

      allow( @deco ).to receive( :decorate_team_player )

      @team_player_service.get_team_player 1, 2 ,3
    end

    it 'should call the decorator with the database record' do
      db_record = { team_id: 1, season: 2, player_id: 3 }

      allow( @repo ).to receive( :get_team_player ).with( 1, 2, 3 ).and_return db_record

      expect( @deco ).to receive( :decorate_team_player ).with db_record

      @team_player_service.get_team_player 1, 2 ,3
    end

    it 'should return the decorated database record' do
      decorated_db_record = { team_id: 1, season: 2, player_id: 3, _links: {} }

      allow( @repo ).to receive( :get_team_player ).and_return Hash.new
      allow( @deco ).to receive( :decorate_team_player ).and_return decorated_db_record

      expect( @team_player_service.get_team_player 1, 2 ,3 ).to be decorated_db_record
    end

    it 'should throw a resource not found error exception if the record is not in the database' do
      allow( @repo ).to receive( :get_team_player ).with( 1, 2, 3 )

      expect { @team_player_service.get_team_player 1, 2 ,3 }.to raise_error ResourceNotFoundError, 'Team Player with Team ID 1, Player ID 3 and Season 2 cannot be located.'
    end
  end

  describe '#save_team_player' do
    it 'should call the repository to save the team player record to the database' do
      expect( @repo ).to receive( :save_team_player ).with( 1, 2, 3 )

      allow( @repo ).to receive( :get_team_player ).and_return Hash.new
      allow( @deco ).to receive( :decorate_team_player )

      @team_player_service.save_team_player 1, 2 ,3
    end

    it 'should return the newly created team player record with decorations' do
      db_record = { team_id: 1, season: 2, player_id: 3 }
      decorated_db_record = { team_id: 1, season: 2, player_id: 3, _links: {} }

      allow( @repo ).to receive( :save_team_player ).with( 1, 2, 3 )

      expect( @repo ).to receive( :get_team_player ).with( 1, 2, 3 ).and_return db_record
      expect( @deco ).to receive( :decorate_team_player ).with( db_record ).and_return decorated_db_record

      expect( @team_player_service.save_team_player 1, 2 ,3 ).to be decorated_db_record
    end

    it 'should raise internal server error if the record is not saved' do
      allow( @repo ).to receive( :save_team_player ).with( 1, 2, 3 )
      allow( @repo ).to receive( :get_team_player )

      expect { @team_player_service.save_team_player 1, 2 ,3 }.to raise_error InternalServerError, 'Team Player with Team ID 1, Player ID 3 and Season 2 was not saved successfully.'
    end
  end

  describe '#delete_team_player' do
    it 'should call the repository to delete the team player record from the database' do
      allow( @repo ).to receive( :get_team_player )

      expect( @repo ).to receive( :delete_team_player ).with( 1, 2, 3 )

      @team_player_service.delete_team_player 1, 2 ,3
    end

    it 'should return an empty hash' do
      allow( @repo ).to receive( :delete_team_player )
      allow( @repo ).to receive( :get_team_player )

      result = @team_player_service.delete_team_player 1, 2 ,3

      expect( result        ).to be_a Hash
      expect( result.length ).to eq 0
    end

    it 'should raise internal server error if the record is not deleted' do
      allow( @repo ).to receive( :delete_team_player )
      allow( @repo ).to receive( :get_team_player ).and_return Hash.new

      expect { @team_player_service.delete_team_player 1, 2 ,3 }.to raise_error InternalServerError, 'Team Player with Team ID 1, Player ID 3 and Season 2 was not deleted successfully.'
    end
  end
end
