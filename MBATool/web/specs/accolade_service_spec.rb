location = File.dirname __FILE__
$: << "#{location}"

require 'accolade_service'

describe AccoladeService do
  before :each do
    @accolade_service = AccoladeService.new
  end

  describe '#get_accolades' do
    it 'should return the accolade list' do
      results = @accolade_service.get_accolades

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     53

      results.each do |entry|
        expect( entry[ :name      ] ).to_not be_nil
        expect( entry[ :name      ] ).to     be_a   String
        expect( entry[ :value     ] ).to_not be_nil
        expect( entry[ :value     ] ).to     be_a   Fixnum
        expect( entry[ :type      ] ).to_not be_nil
        expect( entry[ :type      ] ).to     be_a   String
        expect( entry[ :automatic ] ).to_not be_nil
        expect( [true, false] ).to include entry[ :automatic ]
      end
    end
  end
end
