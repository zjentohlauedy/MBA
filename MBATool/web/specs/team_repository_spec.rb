location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'team_repository'
require 'accolades'
require 'phases'

describe TeamRepository do
  before :each do
    @db = SQLite3::Database.new "test_mba.db"

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
      @db.execute 'insert into division_teams_t values (1, 1)'
      @db.execute 'insert into division_teams_t values (2, 2)'
      @db.execute 'insert into division_teams_t values (3, 3)'
      @db.execute 'insert into teams_t values (1, "Name1", "Location1", 0, 1)'
      @db.execute 'insert into teams_t values (2, "Name2", "Location2", 5, 3)'
      @db.execute 'insert into teams_t values (3, "Name3", "Location3", 4, 2)'

      result = @team_repository.get_division_teams

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     3

      expect( result[0][:division_id    ] ).to eq 1
      expect( result[0][:team_id        ] ).to eq 1
      expect( result[0][:name           ] ).to eq 'Name1'
      expect( result[0][:location       ] ).to eq 'Location1'
      expect( result[0][:primary_color  ] ).to eq 0
      expect( result[0][:secondary_color] ).to eq 1

      expect( result[1][:division_id    ] ).to eq 2
      expect( result[1][:team_id        ] ).to eq 2
      expect( result[1][:name           ] ).to eq 'Name2'
      expect( result[1][:location       ] ).to eq 'Location2'
      expect( result[1][:primary_color  ] ).to eq 5
      expect( result[1][:secondary_color] ).to eq 3

      expect( result[2][:division_id    ] ).to eq 3
      expect( result[2][:team_id        ] ).to eq 3
      expect( result[2][:name           ] ).to eq 'Name3'
      expect( result[2][:location       ] ).to eq 'Location3'
      expect( result[2][:primary_color  ] ).to eq 4
      expect( result[2][:secondary_color] ).to eq 2
    end

    it 'should return an empty array if there are no teams' do
      result = @team_repository.get_division_teams

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
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
      @db.execute 'insert into division_teams_t values (1, 1)'
      @db.execute 'insert into division_teams_t values (2, 2)'
      @db.execute 'insert into division_teams_t values (3, 3)'
      @db.execute 'insert into teams_t values (1, "Name1", "Location1", 0, 1)'
      @db.execute 'insert into teams_t values (2, "Name2", "Location2", 5, 3)'
      @db.execute 'insert into teams_t values (3, "Name3", "Location3", 4, 2)'
      @db.execute "insert into team_stats_t values (1,1,#{Phases::RegularSeason},12,10,6,4,7,2,5,1,3,8,25,32)"
      @db.execute "insert into team_stats_t values (2,1,#{Phases::RegularSeason},15,12,8,1,6,4,3,7,5,2,28,22)"
      @db.execute "insert into team_stats_t values (3,1,#{Phases::RegularSeason},14,16,9,7,5,1,6,8,4,2,37,21)"

      result = @team_repository.get_division_teams_with_stats 1, Phases::RegularSeason

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     3

      expect( result[0][:division_id    ] ).to eq 1
      expect( result[0][:team_id        ] ).to eq 1
      expect( result[0][:name           ] ).to eq 'Name1'
      expect( result[0][:location       ] ).to eq 'Location1'
      expect( result[0][:primary_color  ] ).to eq 0
      expect( result[0][:secondary_color] ).to eq 1
      expect( result[0][:season         ] ).to eq 1
      expect( result[0][:season_phase   ] ).to eq Phases::RegularSeason
      expect( result[0][:wins           ] ).to eq 12
      expect( result[0][:losses         ] ).to eq 10
      expect( result[0][:home_wins      ] ).to eq 6
      expect( result[0][:home_losses    ] ).to eq 4
      expect( result[0][:road_wins      ] ).to eq 7
      expect( result[0][:road_losses    ] ).to eq 2
      expect( result[0][:division_wins  ] ).to eq 5
      expect( result[0][:division_losses] ).to eq 1
      expect( result[0][:league_wins    ] ).to eq 3
      expect( result[0][:league_losses  ] ).to eq 8
      expect( result[0][:runs_scored    ] ).to eq 25
      expect( result[0][:runs_allowed   ] ).to eq 32

      expect( result[1][:division_id    ] ).to eq 2
      expect( result[1][:team_id        ] ).to eq 2
      expect( result[1][:name           ] ).to eq 'Name2'
      expect( result[1][:location       ] ).to eq 'Location2'
      expect( result[1][:primary_color  ] ).to eq 5
      expect( result[1][:secondary_color] ).to eq 3
      expect( result[1][:season         ] ).to eq 1
      expect( result[1][:season_phase   ] ).to eq Phases::RegularSeason
      expect( result[1][:wins           ] ).to eq 15
      expect( result[1][:losses         ] ).to eq 12
      expect( result[1][:home_wins      ] ).to eq 8
      expect( result[1][:home_losses    ] ).to eq 1
      expect( result[1][:road_wins      ] ).to eq 6
      expect( result[1][:road_losses    ] ).to eq 4
      expect( result[1][:division_wins  ] ).to eq 3
      expect( result[1][:division_losses] ).to eq 7
      expect( result[1][:league_wins    ] ).to eq 5
      expect( result[1][:league_losses  ] ).to eq 2
      expect( result[1][:runs_scored    ] ).to eq 28
      expect( result[1][:runs_allowed   ] ).to eq 22

      expect( result[2][:division_id    ] ).to eq 3
      expect( result[2][:team_id        ] ).to eq 3
      expect( result[2][:name           ] ).to eq 'Name3'
      expect( result[2][:location       ] ).to eq 'Location3'
      expect( result[2][:primary_color  ] ).to eq 4
      expect( result[2][:secondary_color] ).to eq 2
      expect( result[2][:season         ] ).to eq 1
      expect( result[2][:season_phase   ] ).to eq Phases::RegularSeason
      expect( result[2][:wins           ] ).to eq 14
      expect( result[2][:losses         ] ).to eq 16
      expect( result[2][:home_wins      ] ).to eq 9
      expect( result[2][:home_losses    ] ).to eq 7
      expect( result[2][:road_wins      ] ).to eq 5
      expect( result[2][:road_losses    ] ).to eq 1
      expect( result[2][:division_wins  ] ).to eq 6
      expect( result[2][:division_losses] ).to eq 8
      expect( result[2][:league_wins    ] ).to eq 4
      expect( result[2][:league_losses  ] ).to eq 2
      expect( result[2][:runs_scored    ] ).to eq 37
      expect( result[2][:runs_allowed   ] ).to eq 21
    end
  end

  describe '#get_team_accolades' do
    it 'should return an array containing team accolades information' do
      @db.execute "insert into team_accolades_t values (1, 1, #{Accolades::Team::League_Title})"

      result = @team_repository.get_team_accolades 1, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:team_id ] ).to eq 1
      expect( result[0][:season  ] ).to eq 1
      expect( result[0][:accolade] ).to eq Accolades::Team::League_Title
    end

    it 'should return only records matching given season' do
      @db.execute "insert into team_accolades_t values (1, 1, #{Accolades::Team::League_Title})"
      @db.execute "insert into team_accolades_t values (1, 2, #{Accolades::Team::League_Title})"

      result = @team_repository.get_team_accolades 1, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:team_id ] ).to eq 1
      expect( result[0][:season  ] ).to eq 1
      expect( result[0][:accolade] ).to eq Accolades::Team::League_Title
    end

    it 'should return multiple seasons if not given a season' do
      @db.execute "insert into team_accolades_t values (1, 1, #{Accolades::Team::League_Title})"
      @db.execute "insert into team_accolades_t values (1, 2, #{Accolades::Team::League_Title})"

      result = @team_repository.get_team_accolades 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:team_id ] ).to eq 1
      expect( result[0][:season  ] ).to eq 1
      expect( result[0][:accolade] ).to eq Accolades::Team::League_Title

      expect( result[1][:team_id ] ).to eq 1
      expect( result[1][:season  ] ).to eq 2
      expect( result[1][:accolade] ).to eq Accolades::Team::League_Title
    end

    it 'should return an empty array if no records are found' do
      result = @team_repository.get_team_accolades 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
 end

  describe '#save_team_accolade' do
    it 'should return an empty array on success' do
      team_accolade = {team_id: 1, season: 3, accolade: Accolades::Team::Division_Title}

      result = @team_repository.save_team_accolade team_accolade

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should insert a team_accolades_t record' do
      team_accolade = {team_id: 1, season: 3, accolade: Accolades::Team::Division_Title}

      @team_repository.save_team_accolade team_accolade

      result = @db.get_first_row "select * from team_accolades_t where team_id = 1 and season = 3 and accolade = #{Accolades::Team::Division_Title}"

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result['Team_Id' ] ).to     eq     1
      expect( result['Season'  ] ).to     eq     3
      expect( result['Accolade'] ).to     eq     Accolades::Team::Division_Title
    end

    it 'should be idempotent' do
      team_accolade = {team_id: 1, season: 3, accolade: Accolades::Team::Division_Title}

      @team_repository.save_team_accolade team_accolade
      @team_repository.save_team_accolade team_accolade
    end
  end
end
