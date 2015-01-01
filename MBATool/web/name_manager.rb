#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"

class NameManager

  def initialize( filename )
    @filename = filename
  end

  def load_names
    @names = File.read( @filename ).split /\n/
  end

  def save_names
    File.write @filename, @names.join( "\n" ) + "\n"
  end

  def get_name
    name = @names.pop.split

    if name.length != 2; raise "Unable to process name with #{name.length} parts." end

    return { first: name[0], last: name[1] }
  end

end
