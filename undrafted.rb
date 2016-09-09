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


def get_organization( organization_id )
  result = Utils::transform_hash @db.execute "select * from organizations_t where organization_id = #{organization_id}"
  result[0]
end


def get_accolades( player_id )
  Utils::transform_hash @db.execute %Q(select 'player'   as type, season, accolade from player_accolades_t  where player_id = #{player_id} union
                                select 'pitching' as type, season, accolade from pitcher_accolades_t where player_id = #{player_id} union
                                select 'batting'  as type, season, accolade from batter_accolades_t  where player_id = #{player_id}      )
end


def get_position( player )
  if player[:player_type] == PlayerTypes::Pitcher; return 'P' end

  result = Utils::transform_hash @db.execute "select primary_position from batters_t where player_id = #{player[:player_id]}"

  return Positions::string_value result[0][:primary_position]
end


current_season  = get_organization(1)[:season]
previous_season = current_season - 1

results = Utils::transform_hash @db.execute %Q(
  select p.player_id, p.first_name, p.last_name, p.player_type, (ifnull(pa.total, 0) + ifnull(pi.total, 0) + ifnull(ba.total, 0)) grand_total
  from team_players_t tp
  join players_t p on tp.player_id = p.player_id
  left outer join (select player_id, count(1) total from player_accolades_t group by player_id) pa on pa.player_id = tp.player_id
  left outer join (select player_id, count(1) total from pitcher_accolades_t group by player_id) pi on pi.player_id = tp.player_id
  left outer join (select player_id, count(1) total from batter_accolades_t group by player_id) ba on ba.player_id = tp.player_id
  where season = #{previous_season} and grand_total > 0 and tp.player_id not in (select player_id from team_players_t where season = #{current_season})
  order by grand_total desc, last_name asc, first_name asc
)

results.each do |player|
  puts "#{get_position(player)} #{player[:first_name]} #{player[:last_name]}:"

  accolades = get_accolades( player[:player_id] )

  accolades.sort! { |a, b| a[:season] <=> b[:season] }

  accolades.each do |accolade|
    puts " - Season #{accolade[:season]} #{Accolades::get_accolade_name accolade[:type], accolade[:accolade]}"
  end
end
