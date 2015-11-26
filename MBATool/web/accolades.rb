module Accolades
  module League
    Type = 'league'

    World_Title    = 1
    Best_Record    = 2
  end

  module Division
    Type = 'division'

    World_Title    = 1
    League_Title   = 2
    Best_Record    = 3
  end

  module Team
    Type = 'team'

    World_Title    = 1
    League_Title   = 2
    Division_Title = 3
    Best_Record    = 4
  end

  module Player
    Type = 'player'

    All_Star       = 1
    World_Title    = 2
    League_Title   = 3
    Division_Title = 4
    Best_Record    = 5
  end

  module Batting
    Type = 'batting'

    Batter_of_the_Year          =  1
    Global_Batter_of_the_Year   =  2
    World_Batter_of_the_Year    =  3
    Atlantic_Batter_of_the_Year =  4
    North_Batter_of_the_Year    =  5
    Pacific_Batter_of_the_Year  =  6
    South_Batter_of_the_Year    =  7
    Best_Batting_Average        =  8
    Most_Doubles                =  9
    Most_Triples                = 10
    Most_Home_Runs              = 11
    Most_Runs                   = 12
    Most_Runs_Batted_In         = 13
    Most_Stolen_Bases           = 14
    Most_Walks                  = 15
    Best_Slugging_Average       = 16
    Best_On_Base_Percentage     = 17
    Best_Strike_Out_Average     = 18
    Best_Runs_per_Game          = 19
  end

  module Pitching
    Type = 'pitching'

    Pitcher_of_the_Year          =  1
    Global_Pitcher_of_the_Year   =  2
    World_Pitcher_of_the_Year    =  3
    Atlantic_Pitcher_of_the_Year =  4
    North_Pitcher_of_the_Year    =  5
    Pacific_Pitcher_of_the_Year  =  6
    South_Pitcher_of_the_Year    =  7
    Most_Wins                    =  8
    Best_Win_Loss_Ratio          =  9
    Best_Win_Percentage          = 10
    Best_Earned_Run_Average      = 11
    Best_vs_Batting_Average      = 12
    Most_Saves                   = 13
    Most_Strike_Outs             = 14
    Most_Innings_per_Game        = 15
    Best_Walks_Hits_Per_Inning   = 16
    Most_Strike_Outs_per_Nine    = 17
    Best_Home_Runs_per_Nine      = 18
    Best_Efficiency              = 19
  end
end
