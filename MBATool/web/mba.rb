#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'sinatra'
require 'json'
require 'decorator'
require 'draft_generator'
require 'name_manager'
require 'player_generator'
require 'repository'
require 'response_mapper'


my_url         = 'http://localhost:4567'
mba_root       = '/mba'
actions_root   = "#{mba_root}/actions"
resources_root = "#{mba_root}/resources"

decorator        = Decorator.new "#{my_url + resources_root}"
response_mapper  = ResponseMapper.new decorator
repository       = Repository.new response_mapper
name_manager     = NameManager.new
player_generator = PlayerGenerator.new repository, name_manager
draft_generator  = DraftGenerator.new repository

get '/' do
  redirect '/index.html'
end

get "#{mba_root}/status/?" do
  content_type 'application/json'

  status = { :status => 'OK', :version => '0.0.1' }
  JSON.generate status
end

get "#{resources_root}/teams/?" do
  content_type 'application/json'

  JSON.generate repository.get_teams
end

get "#{resources_root}/teams/:team_id/?" do
  content_type 'application/json'

  JSON.generate repository.get_team params
end

get "#{resources_root}/teams/:team_id/stats/?" do
  content_type 'application/json'

  JSON.generate repository.get_team_stats params
end

get "#{resources_root}/teams/:team_id/players/?" do
  content_type 'application/json'

  JSON.generate repository.get_team_players params
end

get "#{resources_root}/teams/:team_id/players/:player_id/season/:season/?" do
  content_type 'application/json'

  JSON.generate repository.get_team_player params
end

post "#{resources_root}/teams/:team_id/players/:player_id/season/:season/?" do
  repository.save_team_player params
end

delete "#{resources_root}/teams/:team_id/players/:player_id/season/:season/?" do
  repository.delete_team_player params
end

get "#{resources_root}/players/?" do
  content_type 'application/json'

  if (params.has_key? 'rookie')  &&  (params[:rookie] == 'true')

    puts "Getting Rookies"

    if !params.has_key? 'season'
      result = repository.get_current_season

      params[:season] = result['Season']
    end

    return JSON.generate repository.get_rookies params
  end

  if (params.has_key? 'freeagent')  &&  (params[:freeagent] == 'true')
    if !params.has_key? 'season'
      result = repository.get_current_season

      params[:season] = result['Season']
    end

    return JSON.generate repository.get_free_agents params
  end

  JSON.generate repository.get_players
end

get "#{resources_root}/players/:player_id/?" do
  content_type 'application/json'

  JSON.generate repository.get_player params
end

get "#{resources_root}/players/:player_id/stats/?" do
  content_type 'application/json'

  JSON.generate repository.get_player_stats params
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

  puts "Start Season directive received with #{params.inspect}"

  current_season = repository.get_current_season['Season']

  name_manager.load_names
  repository.start_transaction

  begin
    puts "Creating Season #{current_season + 1}"
    repository.copy_team_players_for_new_season current_season, current_season + 1

    (1..32).each do
      rookie_pitcher = player_generator.generate_pitcher

      repository.save_pitcher rookie_pitcher[:details]
      repository.save_player  rookie_pitcher
    end

    (1..32).each do
      rookie_batter = player_generator.generate_batter

      repository.save_batter rookie_batter[:details]
      repository.save_player rookie_batter
    end

    repository.commit
  rescue Exception => e
    repository.rollback

    raise e
  end

  name_manager.save_names

  JSON.generate repository.get_current_season
end
