#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'ProgRunner'
require 'TopPlayers'


if ARGV.size == 1
  path   = '.'
  season = ARGV[0]
elsif ARGV.size > 1
  path   = ARGV[0] || '.'
  season = ARGV[1]
else
  path   = '.'
  season = '0'
end

extract_data = ProgRunner.new location, "extract_data"

extract_data.execute "#{path}/LeagName.Dat", "#{path}/Players.S"

org = JSON.parse extract_data.get_output, {:symbolize_names => true}

org[:leagues].each do |league|
  league[:divisions].each do |division|
    division[:teams].each do |team|
      team[:players].select! {|player| player[:rookie_season] == season}
    end
  end
end

sr = StatRankings.new org

sr.process_categories @categories
