#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"

ARGV.each do |file|
  File::foreach(file) do |line|
    fields = line.strip.split %r{ +}

    next if fields[0] == 'TOTAL'

    if fields.size == 15
      hits    = fields[3].to_i
      doubles = fields[4].to_i
      triples = fields[5].to_i
      homers  = fields[6].to_i
      xbase   = doubles + triples + homers

      if doubles > 0 and triples > 0 and homers > 0 and hits > xbase
        puts "#{file}: #{line.strip}"
      end
    end
  end
end
