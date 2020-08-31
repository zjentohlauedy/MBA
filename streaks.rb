class Streaks
  attr_reader :all_streaks

  def initialize( season, detector )
    @season      = season
    @detector    = detector
    @streaks     = {}
    @all_streaks = []
  end

  def update_streaks( player, game )
    streak_key = "#{player[:team_name]}.#{player[:player_name]}"

    if (streak = @streaks[streak_key]).nil?
      streak = {
        team_name: player[:team_name],
        player_name: player[:player_name],
        best: {
          start_game: nil,
          end_game: nil,
          games: 0
        },
        current: {
          start_game: nil,
          end_game: nil,
          games: 0
        },
        streaking: false
      }
    end

    if streak[:streaking]
      if @detector.is_streak? player
        streak[:current][:end_game]    = game
        streak[:current][:games]      += 1
      else
        player_streak = {
          team_name:   streak[ :team_name  ],
          player_name: streak[ :player_name],
          season:      @season,
          games:       streak[ :current    ][ :games      ],
          start_game:  streak[ :current    ][ :start_game ],
          end_game:    streak[ :current    ][ :end_game   ]
        }

        @all_streaks.push player_streak

        if streak[:current][:games] > streak[:best][:games]
          streak[:best] = streak[:current]
        end

        streak[:streaking] = false
        streak[:current]   = {
          start_game: nil,
          end_game: nil,
          games: 0
        }
      end

      @streaks[streak_key] = streak
    else
      if @detector.is_streak? player
        streak[:streaking]             = true
        streak[:current][:start_game]  = game
        streak[:current][:end_game]    = game
        streak[:current][:games]      += 1
      end

      @streaks[streak_key] = streak
    end
  end

  def compile_streaks()
    @streaks.each_value do |streak|
      next if streak[:streaking] == false

      player_streak = {
        team_name:   streak[ :team_name  ],
        player_name: streak[ :player_name],
        season:      @season,
        games:       streak[ :current    ][ :games      ],
        start_game:  streak[ :current    ][ :start_game ],
        end_game:    streak[ :current    ][ :end_game   ]
      }

      @all_streaks.push player_streak

      if streak[:current][:games] > streak[:best][:games]
        streak[:best] = streak[:current]
      end
    end
  end

  def add_new_season( season )
    @season  = season
    @streaks = {}
  end
end
