#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

class OrgDecorator

  def initialize( base_url )
    @base_url = base_url
  end

  def decorate_org( org )
    _links = {}

    _links[:self] = { rel:  'self',
                      href: "#{@base_url}/organizations/#{org[:organization_id]}",
                      desc: 'This resource' }

    org[:_links] = _links

    return org
  end
end
