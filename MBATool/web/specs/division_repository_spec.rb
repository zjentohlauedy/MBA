location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'division_repository'
require 'accolades'
require 'phases'

describe DivisionRepository do
  before :each do
    @db = SQLite3::Database.new "test_mba.db"

    @db.results_as_hash  = true
    @db.type_translation = true

    @division_repository = DivisionRepository.new @db

    @db.transaction
  end

  after :each do
    @db.rollback
  end

  describe '#get_division_stats_by_highest' do
    it 'should return an array with at least one entry' do
      @db.execute "insert into division_stats_t values (1, 1, #{Phases::RegularSeason}, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @division_repository.get_division_stats_by_highest 'wins'

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     be     >= 1
    end

    it 'should return the division stats record with the highest value of the given stat' do
      @db.execute "insert into division_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @division_repository.get_division_stats_by_highest 'wins'

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:division_id] ).to eq 2
    end

    it 'should return multiple records if there is a tie' do
      @db.execute "insert into division_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (2, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @division_repository.get_division_stats_by_highest 'wins'

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2
    end

    it 'should work for any given stat' do
      @db.execute "insert into division_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (2, 1, #{Phases::RegularSeason}, 11, 11, 13, 12, 12, 10, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (3, 1, #{Phases::RegularSeason}, 12, 13, 12, 11, 13, 12, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (4, 1, #{Phases::RegularSeason}, 13, 12, 11, 10, 11, 11, 1, 1, 1, 1)"

      expect( (@division_repository.get_division_stats_by_highest 'wins'       )[0][:division_id] ).to eq 4
      expect( (@division_repository.get_division_stats_by_highest 'losses'     )[0][:division_id] ).to eq 3
      expect( (@division_repository.get_division_stats_by_highest 'home_wins'  )[0][:division_id] ).to eq 2
      expect( (@division_repository.get_division_stats_by_highest 'home_losses')[0][:division_id] ).to eq 1
      expect( (@division_repository.get_division_stats_by_highest 'road_wins'  )[0][:division_id] ).to eq 3
      expect( (@division_repository.get_division_stats_by_highest 'road_losses')[0][:division_id] ).to eq 1
    end

    it 'should work for a calculation of stats' do
      @db.execute "insert into division_stats_t values (1, 1, #{Phases::RegularSeason}, 10,  5, 10,  5, 10,  5, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (2, 1, #{Phases::RegularSeason},  5, 10,  5, 10,  5, 10, 1, 1, 1, 1)"

      expect( (@division_repository.get_division_stats_by_highest 'wins + home_wins'    )[0][:division_id] ).to eq 1
      expect( (@division_repository.get_division_stats_by_highest 'losses - road_wins'  )[0][:division_id] ).to eq 2
    end

    it 'should only consider the given season' do
      @db.execute "insert into division_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (4, 2, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @division_repository.get_division_stats_by_highest 'wins', 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:division_id] ).to eq 2
    end

    it 'should only consider the given season and phase' do
      @db.execute "insert into division_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @division_repository.get_division_stats_by_highest 'wins', 1, Phases::Playoff

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:division_id] ).to eq 2
    end

    it 'should consider all records if no season or phase are given' do
      @db.execute "insert into division_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into division_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @division_repository.get_division_stats_by_highest 'wins'

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:division_id] ).to eq 3
    end

    it 'should return an empty array if there are no qualifying stats records' do
      result = @division_repository.get_division_stats_by_highest 'wins'

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should raise an error if an invalid stat is given' do
      expect { @division_repository.get_division_stats_by_highest 'invalid' }.to raise_error SQLite3::SQLException
    end
  end

  describe '#get_division_accolades' do
    it 'should return an array containing division accolades information' do
      @db.execute "insert into division_accolades_t values (1, 1, #{Accolades::Division::League_Title})"

      result = @division_repository.get_division_accolades 1, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:division_id] ).to eq 1
      expect( result[0][:season     ] ).to eq 1
      expect( result[0][:accolade   ] ).to eq Accolades::Division::League_Title
    end

    it 'should return only records matching given season' do
      @db.execute "insert into division_accolades_t values (1, 1, #{Accolades::Division::League_Title})"
      @db.execute "insert into division_accolades_t values (1, 2, #{Accolades::Division::League_Title})"

      result = @division_repository.get_division_accolades 1, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:division_id] ).to eq 1
      expect( result[0][:season     ] ).to eq 1
      expect( result[0][:accolade   ] ).to eq Accolades::Division::League_Title
    end

    it 'should return multiple seasons if not given a season' do
      @db.execute "insert into division_accolades_t values (1, 1, #{Accolades::Division::League_Title})"
      @db.execute "insert into division_accolades_t values (1, 2, #{Accolades::Division::League_Title})"

      result = @division_repository.get_division_accolades 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:division_id] ).to eq 1
      expect( result[0][:season     ] ).to eq 1
      expect( result[0][:accolade   ] ).to eq Accolades::Division::League_Title

      expect( result[1][:division_id] ).to eq 1
      expect( result[1][:season     ] ).to eq 2
      expect( result[1][:accolade   ] ).to eq Accolades::Division::League_Title
    end

    it 'should return an empty array if no records are found' do
      result = @division_repository.get_division_accolades 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#save_division_accolade' do
    it 'should return an empty array on success' do
      division_accolade = {division_id: 1, season: 3, accolade: Accolades::Division::World_Title}

      result = @division_repository.save_division_accolade division_accolade

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should insert a division_accolades_t record' do
      division_accolade = {division_id: 1, season: 3, accolade: Accolades::Division::World_Title}

      @division_repository.save_division_accolade division_accolade

      result = @db.get_first_row "select * from division_accolades_t where division_id = 1 and season = 3 and accolade = #{Accolades::Division::World_Title}"

      expect( result                ).to_not be_nil
      expect( result                ).to     be_a   Hash
      expect( result['Division_Id'] ).to     eq     1
      expect( result['Season'     ] ).to     eq     3
      expect( result['Accolade'   ] ).to     eq     Accolades::Division::World_Title
    end

    it 'should be idempotent' do
      division_accolade = {division_id: 1, season: 3, accolade: Accolades::Division::World_Title}

      @division_repository.save_division_accolade division_accolade
      @division_repository.save_division_accolade division_accolade
    end
  end
end
