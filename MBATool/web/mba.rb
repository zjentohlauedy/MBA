#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'sinatra'
require 'json'
require 'decorator'
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
player_generator = PlayerGenerator.new repository

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

get "#{resources_root}/players/?" do
  content_type 'application/json'

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

post "#{actions_root}/start_season" do
  
end
