#!/usr/bin/env ruby
#
require 'csv'
require 'json'
require 'optparse'


def process_pitcher(line)
  pitcher = {
    type: :pitcher,
    name: "#{line['Last,']} #{line['First']}",
    wins: { value: line['W'].to_i, rating: nil },
    losses: { value: line['L'].to_i, rating: nil },
    era: { value: line['ERA'].to_f, rating: nil },
    games: { value: line['G'].to_i, rating: nil },
    saves: { value: line['SV'].to_i, rating: nil },
    inn: { value: line['INN'].to_f, rating: nil },
    hits: { value: line['H'].to_i, rating: nil },
    er: { value: line['ER'].to_i, rating: nil },
    hr: { value: line['HR'].to_i, rating: nil },
    bb: { value: line['BB'].to_i, rating: nil },
    so: { value: line['SO'].to_i, rating: nil },
    vsba: { value: line['VSBA'].to_f, rating: nil },
    innp9: { value: line['INN/G'].to_f, rating: nil },
    whip: { value: line['WHIP'].to_f, rating: nil },
    sop9: { value: line['SO/9'].to_f, rating: nil },
    hrp9: { value: line['HR/9'].to_f, rating: nil },
    eff: { value: line['Eff'].to_f, rating: nil },
    rating: 0
  }

  return pitcher
end

def process_batter(line)
  batter = {
    type: :batter,
    name: "#{line['Last,']} #{line['First']}",
    avg: { value: line['BA'].to_f, rating: nil },
    games: { value: line['G'].to_i, rating: nil },
    ab: { value: line['AB'].to_i, rating: nil },
    runs: { value: line['R'].to_i, rating: nil },
    hits: { value: line['H'].to_i, rating: nil },
    _2b: { value: line['2B'].to_i, rating: nil },
    _3b: { value: line['3B'].to_i, rating: nil },
    hr: { value: line['HR'].to_i, rating: nil },
    rbi: { value: line['RBI'].to_i, rating: nil },
    bb: { value: line['BB'].to_i, rating: nil },
    so: { value: line['SO'].to_i, rating: nil },
    sb: { value: line['SB'].to_i, rating: nil },
    errors: { value: line['E'].to_i, rating: nil },
    sa: { value: line['SA'].to_f, rating: nil },
    oba: { value: line['OBA'].to_f, rating: nil },
    soa: { value: line['SOA'].to_f, rating: nil },
    rpg: { value: line['RPG'].to_f, rating: nil },
    rating: 0
  }

  return batter
end

def rank_stat(stat, target, players, max_points)
  values = players.map { |p| p[stat][:value] }

  target[:rating] = max_points.to_i

  last_value = nil

  ordered_values = nil

  lower_is_better_stats = []

  if players[0][:type] == :pitcher
    lower_is_better_stats = [:era, :hits, :er, :hr, :bb, :vsba, :whip, :hrp9]
  else
    lower_is_better_stats = [:so, :soa]
  end

  if lower_is_better_stats.include? stat
    ordered_values = values.sort
  else
    ordered_values = values.sort.reverse
  end

  ordered_values.each do |value|
    break if target[:value] == value
    next if value == last_value

    last_value = value
    target[:rating] -= 1

    if target[:rating] < 0; target[:rating] = 0; end
  end
end

def rate_player(player)
  stats = []

  if player[:type] == :pitcher
    if @options[:closers]
      stats = [:saves, :era, :vsba, :whip, :eff]
    else
      stats = [:wins, :era, :vsba, :whip, :eff]
    end
  else
    stats = [:sa, :oba, :soa, :rpg]
  end

  stats.each do |stat|
    player[:rating] += player[stat][:rating]
  end
end


@options = {}

OptionParser.new do |opt|
  opt.on( '-c', '--closers' ) { |o| @options[ :closers ] = o }
end.parse!


file = ARGV[0]
max_points = ARGV[1]

players = []

CSV::foreach(ARGV[0], headers: true, col_sep: "\t") do |line|
  if line['P'] == 'P'
    players.push process_pitcher(line)
  else
    players.push  process_batter(line)
  end
end

players.each do |player|
  player.keys.each do |key|
    next if [:type, :name, :rating].include? key

    rank_stat key, player[key], players, max_points
  end

  rate_player player
end

#puts JSON::pretty_generate players

if players[0][:type] == :pitcher
  puts "   Pitchers              W  L   ERA   G SV   INN   H  ER  HR  BB  SO  VSBA INN/G  WHIP  SO/9  HR/9    Eff  Rating"
else
  puts "   Batters                 BA   G  AB   R   H  2B  3B  HR RBI  BB  SO  SB  E    SA   OBA   SOA  RPG  Rating"
end

players.each do |player|
  if players[0][:type] == :pitcher
    printf "   %-19s  %2d %2d    %2d  %2d %2d    %2d  %2d  %2d  %2d  %2d  %2d    %2d    %2d    %2d    %2d    %2d     %2d  %3d\n",
           player[:name],
           player[:wins][:rating],
           player[:losses][:rating],
           player[:era][:rating],
           player[:games][:rating],
           player[:saves][:rating],
           player[:inn][:rating],
           player[:hits][:rating],
           player[:er][:rating],
           player[:hr][:rating],
           player[:bb][:rating],
           player[:so][:rating],
           player[:vsba][:rating],
           player[:innp9][:rating],
           player[:whip][:rating],
           player[:sop9][:rating],
           player[:hrp9][:rating],
           player[:eff][:rating],
           player[:rating]
  else
    printf "   %-22s  %2d  %2d  %2d  %2d  %2d  %2d  %2d  %2d  %2d  %2d  %2d  %2d %2d    %2d    %2d    %2d   %2d  %3d\n",
           player[:name],
           player[:avg][:rating],
           player[:games][:rating],
           player[:ab][:rating],
           player[:runs][:rating],
           player[:hits][:rating],
           player[:_2b][:rating],
           player[:_3b][:rating],
           player[:hr][:rating],
           player[:rbi][:rating],
           player[:bb][:rating],
           player[:so][:rating],
           player[:sb][:rating],
           player[:errors][:rating],
           player[:sa][:rating],
           player[:oba][:rating],
           player[:soa][:rating],
           player[:rpg][:rating],
           player[:rating]
  end
end
