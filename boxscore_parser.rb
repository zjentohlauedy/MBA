module BoxscoreParser
  def self.parse( filename )
    boxscore      = {}
    teams         = {}
    current_team  = nil
    winner        = nil
    loser         = nil
    save          = nil
    gwrbi         = nil
    first_pitcher = true

    File::foreach( filename ) do |line|
      basic_line = line.strip

      next if basic_line.match %r(^$)
      next if basic_line.match %r([A-Z][a-z]+ vs [A-Z][a-z]+)
      next if basic_line.match %r(1  2  3  4  5  6  7  8  9     R  H  E)

      fields = basic_line.split %r{ +}

      next if fields[0] == 'AB'
      next if fields[0] == 'IP'
      next if fields[0] == 'TOTAL'

      if fields.length == 13 or fields.length == 14
        teams[fields[0]] = {}
        teams[fields[0]][:name]    = fields[0]
        teams[fields[0]][:innings] = fields[1..-4].map { |x| x.to_i }
        teams[fields[0]][:games]   = 1
        teams[fields[0]][:runs]    = fields[-3].to_i
        teams[fields[0]][:hits]    = fields[-2].to_i
        teams[fields[0]][:errors]  = fields[-1].to_i

        if teams[fields[0]][:innings].length == 9
          teams[fields[0]][:innings][9] = 0
        end
      end

      if fields[0] == 'W'
        winner = fields[1]
        teams[fields[3]][:double_plays] = fields[4].to_i
      end

      if fields[0] == 'L'
        loser = fields[1]
        teams[fields[2]][:double_plays] = fields[3].to_i
      end

      if fields[0] == 'SV'
        save = fields[1]
      end

      if fields[0] == 'GWRBI:'
        gwrbi = fields[1]
      end

      if fields[0] == 'Hitters:' or fields[0] == 'Hitters'
        current_team = teams[fields[1]]
      end

      if fields[0] == 'Pitchers:' or fields[0] == 'Pitchers'
        current_team = teams[fields[1]]
        first_pitcher = true
      end

      if fields.length == 15
        # batter
        player = {
          team_name:       current_team[:name],
          player_name:     fields[ 0],
          player_type:     2,
          games:           1,
          at_bats:         fields[ 1].to_i,
          runs:            fields[ 2].to_i,
          hits:            fields[ 3].to_i,
          doubles:         fields[ 4].to_i,
          triples:         fields[ 5].to_i,
          home_runs:       fields[ 6].to_i,
          runs_batted_in:  fields[ 7].to_i,
          sacrifice_hits:  fields[ 8].to_i,
          walks:           fields[ 9].to_i,
          strike_outs:     fields[10].to_i,
          steals:          fields[11].to_i,
          caught_stealing: fields[12].to_i,
          errors:          fields[13].to_i,
          passed_balls:    fields[14].to_i
        }

        player[:gwrbi] = (fields[0] == gwrbi) ? 1 : 0

        if current_team[:players].nil?
          current_team[:players] = []
        end

        current_team[:players].push player
      end

      if fields.length == 11
        # pitcher
        player = {
          team_name:       current_team[:name],
          player_name:     fields[ 0],
          player_type:     1,
          games:           1,
          hits:            fields[ 2].to_i,
          runs:            fields[ 3].to_i,
          earned_runs:     fields[ 4].to_i,
          home_runs:       fields[ 5].to_i,
          walks:           fields[ 6].to_i,
          strike_outs:     fields[ 7].to_i,
          wild_pitches:    fields[ 8].to_i,
          hit_batsmen:     fields[ 9].to_i,
          balks:           fields[10].to_i
        }

        (innings, outs) = fields[1].split '.'

        player[:innings_pitched] = { innings: innings.to_i, outs: outs.to_i }
        player[:wins]   = (fields[0] == winner) ? 1 : 0
        player[:losses] = (fields[0] == loser)  ? 1 : 0
        player[:saves]  = (fields[0] == save)   ? 1 : 0

        if first_pitcher
          player[:games_started] = 1
          first_pitcher = false
        else
          player[:games_started] = 0
        end

        if current_team[:players].nil?
          current_team[:players] = []
        end

        current_team[:players].push player
      end
    end

    return teams
  end
end
