#!/usr/bin/env ruby
#
# Parses output of print_rosters and displays
# league leaders in several categories
location = File.dirname __FILE__

$: << "#{location}"

class DraftGenerator

  def initialize( repository )
    @repository = repository
  end

  def compare_teams(a, b)
    if (a['Wins'] <=> b['Wins']) != 0; return a['Wins'] <=> b['Wins'] end

    if a['Division_Id'] == b['Division_Id']
      if (a['Division_Wins'] <=> b['Division_Wins']) != 0
        return a['Division_Wins'] <=> b['Division_Wins']
      end
    end

    if ((a['Runs_Scored'] - a['Runs_Allowed']) <=> (b['Runs_Scored'] - b['Runs_Allowed'])) != 0
      return (a['Runs_Scored'] - a['Runs_Allowed']) <=> (b['Runs_Scored'] - b['Runs_Allowed'])
    end

    return a['Runs_Scored'] <=> b['Runs_Scored']
  end

  def compare_teams_with_playoffs(a, b)
    if (a['Playoff_Wins'] <=> b['Playoff_Wins']) != 0; return a['Playoff_Wins'] <=> b['Playoff_Wins'] end

    if (a['Wins'] <=> b['Wins']) != 0; return a['Wins'] <=> b['Wins'] end

    if a['Division_Id'] == b['Division_Id']
      if (a['Division_Wins'] <=> b['Division_Wins']) != 0
        return a['Division_Wins'] <=> b['Division_Wins']
      end
    end

    if ((a['Runs_Scored'] - a['Runs_Allowed']) <=> (b['Runs_Scored'] - b['Runs_Allowed'])) != 0
      return (a['Runs_Scored'] - a['Runs_Allowed']) <=> (b['Runs_Scored'] - b['Runs_Allowed'])
    end

    return a['Runs_Scored'] <=> b['Runs_Scored']
  end

  def get_rookie_draft( season )
    args =  { :season => season, :phase => 1 }

    regular = @repository.get_teams_with_stats args

    args[:phase] = 2

    playoff = @repository.get_teams_with_stats args

    teams = regular.map do |team|
      playoff.each do |t2|
        if t2['Team_Id'] == team['Team_Id']
          team[ 'Playoff_Wins'   ] = t2[ 'Wins'   ]
          team[ 'Playoff_Losses' ] = t2[ 'Losses' ]
          break;
        end
      end
      team
    end

    teams.sort! { |a, b| compare_teams_with_playoffs(a, b) }

    draft = teams.map { |team| team['Team_Id'] }

    return draft + draft
  end

  def get_free_agent_draft( season )
    args = { :season => season, :phase => 1 }

    teams = @repository.get_teams_with_stats args

    teams.sort! { |a, b| compare_teams(a, b) }

    draft = teams.map { |team| team['Team_Id'] }

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
