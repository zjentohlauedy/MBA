#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

require 'sinatra'
require 'sqlite3'
require 'json'

require 'team_decorator'
require 'team_repository'
require 'team_response_mapper'
require 'team_service'

require 'team_player_decorator'
require 'team_player_repository'
require 'team_player_service'

require 'player_decorator'
require 'player_repository'
require 'player_response_mapper'
require 'player_service'

require 'draft_generator'
require 'name_manager'
require 'player_generator'
require 'season_service'


if ARGV.length != 2
  abort "Usage: #{__FILE__} <db file> <names file>\n"
end

db_file    = ARGV[0]
names_file = ARGV[1]

set :show_exceptions, false

my_url         = 'http://localhost:4567'
mba_root       = '/mba'
actions_root   = "#{mba_root}/actions"
resources_root = "#{mba_root}/resources"
href_base_url  = "#{my_url + resources_root}"


db = SQLite3::Database.new db_file

db.results_as_hash  = true
db.type_translation = true


team_repository     = TeamRepository.new db
player_repository   = PlayerRepository.new db
team_service        = TeamService.new( team_repository, TeamResponseMapper.new, TeamDecorator.new( href_base_url ) )
team_player_service = TeamPlayerService.new( TeamPlayerRepository.new( db ), TeamPlayerDecorator.new( href_base_url ) )
player_service      = PlayerService.new( player_repository, PlayerResponseMapper.new, PlayerDecorator.new( href_base_url ) )
name_manager        = NameManager.new names_file
player_generator    = PlayerGenerator.new db, name_manager
season_service      = SeasonService.new db, player_repository, name_manager, player_generator
draft_generator     = DraftGenerator.new team_repository


get '/' do
  redirect '/index.html'
end

get "#{mba_root}/status/?" do
  content_type 'application/json'

  status = { :status => 'OK', :version => '0.0.2' }
  JSON.generate status
end

get "#{resources_root}/teams/?" do
  content_type 'application/json'

  JSON.generate team_service.get_teams params[:season], params[:phase]
end

get "#{resources_root}/teams/:team_id/?" do
  content_type 'application/json'

  JSON.generate team_service.get_team params[:team_id], params[:season], params[:phase]
end

get "#{resources_root}/teams/:team_id/stats/?" do
  content_type 'application/json'

  JSON.generate team_service.get_team_stats params[:team_id], params[:season], params[:phase]
end

get "#{resources_root}/teams/:team_id/players/?" do
  content_type 'application/json'

  JSON.generate player_service.get_team_players params[:team_id], params[:season], params[:phase]
end

get "#{resources_root}/teams/:team_id/players/:player_id/season/:season/?" do
  content_type 'application/json'

  JSON.generate team_player_service.get_team_player params[:team_id], params[:season], params[:player_id]
end

post "#{resources_root}/teams/:team_id/players/:player_id/season/:season/?" do
  content_type 'application/json'

  JSON.generate team_player_service.save_team_player params[:team_id], params[:season], params[:player_id]
end

delete "#{resources_root}/teams/:team_id/players/:player_id/season/:season/?" do
  content_type 'application/json'

  JSON.generate team_player_service.delete_team_player params[:team_id], params[:season], params[:player_id]
end

get "#{resources_root}/players/?" do
  content_type 'application/json'

  if params[:rookie] == 'true'
    return JSON.generate player_service.get_rookies params[:season], params[:phase]
  end

  if params[:freeagent] == 'true'
    return JSON.generate player_service.get_free_agents params[:season], params[:phase]
  end

  JSON.generate player_service.get_players params[:season], params[:phase]
end

get "#{resources_root}/players/:player_id/?" do
  content_type 'application/json'

  JSON.generate player_service.get_player params[:player_id], params[:season], params[:phase]
end

get "#{resources_root}/players/:player_id/stats/?" do
  content_type 'application/json'

  JSON.generate player_service.get_player_stats params[:player_id], params[:season], params[:phase]
end

get "#{resources_root}/drafts/rookie/season/:season/?" do |season|
  content_type 'application/json'

  JSON.generate draft_generator.get_rookie_draft season
end

get "#{resources_root}/drafts/free-agent/season/:season/?" do |season|
  content_type 'application/json'

  JSON.generate draft_generator.get_free_agent_draft season
end

post "#{actions_root}/start_season" do
  content_type 'application/json'

  db.transaction

  begin
    response = season_service.start_new_season

    db.commit
  rescue Exception => e
    db.rollback

    raise e
  end

  JSON.generate response
end


error InternalServerError do
  status       500
  content_type 'application/json'

  response = {error: env['sinatra.error'].message}

  JSON.generate response
end

error ResourceNotFoundError do
  status       404
  content_type 'application/json'

  response = {error: env['sinatra.error'].message}

  JSON.generate response
end

error SQLite3::ConstraintException do
  status 400
  content_type 'application/json'

  response = {error: 'Resource already exists.'}

  JSON.generate response
end
