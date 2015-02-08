#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

class TeamPlayerDecorator

  def initialize( base_url )
    @base_url = base_url
  end

  def decorate_team_player( team_player )
    _links = {}

    _links[:self] = { rel:  'self',
                      href: "#{@base_url}/teams/#{team_player[:team_id]}/players/#{team_player[:player_id]}/season/#{team_player[:season]}",
                      desc: 'This resource' }

    _links[:player] = { rel:  'player',
                        href: "#{@base_url}/players/#{team_player[:player_id]}",
                        desc: 'Information about this player' }

    _links[:team] = { rel:  'team',
                      href: "#{@base_url}/teams/#{team_player[:team_id]}",
                      desc: 'Information about this team' }

    team_player[:_links] = _links

    return team_player
  end
end
