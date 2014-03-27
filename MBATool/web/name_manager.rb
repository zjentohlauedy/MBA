#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"

class NameManager

  def initialize
    @filename = "/home/zjentohlauedy/Amiga/HD/MBA/names.txt"
    @names    = nil
  end

  def load_names
    @names = File.read( @filename ).split /\n/
  end

  def save_names
    File.write @filename, @names.join( "\n" ) + "\n"
  end

  def get_name
    return @names.pop
  end

end
