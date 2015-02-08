location = File.dirname __FILE__
$: << "#{location}"

require 'team_player_decorator'

describe TeamPlayerDecorator do
  before :each do
    @base_url       = 'http://www.mba.org/mba/resources'
    @team_player_decorator = TeamPlayerDecorator.new @base_url
  end

  describe '#decorate_team_player' do
    it 'should return the given team player hash with links added' do
      team_player = {team_id: 1, season: 2, player_id: 3}

      result = @team_player_decorator.decorate_team_player team_player

      expect( result                    ).to_not be_nil
      expect( result                    ).to     be_a   Hash
      expect( result[:team_id         ] ).to_not be_nil
      expect( result[:season          ] ).to_not be_nil
      expect( result[:player_id       ] ).to_not be_nil
      expect( result[:_links          ] ).to_not be_nil
    end

    it 'should add a self link' do
      team_player = {team_id: 1, season: 2, player_id: 3}

      result = @team_player_decorator.decorate_team_player team_player

      self_link = result[:_links][:self]

      expect( self_link        ).to_not be_nil
      expect( self_link[:rel ] ).to     eq     'self'
      expect( self_link[:href] ).to     eq     "#{@base_url}/teams/1/players/3/season/2"
      expect( self_link[:desc] ).to     eq     "This resource"
    end

    it 'should add a player link' do
      team_player = {team_id: 1, season: 2, player_id: 3}

      result = @team_player_decorator.decorate_team_player team_player

      player_link = result[:_links][:player]

      expect( player_link        ).to_not be_nil
      expect( player_link[:rel ] ).to     eq     'player'
      expect( player_link[:href] ).to     eq     "#{@base_url}/players/3"
      expect( player_link[:desc] ).to     eq     "Information about this player"
    end

    it 'should add a team link' do
      team_player = {team_id: 1, season: 2, player_id: 3}

      result = @team_player_decorator.decorate_team_player team_player

      team_link = result[:_links][:team]

      expect( team_link        ).to_not be_nil
      expect( team_link[:rel ] ).to     eq     'team'
      expect( team_link[:href] ).to     eq     "#{@base_url}/teams/1"
      expect( team_link[:desc] ).to     eq     "Information about this team"
    end
  end
end
