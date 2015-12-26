module Accolades
  module League
    Type = 'league'

    World_Title       = 1
    Best_Record       = 2
    All_Star_Champion = 3
  end

  module Division
    Type = 'division'

    World_Title    = 1
    League_Title   = 2
    Best_Record    = 3
  end

  module Team
    Type = 'team'

    World_Title       = 1
    League_Title      = 2
    Division_Title    = 3
    Best_Record       = 4
    All_Star_Champion = 5
  end

  module Player
    Type = 'player'

    All_Star          = 1
    World_Title       = 2
    League_Title      = 3
    Division_Title    = 4
    Best_Record       = 5
    World_Series_MVP  = 6
    All_Star_Champion = 7
    All_Star_MVP      = 8
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

  AccoladeList = [
    { name: "World Title",                       value: Accolades::League::World_Title,                    type: Accolades::League::Type,   automatic: true  },
    { name: "Best Record",                       value: Accolades::League::Best_Record,                    type: Accolades::League::Type,   automatic: true  },
    { name: "All Star Champion",                 value: Accolades::League::All_Star_Champion,              type: Accolades::League::Type,   automatic: true  },

    { name: "World Title",                       value: Accolades::Division::World_Title,                  type: Accolades::Division::Type, automatic: true  },
    { name: "League Title",                      value: Accolades::Division::League_Title,                 type: Accolades::Division::Type, automatic: true  },
    { name: "Best Record",                       value: Accolades::Division::Best_Record,                  type: Accolades::Division::Type, automatic: true  },

    { name: "World Title",                       value: Accolades::Team::World_Title,                      type: Accolades::Team::Type,     automatic: true  },
    { name: "League Title",                      value: Accolades::Team::League_Title,                     type: Accolades::Team::Type,     automatic: true  },
    { name: "Division Title",                    value: Accolades::Team::Division_Title,                   type: Accolades::Team::Type,     automatic: true  },
    { name: "Best Record",                       value: Accolades::Team::Best_Record,                      type: Accolades::Team::Type,     automatic: true  },
    { name: "All Star Champion",                 value: Accolades::Team::All_Star_Champion,                type: Accolades::Team::Type,     automatic: true  },

    { name: "All Star",                          value: Accolades::Player::All_Star,                       type: Accolades::Player::Type,   automatic: true  },
    { name: "World Title",                       value: Accolades::Player::World_Title,                    type: Accolades::Player::Type,   automatic: true  },
    { name: "League Title",                      value: Accolades::Player::League_Title,                   type: Accolades::Player::Type,   automatic: true  },
    { name: "Division Title",                    value: Accolades::Player::Division_Title,                 type: Accolades::Player::Type,   automatic: true  },
    { name: "Best Record",                       value: Accolades::Player::Best_Record,                    type: Accolades::Player::Type,   automatic: true  },
    { name: "World Series MVP",                  value: Accolades::Player::World_Series_MVP,               type: Accolades::Player::Type,   automatic: false },
    { name: "All Star Champion",                 value: Accolades::Player::All_Star_Champion,              type: Accolades::Player::Type,   automatic: true  },
    { name: "All Star MVP",                      value: Accolades::Player::All_Star_MVP,                   type: Accolades::Player::Type,   automatic: false },

    { name: "MBA Batter of the Year",            value: Accolades::Batting::Batter_of_the_Year,            type: Accolades::Batting::Type,  automatic: false },
    { name: "Global League Batter of the Year",  value: Accolades::Batting::Global_Batter_of_the_Year,     type: Accolades::Batting::Type,  automatic: false },
    { name: "World League Batter of the Year",   value: Accolades::Batting::World_Batter_of_the_Year,      type: Accolades::Batting::Type,  automatic: false },
    { name: "Atlantic Batter of the Year",       value: Accolades::Batting::Atlantic_Batter_of_the_Year,   type: Accolades::Batting::Type,  automatic: false },
    { name: "North Batter of the Year",          value: Accolades::Batting::North_Batter_of_the_Year,      type: Accolades::Batting::Type,  automatic: false },
    { name: "Pacific Batter of the Year",        value: Accolades::Batting::Pacific_Batter_of_the_Year,    type: Accolades::Batting::Type,  automatic: false },
    { name: "South Batter of the Year",          value: Accolades::Batting::South_Batter_of_the_Year,      type: Accolades::Batting::Type,  automatic: false },
    { name: "Best Batting Average",              value: Accolades::Batting::Best_Batting_Average,          type: Accolades::Batting::Type,  automatic: true  },
    { name: "Most Doubles",                      value: Accolades::Batting::Most_Doubles,                  type: Accolades::Batting::Type,  automatic: true  },
    { name: "Most Triples",                      value: Accolades::Batting::Most_Triples,                  type: Accolades::Batting::Type,  automatic: true  },
    { name: "Most Home Runs",                    value: Accolades::Batting::Most_Home_Runs,                type: Accolades::Batting::Type,  automatic: true  },
    { name: "Most Runs",                         value: Accolades::Batting::Most_Runs,                     type: Accolades::Batting::Type,  automatic: true  },
    { name: "Most Runs Batted In",               value: Accolades::Batting::Most_Runs_Batted_In,           type: Accolades::Batting::Type,  automatic: true  },
    { name: "Most Stolen Bases",                 value: Accolades::Batting::Most_Stolen_Bases,             type: Accolades::Batting::Type,  automatic: true  },
    { name: "Most Walks",                        value: Accolades::Batting::Most_Walks,                    type: Accolades::Batting::Type,  automatic: true  },
    { name: "Best Slugging Average",             value: Accolades::Batting::Best_Slugging_Average,         type: Accolades::Batting::Type,  automatic: true  },
    { name: "Best On Base Percentage",           value: Accolades::Batting::Best_On_Base_Percentage,       type: Accolades::Batting::Type,  automatic: true  },
    { name: "Best Strike Out Average",           value: Accolades::Batting::Best_Strike_Out_Average,       type: Accolades::Batting::Type,  automatic: true  },
    { name: "Best Runs per Game",                value: Accolades::Batting::Best_Runs_per_Game,            type: Accolades::Batting::Type,  automatic: true  },

    { name: "MBA Pitcher of the Year",           value: Accolades::Pitching::Pitcher_of_the_Year,          type: Accolades::Pitching::Type, automatic: false },
    { name: "Global League Pitcher of the Year", value: Accolades::Pitching::Global_Pitcher_of_the_Year,   type: Accolades::Pitching::Type, automatic: false },
    { name: "World League Pitcher of the Year",  value: Accolades::Pitching::World_Pitcher_of_the_Year,    type: Accolades::Pitching::Type, automatic: false },
    { name: "Atlantic Pitcher of the Year",      value: Accolades::Pitching::Atlantic_Pitcher_of_the_Year, type: Accolades::Pitching::Type, automatic: false },
    { name: "North Pitcher of the Year",         value: Accolades::Pitching::North_Pitcher_of_the_Year,    type: Accolades::Pitching::Type, automatic: false },
    { name: "Pacific Pitcher of the Year",       value: Accolades::Pitching::Pacific_Pitcher_of_the_Year,  type: Accolades::Pitching::Type, automatic: false },
    { name: "South Pitcher of the Year",         value: Accolades::Pitching::South_Pitcher_of_the_Year,    type: Accolades::Pitching::Type, automatic: false },
    { name: "Most Wins",                         value: Accolades::Pitching::Most_Wins,                    type: Accolades::Pitching::Type, automatic: true  },
    { name: "Best Win Loss Ratio",               value: Accolades::Pitching::Best_Win_Loss_Ratio,          type: Accolades::Pitching::Type, automatic: true  },
    { name: "Best Win Percentage",               value: Accolades::Pitching::Best_Win_Percentage,          type: Accolades::Pitching::Type, automatic: true  },
    { name: "Best Earned Run Average",           value: Accolades::Pitching::Best_Earned_Run_Average,      type: Accolades::Pitching::Type, automatic: true  },
    { name: "Best vs Batting Average",           value: Accolades::Pitching::Best_vs_Batting_Average,      type: Accolades::Pitching::Type, automatic: true  },
    { name: "Most Saves",                        value: Accolades::Pitching::Most_Saves,                   type: Accolades::Pitching::Type, automatic: true  },
    { name: "Most Strike Outs",                  value: Accolades::Pitching::Most_Strike_Outs,             type: Accolades::Pitching::Type, automatic: true  },
    { name: "Most Innings per Game",             value: Accolades::Pitching::Most_Innings_per_Game,        type: Accolades::Pitching::Type, automatic: true  },
    { name: "Best Walks Hits Per Inning",        value: Accolades::Pitching::Best_Walks_Hits_Per_Inning,   type: Accolades::Pitching::Type, automatic: true  },
    { name: "Most Strike Outs per Nine",         value: Accolades::Pitching::Most_Strike_Outs_per_Nine,    type: Accolades::Pitching::Type, automatic: true  },
    { name: "Best Home Runs per Nine",           value: Accolades::Pitching::Best_Home_Runs_per_Nine,      type: Accolades::Pitching::Type, automatic: true  },
    { name: "Best Efficiency",                   value: Accolades::Pitching::Best_Efficiency,              type: Accolades::Pitching::Type, automatic: true  } ]

  def self.valid?( type, value )
    AccoladeList.each do |accolade|
      if accolade[:type] == type and accolade[:value] == value
        return true
      end
    end

    return false
  end

  def self.get_accolade_name( type, value )
    Accolades::AccoladeList.each do |accolade|
      if accolade[:type] == type and accolade[:value] == value
        return accolade[:name]
      end
    end

    return nil
  end
end
