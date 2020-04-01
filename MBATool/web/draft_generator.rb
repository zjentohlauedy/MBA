#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

require 'phases'

class DraftGenerator

  def initialize( repository )
    @repository = repository
  end

  def compare_teams(a, b)
    if (a[:wins] <=> b[:wins]) != 0; return a[:wins] <=> b[:wins] end

    if ((a[:runs_scored] - a[:runs_allowed]) <=> (b[:runs_scored] - b[:runs_allowed])) != 0
      return (a[:runs_scored] - a[:runs_allowed]) <=> (b[:runs_scored] - b[:runs_allowed])
    end

    return a[:runs_scored] <=> b[:runs_scored]
  end

  def adjust_for_head2head( teams )
    workingset = teams.map.with_index { |t,i| {index: i, team_id: t[:team_id], season: t[:season], season_phase: t[:season_phase], wins: t[:wins]} }

    swaps = []

    workingset.each do |team|
      subset = workingset.select { |t| t[:wins] == team[:wins] }

      next if subset.length != 2

      versus = @repository.get_team_versus_stats subset[0][:team_id], subset[0][:season], subset[0][:season_phase], subset[1][:team_id]

      if versus[:wins] > versus[:losses]
        swap = [subset[0][:index], subset[1][:index]]

        unless swaps.include? swap
          swaps.push swap
        end
      end
    end

    swaps.each do |swap|
      teams[swap[0]], teams[swap[1]] = teams[swap[1]], teams[swap[0]]
    end
  end

  def adjust_for_playoffs( teams )
    playoff_teams = teams.select { |t| t[:playoffs] && t[:playoffs][:division_losses] == 0 }
    playoff_team_ids = playoff_teams.map { |t| t[:team_id] }

    teams.reject! { |t| playoff_team_ids.include? t[:team_id] }

    lcs_losers = playoff_teams.select { |t| t[:playoffs][:league_losses] == 4 }

    ws_losers = playoff_teams.select { |t| (t[:playoffs][:losses] - t[:playoffs][:league_losses]) == 5 }
    ws_champs = playoff_teams.select { |t| (t[:playoffs][:wins]   - t[:playoffs][:league_wins])   == 5 }

    teams.concat lcs_losers
    teams.concat ws_losers
    teams.concat ws_champs
  end

  def get_rookie_draft( season )
    regular = @repository.get_division_teams_with_stats season, Phases::RegularSeason
    playoff = @repository.get_division_teams_with_stats season, Phases::Playoff

    teams = regular.map do |team|
      playoff.each do |t2|
        if t2[:team_id] == team[:team_id]
          team[ :playoffs ] = t2
          team[ :playoffs ] = t2
          break;
        end
      end

      team
    end

    teams.sort! { |a, b| compare_teams(a, b) }

    adjust_for_head2head teams
    adjust_for_playoffs  teams

    draft = teams.map { |team| team[:team_id] }

    return draft + draft
  end

  def get_free_agent_draft( season )
    teams = @repository.get_division_teams_with_stats season, Phases::RegularSeason

    teams.sort! { |a, b| compare_teams(a, b) }

    adjust_for_head2head teams

    draft = teams.map { |team| team[:team_id] }

    return draft + draft.reverse +
           draft + draft.reverse +
           draft + draft.reverse +
           draft + draft.reverse +
           draft + draft.reverse +
           draft + draft.reverse +
           draft + draft.reverse
  end

end
