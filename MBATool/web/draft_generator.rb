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

    if a[:division_id] == b[:division_id]
      if (a[:division_wins] <=> b[:division_wins]) != 0
        return a[:division_wins] <=> b[:division_wins]
      end
    end

    if ((a[:runs_scored] - a[:runs_allowed]) <=> (b[:runs_scored] - b[:runs_allowed])) != 0
      return (a[:runs_scored] - a[:runs_allowed]) <=> (b[:runs_scored] - b[:runs_allowed])
    end

    return a[:runs_scored] <=> b[:runs_scored]
  end

  def compare_teams_with_playoffs(a, b)
    if (a[:playoff_wins] <=> b[:playoff_wins]) != 0; return a[:playoff_wins] <=> b[:playoff_wins] end

    if (a[:wins] <=> b[:wins]) != 0; return a[:wins] <=> b[:wins] end

    if a[:division_id] == b[:division_id]
      if (a[:division_wins] <=> b[:division_wins]) != 0
        return a[:division_wins] <=> b[:division_wins]
      end
    end

    if ((a[:runs_scored] - a[:runs_allowed]) <=> (b[:runs_scored] - b[:runs_allowed])) != 0
      return (a[:runs_scored] - a[:runs_allowed]) <=> (b[:runs_scored] - b[:runs_allowed])
    end

    return a[:runs_scored] <=> b[:runs_scored]
  end

  def get_rookie_draft( season )
    regular = @repository.get_division_teams_with_stats season, Phases::RegularSeason
    playoff = @repository.get_division_teams_with_stats season, Phases::Playoff

    teams = regular.map do |team|
      playoff.each do |t2|
        if t2[:team_id] == team[:team_id]
          team[ :playoff_wins   ] = t2[ :wins   ]
          team[ :playoff_losses ] = t2[ :losses ]
          break;
        end
      end
      team
    end

    teams.sort! { |a, b| compare_teams_with_playoffs(a, b) }

    draft = teams.map { |team| team[:team_id] }

    return draft + draft
  end

  def get_free_agent_draft( season )
    teams = @repository.get_division_teams_with_stats season, Phases::RegularSeason

    teams.sort! { |a, b| compare_teams(a, b) }

    draft = teams.map { |team| team[:team_id] }

    return draft + draft.reverse +
           draft + draft.reverse +
           draft + draft.reverse +
           draft + draft.reverse +
           draft + draft.reverse +
           draft + draft.reverse +
           draft + draft.reverse +
           draft
  end

end
