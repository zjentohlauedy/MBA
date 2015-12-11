require 'utils'

class DivisionRepository

  def initialize( db )
    @db = db
  end

  def get_division_stats_by_highest( stat, season = nil, phase = nil )
    args = {}

    query = "select max(#{stat}) value from division_stats_t"

    unless season.nil?; query = "#{query} where season     = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase = :phase";  args[:phase ] = phase  end

    result = @db.get_first_row query, args

    args[:value] = result['value']

    query = "select * from division_stats_t where #{stat} = :value"

    unless season.nil?; query = "#{query} and season       = :season"; args[:season] = season end
    unless phase.nil?;  query = "#{query} and season_phase = :phase";  args[:phase ] = phase  end

    return Utils::transform_hash @db.execute query, args
  end

  def get_division_accolades( division_id, season = nil )
    args = { division_id: division_id }

    query = 'select * from division_accolades_t where division_id = :division_id'

    unless season.nil?; query = "#{query} and season = :season"; args[:season] = season end

    return Utils::transform_hash @db.execute query, args
  end

  def save_division_accolade( division_accolade )
    result = nil
    args   = {}

    query = 'insert into division_accolades_t values ( :division_id, :season, :accolade )'

    args[ :division_id ] = division_accolade[ :division_id ]
    args[ :season      ] = division_accolade[ :season      ]
    args[ :accolade    ] = division_accolade[ :accolade    ]

    begin
      result = @db.execute query, args
    rescue SQLite3::ConstraintException
    end
  end
end
