module Utils
  def self.transform_hash( hash )
    unless hash.is_a? Hash or hash.is_a? Array
      return nil
    end

    if hash.is_a? Array
      result = []

      hash.each do |element|
        unless element.is_a? Hash
          return nil
        end
        result.push Hash[element.map {|key,value| [key.downcase.to_sym, value]}]
      end

      return result
    else
      return Hash[hash.map {|key,value| [key.downcase.to_sym, value]}]
    end
  end
end
