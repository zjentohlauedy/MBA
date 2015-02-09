location = File.dirname __FILE__
$: << "#{location}"

require 'org_decorator'
require 'org_repository'
require 'org_service'

describe OrgService do
  before :each do
    @repo = double OrgRepository
    @deco = double OrgDecorator

    @org_service = OrgService.new @repo, @deco
  end

  describe '#get_org' do
    it 'should call the repository to get the organiation record from the database' do
      expect( @repo ).to receive( :get_org ).with( 1 ).and_return Hash.new

      allow( @deco ).to receive( :decorate_org )

      @org_service.get_org 1
    end

    it 'should call the decorator with the database record' do
      db_record = {organization_id: 4, name: 'Something Else', abbreviation: 'SE', season: 5, stage: 1, draft_round: 2, pick_number: 26 }

      allow( @repo ).to receive( :get_org ).with( 1 ).and_return db_record

      expect( @deco ).to receive( :decorate_org ).with db_record

      @org_service.get_org 1
    end

    it 'should return the decorated database record' do
      decorated_db_record = {organization_id: 4, name: 'Something Else', abbreviation: 'SE', season: 5, stage: 1, draft_round: 2, pick_number: 26, _links: {} }

      allow( @repo ).to receive( :get_org ).and_return Hash.new
      allow( @deco ).to receive( :decorate_org ).and_return decorated_db_record

      expect( @org_service.get_org 1 ).to be decorated_db_record
    end

    it 'should raise a resource not found error exception if the record is not in the database' do
      allow( @repo ).to receive( :get_org ).with( 1 )

      expect { @org_service.get_org 1 }.to raise_error ResourceNotFoundError, 'Organization with Organization ID 1 cannot be located.'
    end
  end

  describe '#save_org' do
    it 'should call the repository to save the organization record to the database' do
      updates = {name: 'Something Else', abbreviation: 'SE', season: 5, stage: 1, draft_round: 2, pick_number: 26}

      expect( @repo ).to receive( :save_org ).with( 1, updates )

      allow( @repo ).to receive( :get_org ).and_return Hash.new
      allow( @deco ).to receive( :decorate_org )

      @org_service.save_org 1, updates
    end

    it 'should return the newly updated organization record with decorations' do
      updates = {name: 'Something Else', abbreviation: 'SE', season: 5, stage: 1, draft_round: 2, pick_number: 26}
      db_record = {organization_id: 4, name: 'Something Else', abbreviation: 'SE', season: 5, stage: 1, draft_round: 2, pick_number: 26 }
      decorated_db_record = {organization_id: 4, name: 'Something Else', abbreviation: 'SE', season: 5, stage: 1, draft_round: 2, pick_number: 26, _links: {} }

      allow( @repo ).to receive( :save_org ).with( 1, updates )

      expect( @repo ).to receive( :get_org ).with( 1 ).and_return db_record
      expect( @deco ).to receive( :decorate_org ).with( db_record ).and_return decorated_db_record

      expect( @org_service.save_org 1, updates ).to be decorated_db_record
    end

    it 'should raise internal server error if the record is not saved' do
      updates = {name: 'Something Else', abbreviation: 'SE', season: 5, stage: 1, draft_round: 2, pick_number: 26}

      allow( @repo ).to receive( :save_org ).with( 1, updates )
      allow( @repo ).to receive( :get_org )

      expect { @org_service.save_org 1, updates }.to raise_error InternalServerError, 'Organization with Organization ID 1 was not saved successfully.'
    end
  end
end
