#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"

require 'boxscore_parser'
require 'consecutive_game_leaders_printer'
require 'consecutive_game_streak_finder'
require 'optparse'
require 'streaks'


class ExtraBaseHitStreakDetector
  def is_streak?( player )
    (player[:doubles] + player[:triples] + player[:home_runs]) > 0
  end
end

@options = {}

OptionParser.new do |opt|
  opt.on( '-s', '--season SEASON' ) { |o| @options[ :season ] = o }
end.parse!


streaks = Streaks.new ' - ', ExtraBaseHitStreakDetector.new
suspicious_players = []
seasons = []

if @options[ :season ]
  seasons.push sprintf("S%02d", @options[ :season ])
else
  seasons = Dir::entries(location).select {|f| f =~ /^S[0-9][0-9]$/}.sort
end

seasons.each do |season|
  printf "\rCompiling streaks for season %s...", season

  streaks.add_new_season season

  cgsf = ConsecutiveGameStreakFinder.new "#{location}/#{season}", streaks

  cgsf.find

  suspicious_players += cgsf.suspicious_players
end

puts ""

ConsecutiveGameLeadersPrinter.print_leaders streaks.all_streaks, suspicious_players
