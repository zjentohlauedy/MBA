location = File.dirname __FILE__
$: << "#{location}"

require 'player_response_mapper'
require 'utils'
require 'player_types'
require 'internal_server_error'

describe PlayerResponseMapper do
  before :each do
    @player_response_mapper = PlayerResponseMapper.new
  end

  describe '#map_player_list' do
    it 'should return an array with the same number of elements as the given array' do
      player_list = [{player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15},
                     {player_id: 2, first_name: 'Firstname2', last_name: 'Lastname2', first_phoenetic: 'FN2', last_phoenetic: 'LN2', skin_tone: SkinTones::Dark, handedness: Handedness::Right, player_type: PlayerTypes::Pitcher, rookie_season: 3, longevity: 4, retired_season: nil}]

      result = @player_response_mapper.map_player_list player_list

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     player_list.length
    end

    it 'should return players with only ID and name' do
      player_list = [{player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15},
                     {player_id: 2, first_name: 'Firstname2', last_name: 'Lastname2', first_phoenetic: 'FN2', last_phoenetic: 'LN2', skin_tone: SkinTones::Dark, handedness: Handedness::Right, player_type: PlayerTypes::Pitcher, rookie_season: 3, longevity: 4, retired_season: nil}]

      result = @player_response_mapper.map_player_list player_list

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0].length      ).to eq 2
      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:name     ] ).to eq 'Firstname1 Lastname1'
      expect( result[1].length      ).to eq 2
      expect( result[1][:player_id] ).to eq 2
      expect( result[1][:name     ] ).to eq 'Firstname2 Lastname2'
    end

    it 'should return nil if not given an array' do
      result = @player_response_mapper.map_player_list Object.new

      expect( result ).to be_nil
    end

    it 'should return nil if not given an array of hashes' do
      result = @player_response_mapper.map_player_list [Object.new, Object.new]

      expect( result ).to be_nil
    end
  end

  describe '#map_player' do
    it 'should return a mapped player hash' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}

      result = @player_response_mapper.map_player player

      expect( result                    ).to_not be_nil
      expect( result                    ).to     be_a   Hash
      expect( result[:player_id       ] ).to_not be_nil
      expect( result[:first_phoenetic ] ).to_not be_nil
      expect( result[:last_phoenetic  ] ).to_not be_nil
      expect( result[:skin_tone       ] ).to_not be_nil
      expect( result[:handedness      ] ).to_not be_nil
      expect( result[:player_type     ] ).to_not be_nil
      expect( result[:rookie_season   ] ).to_not be_nil
    end

    it 'should replace first name and last name with a combined name field' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}

      result = @player_response_mapper.map_player player

      expect( result              ).to_not be_nil
      expect( result              ).to     be_a   Hash
      expect( result[:name      ] ).to     eq     'Firstname1 Lastname1'
      expect( result[:first_name] ).to     be_nil
      expect( result[:last_name ] ).to     be_nil
    end

    it 'should remove longevity from the hash' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}

      result = @player_response_mapper.map_player player

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:longevity] ).to     be_nil
    end

    it 'should replace skin tone with a user friendly value' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}

      result = @player_response_mapper.map_player player

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:skin_tone] ).to     eq     'Light'
    end

    it 'should replace handedness with a user friendly value' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}

      result = @player_response_mapper.map_player player

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:handedness] ).to     eq     'S'
    end

    it 'should replace player type with a user friendly value' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}

      result = @player_response_mapper.map_player player

      expect( result               ).to_not be_nil
      expect( result               ).to     be_a   Hash
      expect( result[:player_type] ).to     eq     'Batter'
    end

    it 'should return nil if not given a hash' do
      result = @player_response_mapper.map_player Object.new

      expect( result ).to be_nil
    end
  end

  describe '#map_pitcher' do
    it 'should return a mapped player hash with pitcher information' do
      player  = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      pitcher = {player_id: 1, speed: 4, control: 6, bunt: 2, fatigue: 7}

      result = @player_response_mapper.map_pitcher player, pitcher

      expect( result                    ).to_not be_nil
      expect( result                    ).to     be_a   Hash
      expect( result[:player_id       ] ).to_not be_nil
      expect( result[:first_phoenetic ] ).to_not be_nil
      expect( result[:last_phoenetic  ] ).to_not be_nil
      expect( result[:skin_tone       ] ).to_not be_nil
      expect( result[:handedness      ] ).to_not be_nil
      expect( result[:player_type     ] ).to_not be_nil
      expect( result[:rookie_season   ] ).to_not be_nil
      expect( result[:speed           ] ).to_not be_nil
      expect( result[:control         ] ).to_not be_nil
      expect( result[:bunt            ] ).to_not be_nil
      expect( result[:fatigue         ] ).to_not be_nil
    end

    it 'should replace first name and last name with a combined name field' do
      player  = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      pitcher = {player_id: 1, speed: 4, control: 6, bunt: 2, fatigue: 7}

      result = @player_response_mapper.map_pitcher player, pitcher

      expect( result              ).to_not be_nil
      expect( result              ).to     be_a   Hash
      expect( result[:name      ] ).to     eq     'Firstname1 Lastname1'
      expect( result[:first_name] ).to     be_nil
      expect( result[:last_name ] ).to     be_nil
    end

    it 'should remove longevity from the hash' do
      player  = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      pitcher = {player_id: 1, speed: 4, control: 6, bunt: 2, fatigue: 7}

      result = @player_response_mapper.map_pitcher player, pitcher

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:longevity] ).to     be_nil
    end

    it 'should replace skin tone with a user friendly value' do
      player  = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      pitcher = {player_id: 1, speed: 4, control: 6, bunt: 2, fatigue: 7}

      result = @player_response_mapper.map_pitcher player, pitcher

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:skin_tone] ).to     eq     'Light'
    end

    it 'should replace handedness with a user friendly value' do
      player  = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      pitcher = {player_id: 1, speed: 4, control: 6, bunt: 2, fatigue: 7}

      result = @player_response_mapper.map_pitcher player, pitcher

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:handedness] ).to     eq     'S'
    end

    it 'should replace player type with a user friendly value' do
      player  = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      pitcher = {player_id: 1, speed: 4, control: 6, bunt: 2, fatigue: 7}

      result = @player_response_mapper.map_pitcher player, pitcher

      expect( result               ).to_not be_nil
      expect( result               ).to     be_a   Hash
      expect( result[:player_type] ).to     eq     'Batter'
    end

    it 'should adjust the pitcher ratings if given the season' do
      player  = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      pitcher = {player_id: 1, speed: 4, control: 6, bunt: 2, fatigue: 7}

      expect( Utils ).to receive( :adjust_rating  ).with 4, 7, 5, 7
      expect( Utils ).to receive( :adjust_rating  ).with 6, 7, 5, 7
      expect( Utils ).to receive( :adjust_rating  ).with 2, 7, 5, 7
      expect( Utils ).to receive( :adjust_fatigue ).with 7, 7, 5, 7

      result = @player_response_mapper.map_pitcher player, pitcher, 7
    end

    it 'should return nil if given player is not a hash' do
      pitcher = {player_id: 1, speed: 4, control: 6, bunt: 2, fatigue: 7}

      result = @player_response_mapper.map_pitcher Object.new, pitcher

      expect( result ).to be_nil
    end

    it 'should return nil if given pitcher is not a hash' do
      player  = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}

      result = @player_response_mapper.map_pitcher player, Object.new

      expect( result ).to be_nil
    end
  end

  describe '#map_batter' do
    it 'should return a mapped player hash with batter information' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      batter = {player_id: 1, power: 6, hit_n_run: 5, bunt: 3, running: 9, range: 7, arm: 3, primary_position: Positions::ThirdBaseman, secondary_position: Positions::LeftFielder}

      result = @player_response_mapper.map_batter player, batter

      expect( result                      ).to_not be_nil
      expect( result                      ).to     be_a   Hash
      expect( result[:player_id         ] ).to_not be_nil
      expect( result[:first_phoenetic   ] ).to_not be_nil
      expect( result[:last_phoenetic    ] ).to_not be_nil
      expect( result[:skin_tone         ] ).to_not be_nil
      expect( result[:handedness        ] ).to_not be_nil
      expect( result[:player_type       ] ).to_not be_nil
      expect( result[:rookie_season     ] ).to_not be_nil
      expect( result[:power             ] ).to_not be_nil
      expect( result[:hit_n_run         ] ).to_not be_nil
      expect( result[:bunt              ] ).to_not be_nil
      expect( result[:running           ] ).to_not be_nil
      expect( result[:range             ] ).to_not be_nil
      expect( result[:arm               ] ).to_not be_nil
      expect( result[:primary_position  ] ).to_not be_nil
      expect( result[:secondary_position] ).to_not be_nil
    end

    it 'should replace first name and last name with a combined name field' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      batter = {player_id: 1, power: 6, hit_n_run: 5, bunt: 3, running: 9, range: 7, arm: 3, primary_position: Positions::ThirdBaseman, secondary_position: Positions::LeftFielder}

      result = @player_response_mapper.map_batter player, batter

      expect( result              ).to_not be_nil
      expect( result              ).to     be_a   Hash
      expect( result[:name      ] ).to     eq     'Firstname1 Lastname1'
      expect( result[:first_name] ).to     be_nil
      expect( result[:last_name ] ).to     be_nil
    end

    it 'should remove longevity from the hash' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      batter = {player_id: 1, power: 6, hit_n_run: 5, bunt: 3, running: 9, range: 7, arm: 3, primary_position: Positions::ThirdBaseman, secondary_position: Positions::LeftFielder}

      result = @player_response_mapper.map_batter player, batter

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:longevity] ).to     be_nil
    end

    it 'should replace skin tone with a user friendly value' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      batter = {player_id: 1, power: 6, hit_n_run: 5, bunt: 3, running: 9, range: 7, arm: 3, primary_position: Positions::ThirdBaseman, secondary_position: Positions::LeftFielder}

      result = @player_response_mapper.map_batter player, batter

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:skin_tone] ).to     eq     'Light'
    end

    it 'should replace handedness with a user friendly value' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      batter = {player_id: 1, power: 6, hit_n_run: 5, bunt: 3, running: 9, range: 7, arm: 3, primary_position: Positions::ThirdBaseman, secondary_position: Positions::LeftFielder}

      result = @player_response_mapper.map_batter player, batter

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:handedness] ).to     eq     'S'
    end

    it 'should replace player type with a user friendly value' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      batter = {player_id: 1, power: 6, hit_n_run: 5, bunt: 3, running: 9, range: 7, arm: 3, primary_position: Positions::ThirdBaseman, secondary_position: Positions::LeftFielder}

      result = @player_response_mapper.map_batter player, batter

      expect( result               ).to_not be_nil
      expect( result               ).to     be_a   Hash
      expect( result[:player_type] ).to     eq     'Batter'
    end

    it 'should replace primary and secondary position with a user friendly value' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      batter = {player_id: 1, power: 6, hit_n_run: 5, bunt: 3, running: 9, range: 7, arm: 3, primary_position: Positions::ThirdBaseman, secondary_position: Positions::LeftFielder}

      result = @player_response_mapper.map_batter player, batter

      expect( result                      ).to_not be_nil
      expect( result                      ).to     be_a   Hash
      expect( result[:primary_position  ] ).to     eq     '3B'
      expect( result[:secondary_position] ).to     eq     'LF'
    end

    it 'should adjust the batter ratings if the player has a season' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}
      batter = {player_id: 1, power: 6, hit_n_run: 5, bunt: 3, running: 9, range: 7, arm: 4, primary_position: Positions::ThirdBaseman, secondary_position: Positions::LeftFielder}

      expect( Utils ).to receive( :adjust_rating  ).with 6, 7, 5, 7
      expect( Utils ).to receive( :adjust_rating  ).with 5, 7, 5, 7
      expect( Utils ).to receive( :adjust_rating  ).with 3, 7, 5, 7
      expect( Utils ).to receive( :adjust_rating  ).with 9, 7, 5, 7
      expect( Utils ).to receive( :adjust_rating  ).with 7, 7, 5, 7
      expect( Utils ).to receive( :adjust_rating  ).with 4, 7, 5, 7

      result = @player_response_mapper.map_batter player, batter, 7
    end

    it 'should return nil if given player is not a hash' do
      batter = {player_id: 1, power: 6, hit_n_run: 5, bunt: 3, running: 9, range: 7, arm: 3, primary_position: Positions::ThirdBaseman, secondary_position: Positions::LeftFielder}

      result = @player_response_mapper.map_batter Object.new, batter

      expect( result ).to be_nil
    end

    it 'should return nil if given batter is not a hash' do
      player  = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7, retired_season: 15}

      result = @player_response_mapper.map_batter player, Object.new

      expect( result ).to be_nil
    end
  end

  describe '#map_pitcher_stats' do
    it 'should return a mapped pitcher stats hash' do
      pitcher_stats = {player_id: 1, season: 1, season_phase: Phases::RegularSeason, wins: 22, losses: 13, games: 37, saves: 5, innings: 303, outs: 1, hits: 309, earned_runs: 150, home_runs: 64, walks: 100, strike_outs: 205}

      result = @player_response_mapper.map_pitcher_stats pitcher_stats

      expect( result                 ).to_not be_nil
      expect( result                 ).to     be_a   Hash
      expect( result[:player_id    ] ).to_not be_nil
      expect( result[:season       ] ).to_not be_nil
      expect( result[:season_phase ] ).to_not be_nil
      expect( result[:wins         ] ).to_not be_nil
      expect( result[:losses       ] ).to_not be_nil
      expect( result[:games        ] ).to_not be_nil
      expect( result[:saves        ] ).to_not be_nil
      expect( result[:innings      ] ).to_not be_nil
      expect( result[:outs         ] ).to_not be_nil
      expect( result[:hits         ] ).to_not be_nil
      expect( result[:earned_runs  ] ).to_not be_nil
      expect( result[:home_runs    ] ).to_not be_nil
      expect( result[:walks        ] ).to_not be_nil
      expect( result[:strike_outs  ] ).to_not be_nil
    end

    it 'should replace season phase with a user friendly value' do
      pitcher_stats = {player_id: 1, season: 1, season_phase: Phases::RegularSeason, wins: 22, losses: 13, games: 37, saves: 5, innings: 303, outs: 1, hits: 309, earned_runs: 150, home_runs: 64, walks: 100, strike_outs: 205}

      result = @player_response_mapper.map_pitcher_stats pitcher_stats

      expect( result                ).to_not be_nil
      expect( result                ).to     be_a   Hash
      expect( result[:season_phase] ).to     eq     'Regular'
    end

    it 'should return nil if not given a hash' do
      result = @player_response_mapper.map_pitcher_stats Object.new

      expect( result ).to be_nil
    end
  end

  describe '#map_batter_stats' do
    it 'should return a mapped batter stats hash' do
      batter_stats = {player_id: 1, season: 1, season_phase: Phases::RegularSeason, games: 152, at_bats: 602, runs: 83, hits: 153, doubles: 25, triples: 2, home_runs: 21, runs_batted_in: 77, walks: 74, strike_outs: 95, steals: 22, errors: 5}

      result = @player_response_mapper.map_batter_stats batter_stats

      expect( result                  ).to_not be_nil
      expect( result                  ).to     be_a   Hash
      expect( result[:player_id     ] ).to_not be_nil
      expect( result[:season        ] ).to_not be_nil
      expect( result[:season_phase  ] ).to_not be_nil
      expect( result[:games         ] ).to_not be_nil
      expect( result[:at_bats       ] ).to_not be_nil
      expect( result[:runs          ] ).to_not be_nil
      expect( result[:hits          ] ).to_not be_nil
      expect( result[:doubles       ] ).to_not be_nil
      expect( result[:triples       ] ).to_not be_nil
      expect( result[:home_runs     ] ).to_not be_nil
      expect( result[:runs_batted_in] ).to_not be_nil
      expect( result[:walks         ] ).to_not be_nil
      expect( result[:strike_outs   ] ).to_not be_nil
      expect( result[:steals        ] ).to_not be_nil
      expect( result[:errors        ] ).to_not be_nil
    end

    it 'should replace season phase with a user friendly value' do
      batter_stats = {player_id: 1, season: 1, season_phase: Phases::RegularSeason, games: 152, at_bats: 602, runs: 83, hits: 153, doubles: 25, triples: 2, home_runs: 21, runs_batted_in: 77, walks: 74, strike_outs: 95, steals: 22, errors: 5}

      result = @player_response_mapper.map_batter_stats batter_stats

      expect( result                ).to_not be_nil
      expect( result                ).to     be_a   Hash
      expect( result[:season_phase] ).to     eq     'Regular'
    end

    it 'should return nil if not given a hash' do
      result = @player_response_mapper.map_batter_stats Object.new

      expect( result ).to be_nil
    end
  end

  describe '#map_player_accolade' do
    it 'should return a mapped player accolade hash' do
      player_accolade = {player_id: 1, season: 1, accolade: 4}

      result = @player_response_mapper.map_player_accolade player_accolade

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:player_id] ).to_not be_nil
      expect( result[:season   ] ).to_not be_nil
      expect( result[:accolade ] ).to_not be_nil
    end

    it 'should replace accolade with a user friendly value' do
      player_accolade = {player_id: 1, season: 1, accolade: 4}

      result = @player_response_mapper.map_player_accolade player_accolade

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:accolade ] ).to     be_a   String
    end

    it 'should return nil if not given a hash' do
      result = @player_response_mapper.map_player_accolade Object.new

      expect( result ).to be_nil
    end

    it 'should raise an internal server error if given invalid accolade' do
      player_accolade = {player_id: 1, season: 1, accolade: 99}

      expect { @player_response_mapper.map_player_accolade player_accolade }.to raise_error InternalServerError, 'Invalid accolade of type player with value 99.'
    end
  end

  describe '#map_batter_accolade' do
    it 'should return a mapped batter accolade hash' do
      batter_accolade = {player_id: 1, season: 1, accolade: 4}

      result = @player_response_mapper.map_batter_accolade batter_accolade

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:player_id] ).to_not be_nil
      expect( result[:season   ] ).to_not be_nil
      expect( result[:accolade ] ).to_not be_nil
    end

    it 'should replace accolade with a user friendly value' do
      batter_accolade = {player_id: 1, season: 1, accolade: 4}

      result = @player_response_mapper.map_batter_accolade batter_accolade

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:accolade ] ).to     be_a   String
    end

    it 'should return nil if not given a hash' do
      result = @player_response_mapper.map_batter_accolade Object.new

      expect( result ).to be_nil
    end

    it 'should raise an internal server error if given invalid accolade' do
      batter_accolade = {player_id: 1, season: 1, accolade: 99}

      expect { @player_response_mapper.map_batter_accolade batter_accolade }.to raise_error InternalServerError, 'Invalid accolade of type batting with value 99.'
    end
  end

  describe '#map_pitcher_accolade' do
    it 'should return a mapped pitcher accolade hash' do
      pitcher_accolade = {player_id: 1, season: 1, accolade: 4}

      result = @player_response_mapper.map_pitcher_accolade pitcher_accolade

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:player_id] ).to_not be_nil
      expect( result[:season   ] ).to_not be_nil
      expect( result[:accolade ] ).to_not be_nil
    end

    it 'should replace accolade with a user friendly value' do
      pitcher_accolade = {player_id: 1, season: 1, accolade: 4}

      result = @player_response_mapper.map_pitcher_accolade pitcher_accolade

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:accolade ] ).to     be_a   String
    end

    it 'should return nil if not given a hash' do
      result = @player_response_mapper.map_pitcher_accolade Object.new

      expect( result ).to be_nil
    end

    it 'should map closing accolades' do
      pitcher_accolade = {player_id: 1, season: 1, accolade: 104}

      result = @player_response_mapper.map_pitcher_accolade pitcher_accolade

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:player_id] ).to_not be_nil
      expect( result[:season   ] ).to_not be_nil
      expect( result[:accolade ] ).to_not be_nil
    end

    it 'should raise an internal server error if given invalid pitching accolade' do
      pitcher_accolade = {player_id: 1, season: 1, accolade: 99}

      expect { @player_response_mapper.map_pitcher_accolade pitcher_accolade }.to raise_error InternalServerError, 'Invalid accolade of type pitching with value 99.'
    end

    it 'should raise an internal server error if given invalid closing accolade' do
      pitcher_accolade = {player_id: 1, season: 1, accolade: 199}

      expect { @player_response_mapper.map_pitcher_accolade pitcher_accolade }.to raise_error InternalServerError, 'Invalid accolade of type closing with value 199.'
    end
  end
end
