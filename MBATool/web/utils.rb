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

  def self.adjust_rating( rating, season, rookie_season, longevity )
    adjustment = (season - rookie_season) - (longevity + 3)

    if adjustment < 0 then adjustment = 0 end

    rating -= adjustment

    return (rating > 1) ? rating : 1
  end

  def self.adjust_fatigue( rating, season, rookie_season, longevity )
    adjustment = (season - rookie_season) - (longevity + 3)

    if adjustment < 0 then adjustment = 0 end

    rating -= adjustment

    return (rating > 4) ? rating : 4
  end

end
