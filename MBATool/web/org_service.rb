#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

require 'internal_server_error'
require 'resource_not_found_error'

class OrgService

  def initialize( repository, decorator )
    @repository = repository
    @decorator  = decorator
  end

  def get_org( org_id )
    org = @repository.get_org org_id

    if org.nil?
      raise ResourceNotFoundError.new "Organization with Organization ID #{org_id} cannot be located."
    end

    return @decorator.decorate_org org
  end

  def save_org( org_id, fields )
    @repository.save_org org_id, fields

    begin
      return get_org org_id
    rescue ResourceNotFoundError => e
      raise InternalServerError.new, "Organization with Organization ID #{org_id} was not saved successfully."
    end
  end

end
