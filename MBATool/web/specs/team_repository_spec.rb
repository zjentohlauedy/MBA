location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'team_repository'
require 'phases'

describe TeamRepository do
  before :each do
    @db = SQLite3::Database.new "mba.db"

    @db.results_as_hash  = true
    @db.type_translation = true

    @team_repository = TeamRepository.new @db

    @db.transaction
  end

  after :each do
    @db.rollback
  end

  describe '#get_team' do
    it 'should return a hash containing team information' do
      result = @team_repository.get_team 8

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      actual = result[0]

      expect( actual[:team_id        ] ).to eq  8
      expect( actual[:name           ] ).to eq 'Drizzle'
      expect( actual[:location       ] ).to eq 'Sao Paulo'
      expect( actual[:primary_color  ] ).to eq  0
      expect( actual[:secondary_color] ).to eq  0
    end
  end

  describe '#get_division_teams' do
    it 'should return an array containing all teams within any division' do
      result = @team_repository.get_division_teams

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     32

      result.each do |team|
        expect( team[:division_id    ] ).to be_a Integer
        expect( team[:name           ] ).to be_a String
        expect( team[:location       ] ).to be_a String
        expect( team[:primary_color  ] ).to be_a Integer
        expect( team[:secondary_color] ).to be_a Integer
      end
    end
  end

  describe '#get_team_stats' do
    it 'should return a hash containing team stats information' do
      result = @team_repository.get_team_stats 1, 1, Phases::RegularSeason

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      actual = result[0]

      expect( actual[:team_id        ] ).to eq   1
      expect( actual[:season         ] ).to eq   1
      expect( actual[:season_phase   ] ).to eq   Phases::RegularSeason
      expect( actual[:wins           ] ).to be_a Integer
      expect( actual[:losses         ] ).to be_a Integer
      expect( actual[:home_wins      ] ).to be_a Integer
      expect( actual[:home_losses    ] ).to be_a Integer
      expect( actual[:road_wins      ] ).to be_a Integer
      expect( actual[:road_losses    ] ).to be_a Integer
      expect( actual[:division_wins  ] ).to be_a Integer
      expect( actual[:division_losses] ).to be_a Integer
      expect( actual[:league_wins    ] ).to be_a Integer
      expect( actual[:league_losses  ] ).to be_a Integer
      expect( actual[:runs_scored    ] ).to be_a Integer
      expect( actual[:runs_allowed   ] ).to be_a Integer
    end

    it 'should return multiple phases if not given a phase' do
      result = @team_repository.get_team_stats 1, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:season_phase] ).to eq Phases::RegularSeason
      expect( result[1][:season_phase] ).to eq Phases::Playoff
    end

    it 'should return multiple seasons if not given a season' do
      result = @team_repository.get_team_stats 1, nil, Phases::RegularSeason

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:season] ).to eq 1
      expect( result[1][:season] ).to eq 2
    end

    it 'should return multiple seasons and phases if only given a team id' do
      result = @team_repository.get_team_stats 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     4

      expect( result[0][:season      ] ).to eq 1
      expect( result[0][:season_phase] ).to eq Phases::RegularSeason
      expect( result[1][:season      ] ).to eq 1
      expect( result[1][:season_phase] ).to eq Phases::Playoff
      expect( result[2][:season      ] ).to eq 2
      expect( result[2][:season_phase] ).to eq Phases::RegularSeason
      expect( result[3][:season      ] ).to eq 2
      expect( result[3][:season_phase] ).to eq Phases::Playoff
    end
  end

  describe '#get_division_teams_with_stats' do
    it 'should return an array containing all teams within any division including team stats' do
      result = @team_repository.get_division_teams_with_stats 1, Phases::RegularSeason

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     32

      result.each do |team|
        expect( team[:division_id    ] ).to be_a Integer
        expect( team[:team_id        ] ).to be_a Integer
        expect( team[:name           ] ).to be_a String
        expect( team[:location       ] ).to be_a String
        expect( team[:primary_color  ] ).to be_a Integer
        expect( team[:secondary_color] ).to be_a Integer
        expect( team[:season         ] ).to eq   1
        expect( team[:season_phase   ] ).to eq   Phases::RegularSeason
        expect( team[:wins           ] ).to be_a Integer
        expect( team[:losses         ] ).to be_a Integer
        expect( team[:home_wins      ] ).to be_a Integer
        expect( team[:home_losses    ] ).to be_a Integer
        expect( team[:road_wins      ] ).to be_a Integer
        expect( team[:road_losses    ] ).to be_a Integer
        expect( team[:division_wins  ] ).to be_a Integer
        expect( team[:division_losses] ).to be_a Integer
        expect( team[:league_wins    ] ).to be_a Integer
        expect( team[:league_losses  ] ).to be_a Integer
        expect( team[:runs_scored    ] ).to be_a Integer
        expect( team[:runs_allowed   ] ).to be_a Integer
      end
    end
  end
end
