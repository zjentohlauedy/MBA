#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'optparse'
require 'ProgRunner'
require 'player_leaders_compiler'
require 'games_based_player_leaders_filter'
require 'relative_player_leaders_filter'
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


def get_max_rookie_season( org )
  rookie_season = '0'

  org[:leagues].each do |league|
    league[:divisions].each do |division|
      division[:teams].each do |team|
        team[:players].each do |player|
          if player[:rookie_season] > rookie_season
            rookie_season = player[:rookie_season]
          end
        end
      end
    end
  end

  return rookie_season
end


@options = {}

OptionParser.new do |opt|
  opt.on( '-g', '--games  GAMES',  'Number of games played'    ) { |o| @options[ :games  ] = o }
  opt.on( '-s', '--season SEASON', 'Rookie season to consider' ) { |o| @options[ :season ] = o }
end.parse!


path = ARGV[0] || '.'

if @options[:season].nil?
#  puts "Usage: #{$PROGRAM_NAME} <-s season> [path]"
#  exit
end


extract_data = ProgRunner.new location, "extract_data"

extract_data.execute "#{path}/LeagName.Dat", "#{path}/Players.S"

org = JSON.parse extract_data.get_output, {:symbolize_names => true}

if @options[:season].nil?
  season = get_max_rookie_season org
else
  season = @options[:season]
end

org[:leagues].each do |league|
  league[:divisions].each do |division|
    division[:teams].each do |team|
      team[:players].select! {|player| player[:rookie_season] == season}
    end
  end
end

printer  = LeadersPrinter.new

if @options[:games]
  filter = GamesBasedPlayerLeadersFilter.new @options[:games].to_i
else
  filter = RelativePlayerLeadersFilter.new
end

compiler = PlayerLeadersCompiler.new org

sr = StatRankings.new printer, filter, compiler

sr.process_categories @categories
