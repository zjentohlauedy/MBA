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

    if fields.size == 11
      if fields[1].to_f >= 7.0 and fields[2].to_i == 0
        puts "#{file}: #{line.strip}"
      end
    end
  end
end
