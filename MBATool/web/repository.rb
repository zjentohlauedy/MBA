#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'sqlite3'

class Repository

    def initialize( mapper )
      @db     = nil
      @mapper = mapper
      connect
    end

    def connect
      @db = SQLite3::Database.new "mba.db"

      @db.results_as_hash  = true
      @db.type_translation = true
    end

    def disconnect
      @db.close
    end

    def get_teams
      return @mapper.map_teams_response @db.execute 'select * from teams_t'
    end

    def get_team( params )
      args = {}

      query = 'select * from teams_t where team_id = :team_id'; args[:team_id] = params[:team_id]

      return @mapper.map_team_response (@db.execute query, args)[0]
    end

    def get_team_stats( params )
      args = {}

      query = 'select * from team_stats_t where team_id = :team_id'; args[:team_id] = params[:team_id]

      if params.has_key? "season"; query = "#{query} and season        = :season"; args[:season] = params[:season] end
      if params.has_key? "phase";  query = "#{query} and season_phase  = :phase";  args[:phase ] = params[:phase ] end

      return @mapper.map_team_stats_response @db.execute query, args
    end

    def get_team_players( params )
      args = {}

      query = 'select * from team_players_t as tp natural join players_t as p where tp.team_id = :team_id'; args[:team_id] = params[:team_id]

      if params.has_key? "season"
        query = "#{query} and tp.season = :season"; args[:season] = params[:season]
      end

      return @mapper.map_players_response @db.execute query, args
    end

    def get_players
      return @mapper.map_players_response @db.execute 'select * from players_t'
    end

    def get_player( params )
      args = {}

      query = 'select * from players_t where player_id = :player_id'; args[:player_id] = params[:player_id]

      player = (@db.execute query, args)[0]

      player['Details'] = get_player_details player

      return @mapper.map_player_response player
    end

    def get_player_details( player )
      args = {}

      if player['Player_Type'] == 1
        query = 'select * from pitchers_t where player_id = :player_id'; args[:player_id] = player['Player_Id']
      end

      if player['Player_Type'] == 2
        query = 'select * from batters_t where player_id = :player_id'; args[:player_id] = player['Player_Id']
      end

      return (@db.execute query, args)[0]
    end

    def get_player_stats( params )
      args = {}

      query = 'select player_type from players_t where player_id = :player_id'; args[:player_id] = params[:player_id]

      player = (@db.execute query, args)[0]

      if player['Player_Type'] == 1
        return get_pitcher_stats params
      end

      if player['Player_Type'] == 2
        return get_batter_stats params
      end

      return []
    end

    def get_pitcher_stats( params )
      args = {}

      query = 'select * from pitcher_stats_t where player_id = :player_id'; args[:player_id] = params[:player_id]

      if params.has_key? "season"; query = "#{query} and season        = :season"; args[:season] = params[:season] end
      if params.has_key? "phase";  query = "#{query} and season_phase  = :phase";  args[:phase ] = params[:phase ] end

      return @mapper.map_pitcher_stats_response @db.execute query, args
    end

    def get_batter_stats( params )
      args = {}

      query = 'select * from batter_stats_t where player_id = :player_id'; args[:player_id] = params[:player_id]

      if params.has_key? "season"; query = "#{query} and season        = :season"; args[:season] = params[:season] end
      if params.has_key? "phase";  query = "#{query} and season_phase  = :phase";  args[:phase ] = params[:phase ] end

      return @mapper.map_batter_stats_response @db.execute query, args
    end

end
