location = File.dirname __FILE__
$: << "#{location}"

require 'phases'
require 'accolades'
require 'accolade_service'
require 'org_repository'
require 'league_repository'
require 'division_repository'
require 'team_repository'
require 'player_repository'

describe AccoladeService do
  before :each do
    @or = double OrgRepository
    @lr = double LeagueRepository
    @dr = double DivisionRepository
    @tr = double TeamRepository
    @pr = double PlayerRepository

    allow( @or ).to receive( :get_org ).with( 1 ).and_return( {organization_id: 1, season: 4} )

    allow( @lr ).to receive( :get_league_stats_by_highest           ).and_return( [] )
    allow( @dr ).to receive( :get_division_stats_by_highest         ).and_return( [] )
    allow( @tr ).to receive( :get_team_stats_by_highest             ).and_return( [] )
    allow( @tr ).to receive( :get_team_accolades_by_season          ).and_return( [] )
    allow( @tr ).to receive( :get_team_stats_by_lowest              ).and_return( [] )
    allow( @tr ).to receive( :get_league_teams                      ).and_return( [] )
    allow( @pr ).to receive( :get_pitcher_stats_by_highest          ).and_return( [] )
    allow( @pr ).to receive( :get_pitcher_stats_by_lowest           ).and_return( [] )
    allow( @pr ).to receive( :get_starting_pitcher_stats_by_highest ).and_return( [] )
    allow( @pr ).to receive( :get_starting_pitcher_stats_by_lowest  ).and_return( [] )
    allow( @pr ).to receive( :get_closing_pitcher_stats_by_highest  ).and_return( [] )
    allow( @pr ).to receive( :get_closing_pitcher_stats_by_lowest   ).and_return( [] )
    allow( @pr ).to receive( :get_batter_stats_by_highest           ).and_return( [] )
    allow( @pr ).to receive( :get_batter_stats_by_lowest            ).and_return( [] )
    allow( @pr ).to receive( :get_players_by_team                   ).and_return( [] )

    @accolade_service = AccoladeService.new @or, @lr, @dr, @tr, @pr
  end

  describe '#get_accolades' do
    it 'should return the accolade list' do
      results = @accolade_service.get_accolades

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     Accolades::AccoladeList.length

      results.each do |entry|
        expect( entry[ :name      ] ).to_not be_nil
        expect( entry[ :name      ] ).to     be_a   String
        expect( entry[ :value     ] ).to_not be_nil
        expect( entry[ :value     ] ).to     be_a   Fixnum
        expect( entry[ :type      ] ).to_not be_nil
        expect( entry[ :type      ] ).to     be_a   String
        expect( entry[ :automatic ] ).to_not be_nil
        expect( [true, false] ).to include entry[ :automatic ]
      end
    end
  end

  describe '#resolve_accolades' do
    it 'should call the organization repository to get the organization record' do
      expect( @or ).to receive( :get_org ).with( 1 ).and_return( {organization_id: 1, season: 4} )

      @accolade_service.resolve_accolades
    end

    it 'should call the league repository to get the highest values for all league accolades for the current season' do
      expect( @lr ).to receive( :get_league_stats_by_highest ).with( anything, 4, anything ).and_return( [] ).at_least :once

      @accolade_service.resolve_accolades
    end

    it 'should call the league repository to create accolades for the leagues returned by the get league stats method' do
      allow( @lr ).to receive( :get_league_stats_by_highest ).and_return( [{league_id: 1, season: 4}] )

      expect( @lr ).to receive( :save_league_accolade ).with( hash_including :league_id => 1, :season => 4, :accolade => Accolades::League::World_Title       )
      expect( @lr ).to receive( :save_league_accolade ).with( hash_including :league_id => 1, :season => 4, :accolade => Accolades::League::Best_Record       )
      expect( @lr ).to receive( :save_league_accolade ).with( hash_including :league_id => 1, :season => 4, :accolade => Accolades::League::All_Star_Champion )

      @accolade_service.resolve_accolades
    end

    it 'should call the division repository to get the highest values for all division accolades for the current season' do
      expect( @dr ).to receive( :get_division_stats_by_highest ).with( anything, 4, anything ).and_return( [] ).at_least :once

      @accolade_service.resolve_accolades
    end

    it 'should call the division repository to create accolades for the divisions returned by the get division stats method' do
      allow( @dr ).to receive( :get_division_stats_by_highest ).and_return( [{division_id: 1, season: 4}] )

      expect( @dr ).to receive( :save_division_accolade ).with( hash_including :division_id => 1, :season => 4, :accolade => Accolades::Division::World_Title  )
      expect( @dr ).to receive( :save_division_accolade ).with( hash_including :division_id => 1, :season => 4, :accolade => Accolades::Division::League_Title )
      expect( @dr ).to receive( :save_division_accolade ).with( hash_including :division_id => 1, :season => 4, :accolade => Accolades::Division::Best_Record  )

      @accolade_service.resolve_accolades
    end

    it 'should call the team repository to get the highest or lowest values for all team accolades for the current season' do
      expect( @tr ).to receive( :get_team_stats_by_highest ).with( anything, 4, anything ).and_return( [] ).at_least :once
      expect( @tr ).to receive( :get_team_stats_by_lowest  ).with( anything, 4, anything ).and_return( [] ).at_least :once

      @accolade_service.resolve_accolades
    end

    it 'should call the team repository to create accolades for the teams returned by the get team stats method' do
      allow( @tr ).to receive( :get_team_stats_by_highest ).and_return( [{team_id: 1, season: 4}] )
      allow( @tr ).to receive( :get_team_stats_by_lowest  ).and_return( [{team_id: 1, season: 4}] )

      expect( @tr ).to receive( :save_team_accolade ).with( hash_including :team_id => 1, :season => 4, :accolade => Accolades::Team::World_Title       )
      expect( @tr ).to receive( :save_team_accolade ).with( hash_including :team_id => 1, :season => 4, :accolade => Accolades::Team::League_Title      )
      expect( @tr ).to receive( :save_team_accolade ).with( hash_including :team_id => 1, :season => 4, :accolade => Accolades::Team::Division_Title    )
      expect( @tr ).to receive( :save_team_accolade ).with( hash_including :team_id => 1, :season => 4, :accolade => Accolades::Team::Best_Record       )
      expect( @tr ).to receive( :save_team_accolade ).with( hash_including :team_id => 1, :season => 4, :accolade => Accolades::Team::All_Star_Champion )

      @accolade_service.resolve_accolades
    end

    it 'should call the player repository to create player accolades for world title' do
      allow( @tr ).to receive( :get_team_accolades_by_season  ).and_return( [{team_id: 1, season: 4, accolade: Accolades::Team::World_Title}] )
      allow( @pr ).to receive( :get_players_by_team ).and_return( [{team_id: 1, player_id: 1}] )

      expect( @pr ).to receive( :save_player_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Player::World_Title )

      @accolade_service.resolve_accolades
    end

    it 'should call the player repository to create player accolades for league title' do
      allow( @tr ).to receive( :get_team_accolades_by_season  ).and_return( [{team_id: 1, season: 4, accolade: Accolades::Team::League_Title}] )
      allow( @pr ).to receive( :get_players_by_team ).and_return( [{team_id: 1, player_id: 1}] )

      expect( @pr ).to receive( :save_player_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Player::League_Title )

      @accolade_service.resolve_accolades
    end

    it 'should call the player repository to create player accolades for division title' do
      allow( @tr ).to receive( :get_team_accolades_by_season  ).and_return( [{team_id: 1, season: 4, accolade: Accolades::Team::Division_Title}] )
      allow( @pr ).to receive( :get_players_by_team ).and_return( [{team_id: 1, player_id: 1}] )

      expect( @pr ).to receive( :save_player_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Player::Division_Title )

      @accolade_service.resolve_accolades
    end

    it 'should call the player repository to create player accolades for best record' do
      allow( @tr ).to receive( :get_team_accolades_by_season  ).and_return( [{team_id: 1, season: 4, accolade: Accolades::Team::Best_Record}] )
      allow( @pr ).to receive( :get_players_by_team ).and_return( [{team_id: 1, player_id: 1}] )

      expect( @pr ).to receive( :save_player_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Player::Best_Record )

      @accolade_service.resolve_accolades
    end

    it 'should call the player repository to create player accolades for all star champion' do
      allow( @tr ).to receive( :get_team_accolades_by_season  ).and_return( [{team_id: 1, season: 4, accolade: Accolades::Team::All_Star_Champion}] )
      allow( @pr ).to receive( :get_players_by_team ).and_return( [{team_id: 1, player_id: 1}] )

      expect( @pr ).to receive( :save_player_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Player::All_Star_Champion )

      @accolade_service.resolve_accolades
    end

    it 'should call the player repository to create player accolades for all star' do
      allow( @tr ).to receive( :get_league_teams ).and_return( [{league_id: 1, team_id: 33},{league_id: 2, team_id: 34}] )
      allow( @pr ).to receive( :get_players_by_team ).with( 33, 4 ).and_return( [{team_id: 33, player_id: 1}] )
      allow( @pr ).to receive( :get_players_by_team ).with( 34, 4 ).and_return( [{team_id: 34, player_id: 2}] )

      expect( @pr ).to receive( :save_player_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Player::All_Star )
      expect( @pr ).to receive( :save_player_accolade ).with( hash_including :player_id => 2, :season => 4, :accolade => Accolades::Player::All_Star )

      @accolade_service.resolve_accolades
    end

    it 'should call the player repository to get the highest or lowest values for all the pitcher accolades for the current season' do
      expect( @pr ).to receive( :get_starting_pitcher_stats_by_highest ).with( anything, 'innings', 185, 4, anything ).and_return( [] ).at_least :once
      expect( @pr ).to receive( :get_starting_pitcher_stats_by_lowest  ).with( anything, 'innings', 185, 4, anything ).and_return( [] ).at_least :once

      @accolade_service.resolve_accolades
    end

    it 'should set the filter value to zero for most saves pitching accolade' do
      expect( @pr ).to receive( :get_starting_pitcher_stats_by_highest  ).with( 'saves', 'innings', 0, 4, Phases::RegularSeason ).and_return( [] ).at_least :once

      @accolade_service.resolve_accolades
    end

    it 'should call the player repository to create accolades for the players returned by the get starting pitcher stats method' do
      allow( @pr ).to receive( :get_starting_pitcher_stats_by_highest ).and_return( [{player_id: 1, season: 4}] )
      allow( @pr ).to receive( :get_starting_pitcher_stats_by_lowest  ).and_return( [{player_id: 1, season: 4}] )

      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Pitching::Most_Wins                  )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Pitching::Best_Win_Loss_Ratio        )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Pitching::Best_Win_Percentage        )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Pitching::Best_Earned_Run_Average    )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Pitching::Best_vs_Batting_Average    )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Pitching::Most_Saves                 )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Pitching::Most_Strike_Outs           )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Pitching::Most_Innings_per_Game      )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Pitching::Best_Walks_Hits_Per_Inning )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Pitching::Most_Strike_Outs_per_Nine  )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Pitching::Best_Home_Runs_per_Nine    )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Pitching::Best_Efficiency            )

      @accolade_service.resolve_accolades
    end

    it 'should call the player repository to get the highest or lowest values for all the batter accolades for the current season' do
      expect( @pr ).to receive( :get_batter_stats_by_highest ).with( anything, 'at_bats', 300, 4, anything ).and_return( [] ).at_least :once
      expect( @pr ).to receive( :get_batter_stats_by_lowest  ).with( anything, 'at_bats', 300, 4, anything ).and_return( [] ).at_least :once

      @accolade_service.resolve_accolades
    end

    it 'should call the player repository to create accolades for the players returned by the get batter stats method' do
      allow( @pr ).to receive( :get_batter_stats_by_highest ).and_return( [{player_id: 1, season: 4}] )
      allow( @pr ).to receive( :get_batter_stats_by_lowest  ).and_return( [{player_id: 1, season: 4}] )

      expect( @pr ).to receive( :save_batter_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Batting::Best_Batting_Average    )
      expect( @pr ).to receive( :save_batter_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Batting::Most_Doubles            )
      expect( @pr ).to receive( :save_batter_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Batting::Most_Triples            )
      expect( @pr ).to receive( :save_batter_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Batting::Most_Home_Runs          )
      expect( @pr ).to receive( :save_batter_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Batting::Most_Runs               )
      expect( @pr ).to receive( :save_batter_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Batting::Most_Runs_Batted_In     )
      expect( @pr ).to receive( :save_batter_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Batting::Most_Stolen_Bases       )
      expect( @pr ).to receive( :save_batter_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Batting::Most_Walks              )
      expect( @pr ).to receive( :save_batter_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Batting::Best_Slugging_Average   )
      expect( @pr ).to receive( :save_batter_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Batting::Best_On_Base_Percentage )
      expect( @pr ).to receive( :save_batter_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Batting::Best_Strike_Out_Average )
      expect( @pr ).to receive( :save_batter_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Batting::Best_Runs_per_Game      )

      @accolade_service.resolve_accolades
    end

    it 'should call the player repository to get the highest or lowest values for all the closer accolades for the current season' do
      expect( @pr ).to receive( :get_closing_pitcher_stats_by_highest ).with( anything, 'innings', 85, 4, anything ).and_return( [] ).at_least :once
      expect( @pr ).to receive( :get_closing_pitcher_stats_by_lowest  ).with( anything, 'innings', 85, 4, anything ).and_return( [] ).at_least :once

      @accolade_service.resolve_accolades
    end

    it 'should call the player repository to create accolades for the players returned by the get closing pitcher stats method' do
      allow( @pr ).to receive( :get_closing_pitcher_stats_by_highest ).and_return( [{player_id: 1, season: 4}] )
      allow( @pr ).to receive( :get_closing_pitcher_stats_by_lowest  ).and_return( [{player_id: 1, season: 4}] )

      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Closing::Most_Wins                  )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Closing::Best_Win_Loss_Ratio        )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Closing::Best_Win_Percentage        )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Closing::Best_Earned_Run_Average    )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Closing::Best_vs_Batting_Average    )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Closing::Most_Saves                 )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Closing::Most_Strike_Outs           )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Closing::Most_Innings_per_Game      )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Closing::Best_Walks_Hits_Per_Inning )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Closing::Most_Strike_Outs_per_Nine  )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Closing::Best_Home_Runs_per_Nine    )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Closing::Best_Efficiency            )
      expect( @pr ).to receive( :save_pitcher_accolade ).with( hash_including :player_id => 1, :season => 4, :accolade => Accolades::Closing::Best_Relief_Pitching_Eff   )

      @accolade_service.resolve_accolades
    end
  end
end
