location = File.dirname __FILE__
$: << "#{location}"

require 'utils'

describe Utils do
  describe '::transform_hash' do
    it 'should convert string keys to symbols given a single element hash' do
      test_hash = { 'Key_One' => 0, 'Key_Two' => 'test', 'Key_Three' => nil }

      result = Utils::transform_hash test_hash

      expect( result ).to_not be_nil
      expect( result ).to     be_a   Hash

      expect( result[:key_one  ] ).to eq 0
      expect( result[:key_two  ] ).to eq 'test'
      expect( result[:key_three] ).to eq nil
    end

    it 'should convert string keys to symbols given an array of hashes' do
      test_array = [{ 'Key_One' => 0, 'Key_Two' => 'test', 'Key_Three' =>  nil   },
                    { 'Key_One' => 1, 'Key_Two' =>  nil,   'Key_Three' => 'test' }]

      result = Utils::transform_hash test_array

      expect( result ).to_not be_nil
      expect( result ).to     be_a   Array

      expect( result.length ).to eq 2

      expect( result[0][:key_one  ] ).to eq 0
      expect( result[0][:key_two  ] ).to eq 'test'
      expect( result[0][:key_three] ).to eq nil

      expect( result[1][:key_one  ] ).to eq 1
      expect( result[1][:key_two  ] ).to eq nil
      expect( result[1][:key_three] ).to eq 'test'
    end

    it 'should return nil if the given object is not a hash or array' do
      expect( Utils::transform_hash nil ).to be_nil
      expect( Utils::transform_hash 123 ).to be_nil
      expect( Utils::transform_hash 'x' ).to be_nil
      expect( Utils::transform_hash Object.new ).to be_nil
    end

    it 'should return nil if the given array is not an array of hashes' do
      expect( Utils::transform_hash [1,2,3] ).to be_nil
    end

    it 'should return an empty hash given an empty hash' do
      expect( Utils::transform_hash Hash.new ).to eq Hash.new
    end

    it 'should return an empty array given an empty array' do
      expect( Utils::transform_hash Array.new ).to eq Array.new
    end
  end

  describe '::adjust_rating' do
    it 'should return an adjusted rating' do
      expect( Utils::adjust_rating 5, 5, 5, 5 ).to be_a Integer
    end

    it 'should adjust the rating based on the given season, rookie season and longevity' do
      expect( Utils::adjust_rating 5,  5, 5, 5 ).to eq 5
      expect( Utils::adjust_rating 5,  6, 5, 5 ).to eq 5
      expect( Utils::adjust_rating 5,  7, 5, 5 ).to eq 5
      expect( Utils::adjust_rating 5,  8, 5, 5 ).to eq 5
      expect( Utils::adjust_rating 5,  9, 5, 5 ).to eq 5
      expect( Utils::adjust_rating 5, 10, 5, 5 ).to eq 5
      expect( Utils::adjust_rating 5, 11, 5, 5 ).to eq 5
      expect( Utils::adjust_rating 5, 12, 5, 5 ).to eq 5
      expect( Utils::adjust_rating 5, 13, 5, 5 ).to eq 5
      expect( Utils::adjust_rating 5, 14, 5, 5 ).to eq 4
      expect( Utils::adjust_rating 5, 15, 5, 5 ).to eq 3
      expect( Utils::adjust_rating 5, 16, 5, 5 ).to eq 2
      expect( Utils::adjust_rating 5, 17, 5, 5 ).to eq 1
      expect( Utils::adjust_rating 5, 18, 5, 5 ).to eq 1
      expect( Utils::adjust_rating 5, 19, 5, 5 ).to eq 1
      expect( Utils::adjust_rating 5, 20, 5, 5 ).to eq 1
    end
  end

  describe '::adjust_fatigue' do
    it 'should return an adjusted fatigue rating' do
      expect( Utils::adjust_fatigue 5, 5, 5, 5 ).to be_a Integer
    end

    it 'should adjust the fatigue rating based on the given season, rookie season and longevity' do
      expect( Utils::adjust_fatigue 9,  5, 5, 5 ).to eq 9
      expect( Utils::adjust_fatigue 9,  6, 5, 5 ).to eq 9
      expect( Utils::adjust_fatigue 9,  7, 5, 5 ).to eq 9
      expect( Utils::adjust_fatigue 9,  8, 5, 5 ).to eq 9
      expect( Utils::adjust_fatigue 9,  9, 5, 5 ).to eq 9
      expect( Utils::adjust_fatigue 9, 10, 5, 5 ).to eq 9
      expect( Utils::adjust_fatigue 9, 11, 5, 5 ).to eq 9
      expect( Utils::adjust_fatigue 9, 12, 5, 5 ).to eq 9
      expect( Utils::adjust_fatigue 9, 13, 5, 5 ).to eq 9
      expect( Utils::adjust_fatigue 9, 14, 5, 5 ).to eq 8
      expect( Utils::adjust_fatigue 9, 15, 5, 5 ).to eq 7
      expect( Utils::adjust_fatigue 9, 16, 5, 5 ).to eq 6
      expect( Utils::adjust_fatigue 9, 17, 5, 5 ).to eq 5
      expect( Utils::adjust_fatigue 9, 18, 5, 5 ).to eq 4
      expect( Utils::adjust_fatigue 9, 19, 5, 5 ).to eq 4
      expect( Utils::adjust_fatigue 9, 20, 5, 5 ).to eq 4
    end

    it 'should should return same rating value if input fatigue is 1' do
      expect( Utils::adjust_fatigue 1, 18, 5, 5 ).to eq 1
    end
  end
end
