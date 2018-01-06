#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'ProgRunner'
require 'stat_rankings'


class LeadersPrinter
  def print_empty_indicator()
    puts "--"
  end

  def print( player, format, index, tied )
    value = player.get_sort_value
    value = player.avgs.include?(player.get_sort_key) ? display_avg(value) : value

    if tied; then printf " -  ";
    else          printf "%2d. ", index + 1;
    end

    printf "%-2s %-20s %-15s #{format}\n", player.pos, player.name, player.team, value
  end

  def print_tie_message( summary, format, index )
    printf "%2d.    %-35s  #{format}\n", index + 1, "#{summary.count} Players Tied At", summary.value
  end

  def display_avg(average)
    avg = sprintf "%5.3f", average
    avg.gsub /^0\./, ' .'
  end
end

class LeadersFilter
  def apply( players, stat )
    if players[0].class == Pitcher
      return filter_pitchers players
    elsif players[0].class == Batter
      return filter_batters players
    end

    return players
  end

  def filter_pitchers( pitchers )
    return pitchers.select { |p| p.innings.to_f >= 185.0 }
  end

  def filter_batters( batters )
    return batters.select { |b| b.at_bats >= 300 }
  end
end

class LeadersCompiler
  def initialize( org )
    @org = org
  end

  def compile_stats( list, target_class, type )
    @org[:leagues].each do |league|
      league[:divisions].each do |division|
        division[:teams].each do |team|
          next if team[:players].nil?

          team[:players].each do |player|
            next if  player[:stats][:simulated].nil?

            if player[:type] == type
              list.push( target_class.new team[:name], player, :simulated )
            end
          end
        end
      end
    end
  end
end


if ARGV.size == 1
  path   = '.'
  season = ARGV[0]
elsif ARGV.size > 1
  path   = ARGV[0] || '.'
  season = ARGV[1]
else
  puts "Usage: #{$PROGRAM_NAME} [path] <season>"
  exit
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

printer  = LeadersPrinter.new
filter   = LeadersFilter.new
compiler = LeadersCompiler.new org

sr = StatRankings.new printer, filter, compiler

sr.process_categories @categories
