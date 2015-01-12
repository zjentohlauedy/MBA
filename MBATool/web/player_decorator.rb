#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

class PlayerDecorator

  def initialize( base_url )
    @base_url = base_url
  end

  def display_phase( phase )
    case phase
    when 1; return 'Regular Season'
    when 2; return 'Playoffs'
    when 3; return 'Allstar Game'
    end

    return 'Unknown'
  end

  def decorate_player( player, season = nil, phase = nil )
    links = []

    links << { rel: 'self',  href: "#{@base_url}/players/#{player[:player_id]}"       }
    links << { rel: 'stats', href: "#{@base_url}/players/#{player[:player_id]}/stats" }

    player[:links] = links

#

    _links = {}

    _links[:self]  = { rel: 'self',   href: "#{@base_url}/players/#{player[:player_id]}",       desc: 'This resource'              }
    _links[:stats] = { rel: 'stats',  href: "#{@base_url}/players/#{player[:player_id]}/stats", desc: 'Statistics for this player' }

    unless season.nil? and phase.nil?
      params = season.nil? ? "?phase=#{phase}" : "?season=#{season}" + (phase.nil? ? "" : "&phase=#{phase}")

      _links[:self][:href] += params

      _links[:stats][:href] += params
      _links[:stats][:desc]  = (season.nil? ? "" : "Season #{season} ") + (phase.nil? ? "" : "#{display_phase phase} ") + "statistics for this player"

      _links[:player] = { rel: 'player',
                          href: "#{@base_url}/players/#{player[:player_id]}",
                          desc: "Information about this player" }
    end

    player[:_links] = _links

    return player
  end

  def decorate_player_stats( player_stats, phase )
    links = []

    links << { rel: 'self',  href: "#{@base_url}/players/#{player_stats[:player_id]}/stats?season=#{player_stats[:season]}&phase=#{phase}" }

    player_stats[:links] = links

#

    _links = {}

    _links[:self] = { rel:  'self',
                      href: "#{@base_url}/players/#{player_stats[:player_id]}/stats?season=#{player_stats[:season]}&phase=#{phase}",
                      desc: 'This resource' }

    _links[:player] = { rel:  'player',
                        href: "#{@base_url}/players/#{player_stats[:player_id]}",
                        desc: 'Information about this player' }

    _links[:stats] = { rel:  'stats',
                       href: "#{@base_url}/players/#{player_stats[:player_id]}/stats",
                       desc: 'All statistics for this player' }

    player_stats[:_links] = _links

    return player_stats
  end
end