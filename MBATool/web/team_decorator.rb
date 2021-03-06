#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

class TeamDecorator

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

  def decorate_team( team, season = nil, phase = nil )
    _links = {}

    _links[:self] = { rel: 'self',
                      href: "#{@base_url}/teams/#{team[:team_id]}",
                      desc: "This resource" }

    _links[:players] = { rel: 'players',
                         href: "#{@base_url}/teams/#{team[:team_id]}/players",
                         desc: "Players associated with this team" }

    _links[:stats] = { rel: 'stats',
                       href: "#{@base_url}/teams/#{team[:team_id]}/stats",
                       desc: "Statistics for this team" }

    _links[:accolades] = { rel: 'accolades',
                           href: "#{@base_url}/teams/#{team[:team_id]}/accolades",
                           desc: "Accolades for this team" }

    unless season.nil? and phase.nil?
      params = season.nil? ? "?phase=#{phase}" : "?season=#{season}" + (phase.nil? ? "" : "&phase=#{phase}")

      _links[:self][:href] += params

      _links[:players][:href] += params
      _links[:players][:desc] += (season.nil? ? "" : " for season #{season}")

      _links[:stats][:href] += params
      _links[:stats][:desc]  = (season.nil? ? "" : "Season #{season} ") + (phase.nil? ? "" : "#{display_phase phase} ") + "statistics for this team"

      _links[:accolades][:href] += season.nil? ? "" : "?season=#{season}"
      _links[:accolades][:desc]  = (season.nil? ? "" : "Season #{season} ") + "accolades for this team"

      _links[:team] = { rel: 'team',
                        href: "#{@base_url}/teams/#{team[:team_id]}",
                        desc: "Information about this team" }
    end

    team[:_links] = _links

    return team
  end

  def decorate_team_stats( team_stats, phase )
    _links = {}

    _links[:self] = { rel: 'self',
                      href: "#{@base_url}/teams/#{team_stats[:team_id]}/stats?season=#{team_stats[:season]}&phase=#{phase}",
                      desc: "This resource" }

    _links[:team] = { rel: 'team',
                      href: "#{@base_url}/teams/#{team_stats[:team_id]}",
                      desc: "Information about this team" }

    _links[:stats] = { rel: 'stats',
                      href: "#{@base_url}/teams/#{team_stats[:team_id]}/stats",
                      desc: "All statistics for this team" }

    _links[:accolades] = { rel: 'accolades',
                           href: "#{@base_url}/teams/#{team_stats[:team_id]}/accolades",
                           desc: "All accolades for this team" }

    team_stats[:_links] = _links

    return team_stats
  end

  def decorate_team_accolade( team_accolade )
    _links = {}

    _links[:team] = { rel:  'team',
                      href: "#{@base_url}/teams/#{team_accolade[:team_id]}",
                      desc: 'Information about this team' }

    _links[:stats] = { rel:  'stats',
                       href: "#{@base_url}/teams/#{team_accolade[:team_id]}/stats",
                       desc: 'All statistics for this team' }

    _links[:accolades] = { rel:  'accolades',
                           href: "#{@base_url}/teams/#{team_accolade[:team_id]}/accolades",
                           desc: 'All accolades for this team' }

    team_accolade[:_links] = _links

    return team_accolade
  end
end
