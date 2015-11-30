location = File.dirname __FILE__
$: << "#{location}"

require 'player_decorator'
require 'player_repository'
require 'player_response_mapper'
require 'player_service'

describe PlayerService do
  before :each do
    @repo   = double PlayerRepository
    @deco   = double PlayerDecorator
    @mapper = double PlayerResponseMapper

    @player_service = PlayerService.new @repo, @mapper, @deco
  end

  describe '#get_team_players' do
    it 'should call the repository to get the player records from the database' do
      allow( @mapper ).to receive( :map_player_list ).and_return Array.new

      expect( @repo ).to receive( :get_players_by_team ).with 1, nil

      @player_service.get_team_players 1
    end

    it 'should call the repository with season to get the player records from the database' do
      allow( @mapper ).to receive( :map_player_list ).and_return Array.new

      expect( @repo ).to receive( :get_players_by_team ).with 1, 3

      @player_service.get_team_players 1, 3
    end

    it 'should call the response mapper with the player list from the repository' do
      player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_players_by_team ).with( 1, 3 ).and_return player_list

      expect( @mapper ).to receive( :map_player_list ).with( player_list ).and_return Array.new

      @player_service.get_team_players 1, 3
    end

    it 'should call the decorator with each player in the mapped player list' do
      mapped_player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_players_by_team ).with( 1, 3 )
      allow( @mapper ).to receive( :map_player_list ).and_return mapped_player_list

      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[0], 3, nil
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[1], 3, nil
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[2], 3, nil

      @player_service.get_team_players 1, 3
    end

    it 'should return the mapped, decorated player list' do
      mapped_player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_players_by_team ).with( 1, 3 )
      allow( @mapper ).to receive( :map_player_list ).and_return mapped_player_list
      allow( @deco ).to receive( :decorate_player ) { |t| t[:_links] = {} }

      results = @player_service.get_team_players 1, 3

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     3

      results.each do |entry|
        expect( entry[:player_id] ).to_not be_nil
        expect( entry[:name     ] ).to_not be_nil
        expect( entry[:_links   ] ).to_not be_nil
      end
    end

    it 'should pass season and phase parameters to the decorator' do
      mapped_player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_players_by_team ).with( 1, 3 )
      allow( @mapper ).to receive( :map_player_list ).and_return mapped_player_list

      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[0], 3, Phases::RegularSeason
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[1], 3, Phases::RegularSeason
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[2], 3, Phases::RegularSeason

      @player_service.get_team_players 1, 3, Phases::RegularSeason
    end
  end

  describe '#get_players' do
    it 'should call the repository to get the player records from the database' do
      allow( @mapper ).to receive( :map_player_list ).and_return Array.new

      expect( @repo ).to receive( :get_players )

      @player_service.get_players
    end

    it 'should call the response mapper with the player list from the repository' do
      player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_players ).and_return player_list

      expect( @mapper ).to receive( :map_player_list ).with( player_list ).and_return Array.new

      @player_service.get_players
    end

    it 'should call the decorator with each player in the mapped player list' do
      mapped_player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_players )
      allow( @mapper ).to receive( :map_player_list ).and_return mapped_player_list

      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[0], nil, nil
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[1], nil, nil
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[2], nil, nil

      @player_service.get_players
    end

    it 'should return the mapped, decorated player list' do
      mapped_player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_players )
      allow( @mapper ).to receive( :map_player_list ).and_return mapped_player_list
      allow( @deco ).to receive( :decorate_player ) { |t| t[:_links] = {} }

      results = @player_service.get_players

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     3

      results.each do |entry|
        expect( entry[:player_id] ).to_not be_nil
        expect( entry[:name     ] ).to_not be_nil
        expect( entry[:_links   ] ).to_not be_nil
      end
    end

    it 'should pass season and phase parameters to the decorator' do
      mapped_player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_players )
      allow( @mapper ).to receive( :map_player_list ).and_return mapped_player_list

      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[0], 3, Phases::RegularSeason
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[1], 3, Phases::RegularSeason
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[2], 3, Phases::RegularSeason

      @player_service.get_players 3, Phases::RegularSeason
    end
  end

  describe '#get_rookies' do
    it 'should call the repository to get the player records from the database' do
      allow( @mapper ).to receive( :map_player_list ).and_return Array.new

      expect( @repo ).to receive( :get_players_by_rookie_season ).with 3

      @player_service.get_rookies 3
    end

    it 'should call the response mapper with the player list from the repository' do
      player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_players_by_rookie_season ).with( 3 ).and_return player_list

      expect( @mapper ).to receive( :map_player_list ).with( player_list ).and_return Array.new

      @player_service.get_rookies 3
    end

    it 'should call the decorator with each player in the mapped player list' do
      mapped_player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_players_by_rookie_season ).with( 3 )
      allow( @mapper ).to receive( :map_player_list ).and_return mapped_player_list

      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[0], 3, nil
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[1], 3, nil
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[2], 3, nil

      @player_service.get_rookies 3
    end

    it 'should return the mapped, decorated player list' do
      mapped_player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_players_by_rookie_season )
      allow( @mapper ).to receive( :map_player_list ).and_return mapped_player_list
      allow( @deco ).to receive( :decorate_player ) { |t| t[:_links] = {} }

      results = @player_service.get_rookies 3

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     3

      results.each do |entry|
        expect( entry[:player_id] ).to_not be_nil
        expect( entry[:name     ] ).to_not be_nil
        expect( entry[:_links   ] ).to_not be_nil
      end
    end

    it 'should pass season and phase parameters to the decorator' do
      mapped_player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_players_by_rookie_season )
      allow( @mapper ).to receive( :map_player_list ).and_return mapped_player_list

      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[0], 3, Phases::RegularSeason
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[1], 3, Phases::RegularSeason
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[2], 3, Phases::RegularSeason

      results = @player_service.get_rookies 3, Phases::RegularSeason
    end
  end

  describe '#get_free_agents' do
    it 'should call the repository to get the player records from the database' do
      allow( @mapper ).to receive( :map_player_list ).and_return Array.new

      expect( @repo ).to receive( :get_free_agents_by_season ).with 3

      @player_service.get_free_agents 3
    end

    it 'should call the response mapper with the player list from the repository' do
      player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_free_agents_by_season ).with( 3 ).and_return player_list

      expect( @mapper ).to receive( :map_player_list ).with( player_list ).and_return Array.new

      @player_service.get_free_agents 3
    end

    it 'should call the decorator with each player in the mapped player list' do
      mapped_player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_free_agents_by_season ).with( 3 )
      allow( @mapper ).to receive( :map_player_list ).and_return mapped_player_list

      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[0], 3, nil
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[1], 3, nil
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[2], 3, nil

      @player_service.get_free_agents 3
    end

    it 'should return the mapped, decorated player list' do
      mapped_player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_free_agents_by_season )
      allow( @mapper ).to receive( :map_player_list ).and_return mapped_player_list
      allow( @deco ).to receive( :decorate_player ) { |t| t[:_links] = {} }

      results = @player_service.get_free_agents 3

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     3

      results.each do |entry|
        expect( entry[:player_id] ).to_not be_nil
        expect( entry[:name     ] ).to_not be_nil
        expect( entry[:_links   ] ).to_not be_nil
      end
    end

    it 'should pass season and phase parameters to the decorator' do
      mapped_player_list = [{player_id: 1, name: 'Player1'},{player_id: 2, name: 'Player2'},{player_id: 3, name: 'Player3'}]

      allow( @repo ).to receive( :get_free_agents_by_season )
      allow( @mapper ).to receive( :map_player_list ).and_return mapped_player_list

      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[0], 3, Phases::RegularSeason
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[1], 3, Phases::RegularSeason
      expect( @deco ).to receive( :decorate_player ).with mapped_player_list[2], 3, Phases::RegularSeason

      results = @player_service.get_free_agents 3, Phases::RegularSeason
    end
  end

  describe '#get_player' do
    it 'should call the repository to get the player record from the database' do
      allow( @mapper ).to receive( :map_player )
      allow( @deco ).to receive( :decorate_player )

      expect( @repo ).to receive( :get_player ).with( 1 ).and_return Hash.new

      @player_service.get_player 1
    end

    it 'should call the repository to get the pitcher record from the database if the player type is pitcher' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Pitcher, rookie_season: 5, longevity: 7}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @mapper ).to receive( :map_pitcher )
      allow( @deco ).to receive( :decorate_player )

      expect( @repo ).to receive( :get_pitcher_details ).with( 1 ).and_return Hash.new

      @player_service.get_player 1
    end

    it 'should call the repository to get the batter record from the database if the player type is batter' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @mapper ).to receive( :map_batter )
      allow( @deco ).to receive( :decorate_player )

      expect( @repo ).to receive( :get_batter_details ).with( 1 ).and_return Hash.new

      @player_service.get_player 1
    end

    it 'should call the response mapper with the player record from the repository if player type is not batter nor pitcher' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::None, rookie_season: 5, longevity: 7}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @deco ).to receive( :decorate_player )

      expect( @mapper ).to receive( :map_player ).with player

      @player_service.get_player 1
    end

    it 'should call the response mapper with the player and pitcher records from the repository if player type is pitcher' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Pitcher, rookie_season: 5, longevity: 7}
      pitcher = {player_id: 1, speed: 4, control: 6, bunt: 2, fatigue: 7}

      allow( @repo ).to receive( :get_player  ).and_return player
      allow( @repo ).to receive( :get_pitcher_details ).and_return pitcher
      allow( @deco ).to receive( :decorate_player )

      expect( @mapper ).to receive( :map_pitcher ).with player, pitcher, nil

      @player_service.get_player 1
    end

    it 'should call the response mapper with the pitcher records and season if given a season' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Pitcher, rookie_season: 5, longevity: 7}
      pitcher = {player_id: 1, speed: 4, control: 6, bunt: 2, fatigue: 7}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_pitcher_details ).and_return pitcher
      allow( @deco ).to receive( :decorate_player )

      expect( @mapper ).to receive( :map_pitcher ).with player, pitcher, 5

      @player_service.get_player 1, 5
    end

    it 'should call the response mapper with the player and batter records from the repository if player type is batter' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}
      batter = {player_id: 1, power: 6, hit_n_run: 5, bunt: 3, running: 9, range: 7, arm: 3, primary_position: Positions::ThirdBaseman, secondary_position: Positions::LeftFielder}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_batter_details ).and_return batter
      allow( @deco ).to receive( :decorate_player )

      expect( @mapper ).to receive( :map_batter ).with player, batter, nil

      @player_service.get_player 1
    end

    it 'should call the response mapper with the batter records and season if given a season' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}
      batter = {player_id: 1, power: 6, hit_n_run: 5, bunt: 3, running: 9, range: 7, arm: 3, primary_position: Positions::ThirdBaseman, secondary_position: Positions::LeftFielder}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_batter_details ).and_return batter
      allow( @deco ).to receive( :decorate_player )

      expect( @mapper ).to receive( :map_batter ).with player, batter, 5

      @player_service.get_player 1, 5
    end

    it 'should call the decorator with the mapped player record if player type is not batter nor pitcher' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::None, rookie_season: 5, longevity: 7}
      mapped_player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: 'Light', handedness: 'Switch', player_type: 'Unknown', rookie_season: 5, longevity: 7}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @mapper ).to receive( :map_player ).and_return mapped_player

      expect( @deco ).to receive( :decorate_player ).with mapped_player, nil, nil

      @player_service.get_player 1
    end

    it 'should call the decorator with the mapped player record if player type is pitcher' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Pitcher, rookie_season: 5, longevity: 7}
      mapped_player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: 'Light', handedness: 'Switch', player_type: 'Pitcher', rookie_season: 5, longevity: 7, speed: 4, control: 6, bunt: 2, fatigue: 7}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_pitcher_details ).and_return Hash.new
      allow( @mapper ).to receive( :map_pitcher ).and_return mapped_player

      expect( @deco ).to receive( :decorate_player ).with mapped_player, nil, nil

      @player_service.get_player 1
    end

    it 'should call the decorator with the mapped player record if player type is batter' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}
      mapped_player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: 'Light', handedness: 'Switch', player_type: 'Batter', rookie_season: 5, longevity: 7, power: 6, hit_n_run: 5, bunt: 3, running: 9, range: 7, arm: 3, primary_position: Positions::ThirdBaseman, secondary_position: Positions::LeftFielder}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_batter_details ).and_return Hash.new
      allow( @mapper ).to receive( :map_batter ).and_return mapped_player

      expect( @deco ).to receive( :decorate_player ).with mapped_player, nil, nil

      @player_service.get_player 1
    end

    it 'should return the mapped, decorated database record if player type is not batter nor pitcher' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::None, rookie_season: 5, longevity: 7}
      mapped_decorated_player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: 'Light', handedness: 'Switch', player_type: 'Unknown', rookie_season: 5, longevity: 7, _links: {}}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @mapper ).to receive( :map_player )
      allow( @deco ).to receive( :decorate_player ).and_return mapped_decorated_player

      expect( @player_service.get_player 1 ).to be mapped_decorated_player
    end

    it 'should return the mapped, decorated database record if player type is pitcher' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Pitcher, rookie_season: 5, longevity: 7}
      mapped_decorated_player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: 'Light', handedness: 'Switch', player_type: 'Pitcher', rookie_season: 5, longevity: 7, _links: {}}

      allow( @repo ).to receive( :get_player  ).and_return player
      allow( @repo ).to receive( :get_pitcher_details ).and_return Hash.new
      allow( @mapper ).to receive( :map_pitcher )
      allow( @deco ).to receive( :decorate_player ).and_return mapped_decorated_player

      expect( @player_service.get_player 1 ).to be mapped_decorated_player
    end

    it 'should return the mapped, decorated database record if player type is batter' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}
      mapped_decorated_player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: 'Light', handedness: 'Switch', player_type: 'Batter', rookie_season: 5, longevity: 7, _links: {}}

      allow( @repo ).to receive( :get_player  ).and_return player
      allow( @repo ).to receive( :get_batter_details ).and_return Hash.new
      allow( @mapper ).to receive( :map_batter )
      allow( @deco ).to receive( :decorate_player ).and_return mapped_decorated_player

      expect( @player_service.get_player 1 ).to be mapped_decorated_player
    end

    it 'should pass season and phase parameters to the decorator if player type is not batter nor pitcher' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::None, rookie_season: 5, longevity: 7}
      mapped_player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: 'Light', handedness: 'Switch', player_type: 'Unknown', rookie_season: 5, longevity: 7}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @mapper ).to receive( :map_player ).and_return mapped_player

      expect( @deco ).to receive( :decorate_player ).with mapped_player, 3, Phases::RegularSeason

      @player_service.get_player 1, 3, Phases::RegularSeason
    end

    it 'should pass season and phase parameters to the decorator if player type is pitcher' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Pitcher, rookie_season: 5, longevity: 7}
      mapped_player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: 'Light', handedness: 'Switch', player_type: 'Pitcher', rookie_season: 5, longevity: 7}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_pitcher_details ).and_return Hash.new
      allow( @mapper ).to receive( :map_pitcher ).and_return mapped_player

      expect( @deco ).to receive( :decorate_player ).with mapped_player, 3, Phases::RegularSeason

      @player_service.get_player 1, 3, Phases::RegularSeason
    end

    it 'should pass season and phase parameters to the decorator if player type is batter' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}
      mapped_player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: 'Light', handedness: 'Switch', player_type: 'Batter', rookie_season: 5, longevity: 7}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_batter_details ).and_return Hash.new
      allow( @mapper ).to receive( :map_batter ).and_return mapped_player

      expect( @deco ).to receive( :decorate_player ).with mapped_player, 3, Phases::RegularSeason

      @player_service.get_player 1, 3, Phases::RegularSeason
    end

    it 'should raise a resource not found error exception if the player record is not in the database' do
      allow( @repo ).to receive( :get_player )

      expect { @player_service.get_player 1 }.to raise_error ResourceNotFoundError, 'Player with Player ID 1 cannot be located.'
    end

    it 'should raise an internal server error if the pitcher record is not in the database' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}

      allow( @repo ).to receive( :get_player  ).and_return player
      allow( @repo ).to receive( :get_pitcher_details )

      expect { @player_service.get_player 1 }.to raise_error InternalServerError, 'There was an error retrieving Pitcher with Player ID 1.'
    end

    it 'should raise an internal server error if the batter record is not in the database' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_batter_details )

      expect { @player_service.get_player 1 }.to raise_error InternalServerError, 'There was an error retrieving Batter with Player ID 1.'
    end
  end

  describe '#get_player_stats' do
    it 'should call the repository to get the player record from the database' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}

      allow( @repo ).to receive( :get_pitcher_stats ).and_return Hash.new
      allow( @mapper ).to receive( :map_pitcher_stats )
      allow( @deco ).to receive( :decorate_player_stats )

      expect( @repo ).to receive( :get_player ).with( 1 ).and_return player

      @player_service.get_player_stats 1, 3, Phases::RegularSeason
    end

    it 'should call the repository to get a pitcher stats record from the database if the player is a pitcher' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Pitcher, rookie_season: 5, longevity: 7}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @mapper ).to receive( :map_pitcher_stats )
      allow( @deco ).to receive( :decorate_player_stats )

      expect( @repo ).to receive( :get_pitcher_stats ).with( 1, 3, Phases::RegularSeason ).and_return Hash.new

      @player_service.get_player_stats 1, 3, Phases::RegularSeason
    end

    it 'should call the response mapper with the pitcher stats record from the repository' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      pitcher_stats = {player_id: 1, season: 3, season_phase: Phases::RegularSeason, wins: 22, losses: 13, games: 37, saves: 5, innings: 303, outs: 1, hits: 309, earned_runs: 150, home_runs: 64, walks: 100, strike_outs: 205}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_pitcher_stats ).and_return pitcher_stats
      allow( @deco ).to receive( :decorate_player_stats )

      expect( @mapper ).to receive( :map_pitcher_stats ).with pitcher_stats

      @player_service.get_player_stats 1, 3, Phases::RegularSeason
    end

    it 'should call the decorator with the mapped pitcher stats hash' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      mapped_pitcher_stats = {player_id: 1, season: 3, season_phase: 'Regular'}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_pitcher_stats ).and_return Hash.new
      allow( @mapper ).to receive( :map_pitcher_stats ).and_return mapped_pitcher_stats

      expect( @deco ).to receive( :decorate_player_stats ).with mapped_pitcher_stats, Phases::RegularSeason

      @player_service.get_player_stats 1, 3, Phases::RegularSeason
    end

    it 'should return the mapped, decorated pitcher stats' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      decorated_pitcher_stats = {player_id: 1, season: 3, season_phase: 'Regular', _links:{}}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_pitcher_stats ).and_return Hash.new
      allow( @mapper ).to receive( :map_pitcher_stats )
      allow( @deco ).to receive( :decorate_player_stats ).and_return decorated_pitcher_stats

      expect( @player_service.get_player_stats 1, 3, Phases::RegularSeason ).to be decorated_pitcher_stats
    end

    it 'should call the repository to get many pitcher stats records from the database if the player is a pitcher' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Pitcher, rookie_season: 5, longevity: 7}

      allow( @repo ).to receive( :get_player ).and_return player

      expect( @repo ).to receive( :get_pitcher_stats ).with( 1, nil, nil ).and_return Array.new

      @player_service.get_player_stats 1
    end

    it 'should call the response mapper with each of the pitcher stats records from the repository' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      pitcher_stats = [{player_id: 1, season: 1, season_phase: Phases::RegularSeason},{player_id: 1, season: 1, season_phase: Phases::Playoff},{player_id: 1, season: 2, season_phase: Phases::RegularSeason},{player_id: 1, season: 2, season_phase: Phases::Playoff}]

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_pitcher_stats ).and_return pitcher_stats
      allow( @deco ).to receive( :decorate_player_stats )

      expect( @mapper ).to receive( :map_pitcher_stats ).with pitcher_stats[0]
      expect( @mapper ).to receive( :map_pitcher_stats ).with pitcher_stats[1]
      expect( @mapper ).to receive( :map_pitcher_stats ).with pitcher_stats[2]
      expect( @mapper ).to receive( :map_pitcher_stats ).with pitcher_stats[3]

      @player_service.get_player_stats 1
    end

    it 'should call the decorator with each player stats record in the mapped player stats list' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      pitcher_stats = [{player_id: 1, season: 1, season_phase: Phases::RegularSeason},{player_id: 1, season: 1, season_phase: Phases::Playoff},{player_id: 1, season: 2, season_phase: Phases::RegularSeason},{player_id: 1, season: 2, season_phase: Phases::Playoff}]
      mapped_pitcher_stats = [{player_id: 1, season: 1, season_phase: 'Regular'},{player_id: 1, season: 1, season_phase: 'Playoff'},{player_id: 1, season: 2, season_phase: 'Regular'},{player_id: 1, season: 2, season_phase: 'Playoff'}]

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_pitcher_stats ).and_return pitcher_stats
      allow( @mapper ).to receive( :map_pitcher_stats ).with( pitcher_stats[0] ).and_return mapped_pitcher_stats[0]
      allow( @mapper ).to receive( :map_pitcher_stats ).with( pitcher_stats[1] ).and_return mapped_pitcher_stats[1]
      allow( @mapper ).to receive( :map_pitcher_stats ).with( pitcher_stats[2] ).and_return mapped_pitcher_stats[2]
      allow( @mapper ).to receive( :map_pitcher_stats ).with( pitcher_stats[3] ).and_return mapped_pitcher_stats[3]

      expect( @deco ).to receive( :decorate_player_stats ).with mapped_pitcher_stats[0], Phases::RegularSeason
      expect( @deco ).to receive( :decorate_player_stats ).with mapped_pitcher_stats[1], Phases::Playoff
      expect( @deco ).to receive( :decorate_player_stats ).with mapped_pitcher_stats[2], Phases::RegularSeason
      expect( @deco ).to receive( :decorate_player_stats ).with mapped_pitcher_stats[3], Phases::Playoff

      @player_service.get_player_stats 1
    end

    it 'should return the mapped, decorated pitcher stats list' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      pitcher_stats = [{player_id: 1, season: 1, season_phase: Phases::RegularSeason},{player_id: 1, season: 1, season_phase: Phases::Playoff},{player_id: 1, season: 2, season_phase: Phases::RegularSeason},{player_id: 1, season: 2, season_phase: Phases::Playoff}]
      mapped_pitcher_stats = [{player_id: 1, season: 1, season_phase: 'Regular'},{player_id: 1, season: 1, season_phase: 'Playoff'},{player_id: 1, season: 2, season_phase: 'Regular'},{player_id: 1, season: 2, season_phase: 'Playoff'}]

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_pitcher_stats ).and_return pitcher_stats
      allow( @mapper ).to receive( :map_pitcher_stats ).with( pitcher_stats[0] ).and_return mapped_pitcher_stats[0]
      allow( @mapper ).to receive( :map_pitcher_stats ).with( pitcher_stats[1] ).and_return mapped_pitcher_stats[1]
      allow( @mapper ).to receive( :map_pitcher_stats ).with( pitcher_stats[2] ).and_return mapped_pitcher_stats[2]
      allow( @mapper ).to receive( :map_pitcher_stats ).with( pitcher_stats[3] ).and_return mapped_pitcher_stats[3]
      allow( @deco ).to receive( :decorate_player_stats ) { |ts| ts[:_links] = {} }

      results = @player_service.get_player_stats 1

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     4

      results.each do |entry|
        expect( entry[:player_id   ] ).to_not be_nil
        expect( entry[:season      ] ).to_not be_nil
        expect( entry[:season_phase] ).to_not be_nil
        expect( entry[:_links      ] ).to_not be_nil
      end
    end

    it 'should call the repository to get a batter stats record from the database if the player is a batter' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @mapper ).to receive( :map_batter_stats )
      allow( @deco ).to receive( :decorate_player_stats )

      expect( @repo ).to receive( :get_batter_stats ).with( 1, 3, Phases::RegularSeason ).and_return Hash.new

      @player_service.get_player_stats 1, 3, Phases::RegularSeason
    end

    it 'should call the response mapper with the batter stats record from the repository' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}
      batter_stats = {player_id: 1, season: 1, season_phase: Phases::RegularSeason, wins: 22, losses: 13, games: 37, saves: 5, innings: 303, outs: 1, hits: 309, earned_runs: 150, home_runs: 64, walks: 100, strike_outs: 205}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_batter_stats ).and_return batter_stats
      allow( @deco ).to receive( :decorate_player_stats )

      expect( @mapper ).to receive( :map_batter_stats ).with batter_stats

      @player_service.get_player_stats 1, 3, Phases::RegularSeason
    end

    it 'should call the decorator with the mapped batter stats hash' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}
      mapped_batter_stats = {player_id: 1, season: 1, season_phase: 'Regular'}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_batter_stats ).and_return Hash.new
      allow( @mapper ).to receive( :map_batter_stats ).and_return mapped_batter_stats

      expect( @deco ).to receive( :decorate_player_stats ).with mapped_batter_stats, Phases::RegularSeason

      @player_service.get_player_stats 1, 3, Phases::RegularSeason
    end

    it 'should return the mapped, decorated batter stats' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}
      decorated_batter_stats = {player_id: 1, season: 3, season_phase: 'Regular', _links:{}}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_batter_stats ).and_return Hash.new
      allow( @mapper ).to receive( :map_batter_stats )
      allow( @deco ).to receive( :decorate_player_stats ).and_return decorated_batter_stats

      expect( @player_service.get_player_stats 1, 3, Phases::RegularSeason ).to be decorated_batter_stats
    end

    it 'should call the repository to get many batter stats records from the database if the player is a batter' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @mapper ).to receive( :map_batter_stats )

      expect( @repo ).to receive( :get_batter_stats ).with( 1, nil, nil ).and_return Array.new

      @player_service.get_player_stats 1
    end

    it 'should call the response mapper with each of the batter stats records from the repository' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}
      batter_stats = [{player_id: 1, season: 1, season_phase: Phases::RegularSeason},{player_id: 1, season: 1, season_phase: Phases::Playoff},{player_id: 1, season: 2, season_phase: Phases::RegularSeason},{player_id: 1, season: 2, season_phase: Phases::Playoff}]

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_batter_stats ).and_return batter_stats
      allow( @deco ).to receive( :decorate_player_stats )

      expect( @mapper ).to receive( :map_batter_stats ).with batter_stats[0]
      expect( @mapper ).to receive( :map_batter_stats ).with batter_stats[1]
      expect( @mapper ).to receive( :map_batter_stats ).with batter_stats[2]
      expect( @mapper ).to receive( :map_batter_stats ).with batter_stats[3]

      @player_service.get_player_stats 1
    end

    it 'should call the decorator with each player stats record in the mapped player stats list' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}
      batter_stats = [{player_id: 1, season: 1, season_phase: Phases::RegularSeason},{player_id: 1, season: 1, season_phase: Phases::Playoff},{player_id: 1, season: 2, season_phase: Phases::RegularSeason},{player_id: 1, season: 2, season_phase: Phases::Playoff}]
      mapped_batter_stats = [{player_id: 1, season: 1, season_phase: 'Regular'},{player_id: 1, season: 1, season_phase: 'Playoff'},{player_id: 1, season: 2, season_phase: 'Regular'},{player_id: 1, season: 2, season_phase: 'Playoff'}]

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_batter_stats ).and_return batter_stats
      allow( @mapper ).to receive( :map_batter_stats ).with( batter_stats[0] ).and_return mapped_batter_stats[0]
      allow( @mapper ).to receive( :map_batter_stats ).with( batter_stats[1] ).and_return mapped_batter_stats[1]
      allow( @mapper ).to receive( :map_batter_stats ).with( batter_stats[2] ).and_return mapped_batter_stats[2]
      allow( @mapper ).to receive( :map_batter_stats ).with( batter_stats[3] ).and_return mapped_batter_stats[3]

      expect( @deco ).to receive( :decorate_player_stats ).with mapped_batter_stats[0], Phases::RegularSeason
      expect( @deco ).to receive( :decorate_player_stats ).with mapped_batter_stats[1], Phases::Playoff
      expect( @deco ).to receive( :decorate_player_stats ).with mapped_batter_stats[2], Phases::RegularSeason
      expect( @deco ).to receive( :decorate_player_stats ).with mapped_batter_stats[3], Phases::Playoff

      @player_service.get_player_stats 1
    end

    it 'should return the mapped, decorated batter stats list' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}
      batter_stats = [{player_id: 1, season: 1, season_phase: Phases::RegularSeason},{player_id: 1, season: 1, season_phase: Phases::Playoff},{player_id: 1, season: 2, season_phase: Phases::RegularSeason},{player_id: 1, season: 2, season_phase: Phases::Playoff}]
      mapped_batter_stats = [{player_id: 1, season: 1, season_phase: 'Regular'},{player_id: 1, season: 1, season_phase: 'Playoff'},{player_id: 1, season: 2, season_phase: 'Regular'},{player_id: 1, season: 2, season_phase: 'Playoff'}]

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :get_batter_stats ).and_return batter_stats
      allow( @mapper ).to receive( :map_batter_stats ).with( batter_stats[0] ).and_return mapped_batter_stats[0]
      allow( @mapper ).to receive( :map_batter_stats ).with( batter_stats[1] ).and_return mapped_batter_stats[1]
      allow( @mapper ).to receive( :map_batter_stats ).with( batter_stats[2] ).and_return mapped_batter_stats[2]
      allow( @mapper ).to receive( :map_batter_stats ).with( batter_stats[3] ).and_return mapped_batter_stats[3]
      allow( @deco ).to receive( :decorate_player_stats ) { |ts| ts[:_links] = {} }

      results = @player_service.get_player_stats 1

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     4

      results.each do |entry|
        expect( entry[:player_id   ] ).to_not be_nil
        expect( entry[:season      ] ).to_not be_nil
        expect( entry[:season_phase] ).to_not be_nil
        expect( entry[:_links      ] ).to_not be_nil
      end
    end

    it 'should raise a resource not found error exception if the player record is not in the database' do
      allow( @repo ).to receive( :get_player )

      expect { @player_service.get_player_stats 1, 3, Phases::RegularSeason }.to raise_error ResourceNotFoundError, 'Player with Player ID 1 cannot be located.'
    end

    it 'should raise a resource not found error exception if the pitcher stats record is not in the database' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}

      allow( @repo ).to receive( :get_player  ).and_return player
      allow( @repo ).to receive( :get_pitcher_stats )

      expect { @player_service.get_player_stats 1, 3, Phases::RegularSeason }.to raise_error ResourceNotFoundError, "Player Stats for Player ID 1, Season 3 and Phase #{Phases::RegularSeason} cannot be located."
    end

    it 'should raise a resource not found error exception if the batter stats record is not in the database' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}

      allow( @repo ).to receive( :get_player  ).and_return player
      allow( @repo ).to receive( :get_batter_stats )

      expect { @player_service.get_player_stats 1, 3, Phases::RegularSeason }.to raise_error ResourceNotFoundError, "Player Stats for Player ID 1, Season 3 and Phase #{Phases::RegularSeason} cannot be located."
    end

    it 'should raise a resource not found error exception if the player type is not batter nor pitcher' do
      player = {player_id: 1, player_type: PlayerTypes::None}

      allow( @repo ).to receive( :get_player  ).and_return player

      expect { @player_service.get_player_stats 1, 3, Phases::RegularSeason }.to raise_error ResourceNotFoundError, "Player Stats for Player ID 1, Season 3 and Phase #{Phases::RegularSeason} cannot be located."
    end

    it 'should return an empty array if the player type is not batter nor pitcher and many records are requested' do
      player = {player_id: 1, player_type: PlayerTypes::None}

      allow( @repo ).to receive( :get_player  ).and_return player

      results = @player_service.get_player_stats 1

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     0
    end
  end

  describe '#get_player_accolades' do
    it 'should call the repository to get the player record from the database' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}

      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return []
      allow( @repo ).to receive( :get_pitcher_accolades ).with( 1, nil ).and_return []

      expect( @repo ).to receive( :get_player ).with( 1 ).and_return player

      @player_service.get_player_accolades 1
    end

    it 'should call the repository to get player accolades records from the database' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      player_accolades = [{player_id: 1, season: 3, accolade: 1},{player_id: 1, season: 4, accolade: 1}]

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_pitcher_accolades ).with( 1, nil ).and_return []
      allow( @mapper ).to receive( :map_player_accolade )
      allow( @deco ).to receive( :decorate_player_accolade )

      expect( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return player_accolades

      @player_service.get_player_accolades 1
    end

    it 'should call the response mapper with each of the player accolades records from the repository' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      player_accolades = [{player_id: 1, season: 3, accolade: 1},{player_id: 1, season: 4, accolade: 1}]

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return player_accolades
      allow( @repo ).to receive( :get_pitcher_accolades ).with( 1, nil ).and_return []
      allow( @deco ).to receive( :decorate_player_accolade )

      expect( @mapper ).to receive( :map_player_accolade ).with( player_accolades[0] )
      expect( @mapper ).to receive( :map_player_accolade ).with( player_accolades[1] )

      @player_service.get_player_accolades 1
    end

    it 'should call the decorator with each player accolades record in the mapped player accolades list' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      player_accolades = [{player_id: 1, season: 3, accolade: 1},{player_id: 1, season: 4, accolade: 1}]
      mapped_player_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade'},{player_id: 1, season: 4, accolade: 'Some Accolade'}]

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return player_accolades
      allow( @repo ).to receive( :get_pitcher_accolades ).with( 1, nil ).and_return []
      allow( @mapper ).to receive( :map_player_accolade ).with( player_accolades[0] ).and_return mapped_player_accolades[0]
      allow( @mapper ).to receive( :map_player_accolade ).with( player_accolades[1] ).and_return mapped_player_accolades[1]

      expect( @deco ).to receive( :decorate_player_accolade ).with( mapped_player_accolades[0] )
      expect( @deco ).to receive( :decorate_player_accolade ).with( mapped_player_accolades[1] )

      @player_service.get_player_accolades 1
    end

    it 'should call the repository to get pitcher accolades records from the database if the player is a pitcher' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}

      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return []
      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @mapper ).to receive( :map_pitcher_accolade )

      expect( @repo ).to receive( :get_pitcher_accolades ).with( 1, nil ).and_return []

      @player_service.get_player_accolades 1
    end

    it 'should call the response mapper with each of the pitcher accolades records from the repository' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      pitcher_accolades = [{player_id: 1, season: 3, accolade: 1},{player_id: 1, season: 4, accolade: 1}]

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return []
      allow( @repo ).to receive( :get_pitcher_accolades ).with( 1, nil ).and_return pitcher_accolades
      allow( @deco ).to receive( :decorate_player_accolade )

      expect( @mapper ).to receive( :map_pitcher_accolade ).with( pitcher_accolades[0] )
      expect( @mapper ).to receive( :map_pitcher_accolade ).with( pitcher_accolades[1] )

      @player_service.get_player_accolades 1
    end

    it 'should call the decorator with each player accolades record in the mapped pitcher accolades list' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      pitcher_accolades = [{player_id: 1, season: 3, accolade: 1},{player_id: 1, season: 4, accolade: 1}]
      mapped_pitcher_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade'},{player_id: 1, season: 4, accolade: 'Some Accolade'}]

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return []
      allow( @repo ).to receive( :get_pitcher_accolades ).with( 1, nil ).and_return pitcher_accolades
      allow( @mapper ).to receive( :map_pitcher_accolade ).with( pitcher_accolades[0] ).and_return mapped_pitcher_accolades[0]
      allow( @mapper ).to receive( :map_pitcher_accolade ).with( pitcher_accolades[1] ).and_return mapped_pitcher_accolades[1]

      expect( @deco ).to receive( :decorate_player_accolade ).with mapped_pitcher_accolades[0]
      expect( @deco ).to receive( :decorate_player_accolade ).with mapped_pitcher_accolades[1]

      @player_service.get_player_accolades 1
    end

    it 'should return the mapped, decorated player and pitcher accolades list' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      player_accolades = [{player_id: 1, season: 3, accolade: 1},{player_id: 1, season: 4, accolade: 1}]
      mapped_player_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade'},{player_id: 1, season: 4, accolade: 'Some Accolade'}]
      decorated_player_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade', _links: {}},
                                    {player_id: 1, season: 4, accolade: 'Some Accolade', _links: {}}]
      pitcher_accolades = [{player_id: 1, season: 3, accolade: 2},{player_id: 1, season: 4, accolade: 2}]
      mapped_pitcher_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade 2'},{player_id: 1, season: 4, accolade: 'Some Accolade 2'}]
      decorated_pitcher_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade 2', _links: {}},
                                     {player_id: 1, season: 4, accolade: 'Some Accolade 2', _links: {}}]

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return player_accolades
      allow( @mapper ).to receive( :map_player_accolade ).with( player_accolades[0] ).and_return mapped_player_accolades[0]
      allow( @mapper ).to receive( :map_player_accolade ).with( player_accolades[1] ).and_return mapped_player_accolades[1]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_player_accolades[0] ).and_return decorated_player_accolades[0]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_player_accolades[1] ).and_return decorated_player_accolades[1]
      allow( @repo ).to receive( :get_pitcher_accolades ).with( 1, nil ).and_return pitcher_accolades
      allow( @mapper ).to receive( :map_pitcher_accolade ).with( pitcher_accolades[0] ).and_return mapped_pitcher_accolades[0]
      allow( @mapper ).to receive( :map_pitcher_accolade ).with( pitcher_accolades[1] ).and_return mapped_pitcher_accolades[1]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_pitcher_accolades[0] ).and_return decorated_pitcher_accolades[0]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_pitcher_accolades[1] ).and_return decorated_pitcher_accolades[1]

      results = @player_service.get_player_accolades 1

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     4

      expect( results[0] ).to eq decorated_player_accolades[0]
      expect( results[1] ).to eq decorated_player_accolades[1]
      expect( results[2] ).to eq decorated_pitcher_accolades[0]
      expect( results[3] ).to eq decorated_pitcher_accolades[1]
    end

    it 'should return all player and pitcher accolades if season is not given' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      player_accolades = [{player_id: 1, season: 3, accolade: 1},{player_id: 1, season: 4, accolade: 1}]
      mapped_player_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade'},{player_id: 1, season: 4, accolade: 'Some Accolade'}]
      decorated_player_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade', _links: {}},
                                    {player_id: 1, season: 4, accolade: 'Some Accolade', _links: {}}]
      pitcher_accolades = [{player_id: 1, season: 3, accolade: 2},{player_id: 1, season: 4, accolade: 2}]
      mapped_pitcher_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade 2'},{player_id: 1, season: 4, accolade: 'Some Accolade 2'}]
      decorated_pitcher_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade 2', _links: {}},
                                     {player_id: 1, season: 4, accolade: 'Some Accolade 2', _links: {}}]

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return player_accolades
      allow( @mapper ).to receive( :map_player_accolade ).with( player_accolades[0] ).and_return mapped_player_accolades[0]
      allow( @mapper ).to receive( :map_player_accolade ).with( player_accolades[1] ).and_return mapped_player_accolades[1]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_player_accolades[0] ).and_return decorated_player_accolades[0]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_player_accolades[1] ).and_return decorated_player_accolades[1]
      allow( @repo ).to receive( :get_pitcher_accolades ).with( 1, nil ).and_return pitcher_accolades
      allow( @mapper ).to receive( :map_pitcher_accolade ).with( pitcher_accolades[0] ).and_return mapped_pitcher_accolades[0]
      allow( @mapper ).to receive( :map_pitcher_accolade ).with( pitcher_accolades[1] ).and_return mapped_pitcher_accolades[1]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_pitcher_accolades[0] ).and_return decorated_pitcher_accolades[0]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_pitcher_accolades[1] ).and_return decorated_pitcher_accolades[1]

      results = @player_service.get_player_accolades 1

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     4
    end

    it 'should return only player and pitcher accolades for the given season' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      player_accolades = [{player_id: 1, season: 3, accolade: 1},{player_id: 1, season: 4, accolade: 1}]
      mapped_player_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade'},{player_id: 1, season: 4, accolade: 'Some Accolade'}]
      decorated_player_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade', _links: {}},
                                    {player_id: 1, season: 4, accolade: 'Some Accolade', _links: {}}]
      pitcher_accolades = [{player_id: 1, season: 3, accolade: 2},{player_id: 1, season: 4, accolade: 2}]
      mapped_pitcher_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade 2'},{player_id: 1, season: 4, accolade: 'Some Accolade 2'}]
      decorated_pitcher_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade 2', _links: {}},
                                     {player_id: 1, season: 4, accolade: 'Some Accolade 2', _links: {}}]

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_player_accolades ).with( 1, 3 ).and_return player_accolades[0..0]
      allow( @mapper ).to receive( :map_player_accolade ).with( player_accolades[0] ).and_return mapped_player_accolades[0]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_player_accolades[0] ).and_return decorated_player_accolades[0]
      allow( @repo ).to receive( :get_pitcher_accolades ).with( 1, 3 ).and_return pitcher_accolades[0..0]
      allow( @mapper ).to receive( :map_pitcher_accolade ).with( pitcher_accolades[0] ).and_return mapped_pitcher_accolades[0]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_pitcher_accolades[0] ).and_return decorated_pitcher_accolades[0]

      results = @player_service.get_player_accolades 1, 3

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     2
    end

    it 'should return an empty array if ther are no player or pitcher accolade records in the database' do
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return []
      allow( @repo ).to receive( :get_pitcher_accolades ).with( 1, nil ).and_return []

      results = @player_service.get_player_accolades 1

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     0
    end

    it 'should call the repository to get batter accolades records from the database if the player is a batter' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}

      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return []
      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @mapper ).to receive( :map_batter_accolade )

      expect( @repo ).to receive( :get_batter_accolades ).with( 1, nil ).and_return []

      @player_service.get_player_accolades 1
    end

    it 'should call the response mapper with each of the batter accolades records from the repository' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}
      batter_accolades = [{player_id: 1, season: 3, accolade: 1},{player_id: 1, season: 4, accolade: 1}]

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return []
      allow( @repo ).to receive( :get_batter_accolades ).with( 1, nil ).and_return batter_accolades
      allow( @deco ).to receive( :decorate_player_accolade )

      expect( @mapper ).to receive( :map_batter_accolade ).with( batter_accolades[0] )
      expect( @mapper ).to receive( :map_batter_accolade ).with( batter_accolades[1] )

      @player_service.get_player_accolades 1
    end

    it 'should call the decorator with each player accolades record in the mapped batter accolades list' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}
      batter_accolades = [{player_id: 1, season: 3, accolade: 1},{player_id: 1, season: 4, accolade: 1}]
      mapped_batter_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade'},{player_id: 1, season: 4, accolade: 'Some Accolade'}]

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return []
      allow( @repo ).to receive( :get_batter_accolades ).with( 1, nil ).and_return batter_accolades
      allow( @mapper ).to receive( :map_batter_accolade ).with( batter_accolades[0] ).and_return mapped_batter_accolades[0]
      allow( @mapper ).to receive( :map_batter_accolade ).with( batter_accolades[1] ).and_return mapped_batter_accolades[1]

      expect( @deco ).to receive( :decorate_player_accolade ).with mapped_batter_accolades[0]
      expect( @deco ).to receive( :decorate_player_accolade ).with mapped_batter_accolades[1]

      @player_service.get_player_accolades 1
    end

    it 'should return the mapped, decorated player and batter accolades list' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}
      player_accolades = [{player_id: 1, season: 3, accolade: 1},{player_id: 1, season: 4, accolade: 1}]
      mapped_player_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade'},{player_id: 1, season: 4, accolade: 'Some Accolade'}]
      decorated_player_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade', _links: {}},
                                    {player_id: 1, season: 4, accolade: 'Some Accolade', _links: {}}]
      batter_accolades = [{player_id: 1, season: 3, accolade: 2},{player_id: 1, season: 4, accolade: 2}]
      mapped_batter_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade 2'},{player_id: 1, season: 4, accolade: 'Some Accolade 2'}]
      decorated_batter_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade 2', _links: {}},
                                     {player_id: 1, season: 4, accolade: 'Some Accolade 2', _links: {}}]

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return player_accolades
      allow( @mapper ).to receive( :map_player_accolade ).with( player_accolades[0] ).and_return mapped_player_accolades[0]
      allow( @mapper ).to receive( :map_player_accolade ).with( player_accolades[1] ).and_return mapped_player_accolades[1]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_player_accolades[0] ).and_return decorated_player_accolades[0]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_player_accolades[1] ).and_return decorated_player_accolades[1]
      allow( @repo ).to receive( :get_batter_accolades ).with( 1, nil ).and_return batter_accolades
      allow( @mapper ).to receive( :map_batter_accolade ).with( batter_accolades[0] ).and_return mapped_batter_accolades[0]
      allow( @mapper ).to receive( :map_batter_accolade ).with( batter_accolades[1] ).and_return mapped_batter_accolades[1]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_batter_accolades[0] ).and_return decorated_batter_accolades[0]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_batter_accolades[1] ).and_return decorated_batter_accolades[1]

      results = @player_service.get_player_accolades 1

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     4

      expect( results[0] ).to eq decorated_player_accolades[0]
      expect( results[1] ).to eq decorated_player_accolades[1]
      expect( results[2] ).to eq decorated_batter_accolades[0]
      expect( results[3] ).to eq decorated_batter_accolades[1]
    end

    it 'should return all player and batter accolades if season is not given' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}
      player_accolades = [{player_id: 1, season: 3, accolade: 1},{player_id: 1, season: 4, accolade: 1}]
      mapped_player_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade'},{player_id: 1, season: 4, accolade: 'Some Accolade'}]
      decorated_player_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade', _links: {}},
                                    {player_id: 1, season: 4, accolade: 'Some Accolade', _links: {}}]
      batter_accolades = [{player_id: 1, season: 3, accolade: 2},{player_id: 1, season: 4, accolade: 2}]
      mapped_batter_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade 2'},{player_id: 1, season: 4, accolade: 'Some Accolade 2'}]
      decorated_batter_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade 2', _links: {}},
                                     {player_id: 1, season: 4, accolade: 'Some Accolade 2', _links: {}}]

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return player_accolades
      allow( @mapper ).to receive( :map_player_accolade ).with( player_accolades[0] ).and_return mapped_player_accolades[0]
      allow( @mapper ).to receive( :map_player_accolade ).with( player_accolades[1] ).and_return mapped_player_accolades[1]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_player_accolades[0] ).and_return decorated_player_accolades[0]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_player_accolades[1] ).and_return decorated_player_accolades[1]
      allow( @repo ).to receive( :get_batter_accolades ).with( 1, nil ).and_return batter_accolades
      allow( @mapper ).to receive( :map_batter_accolade ).with( batter_accolades[0] ).and_return mapped_batter_accolades[0]
      allow( @mapper ).to receive( :map_batter_accolade ).with( batter_accolades[1] ).and_return mapped_batter_accolades[1]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_batter_accolades[0] ).and_return decorated_batter_accolades[0]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_batter_accolades[1] ).and_return decorated_batter_accolades[1]

      results = @player_service.get_player_accolades 1

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     4
    end

    it 'should return only player and batter accolades for the given season' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}
      player_accolades = [{player_id: 1, season: 3, accolade: 1},{player_id: 1, season: 4, accolade: 1}]
      mapped_player_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade'},{player_id: 1, season: 4, accolade: 'Some Accolade'}]
      decorated_player_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade', _links: {}},
                                    {player_id: 1, season: 4, accolade: 'Some Accolade', _links: {}}]
      batter_accolades = [{player_id: 1, season: 3, accolade: 2},{player_id: 1, season: 4, accolade: 2}]
      mapped_batter_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade 2'},{player_id: 1, season: 4, accolade: 'Some Accolade 2'}]
      decorated_batter_accolades = [{player_id: 1, season: 3, accolade: 'Some Accolade 2', _links: {}},
                                     {player_id: 1, season: 4, accolade: 'Some Accolade 2', _links: {}}]

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_player_accolades ).with( 1, 3 ).and_return player_accolades[0..0]
      allow( @mapper ).to receive( :map_player_accolade ).with( player_accolades[0] ).and_return mapped_player_accolades[0]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_player_accolades[0] ).and_return decorated_player_accolades[0]
      allow( @repo ).to receive( :get_batter_accolades ).with( 1, 3 ).and_return batter_accolades[0..0]
      allow( @mapper ).to receive( :map_batter_accolade ).with( batter_accolades[0] ).and_return mapped_batter_accolades[0]
      allow( @deco ).to receive( :decorate_player_accolade ).with( mapped_batter_accolades[0] ).and_return decorated_batter_accolades[0]

      results = @player_service.get_player_accolades 1, 3

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     2
    end

    it 'should return an empty array if ther are no player or batter accolade records in the database' do
      player = {player_id: 1, player_type: PlayerTypes::Batter}

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_player_accolades ).with( 1, nil ).and_return []
      allow( @repo ).to receive( :get_batter_accolades ).with( 1, nil ).and_return []

      results = @player_service.get_player_accolades 1

      expect( results        ).to_not be_nil
      expect( results        ).to     be_a   Array
      expect( results.length ).to     eq     0
    end

    it 'should raise a resource not found error exception if the player record is not in the database' do
      allow( @repo ).to receive( :get_player ).with( 1 ).and_return nil

      expect { @player_service.get_player_accolades 1 }.to raise_error ResourceNotFoundError, 'Player with Player ID 1 cannot be located.'
    end
  end

  describe '#save_player_accolade' do
    it 'should call the repository to get the player record from the database' do
      request = {player_id: 1, season: 2, accolade: 3, type: 'pitching'}
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}

      allow( @repo ).to receive( :get_pitcher_accolade )
      allow( @repo ).to receive( :save_pitcher_accolade )
      allow( @deco ).to receive( :decorate_player_accolade )

      expect( @repo ).to receive( :get_player ).with( 1 ).and_return player

      @player_service.save_player_accolade request
    end

    it 'should call the repository to save the pitcher accolade record to the database if the player is a pitcher' do
      request = {player_id: 1, season: 2, accolade: 3, type: 'pitching'}
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_pitcher_accolade )
      allow( @deco ).to receive( :decorate_player_accolade )

      expect( @repo ).to receive( :save_pitcher_accolade ).with( {player_id: 1, season: 2, accolade: 3} )

      @player_service.save_player_accolade request
    end

    it 'should return the newly created pitcher accolade record with decorations if the player is a pitcher' do
      request = {player_id: 1, season: 2, accolade: 3, type: 'pitching'}
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}
      accolade = {player_id: 1, season: 2, accolade: 3}
      decorated_accolade = {player_id: 1, season: 2, accolade: 3, _links:{}}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :save_pitcher_accolade )

      expect( @repo ).to receive( :get_pitcher_accolade ).with( 1, 2, 3).and_return accolade
      expect( @deco ).to receive( :decorate_player_accolade ).with( accolade ).and_return decorated_accolade

      result = @player_service.save_player_accolade request

      expect( result ).to be decorated_accolade
    end

    it 'should throw a bad request exception if the pitcher accolade is invalid' do
      request = {player_id: 1, season: 2, accolade: 99, type: 'pitching'}
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player

      expect { @player_service.save_player_accolade request }.to raise_error BadRequestError, 'Accolade with value 99 is not a valid pitching accolade.'
    end

    it 'should throw a bad request exception if the pitching accolade type does not match the player type' do
      request = {player_id: 1, season: 2, accolade: 3, type: 'pitching'}
      player = {player_id: 1, player_type: PlayerTypes::Batter}

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player

      expect { @player_service.save_player_accolade request }.to raise_error BadRequestError, 'Request to create pitching accolade failed because player with ID 1 is a batter.'
    end

    it 'should call the repository to save the batter accolade record to the database if the player is a batter' do
      request = {player_id: 1, season: 2, accolade: 3, type: 'batting'}
      player = {player_id: 1, player_type: PlayerTypes::Batter}

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player
      allow( @repo ).to receive( :get_batter_accolade )
      allow( @deco ).to receive( :decorate_player_accolade )

      expect( @repo ).to receive( :save_batter_accolade ).with( {player_id: 1, season: 2, accolade: 3} )

      @player_service.save_player_accolade request
    end

    it 'should return the newly created batter accolade record with decorations if the player is a batter' do
      request = {player_id: 1, season: 2, accolade: 3, type: 'batting'}
      player = {player_id: 1, player_type: PlayerTypes::Batter}
      accolade = {player_id: 1, season: 2, accolade: 3}
      decorated_accolade = {player_id: 1, season: 2, accolade: 3, _links: {}}

      allow( @repo ).to receive( :get_player ).and_return player
      allow( @repo ).to receive( :save_batter_accolade )

      expect( @repo ).to receive( :get_batter_accolade ).with( 1, 2, 3).and_return accolade
      expect( @deco ).to receive( :decorate_player_accolade ).with( accolade ).and_return decorated_accolade

      result = @player_service.save_player_accolade request

      expect( result ).to be decorated_accolade
    end

    it 'should throw a bad request exception if the batter accolade is invalid' do
      request = {player_id: 1, season: 2, accolade: 99, type: 'batting'}
      player = {player_id: 1, player_type: PlayerTypes::Batter}

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player

      expect { @player_service.save_player_accolade request }.to raise_error BadRequestError, 'Accolade with value 99 is not a valid batting accolade.'
    end

    it 'should throw a bad request exception if the batting accolade type does not match the player type' do
      request = {player_id: 1, season: 2, accolade: 3, type: 'batting'}
      player = {player_id: 1, player_type: PlayerTypes::Pitcher}

      allow( @repo ).to receive( :get_player ).with( 1 ).and_return player

      expect { @player_service.save_player_accolade request }.to raise_error BadRequestError, 'Request to create batting accolade failed because player with ID 1 is a pitcher.'
    end

    it 'should throw a bad request exception if the request accolade type is not batting or pitching' do
      request = {player_id: 1, season: 2, accolade: 3, type: 'something-else'}

      expect { @player_service.save_player_accolade request }.to raise_error BadRequestError, 'Accolade type something-else is not supported. Only batting and pitching accolade types are allowed.'
    end
  end
end
