class TeamRecords
  attr_reader :team, :overall, :division, :league, :home, :road, :points_scored, :points_allowed, :opponents

  TeamDivisions = {
    'Sabres'    => 0, 'Scorpions' => 1, 'Aces'      => 2, 'Eclipse'   => 3, 'Global' => 4,
    'Portsmen'  => 0, 'Lightning' => 1, 'Cyclone'   => 2, 'Legends'   => 3, 'World'  => 4,
    'Settlers'  => 0, 'Goblins'   => 1, 'Stormers'  => 2, 'Waves'     => 3,
    'Kings'     => 0, 'Photons'   => 1, 'Express'   => 2, 'Horizon'   => 3,
    'Voyagers'  => 0, 'Dragons'   => 1, 'Warriors'  => 2, 'Sharks'    => 3,
    'Rockets'   => 0, 'Hammers'   => 1, 'Wanderers' => 2, 'Flames'    => 3,
    'Knights'   => 0, 'Expos'     => 1, 'Thunder'   => 2, 'Techs'     => 3,
    'Drizzle'   => 0, 'Dynamo'    => 1, 'Glory'     => 2, 'Quasars'   => 3 }

  TeamLocations = {
    'Sabres'    => 'Bordeaux',     'Scorpions' => 'Alexandria', 'Aces'      => 'Auckland',   'Eclipse'   => 'Guadalajara', 'Global' => 'Global',
    'Portsmen'  => 'Buenos Aires', 'Lightning' => 'Barcelona',  'Cyclone'   => 'Brasilia',   'Legends'   => 'Los Angeles', 'World'  => 'World',
    'Settlers'  => 'Freetown',     'Goblins'   => 'Chicago',    'Stormers'  => 'Cape Town',  'Waves'     => 'Maui',
    'Kings'     => 'London',       'Photons'   => 'Frankfurt',  'Express'   => 'Cordoba',    'Horizon'   => 'Santiago',
    'Voyagers'  => 'Miami',        'Dragons'   => 'Hong Kong',  'Warriors'  => 'Melbourne',  'Sharks'    => 'Shanghai',
    'Rockets'   => 'Monrovia',     'Hammers'   => 'Houston',    'Wanderers' => 'Montevideo', 'Flames'    => 'Sydney',
    'Knights'   => 'New York',     'Expos'     => 'Montreal',   'Thunder'   => 'Nairobi',    'Techs'     => 'Tokyo',
    'Drizzle'   => 'Sao Paulo',    'Dynamo'    => 'Moscow',     'Glory'     => 'Perth',      'Quasars'   => 'Vancouver' }

  class Record
    attr_accessor :wins, :losses

    def initialize
      @wins   = 0
      @losses = 0
    end

    def update( score, opp_score )
      if score > opp_score
        @wins += 1
      else
        @losses += 1
      end
    end

    def to_s( precision = 2 )
      return sprintf "%#{precision}d-%#{precision}d", @wins, @losses
    end
  end


  def initialize( team )
    @team     = team
    @overall  = Record.new
    @division = Record.new
    @league   = Record.new
    @home     = Record.new
    @road     = Record.new

    @points_scored  = 0
    @points_allowed = 0

    @opponents = {}
  end

  def print_records
    printf "%s  %s  %s  %s  %4d  %4d\n", @overall.to_s(3), @division.to_s, @home.to_s, @road.to_s, @points_scored, @points_allowed

    count = 0
    @opponents.sort.each do |opponent, record|
      printf "   %9s %s", opponent, record.to_s(1)

      count += 1

      if count == 4
        print "\n"
        count = 0
      end
    end

    print "\n\n"
  end

  def update( game, is_home )
    score     = is_home ? game.home_score : game.road_score
    opp_score = is_home ? game.road_score : game.home_score
    opponent  = is_home ? game.road_team  : game.home_team

    @points_scored += score
    @points_allowed += opp_score
    @overall.update score, opp_score

    if TeamRecords.in_division? game.home_team, game.road_team
      @division.update score, opp_score
    end

    if TeamRecords.in_league? game.home_team, game.road_team
      @league.update score, opp_score
    end

    if is_home
      @home.update score, opp_score
    else
      @road.update score, opp_score
    end

    if @opponents.has_key? opponent
      record = @opponents[opponent]
    else
      record = Record.new
    end

    record.update score, opp_score
    @opponents[opponent] = record
  end

  def self.in_division?( home_team, road_team )
    return (TeamDivisions[home_team] == TeamDivisions[road_team])
  end

  def self.in_league?( home_team, road_team )
    return (TeamDivisions[home_team] / 2) == (TeamDivisions[road_team] / 2)
  end

  def self.is_allstar?( team )
    return (TeamDivisions[team] == 4)
  end
end
