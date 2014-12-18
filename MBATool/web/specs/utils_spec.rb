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
  end
end
