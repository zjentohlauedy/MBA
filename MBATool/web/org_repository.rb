require 'utils'

class OrgRepository

  def initialize( db )
    @db = db
  end

  def get_org( org_id )
    args = { org_id: org_id }

    query = 'select * from organizations_t where organization_id = :org_id'

    return Utils::transform_hash @db.get_first_row query, args
  end

  def save_org( org_id, fields )
    args = fields.clone
    args[:organization_id] = org_id

    query = 'update organizations_t set '

    first_field = true

    fields.keys.each do |field|
      unless first_field
        query += ','
      end

      query += "#{field} = :#{field}"

      first_field = false
    end

    query += ' where organization_id = :organization_id'

    return @db.execute query, args
  end
end
