location = File.dirname __FILE__
$: << "#{location}"

require 'team_decorator'
require 'team_repository'
require 'team_response_mapper'
require 'team_service'

describe TeamService do
  before :each do
    @repo   = double TeamRepository
    @deco   = double TeamDecorator
    @mapper = double TeamResponseMapper

    @team_service = TeamService.new @repo, @mapper, @deco
  end

  describe '#get_teams' do
    it 'should call the repository to get the team records from the database' do
      allow( @mapper ).to receive( :map_team_list ).and_return Array.new

      expect( @repo ).to receive :get_division_teams

      @team_service.get_teams
    end

    it 'should call the response mapper with the team list from the repository' do
      team_list = [{team_id: 1, name: 'Team1'},{team_id: 2, name: 'Team2'},{team_id: 3, name: 'Team3'}]

      allow( @repo ).to receive( :get_division_teams ).and_return team_list

      expect( @mapper ).to receive( :map_team_list ).with( team_list ).and_return Array.new

      @team_service.get_teams
    end

    it 'should call the decorator with each team in the mapped team list' do
      mapped_team_list = [{team_id: 1, name: 'Team1'},{team_id: 2, name: 'Team2'},{team_id: 3, name: 'Team3'}]

      allow( @repo ).to receive( :get_division_teams )
      allow( @mapper ).to receive( :map_team_list ).and_return mapped_team_list

      expect( @deco ).to receive( :decorate_team ).with mapped_team_list[0], nil, nil
      expect( @deco ).to receive( :decorate_team ).with mapped_team_list[1], nil, nil
      expect( @deco ).to receive( :decorate_team ).with mapped_team_list[2], nil, nil

      @team_service.get_teams
    end

    it 'should return the mapped, decorated team list' do
      team_list = [{team_id: 1, name: 'Team1'},{team_id: 2, name: 'Team2'},{team_id: 3, name: 'Team3'}]
      mapped_team_list = [{team_id: 1, name: 'Team1'},{team_id: 2, name: 'Team2'},{team_id: 3, name: 'Team3'}]

      allow( @repo ).to receive( :get_division_teams ).and_return team_list
      allow( @mapper ).to receive( :map_team_list ).and_return mapped_team_list
      allow( @deco ).to receive( :decorate_team ) { |t| t[:_links] = {} }

      results = @team_service.get_teams

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     3

      results.each do |entry|
        expect( entry[:team_id] ).to_not be_nil
        expect( entry[:name   ] ).to_not be_nil
        expect( entry[:_links ] ).to_not be_nil
      end
    end

    it 'should pass season and phase parameters to the decorator' do
      mapped_team_list = [{team_id: 1, name: 'Team1'},{team_id: 2, name: 'Team2'},{team_id: 3, name: 'Team3'}]

      allow( @repo ).to receive( :get_division_teams )
      allow( @mapper ).to receive( :map_team_list ).and_return mapped_team_list

      expect( @deco ).to receive( :decorate_team ).with mapped_team_list[0], 3, Phases::Playoff
      expect( @deco ).to receive( :decorate_team ).with mapped_team_list[1], 3, Phases::Playoff
      expect( @deco ).to receive( :decorate_team ).with mapped_team_list[2], 3, Phases::Playoff

      @team_service.get_teams 3, Phases::Playoff
    end
  end

  describe '#get_team' do
    it 'should call the repository to get the team record from the database' do
      allow( @deco ).to receive( :decorate_team )

      expect( @repo ).to receive( :get_team ).with( 4 ).and_return Hash.new

      @team_service.get_team 4
    end

    it 'should call the decorator with the database record' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      allow( @repo ).to receive( :get_team ).with( 1 ).and_return team

      expect( @deco ).to receive( :decorate_team ).with team, anything, anything

      @team_service.get_team 1
    end

    it 'should return the decorated database record' do
      decorated_team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2, _links: {}}

      allow( @repo ).to receive( :get_team ).and_return Hash.new
      allow( @deco ).to receive( :decorate_team ).and_return decorated_team

      expect( @team_service.get_team 1 ).to be decorated_team
    end

    it 'should pass season and phase parameters to the decorator' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      allow( @repo ).to receive( :get_team ).with( 1 ).and_return team

      expect( @deco ).to receive( :decorate_team ).with team, 5, Phases::Playoff

      @team_service.get_team 1, 5, Phases::Playoff
    end

    it 'should raise a resource not found error exception if the record is not in the database' do
      allow( @repo ).to receive( :get_team )

      expect { @team_service.get_team 1 }.to raise_error ResourceNotFoundError, 'Team with Team ID 1 cannot be located.'
    end
  end

  describe '#get_team_stats' do
    it 'should call the repository to get a team stats record from the database' do
      allow( @mapper ).to receive( :map_team_stats )
      allow( @deco ).to receive( :decorate_team_stats )

      expect( @repo ).to receive( :get_team_stats ).with( 1, 1, Phases::RegularSeason ).and_return Hash.new

      @team_service.get_team_stats 1, 1, Phases::RegularSeason
    end

    it 'should call the response mapper with the team stats record from the repository' do
      team_stats = {team_id: 1, season: 1, season_phase: Phases::RegularSeason, wins: 79, losses: 73, home_wins: 40, home_losses: 36, road_wins: 39, road_losses: 37, division_wins: 28, division_losses: 28, league_wins: 44, league_losses: 43, runs_scored: 729, runs_allowed: 797}

      allow( @repo ).to receive( :get_team_stats ).with( 1, 1, Phases::RegularSeason ).and_return team_stats
      allow( @deco ).to receive( :decorate_team_stats )

      expect( @mapper ).to receive( :map_team_stats ).with team_stats

      @team_service.get_team_stats 1, 1, Phases::RegularSeason
    end

    it 'should call the decorator with the mapped team stats hash' do
      mapped_team_stats = {team_id: 1, season: 1, season_phase: 'Regular', wins: 79, losses: 73, home_wins: 40, home_losses: 36, road_wins: 39, road_losses: 37, division_wins: 28, division_losses: 28, league_wins: 44, league_losses: 43, runs_scored: 729, runs_allowed: 797}

      allow( @repo ).to receive( :get_team_stats ).with( 1, 1, Phases::RegularSeason ).and_return Hash.new
      allow( @mapper ).to receive( :map_team_stats ).and_return mapped_team_stats

      expect( @deco ).to receive( :decorate_team_stats ).with mapped_team_stats

      @team_service.get_team_stats 1, 1, Phases::RegularSeason
    end

    it 'should return the mapped, decorated team stats' do
      decorated_team_stats = {team_id: 1, season: 1, season_phase: 'Regular', wins: 79, losses: 73, home_wins: 40, home_losses: 36, road_wins: 39, road_losses: 37, division_wins: 28, division_losses: 28, league_wins: 44, league_losses: 43, runs_scored: 729, runs_allowed: 797, _links: {}}

      allow( @repo ).to receive( :get_team_stats ).with( 1, 1, Phases::RegularSeason ).and_return Hash.new
      allow( @mapper ).to receive( :map_team_stats )
      allow( @deco ).to receive( :decorate_team_stats ).and_return decorated_team_stats

      expect( @team_service.get_team_stats 1, 1, Phases::RegularSeason ).to be decorated_team_stats
    end

    it 'should raise a resource not found error exception if the record is not in the database' do
      allow( @repo ).to receive( :get_team_stats ).with( 1, 1, Phases::RegularSeason )

      expect { @team_service.get_team_stats 1, 1, Phases::RegularSeason }.to raise_error ResourceNotFoundError, "Team Stats for Team ID 1, Season 1 and Phase #{Phases::RegularSeason} cannot be located."
    end

    it 'should call the repository to get many team stats records from the database' do
      expect( @repo ).to receive( :get_team_stats ).with( 1, nil, nil ).and_return Array.new

      @team_service.get_team_stats 1
    end

    it 'should call the response mapper with each of the team stats records from the repository' do
      team_stats_list = [{team_id: 1, season: 1, season_phase: Phases::RegularSeason},{team_id: 1, season: 1, season_phase: Phases::Playoff},{team_id: 1, season: 2, season_phase: Phases::RegularSeason},{team_id: 1, season: 2, season_phase: Phases::Playoff}]

      allow( @repo ).to receive( :get_team_stats ).with( 1, nil, nil ).and_return team_stats_list
      allow( @deco ).to receive( :decorate_team_stats )

      expect( @mapper ).to receive( :map_team_stats ).with team_stats_list[0]
      expect( @mapper ).to receive( :map_team_stats ).with team_stats_list[1]
      expect( @mapper ).to receive( :map_team_stats ).with team_stats_list[2]
      expect( @mapper ).to receive( :map_team_stats ).with team_stats_list[3]

      @team_service.get_team_stats 1
    end

    it 'should call the decorator with each team stats record in the mapped team stats list' do
      team_stats_list = [{team_id: 1, season: 1, season_phase: Phases::RegularSeason},{team_id: 1, season: 1, season_phase: Phases::Playoff},{team_id: 1, season: 2, season_phase: Phases::RegularSeason},{team_id: 1, season: 2, season_phase: Phases::Playoff}]
      mapped_team_stats_list = [{team_id: 1, season: 1, season_phase: 'Regular'},{team_id: 1, season: 1, season_phase: 'Playoff'},{team_id: 1, season: 2, season_phase: 'Regular'},{team_id: 1, season: 2, season_phase: 'Playoff'}]

      allow( @repo ).to receive( :get_team_stats ).with( 1, nil, nil ).and_return team_stats_list
      allow( @mapper ).to receive( :map_team_stats ).with( team_stats_list[0] ).and_return mapped_team_stats_list[0]
      allow( @mapper ).to receive( :map_team_stats ).with( team_stats_list[1] ).and_return mapped_team_stats_list[1]
      allow( @mapper ).to receive( :map_team_stats ).with( team_stats_list[2] ).and_return mapped_team_stats_list[2]
      allow( @mapper ).to receive( :map_team_stats ).with( team_stats_list[3] ).and_return mapped_team_stats_list[3]

      expect( @deco ).to receive( :decorate_team_stats ).with mapped_team_stats_list[0]
      expect( @deco ).to receive( :decorate_team_stats ).with mapped_team_stats_list[1]
      expect( @deco ).to receive( :decorate_team_stats ).with mapped_team_stats_list[2]
      expect( @deco ).to receive( :decorate_team_stats ).with mapped_team_stats_list[3]

      @team_service.get_team_stats 1
    end

    it 'should return the mapped, decorated team stats list' do
      team_stats_list = [{team_id: 1, season: 1, season_phase: Phases::RegularSeason},{team_id: 1, season: 1, season_phase: Phases::Playoff},{team_id: 1, season: 2, season_phase: Phases::RegularSeason},{team_id: 1, season: 2, season_phase: Phases::Playoff}]
      mapped_team_stats_list = [{team_id: 1, season: 1, season_phase: 'Regular'},{team_id: 1, season: 1, season_phase: 'Playoff'},{team_id: 1, season: 2, season_phase: 'Regular'},{team_id: 1, season: 2, season_phase: 'Playoff'}]

      allow( @repo ).to receive( :get_team_stats ).with( 1, nil, nil ).and_return team_stats_list
      allow( @mapper ).to receive( :map_team_stats ).with( team_stats_list[0] ).and_return mapped_team_stats_list[0]
      allow( @mapper ).to receive( :map_team_stats ).with( team_stats_list[1] ).and_return mapped_team_stats_list[1]
      allow( @mapper ).to receive( :map_team_stats ).with( team_stats_list[2] ).and_return mapped_team_stats_list[2]
      allow( @mapper ).to receive( :map_team_stats ).with( team_stats_list[3] ).and_return mapped_team_stats_list[3]
      allow( @deco ).to receive( :decorate_team_stats ) { |ts| ts[:_links] = {} }

      results = @team_service.get_team_stats 1

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     4

      results.each do |entry|
        expect( entry[:team_id     ] ).to_not be_nil
        expect( entry[:season      ] ).to_not be_nil
        expect( entry[:season_phase] ).to_not be_nil
        expect( entry[:_links      ] ).to_not be_nil
      end
    end
  end
end
