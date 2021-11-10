#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'optparse'
require 'sqlite3'
require 'stat_rankings'

require_relative 'MBATool/web/utils'


class LeadersPrinter
  def print_empty_indicator()
    puts "--"
  end

  def print( team, format, index, tied )
    value = team.get_sort_value
    value = team.avgs.include?(team.get_sort_key) ? display_avg(value) : value

    if tied; then printf "  -  ";
    else          printf "%3d. ", index + 1;
    end

    printf "%-20s %-15s S%02d  #{format}\n", team.name, team.team, team.season, value
  end

  def print_tie_message( summary, format, index )
    printf "%3d. %-40s  #{format}\n", index + 1, "#{summary.count} Teams Tied At", summary.value
  end

  def display_avg(average)
    avg = sprintf "%5.3f", average
    avg.gsub /^0\./, ' .'
  end
end

class LeadersFilter
  def apply( teams, stat )
    if stat == :errors
      return teams.select {|team| team.season.nil? || team.season >= 3}
    end

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


@options = {}

OptionParser.new do |opt|
  opt.on( '-m', '--[no-]modern-era' ) { |o| @options[ :modern_era ] = o }
end.parse!


@db = SQLite3::Database.new "#{location}/mba.db"

@db.results_as_hash  = true
@db.type_translation = true


def get_organization( organization_id )
  result = Utils::transform_hash @db.execute "select * from organizations_t where organization_id = #{organization_id}"
  result[0]
end

def get_leagues_by_org( organization )
  Utils::transform_hash @db.execute "select l.* from organization_leagues_t ol join leagues_t l on ol.league_id = l.league_id where ol.organization_id = #{organization[:organization_id]}"
end

def get_divisions_by_league( league )
  Utils::transform_hash @db.execute "select d.* from league_divisions_t ld join divisions_t d on ld.division_id = d.division_id where ld.league_id = #{league[:league_id]}"
end

def get_teams_by_division( division )
  Utils::transform_hash @db.execute "select t.*, ts.season from division_teams_t dt join teams_t t on dt.team_id = t.team_id join team_stats_t ts on t.team_id = ts.team_id where dt.division_id = #{division[:division_id]} and ts.season_phase = 1"
end

def get_team_stats_by_team_id_and_season(team_id, season)
  result = Utils::transform_hash @db.execute "select * from team_stats_t where team_id = #{team_id} and season = #{season} and season_phase = 1"
  result[0]
end

def get_team_pitching_stats_by_team_id_and_season(team_id, season)
  result = Utils::transform_hash @db.execute "select * from team_pitching_stats_t where team_id = #{team_id} and season = #{season} and season_phase = 1"
  result[0]
end

def get_team_batting_stats_by_team_id_and_season(team_id, season)
  result = Utils::transform_hash @db.execute "select * from team_batting_stats_t where team_id = #{team_id} and season = #{season} and season_phase = 1"
  result[0]
end


org = get_organization 1

org[:leagues] = get_leagues_by_org org

org[:leagues].each do |league|
  league[:divisions] = get_divisions_by_league league

  league[:divisions].each do |division|
    division[:teams] = get_teams_by_division division

    division[:teams].each do |team|
      team[ :stats          ] =              get_team_stats_by_team_id_and_season(          team[:team_id], team[:season] )
      team[ :pitching_stats ] = { simulated: get_team_pitching_stats_by_team_id_and_season( team[:team_id], team[:season] ) }
      team[ :batting_stats  ] = { simulated: get_team_batting_stats_by_team_id_and_season(  team[:team_id], team[:season] ) }
    end
  end
end

printer  = LeadersPrinter.new
filter   = LeadersFilter.new
compiler = LeadersCompiler.new org

sr = StatRankings.new printer, filter, compiler, @options

sr.process_categories @team_categories
