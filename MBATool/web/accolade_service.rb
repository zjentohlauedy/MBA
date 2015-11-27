#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

require 'accolades'

class AccoladeService

  def initialize
    @accolades = Accolades::AccoladeList
  end

  def get_accolades
    return @accolades
  end
end
