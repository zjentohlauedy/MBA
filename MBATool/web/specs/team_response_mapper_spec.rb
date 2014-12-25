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
end
