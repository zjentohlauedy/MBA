#!/usr/bin/env ruby
#
# Prints and tracks injuried players
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'ProgRunner'
require_relative 'MBATool/web/positions'


def get_position_value(player)
  if player[:type] == 'pitcher'; return 'P'; end

  return Positions::string_value player[:primary_position]
end


path = ARGV[0] || '.'
state_filename = "#{path}/.injuries"

extract_data = ProgRunner.new location, "extract_data"

extract_data.execute "#{path}/LeagName.Dat", "#{path}/Players.S"

org = JSON.parse extract_data.get_output, {:symbolize_names => true}

previous_state = begin
                   JSON::parse File::read state_filename
                 rescue Exception => e
                   []
                 end

injuries = []

org[:leagues].each do |league|
  league[:divisions].each do |division|
    division[:teams].each do |team|
      next if team[:players].nil?

      should_print_team = true

      team[:players].each do |player|
        next if player[:injury_days] == 0

        player_before = previous_state.select {|pb| pb['id'] == player[:id]}[0]

        next if player[:type] == 'pitcher' and player[:injury_days] <= 4 and player_before.nil?

        if should_print_team
          puts "#{team[:name]}:"

          should_print_team = false
        end

        injuries.push player

        notice = player_before ? 'Games Remaining' : 'Games'

        printf "%-2s %-20s %2d %s\n",
               get_position_value( player ),
               "#{player[:last_name]}, #{player[:first_name]}",
               player[:injury_days],
               notice
      end

      unless should_print_team; puts ""; end
    end
  end
end

state_file = File.new state_filename, File::CREAT|File::TRUNC|File::RDWR, 0644

state_file.write JSON.pretty_generate injuries
state_file.close
