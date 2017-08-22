#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'sqlite3'

require_relative 'MBATool/web/accolades'
require_relative 'MBATool/web/player_types'
require_relative 'MBATool/web/positions'
require_relative 'MBATool/web/utils'


@db = SQLite3::Database.new "#{location}/mba.db"

@db.results_as_hash  = true
@db.type_translation = true


def get_current_season()
  result = Utils::transform_hash @db.execute "select * from organizations_t where organization_id = 1"

  if result.nil? or result.length == 0
    raise "Unable to determine current season."
  end

  return result[0][:season]
end

def get_undrafted_batters( season )
  args = { season: season, player_type: PlayerTypes::Batter }
  query = '''
  SELECT p.*, b.*
  FROM players_t p
  LEFT JOIN team_players_t tp ON p.player_id = tp.player_id AND tp.season = :season
       JOIN batters_t       b ON p.player_id =  b.player_id
  WHERE  p.player_type     = :player_type
  AND    p.rookie_season  <= :season
  AND    p.retired_season IS NULL
  AND   tp.player_id      IS NULL
  '''

  return Utils::transform_hash @db.execute query, args
end

def get_undrafted_pitchers( season )
  args = { season: season, player_type: PlayerTypes::Pitcher }
  query = '''
  SELECT p.*, pi.*
  FROM players_t p
  LEFT JOIN team_players_t tp ON p.player_id = tp.player_id AND tp.season = :season
       JOIN pitchers_t     pi ON p.player_id = pi.player_id
  WHERE  p.player_type     = :player_type
  AND    p.rookie_season  <= :season
  AND    p.retired_season IS NULL
  AND   tp.player_id      IS NULL
  '''

  return Utils::transform_hash @db.execute query, args
end

def get_player_accolades( player_id )
  args = { player_id:   player_id,
           allstar:     Accolades::Player::All_Star,
           allstar_mvp: Accolades::Player::All_Star_MVP,
           ws_mvp:      Accolades::Player::World_Series_MVP }
  query = '''
  SELECT *
  FROM player_accolades_t
  WHERE player_id = :player_id
  AND accolade in ( :allstar, :allstar_mvp, :ws_mvp )
  '''

  results = Utils::transform_hash @db.execute query, args

  results.each do |result|
    result[:accolade_name] = Accolades::get_accolade_name Accolades::Player::Type, result[:accolade]
  end

  return results
end

def get_batter_accolades( player_id )
  args = { player_id:   player_id }
  query = '''
  SELECT *
  FROM batter_accolades_t
  WHERE player_id = :player_id
  '''

  results = Utils::transform_hash @db.execute query, args

  results.each do |result|
    result[:accolade_name] = Accolades::get_accolade_name Accolades::Batting::Type, result[:accolade]
  end

  return results
end

def get_pitcher_accolades( player_id )
  args = { player_id:   player_id }
  query = '''
  SELECT *
  FROM pitcher_accolades_t
  WHERE player_id = :player_id
  '''

  results = Utils::transform_hash @db.execute query, args

  results.each do |result|
    result[:accolade_name] = Accolades::get_accolade_name Accolades::Pitching::Type, result[:accolade]
  end

  return results
end

def mark_player_retired( player_id, season )
  args = { player_id: player_id, season: season }
  query = '''
  UPDATE players_t
  SET    retired_season = :season
  WHERE  player_id = :player_id
  '''

  @db.execute query, args
end

def retire_batters( season )
  nominees = get_undrafted_batters season
  retirees = []

  nominees.each do |nominee|
    nominee[ :power     ] = Utils::adjust_rating nominee[ :power     ], season, nominee[:rookie_season], nominee[:longevity]
    nominee[ :hit_n_run ] = Utils::adjust_rating nominee[ :hit_n_run ], season, nominee[:rookie_season], nominee[:longevity]
    nominee[ :bunt      ] = Utils::adjust_rating nominee[ :bunt      ], season, nominee[:rookie_season], nominee[:longevity]
    nominee[ :running   ] = Utils::adjust_rating nominee[ :running   ], season, nominee[:rookie_season], nominee[:longevity]
    nominee[ :range     ] = Utils::adjust_rating nominee[ :range     ], season, nominee[:rookie_season], nominee[:longevity]
    nominee[ :arm       ] = Utils::adjust_rating nominee[ :arm       ], season, nominee[:rookie_season], nominee[:longevity]

    nominee[:avg]      = (nominee[:power] + nominee[:hit_n_run] + nominee[:running]).to_f / 3.0;
    nominee[:over_six] = [
      nominee[ :power     ],
      nominee[ :hit_n_run ],
      nominee[ :bunt      ],
      nominee[ :running   ],
      nominee[ :range     ],
      nominee[ :arm       ]
    ].max > 6 ? true : false

    if nominee[:avg] < 4.0 and ! nominee[:over_six]
      mark_player_retired nominee[:player_id], season

      retirees.push nominee
    end
  end

  return retirees
end

def retire_pitchers( season )
  nominees = get_undrafted_pitchers season
  retirees = []

  nominees.each do |nominee|
    nominee[ :speed   ] = Utils::adjust_rating nominee[ :speed   ], season, nominee[:rookie_season], nominee[:longevity]
    nominee[ :control ] = Utils::adjust_rating nominee[ :control ], season, nominee[:rookie_season], nominee[:longevity]
    nominee[ :fatigue ] = Utils::adjust_rating nominee[ :fatigue ], season, nominee[:rookie_season], nominee[:longevity]

    nominee[:avg] = (nominee[:speed] + nominee[:control]).to_f / 2.0;
    nominee[:over_six] = [
      nominee[ :speed   ],
      nominee[ :control ],
      nominee[ :fatigue ]
    ].max > 6 ? true : false

    if nominee[:avg] < 4.0 and ! nominee[:over_six]
      mark_player_retired nominee[:player_id], season

      retirees.push nominee
    end
  end

  return retirees
end


if ARGV.length == 1
  @season = ARGV[0].to_i
else
  @season = get_current_season
end


@notables = []


retired_batters = retire_batters @season

puts "#{retired_batters.length} batters have retired after season #{@season}"

retired_batters.each do |batter|
  player_accolades = get_player_accolades batter[:player_id]
  batter_accolades = get_batter_accolades batter[:player_id]

  batter[:accolades] = player_accolades.concat batter_accolades

  if batter[:accolades].size > 0
    @notables.push batter
  end
end

retired_pitchers = retire_pitchers @season

puts "#{retired_pitchers.length} pitchers have retired after season #{@season}"

retired_pitchers.each do |pitcher|
  player_accolades  = get_player_accolades  pitcher[:player_id]
  pitcher_accolades = get_pitcher_accolades pitcher[:player_id]

  pitcher[:accolades] = player_accolades.concat pitcher_accolades

  if pitcher[:accolades].size > 0
    @notables.push pitcher
  end
end

if @notables.length == 0
  exit
end

puts ""
puts "Notables:"

@notables.sort {|a,b| a[:accolades].length <=> b[:accolades].length}.reverse.each do |player|
  if player[:player_type] == PlayerTypes::Pitcher
    printf 'P '
  else
    printf "#{Positions::string_value player[:primary_position]} "
  end

  printf "#{player[:first_name]} #{player[:last_name]} - #{@season - player[:rookie_season]} seasons "

  if player[:rookie_season] < 1
    puts "(#{@season - 1} in MBA)"
  else
    puts ""
  end

  player[:accolades].sort {|a,b| a[:season] <=> b[:season]}.each do |accolade|
    printf " - S%02d %s\n", accolade[:season], accolade[:accolade_name]
  end

  puts ""
end
