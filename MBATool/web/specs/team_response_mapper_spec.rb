location = File.dirname __FILE__
$: << "#{location}"

require 'team_response_mapper'

describe TeamResponseMapper do
  before :each do
    @team_response_mapper = TeamResponseMapper.new
  end

  describe '#map_team_list' do
    it 'should return an array with the same number of elements as the given array' do
      team_list = [{team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2},
                   {team_id: 2, name: 'Name2', location: 'Location2', primary_color: 8, secondary_color: 4}]

      result = @team_response_mapper.map_team_list team_list

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     team_list.length
    end

    it 'should return teams with only ID and name' do
      team_list = [{team_id: 1, name: 'Name1', location: 'Location1', primary_color: 5, secondary_color: 2},
                   {team_id: 2, name: 'Name2', location: 'Location2', primary_color: 8, secondary_color: 4}]

      result = @team_response_mapper.map_team_list team_list

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0].length    ).to eq 2
      expect( result[0][:team_id] ).to eq 1
      expect( result[0][:name   ] ).to eq 'Name1'
      expect( result[1].length    ).to eq 2
      expect( result[1][:team_id] ).to eq 2
      expect( result[1][:name   ] ).to eq 'Name2'
    end

    it 'should return nil if not given an array' do
      result = @team_response_mapper.map_team_list Object.new

      expect( result ).to be_nil
    end

    it 'should return nil if not given an array of hashes' do
      result = @team_response_mapper.map_team_list [Object.new, Object.new]

      expect( result ).to be_nil
    end
  end

  describe '#map_team_stats' do
    it 'should return a mapped team stats hash' do
      team_stats = {team_id: 1, season: 1, season_phase: Phases::RegularSeason, wins: 79, losses: 73, home_wins: 40, home_losses: 36, road_wins: 39, road_losses: 37, division_wins: 28, division_losses: 28, league_wins: 44, league_losses: 43, runs_scored: 729, runs_allowed: 797}

      result = @team_response_mapper.map_team_stats team_stats

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
    end

    it 'should replace season phase with a user friendly value' do
      team_stats = {team_id: 1, season: 1, season_phase: Phases::RegularSeason, wins: 79, losses: 73, home_wins: 40, home_losses: 36, road_wins: 39, road_losses: 37, division_wins: 28, division_losses: 28, league_wins: 44, league_losses: 43, runs_scored: 729, runs_allowed: 797}

      result = @team_response_mapper.map_team_stats team_stats

      expect( result                ).to_not be_nil
      expect( result                ).to     be_a   Hash
      expect( result[:season_phase] ).to     eq     'Regular'
    end

    it 'should return nil if not given a hash' do
      result = @team_response_mapper.map_team_stats Object.new

      expect( result ).to be_nil
    end
  end
end
