location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'division_repository'
require 'accolades'

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
