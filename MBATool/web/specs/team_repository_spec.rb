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
      @db.execute 'insert into teams_t values ( 99, "My Team", "My Location", 7, 2 )'

      result = @team_repository.get_team 99

      expect( result                   ).to_not be_nil
      expect( result                   ).to     be_a    Hash
      expect( result[:team_id        ] ).to     eq      99
      expect( result[:name           ] ).to     eq     'My Team'
      expect( result[:location       ] ).to     eq     'My Location'
      expect( result[:primary_color  ] ).to     eq      7
      expect( result[:secondary_color] ).to     eq      2
    end

    it 'should return nil if the team is not found' do
      result = @team_repository.get_team 99

      expect( result ).to be_nil
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
      @db.execute "insert into team_stats_t values ( 99, 5, #{Phases::RegularSeason}, 54, 76, 32, 45, 22, 31, 25, 17, 37, 38, 166, 217 )"

      result = @team_repository.get_team_stats 99, 5, Phases::RegularSeason

      expect( result                   ).to_not be_nil
      expect( result                   ).to     be_a   Hash
      expect( result[:team_id        ] ).to     eq     99
      expect( result[:season         ] ).to     eq     5
      expect( result[:season_phase   ] ).to     eq     Phases::RegularSeason
      expect( result[:wins           ] ).to     eq     54
      expect( result[:losses         ] ).to     eq     76
      expect( result[:home_wins      ] ).to     eq     32
      expect( result[:home_losses    ] ).to     eq     45
      expect( result[:road_wins      ] ).to     eq     22
      expect( result[:road_losses    ] ).to     eq     31
      expect( result[:division_wins  ] ).to     eq     25
      expect( result[:division_losses] ).to     eq     17
      expect( result[:league_wins    ] ).to     eq     37
      expect( result[:league_losses  ] ).to     eq     38
      expect( result[:runs_scored    ] ).to     eq     166
      expect( result[:runs_allowed   ] ).to     eq     217
    end

    it 'should return multiple phases if not given a phase' do
      @db.execute "insert into team_stats_t (team_id, season, season_phase) values ( 99, 5, #{Phases::RegularSeason} )"
      @db.execute "insert into team_stats_t (team_id, season, season_phase) values ( 99, 5, #{Phases::Playoff} )"

      result = @team_repository.get_team_stats 99, 5

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:season_phase] ).to eq Phases::RegularSeason
      expect( result[1][:season_phase] ).to eq Phases::Playoff
    end

    it 'should return multiple seasons if not given a season' do
      @db.execute "insert into team_stats_t (team_id, season, season_phase) values ( 99, 5, #{Phases::RegularSeason} )"
      @db.execute "insert into team_stats_t (team_id, season, season_phase) values ( 99, 6, #{Phases::RegularSeason} )"

      result = @team_repository.get_team_stats 99, nil, Phases::RegularSeason

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:season] ).to eq 5
      expect( result[1][:season] ).to eq 6
    end

    it 'should return multiple seasons and phases if only given a team id' do
      @db.execute "insert into team_stats_t (team_id, season, season_phase) values ( 99, 5, #{Phases::RegularSeason} )"
      @db.execute "insert into team_stats_t (team_id, season, season_phase) values ( 99, 5, #{Phases::Playoff} )"
      @db.execute "insert into team_stats_t (team_id, season, season_phase) values ( 99, 6, #{Phases::RegularSeason} )"
      @db.execute "insert into team_stats_t (team_id, season, season_phase) values ( 99, 6, #{Phases::Playoff} )"

      result = @team_repository.get_team_stats 99

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     4

      expect( result[0][:season      ] ).to eq 5
      expect( result[0][:season_phase] ).to eq Phases::RegularSeason
      expect( result[1][:season      ] ).to eq 5
      expect( result[1][:season_phase] ).to eq Phases::Playoff
      expect( result[2][:season      ] ).to eq 6
      expect( result[2][:season_phase] ).to eq Phases::RegularSeason
      expect( result[3][:season      ] ).to eq 6
      expect( result[3][:season_phase] ).to eq Phases::Playoff
    end

    it 'should return nil if the record is not found' do
      result = @team_repository.get_team_stats 99, 5, Phases::RegularSeason

      expect( result ).to be_nil
    end

    it 'should return an empty array if no records are found' do
      result = @team_repository.get_team_stats 99

      expect( result ).to_not be_nil
      expect( result ).to     be_a   Array

      expect( result.length ).to eq 0
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
