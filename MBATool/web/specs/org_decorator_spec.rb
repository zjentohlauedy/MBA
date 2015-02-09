location = File.dirname __FILE__
$: << "#{location}"

require 'org_decorator'

describe OrgDecorator do
  before :each do
    @base_url       = 'http://www.mba.org/mba/resources'
    @org_decorator = OrgDecorator.new @base_url
  end

  describe '#decorate_org' do
    it 'should return the given organization hash with links added' do
      org = { organization_id: 4, name: 'Something Else', abbreviation: 'SE', season: 5, stage: 1, draft_round: 2, pick_number: 26 }

      result = @org_decorator.decorate_org org

      expect( result                   ).to_not be_nil
      expect( result                   ).to     be_a   Hash
      expect( result[:organization_id] ).to_not be_nil
      expect( result[:name           ] ).to_not be_nil
      expect( result[:abbreviation   ] ).to_not be_nil
      expect( result[:season         ] ).to_not be_nil
      expect( result[:stage          ] ).to_not be_nil
      expect( result[:draft_round    ] ).to_not be_nil
      expect( result[:pick_number    ] ).to_not be_nil
      expect( result[:_links         ] ).to_not be_nil
    end

    it 'should add a self link' do
      org = {
        organization_id: 4,
        name:            'Something Else',
        abbreviation:    'SE',
        season:          5,
        stage:           1,
        draft_round:     2,
        pick_number:     26
      }

      result = @org_decorator.decorate_org org

      self_link = result[:_links][:self]

      expect( self_link        ).to_not be_nil
      expect( self_link[:rel ] ).to     eq     'self'
      expect( self_link[:href] ).to     eq     "#{@base_url}/organizations/4"
      expect( self_link[:desc] ).to     eq     "This resource"
    end
  end
end
