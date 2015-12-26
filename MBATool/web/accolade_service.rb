#!/usr/bin/env ruby
#
location = File.dirname __FILE__
$: << "#{location}"

require 'accolades'
require 'phases'
require 'internal_server_error'

AccoladeLookups = [
  { accolade: Accolades::League::World_Title,                    method: :get_league_stats_by_highest,   stat: 'wins',              phase: Phases::Playoff       },
  { accolade: Accolades::League::Best_Record,                    method: :get_league_stats_by_highest,   stat: 'wins',              phase: Phases::RegularSeason },
  { accolade: Accolades::League::All_Star_Champion,              method: :get_league_stats_by_highest,   stat: 'wins',              phase: Phases::Allstar       },

  { accolade: Accolades::Division::World_Title,                  method: :get_division_stats_by_highest, stat: 'world_series_wins', phase: Phases::Playoff       },
  { accolade: Accolades::Division::League_Title,                 method: :get_division_stats_by_highest, stat: 'league_wins',       phase: Phases::Playoff       },
  { accolade: Accolades::Division::Best_Record,                  method: :get_division_stats_by_highest, stat: 'wins',              phase: Phases::RegularSeason },

  { accolade: Accolades::Team::World_Title,                      method: :get_team_stats_by_highest,     stat: 'world_series_wins', phase: Phases::Playoff       },
  { accolade: Accolades::Team::League_Title,                     method: :get_team_stats_by_highest,     stat: 'lcs_wins',          phase: Phases::Playoff       },
  { accolade: Accolades::Team::Division_Title,                   method: :get_team_stats_by_lowest,      stat: 'division_losses',   phase: Phases::Playoff       },
  { accolade: Accolades::Team::Best_Record,                      method: :get_team_stats_by_highest,     stat: 'wins',              phase: Phases::RegularSeason },
  { accolade: Accolades::Team::All_Star_Champion,                method: :get_team_stats_by_highest,     stat: 'wins',              phase: Phases::Allstar       },

  { accolade: Accolades::Player::All_Star,                        },
  { accolade: Accolades::Player::World_Title,                     },
  { accolade: Accolades::Player::League_Title,                    },
  { accolade: Accolades::Player::Division_Title,                  },
  { accolade: Accolades::Player::Best_Record,                     },
  { accolade: Accolades::Player::World_Series_MVP,                },
  { accolade: Accolades::Player::All_Star_Champion,               },
  { accolade: Accolades::Player::All_Star_MVP,                    },

  { accolade: Accolades::Batting::Batter_of_the_Year,             },
  { accolade: Accolades::Batting::Global_Batter_of_the_Year,      },
  { accolade: Accolades::Batting::World_Batter_of_the_Year,       },
  { accolade: Accolades::Batting::Atlantic_Batter_of_the_Year,    },
  { accolade: Accolades::Batting::North_Batter_of_the_Year,       },
  { accolade: Accolades::Batting::Pacific_Batter_of_the_Year,     },
  { accolade: Accolades::Batting::South_Batter_of_the_Year,       },
  { accolade: Accolades::Batting::Best_Batting_Average,          method: :get_batter_stats_by_highest,   stat: 'batting_average',   phase: Phases::RegularSeason },
  { accolade: Accolades::Batting::Most_Doubles,                  method: :get_batter_stats_by_highest,   stat: 'doubles',           phase: Phases::RegularSeason },
  { accolade: Accolades::Batting::Most_Triples,                  method: :get_batter_stats_by_highest,   stat: 'triples',           phase: Phases::RegularSeason },
  { accolade: Accolades::Batting::Most_Home_Runs,                method: :get_batter_stats_by_highest,   stat: 'home_runs',         phase: Phases::RegularSeason },
  { accolade: Accolades::Batting::Most_Runs,                     method: :get_batter_stats_by_highest,   stat: 'runs',              phase: Phases::RegularSeason },
  { accolade: Accolades::Batting::Most_Runs_Batted_In,           method: :get_batter_stats_by_highest,   stat: 'runs_batted_in',    phase: Phases::RegularSeason },
  { accolade: Accolades::Batting::Most_Stolen_Bases,             method: :get_batter_stats_by_highest,   stat: 'steals',            phase: Phases::RegularSeason },
  { accolade: Accolades::Batting::Most_Walks,                    method: :get_batter_stats_by_highest,   stat: 'walks',             phase: Phases::RegularSeason },
  { accolade: Accolades::Batting::Best_Slugging_Average,         method: :get_batter_stats_by_highest,   stat: 'slugging_average',  phase: Phases::RegularSeason },
  { accolade: Accolades::Batting::Best_On_Base_Percentage,       method: :get_batter_stats_by_highest,   stat: 'on_base_pct',       phase: Phases::RegularSeason },
  { accolade: Accolades::Batting::Best_Strike_Out_Average,       method: :get_batter_stats_by_lowest,    stat: 'strike_out_avg',    phase: Phases::RegularSeason },
  { accolade: Accolades::Batting::Best_Runs_per_Game,            method: :get_batter_stats_by_highest,   stat: 'runs_per_game',     phase: Phases::RegularSeason },

  { accolade: Accolades::Pitching::Pitcher_of_the_Year,           },
  { accolade: Accolades::Pitching::Global_Pitcher_of_the_Year,    },
  { accolade: Accolades::Pitching::World_Pitcher_of_the_Year,     },
  { accolade: Accolades::Pitching::Atlantic_Pitcher_of_the_Year,  },
  { accolade: Accolades::Pitching::North_Pitcher_of_the_Year,     },
  { accolade: Accolades::Pitching::Pacific_Pitcher_of_the_Year,   },
  { accolade: Accolades::Pitching::South_Pitcher_of_the_Year,     },
  { accolade: Accolades::Pitching::Most_Wins,                    method: :get_pitcher_stats_by_highest,  stat: 'wins',              phase: Phases::RegularSeason },
  { accolade: Accolades::Pitching::Best_Win_Loss_Ratio,          method: :get_pitcher_stats_by_highest,  stat: 'win_loss_ratio',    phase: Phases::RegularSeason },
  { accolade: Accolades::Pitching::Best_Win_Percentage,          method: :get_pitcher_stats_by_highest,  stat: 'win_pct',           phase: Phases::RegularSeason },
  { accolade: Accolades::Pitching::Best_Earned_Run_Average,      method: :get_pitcher_stats_by_lowest,   stat: 'earned_run_avg',    phase: Phases::RegularSeason },
  { accolade: Accolades::Pitching::Best_vs_Batting_Average,      method: :get_pitcher_stats_by_lowest,   stat: 'vs_batting_avg',    phase: Phases::RegularSeason },
  { accolade: Accolades::Pitching::Most_Saves,                   method: :get_pitcher_stats_by_highest,  stat: 'saves',             phase: Phases::RegularSeason },
  { accolade: Accolades::Pitching::Most_Strike_Outs,             method: :get_pitcher_stats_by_highest,  stat: 'strike_outs',       phase: Phases::RegularSeason },
  { accolade: Accolades::Pitching::Most_Innings_per_Game,        method: :get_pitcher_stats_by_highest,  stat: 'innings_per_game',  phase: Phases::RegularSeason },
  { accolade: Accolades::Pitching::Best_Walks_Hits_Per_Inning,   method: :get_pitcher_stats_by_lowest,   stat: 'whip',              phase: Phases::RegularSeason },
  { accolade: Accolades::Pitching::Most_Strike_Outs_per_Nine,    method: :get_pitcher_stats_by_highest,  stat: 'so_per_nine',       phase: Phases::RegularSeason },
  { accolade: Accolades::Pitching::Best_Home_Runs_per_Nine,      method: :get_pitcher_stats_by_lowest,   stat: 'hr_per_nine',       phase: Phases::RegularSeason },
  { accolade: Accolades::Pitching::Best_Efficiency,              method: :get_pitcher_stats_by_highest,  stat: 'efficiency',        phase: Phases::RegularSeason } ]


class AccoladeService

  def initialize( org_repository, league_repository, division_repository, team_repository, player_repository )
    @accolades = Accolades::AccoladeList

    @repos = {
      'organization'            => { repo: org_repository,        method: nil                     },
      Accolades::League::Type   => { repo: league_repository,     method: :save_league_accolade   },
      Accolades::Division::Type => { repo: division_repository,   method: :save_division_accolade },
      Accolades::Team::Type     => { repo: team_repository,       method: :save_team_accolade     },
      Accolades::Player::Type   => { repo: player_repository,     method: nil                     },
      Accolades::Pitching::Type => { repo: player_repository,     method: :save_pitcher_accolade, filter: { field: 'innings', value: 185 } },
      Accolades::Batting::Type  => { repo: player_repository,     method: :save_batter_accolade,  filter: { field: 'at_bats', value: 300 } }
    }
  end

  def get_accolades
    return @accolades
  end

  def resolve_accolades
    org = @repos['organization'][:repo].get_org( 1 )

    @accolades.each_with_index do |accolade, idx|
      unless accolade[:value] == AccoladeLookups[idx][:accolade]
        raise InternalServerError.new "Accolade mismatch at #{idx}."
      end

      next if AccoladeLookups[idx][:method].nil?

      lookup               = AccoladeLookups[idx]
      repo                 = @repos[ accolade[:type] ][ :repo   ]
      filter               = @repos[ accolade[:type] ][ :filter ]
      save_accolade_method = @repos[ accolade[:type] ][ :method ]

      unless filter.nil?
        value = (accolade[:value] == Accolades::Pitching::Most_Saves) ? 0 : filter[:value]

        recipients = repo.send lookup[:method], lookup[:stat], filter[:field], value, org[:season], lookup[:phase]
      else
        recipients = repo.send lookup[:method], lookup[:stat], org[:season], lookup[:phase]
      end

      recipients.each do |recipient|
        recipient[:accolade] = accolade[:value]

        repo.send save_accolade_method, recipient
      end
    end

    resolve_player_accolades org[:season]
  end

  def resolve_player_accolades( season )
    team_accolades = @repos[Accolades::Team::Type][:repo].get_team_accolades_by_season season

    team_accolades.each do |team_accolade|
      players = @repos[Accolades::Player::Type][:repo].get_players_by_team team_accolade[:team_id], season

      players.each do |player|
        player_accolade = { player_id: player[:player_id], season: season }

        case team_accolade[:accolade]
        when Accolades::Team::World_Title;       player_accolade[:accolade] = Accolades::Player::World_Title
        when Accolades::Team::League_Title;      player_accolade[:accolade] = Accolades::Player::League_Title
        when Accolades::Team::Division_Title;    player_accolade[:accolade] = Accolades::Player::Division_Title
        when Accolades::Team::Best_Record;       player_accolade[:accolade] = Accolades::Player::Best_Record
        when Accolades::Team::All_Star_Champion; player_accolade[:accolade] = Accolades::Player::All_Star_Champion
        else
          raise InternalServerError.new "Unknown team accolade: #{team_accolade[:value]}."
        end

        @repos[Accolades::Player::Type][:repo].save_player_accolade player_accolade
      end
    end

    all_star_teams = @repos[Accolades::Team::Type][:repo].get_league_teams

    all_star_teams.each do |team|
      players = @repos[Accolades::Player::Type][:repo].get_players_by_team team[:team_id], season

      players.each do |player|
        player_accolade = { player_id: player[:player_id], season: season, accolade: Accolades::Player::All_Star }

        @repos[Accolades::Player::Type][:repo].save_player_accolade player_accolade
      end
    end
  end
end
