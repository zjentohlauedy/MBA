#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'tmpdir'
require 'FileParser'
require 'ProgRunner'
require 'ScheduleParser'
require 'TeamRecords'
require 'stat_rankings'


class LeadersPrinter
  def print_empty_indicator()
    puts "--"
  end

  def print( team, format, index, tied )
    value = team.get_sort_value
    value = team.avgs.include?(team.get_sort_key) ? display_avg(value) : value

    if tied; then printf " -  ";
    else          printf "%2d. ", index + 1;
    end

    printf "%-20s #{format}\n", team.team, value
  end

  def print_tie_message( summary, format, index )
    printf "%2d. %-20s #{format}\n", index + 1, "#{summary.count} Teams Tied At", summary.value
  end

  def display_avg(average)
    avg = sprintf "%5.3f", average
    avg.gsub /^0\./, ' .'
  end
end

class LeadersFilter
  def apply( teams, stat )
    return teams
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
          if    type == 'records'  and !team[:stats].nil?
            list.push( target_class.new team )
          elsif type == 'pitching' and !team[:pitching_stats][:simulated].nil?
            list.push( target_class.new team, :simulated )
          elsif type == 'batting'  and !team[:batting_stats][:simulated].nil?
            list.push( target_class.new team, :simulated )
          end
        end
      end
    end
  end
end


path = ARGV[0] || '.'


def add_game( team_records, game )
  return if !game.played

  # road team
  if team_records.has_key? game.road_team
    team_record = team_records[game.road_team]
  else
    team_record = TeamRecords.new game.road_team
  end

  team_record.update game, false
  team_records[game.road_team] = team_record

  # home team
  if team_records.has_key? game.home_team
    team_record = team_records[game.home_team]
  else
    team_record = TeamRecords.new game.home_team
  end

  team_record.update game, true
  team_records[game.home_team] = team_record
end


def init_pitching_stats( team, style )
  team[:pitching_stats][style] = {
    wins:        0,
    losses:      0,
    games:       0,
    innings:     0,
    outs:        0,
    saves:       0,
    hits:        0,
    earned_runs: 0,
    home_runs:   0,
    walks:       0,
    strike_outs: 0
  }
end

def init_batting_stats( team, style )
  team[:batting_stats][style] = {
    games:          0,
    at_bats:        0,
    runs:           0,
    hits:           0,
    doubles:        0,
    triples:        0,
    home_runs:      0,
    runs_batted_in: 0,
    walks:          0,
    strike_outs:    0,
    steals:         0,
    errors:         0
  }
end

def add_pitching_stats( team, player, style )
  tps = team[:pitching_stats][style]
  pps = player[:stats][style]

  tps[ :wins        ] += pps[ :wins        ]
  tps[ :losses      ] += pps[ :losses      ]
  tps[ :games       ] += pps[ :games       ]
  tps[ :innings     ] += pps[ :innings     ]
  tps[ :outs        ] += pps[ :outs        ]
  tps[ :saves       ] += pps[ :saves       ]
  tps[ :hits        ] += pps[ :hits        ]
  tps[ :earned_runs ] += pps[ :earned_runs ]
  tps[ :home_runs   ] += pps[ :home_runs   ]
  tps[ :walks       ] += pps[ :walks       ]
  tps[ :strike_outs ] += pps[ :strike_outs ]
end

def add_batting_stats( team, player, style )
  tbs = team[:batting_stats][style]
  pbs = player[:stats][style]

  tbs[ :games          ] += pbs[ :games          ]
  tbs[ :at_bats        ] += pbs[ :at_bats        ]
  tbs[ :runs           ] += pbs[ :runs           ]
  tbs[ :hits           ] += pbs[ :hits           ]
  tbs[ :doubles        ] += pbs[ :doubles        ]
  tbs[ :triples        ] += pbs[ :triples        ]
  tbs[ :home_runs      ] += pbs[ :home_runs      ]
  tbs[ :runs_batted_in ] += pbs[ :runs_batted_in ]
  tbs[ :walks          ] += pbs[ :walks          ]
  tbs[ :strike_outs    ] += pbs[ :strike_outs    ]
  tbs[ :steals         ] += pbs[ :steals         ]
  tbs[ :errors         ] += pbs[ :errors         ]
end

def add_stats(team, player, style)
  if player[:stats][style].nil?; then return; end

  if player[:type] == 'pitcher'
    if team[:pitching_stats][style].nil?
      init_pitching_stats team, style
    end

    add_pitching_stats team, player, style
  else
    if team[:batting_stats][style].nil?
      init_batting_stats team, style
    end

    add_batting_stats team, player, style
  end
end

def init_team_stats( team, records )
  ts = team[:stats]

  ts[ :wins            ] = records.overall.wins
  ts[ :losses          ] = records.overall.losses
  ts[ :home_wins       ] = records.home.wins
  ts[ :home_losses     ] = records.home.losses
  ts[ :road_wins       ] = records.road.wins
  ts[ :road_losses     ] = records.road.losses
  ts[ :division_wins   ] = records.division.wins
  ts[ :division_losses ] = records.division.losses
  ts[ :league_wins     ] = records.league.wins
  ts[ :league_losses   ] = records.league.losses
  ts[ :runs_scored     ] = records.points_scored
  ts[ :runs_allowed    ] = records.points_allowed
end

def compile_team_stats( team, records )
  team[ :stats          ] = {}
  team[ :pitching_stats ] = {}
  team[ :batting_stats  ] = {}

  init_team_stats team, records

  if team[:players].nil?; then return; end

  team[:players].each do |player|
    add_stats team, player, :potential
    add_stats team, player, :simulated
  end
end

extract_data = ProgRunner.new location, "extract_data"

extract_data.execute "#{path}/LeagName.Dat", "#{path}/Players.S"

convert_schedule = ProgRunner.new '/usr/bin', 'soffice'

convert_schedule.execute "-env:UserInstallation=file://#{ENV['HOME']}/.libreoffice-alt", '--headless', '--convert-to', 'csv', '--outdir', Dir::tmpdir, './schedule.ods'

if !convert_schedule.success?
  puts "Error converting schedule: "
  puts convert_schedule.get_output
  raise "Processing Failed"
end

sp = ScheduleParser.new
fp = FileParser.new sp

fp.process_file "#{Dir::tmpdir}/schedule.csv"

team_records = {}

sp.schedule.days.each do |day|
  next if day.day > 152

  day.games.each do |game|
    add_game team_records, game
  end
end

org = JSON.parse extract_data.get_output, {:symbolize_names => true}

org[:leagues].each do |league|
  league[:divisions].each do |division|
    division[:teams].each do |team|
      compile_team_stats team, team_records[team[:name]]
    end
  end
end

printer  = LeadersPrinter.new
filter   = LeadersFilter.new
compiler = LeadersCompiler.new org

sr = StatRankings.new printer, filter, compiler

sr.process_categories @team_categories
