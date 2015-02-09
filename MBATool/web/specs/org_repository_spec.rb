location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'org_repository'

describe OrgRepository do
  before :each do
    @db = SQLite3::Database.new "test_mba.db"

    @db.results_as_hash  = true
    @db.type_translation = true

    @org_repository = OrgRepository.new @db

    @db.transaction
  end

  after :each do
    @db.rollback
  end

  describe '#get_org' do
    it 'should return a hash containing organization information' do
      @db.execute 'insert into organizations_t values (7, "Org Name", "ON", 2, 4, 3, 17)'

      result = @org_repository.get_org 7

      expect( result                   ).to_not be_nil
      expect( result                   ).to     be_a   Hash
      expect( result[:organization_id] ).to     eq     7
      expect( result[:name           ] ).to     eq     'Org Name'
      expect( result[:abbreviation   ] ).to     eq     'ON'
      expect( result[:season         ] ).to     eq     2
      expect( result[:stage          ] ).to     eq     4
      expect( result[:draft_round    ] ).to     eq     3
      expect( result[:pick_number    ] ).to     eq     17
    end

    it 'should return nil if record is not found' do
      result = @org_repository.get_org 3

      expect( result ).to be_nil
    end
  end

  describe '#save_org' do
    it 'should return an empty array on success' do
      result = @org_repository.save_org 3, {season: 6}

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should update the organizations_t record' do
      @db.execute 'insert into organizations_t values (7, "Org Name", "ON", 2, 4, 3, 17)'

      update = { name: 'Something Else', abbreviation: 'SE', season: 5, stage: 1, draft_round: 2, pick_number: 26 }

      @org_repository.save_org 7, update

      result = @db.get_first_row 'select * from organizations_t where organization_id = 7'

      expect( result                     ).to_not be_nil
      expect( result                     ).to     be_a   Hash
      expect( result['Organization_Id' ] ).to     eq     7
      expect( result['Name'            ] ).to     eq     'Something Else'
      expect( result['Abbreviation'    ] ).to     eq     'SE'
      expect( result['Season'          ] ).to     eq     5
      expect( result['Stage'           ] ).to     eq     1
      expect( result['Draft_Round'     ] ).to     eq     2
      expect( result['Pick_Number'     ] ).to     eq     26
    end

    it 'should not modify the given fields hash' do
      fields = {season: 3}

      @org_repository.save_org 5, fields

      expect( fields.length ).to eq 1
    end

    it 'should not insert a record into the database' do
      @org_repository.save_org 3, {season: 6}

      result = @db.get_first_row 'select * from organizations_t where organization_id = 3'

      expect( result ).to be_nil
    end

    it 'should raise an error if no fields are given' do
      expect { @org_repository.save_org 5, Hash.new }.to raise_error
    end
  end
end
