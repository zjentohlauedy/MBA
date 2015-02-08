location = File.dirname __FILE__
$: << "#{location}"

require 'team_decorator'

describe TeamDecorator do
  before :each do
    @base_url       = 'http://www.mba.org/mba/resources'
    @team_decorator = TeamDecorator.new @base_url
  end

  describe '#decorate_team' do
    it 'should return the given team hash with links added' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team

      expect( result                    ).to_not be_nil
      expect( result                    ).to     be_a   Hash
      expect( result[:team_id         ] ).to_not be_nil
      expect( result[:name            ] ).to_not be_nil
      expect( result[:location        ] ).to_not be_nil
      expect( result[:primary_color   ] ).to_not be_nil
      expect( result[:secondary_color ] ).to_not be_nil
      expect( result[:_links          ] ).to_not be_nil
    end

    it 'should add a self link' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team

      self_link = result[:_links][:self]

      expect( self_link        ).to_not be_nil
      expect( self_link[:rel ] ).to     eq     'self'
      expect( self_link[:href] ).to     eq     "#{@base_url}/teams/1"
      expect( self_link[:desc] ).to     eq     "This resource"
    end

    it 'should append season parameter to self link if given a season' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team, 3

      expect( result[:_links][:self][:href] ).to eq "#{@base_url}/teams/1?season=3"
    end

    it 'should append phase parameter to self link if given a phase' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team, nil, Phases::RegularSeason

      expect( result[:_links][:self][:href] ).to eq "#{@base_url}/teams/1?phase=#{Phases::RegularSeason}"
    end

    it 'should append season and phase parameters to self link if given season and phase' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team, 3, Phases::RegularSeason

      expect( result[:_links][:self][:href] ).to eq "#{@base_url}/teams/1?season=3&phase=#{Phases::RegularSeason}"
    end

    it 'should add a players link' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team

      players_link = result[:_links][:players]

      expect( players_link        ).to_not be_nil
      expect( players_link[:rel ] ).to     eq     'players'
      expect( players_link[:href] ).to     eq     "#{@base_url}/teams/1/players"
      expect( players_link[:desc] ).to     eq     "Players associated with this team"
    end

    it 'should append season parameter to players link if given a season' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team, 3

      expect( result[:_links][:players][:href] ).to eq "#{@base_url}/teams/1/players?season=3"
      expect( result[:_links][:players][:desc] ).to eq "Players associated with this team for season 3"
    end

    it 'should append phase parameter to players link if given a phase' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team, nil, Phases::RegularSeason

      expect( result[:_links][:players][:href] ).to eq "#{@base_url}/teams/1/players?phase=#{Phases::RegularSeason}"
    end

    it 'should append season and phase parameters to players link if given season and phase' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team, 3, Phases::RegularSeason

      expect( result[:_links][:players][:href] ).to eq "#{@base_url}/teams/1/players?season=3&phase=#{Phases::RegularSeason}"
      expect( result[:_links][:players][:desc] ).to eq "Players associated with this team for season 3"
    end

    it 'should add a stats link' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team

      stats_link = result[:_links][:stats]

      expect( stats_link        ).to_not be_nil
      expect( stats_link[:rel ] ).to     eq     'stats'
      expect( stats_link[:href] ).to     eq     "#{@base_url}/teams/1/stats"
      expect( stats_link[:desc] ).to     eq     "Statistics for this team"
    end

    it 'should append season parameter to stats link if given a season' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team, 3

      expect( result[:_links][:stats][:href] ).to eq "#{@base_url}/teams/1/stats?season=3"
      expect( result[:_links][:stats][:desc] ).to eq "Season 3 statistics for this team"
    end

    it 'should append phase parameter to stats link if given a phase' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team, nil, Phases::RegularSeason

      expect( result[:_links][:stats][:href] ).to eq "#{@base_url}/teams/1/stats?phase=#{Phases::RegularSeason}"
      expect( result[:_links][:stats][:desc] ).to eq "Regular Season statistics for this team"
    end

    it 'should append season and phase parameters to stats link if given season and phase' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team, 3, Phases::RegularSeason

      expect( result[:_links][:stats][:href] ).to eq "#{@base_url}/teams/1/stats?season=3&phase=#{Phases::RegularSeason}"
      expect( result[:_links][:stats][:desc] ).to eq "Season 3 Regular Season statistics for this team"
    end

    it 'should add a team link if given a season' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team, 3

      team_link = result[:_links][:team]

      expect( team_link        ).to_not be_nil
      expect( team_link[:rel ] ).to     eq     'team'
      expect( team_link[:href] ).to     eq     "#{@base_url}/teams/1"
      expect( team_link[:desc] ).to     eq     "Information about this team"
    end

    it 'should add a team link if given a phase' do
      team = {team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2}

      result = @team_decorator.decorate_team team, nil, Phases::RegularSeason

      team_link = result[:_links][:team]

      expect( team_link        ).to_not be_nil
      expect( team_link[:rel ] ).to     eq     'team'
      expect( team_link[:href] ).to     eq     "#{@base_url}/teams/1"
      expect( team_link[:desc] ).to     eq     "Information about this team"
    end
  end

  describe '#decorate_team_stats' do
    it 'should return the given team stats hash with links added' do
      team_stats = {team_id: 1, season: 1, season_phase: Phases::RegularSeason, wins: 79, losses: 73, home_wins: 40, home_losses: 36, road_wins: 39, road_losses: 37, division_wins: 28, division_losses: 28, league_wins: 44, league_losses: 43, runs_scored: 729, runs_allowed: 797}

      result = @team_decorator.decorate_team_stats team_stats, Phases::RegularSeason

      expect( result                   ).to_not be_nil
      expect( result                   ).to     be_a   Hash
      expect( result[:team_id        ] ).to_not be_nil
      expect( result[:season         ] ).to_not be_nil
      expect( result[:season_phase   ] ).to_not be_nil
      expect( result[:wins           ] ).to_not be_nil
      expect( result[:losses         ] ).to_not be_nil
      expect( result[:home_wins      ] ).to_not be_nil
      expect( result[:home_losses    ] ).to_not be_nil
      expect( result[:road_wins      ] ).to_not be_nil
      expect( result[:road_losses    ] ).to_not be_nil
      expect( result[:division_wins  ] ).to_not be_nil
      expect( result[:division_losses] ).to_not be_nil
      expect( result[:league_wins    ] ).to_not be_nil
      expect( result[:league_losses  ] ).to_not be_nil
      expect( result[:runs_scored    ] ).to_not be_nil
      expect( result[:runs_allowed   ] ).to_not be_nil
      expect( result[:_links         ] ).to_not be_nil
    end

    it 'should add a self link' do
      team_stats = {team_id: 1, season: 1, season_phase: Phases::RegularSeason, wins: 79, losses: 73, home_wins: 40, home_losses: 36, road_wins: 39, road_losses: 37, division_wins: 28, division_losses: 28, league_wins: 44, league_losses: 43, runs_scored: 729, runs_allowed: 797}

      result = @team_decorator.decorate_team_stats team_stats, Phases::RegularSeason

      self_link = result[:_links][:self]

      expect( self_link        ).to_not be_nil
      expect( self_link[:rel ] ).to     eq     'self'
      expect( self_link[:href] ).to     eq     "#{@base_url}/teams/1/stats?season=1&phase=#{Phases::RegularSeason}"
      expect( self_link[:desc] ).to     eq     "This resource"
    end

    it 'should add a team link' do
      team_stats = {team_id: 1, season: 1, season_phase: Phases::RegularSeason, wins: 79, losses: 73, home_wins: 40, home_losses: 36, road_wins: 39, road_losses: 37, division_wins: 28, division_losses: 28, league_wins: 44, league_losses: 43, runs_scored: 729, runs_allowed: 797}

      result = @team_decorator.decorate_team_stats team_stats, Phases::RegularSeason

      team_link = result[:_links][:team]

      expect( team_link        ).to_not be_nil
      expect( team_link[:rel ] ).to     eq     'team'
      expect( team_link[:href] ).to     eq     "#{@base_url}/teams/1"
      expect( team_link[:desc] ).to     eq     "Information about this team"
    end

    it 'should add a stats link' do
      team_stats = {team_id: 1, season: 1, season_phase: Phases::RegularSeason, wins: 79, losses: 73, home_wins: 40, home_losses: 36, road_wins: 39, road_losses: 37, division_wins: 28, division_losses: 28, league_wins: 44, league_losses: 43, runs_scored: 729, runs_allowed: 797}

      result = @team_decorator.decorate_team_stats team_stats, Phases::RegularSeason

      stats_link = result[:_links][:stats]

      expect( stats_link        ).to_not be_nil
      expect( stats_link[:rel ] ).to     eq     'stats'
      expect( stats_link[:href] ).to     eq     "#{@base_url}/teams/1/stats"
      expect( stats_link[:desc] ).to     eq     "All statistics for this team"
    end
  end
end
