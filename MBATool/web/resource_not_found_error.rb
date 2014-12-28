#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

class ResourceNotFoundError < StandardError
end
