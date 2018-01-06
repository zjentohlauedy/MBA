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
    max = pitchers.sort { |a,b| b.innings.to_f <=> a.innings.to_f }[0].innings.to_f

    return pitchers.select { |p| p.innings.to_f >= (max * 0.4) }
  end

  def filter_batters( batters )
    max = batters.sort { |a,b| b.at_bats <=> a.at_bats }[0].at_bats

    return batters.select { |b| b.at_bats >= (max * 0.4) }
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


path = ARGV[0] || '.'

extract_data = ProgRunner.new location, "extract_data"

extract_data.execute "#{path}/LeagName.Dat", "#{path}/Players.S"

org = JSON.parse extract_data.get_output, {:symbolize_names => true}

printer  = LeadersPrinter.new
filter   = LeadersFilter.new
compiler = LeadersCompiler.new org

sr = StatRankings.new printer, filter, compiler

sr.process_categories @categories
