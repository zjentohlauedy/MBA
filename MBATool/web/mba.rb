#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"
require 'sinatra'
require 'json'

get '/' do
  redirect '/index.html'
end

get '/status' do
  content_type 'application/json'

  status = { :status => 'OK', :version => '0.0.1' }
  JSON.generate status
end
