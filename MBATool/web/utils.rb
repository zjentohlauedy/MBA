module Utils
  def self.transform_hash( hash )
#    result = []

#    db_hash.each do |element|
#      hash = {}

#      element.each do|key, value|
#        hash.store key.downcase.to_sym, value
#      end

#      result.push hash
#    end

#    return result

    if hash.is_a? Array
      result = []

      hash.each do |element|
        result.push Hash[element.map {|key,value| [key.downcase.to_sym, value]}]
      end

      return result
    else
      return Hash[hash.map {|key,value| [key.downcase.to_sym, value]}]
    end
  end
end
