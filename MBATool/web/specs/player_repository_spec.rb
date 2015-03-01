location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'phases'
require 'player_repository'

describe PlayerRepository do
  before :each do
    @db = SQLite3::Database.new "test_mba.db"

    @db.results_as_hash  = true
    @db.type_translation = true

    @player_repository = PlayerRepository.new @db

    @db.transaction
  end

  after :each do
    @db.rollback
  end

  describe '#get_players_by_team' do
    it 'should return an array containing player info for players on the given team' do
      @db.execute 'insert into team_players_t values (18, 5, 1)'
      @db.execute 'insert into team_players_t values (18, 5, 2)'
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10)"

      result = @player_repository.get_players_by_team 18, 5

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:team_id         ] ).to eq 18
      expect( result[0][:season          ] ).to eq 5
      expect( result[0][:player_id       ] ).to eq 1
      expect( result[0][:first_name      ] ).to eq 'Dave'
      expect( result[0][:last_name       ] ).to eq 'Bot'
      expect( result[0][:first_phoenetic ] ).to eq 'DAY3V'
      expect( result[0][:last_phoenetic  ] ).to eq 'BAA4T'
      expect( result[0][:skin_tone       ] ).to eq SkinTones::Dark
      expect( result[0][:handedness      ] ).to eq Handedness::Left
      expect( result[0][:player_type     ] ).to eq PlayerTypes::Pitcher
      expect( result[0][:rookie_season   ] ).to eq 3
      expect( result[0][:longevity       ] ).to eq 8

      expect( result[1][:team_id         ] ).to eq 18
      expect( result[1][:season          ] ).to eq 5
      expect( result[1][:player_id       ] ).to eq 2
      expect( result[1][:first_name      ] ).to eq 'Jim'
      expect( result[1][:last_name       ] ).to eq 'Poke'
      expect( result[1][:first_phoenetic ] ).to eq 'JIH4M'
      expect( result[1][:last_phoenetic  ] ).to eq 'POW3K'
      expect( result[1][:skin_tone       ] ).to eq SkinTones::Light
      expect( result[1][:handedness      ] ).to eq Handedness::Switch
      expect( result[1][:player_type     ] ).to eq PlayerTypes::Batter
      expect( result[1][:rookie_season   ] ).to eq 4
      expect( result[1][:longevity       ] ).to eq 10
    end

    it 'should only include players on the given team in the given season' do
      @db.execute 'insert into team_players_t values (18, 6, 3)'
      @db.execute 'insert into team_players_t values (18, 5, 1)'
      @db.execute 'insert into team_players_t values (19, 5, 2)'
      @db.execute "insert into players_t values (3, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6)"
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10)"

      result = @player_repository.get_players_by_team 18, 5

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:team_id         ] ).to eq 18
      expect( result[0][:season          ] ).to eq 5
      expect( result[0][:player_id       ] ).to eq 1
      expect( result[0][:first_name      ] ).to eq 'Dave'
      expect( result[0][:last_name       ] ).to eq 'Bot'
      expect( result[0][:first_phoenetic ] ).to eq 'DAY3V'
      expect( result[0][:last_phoenetic  ] ).to eq 'BAA4T'
      expect( result[0][:skin_tone       ] ).to eq SkinTones::Dark
      expect( result[0][:handedness      ] ).to eq Handedness::Left
      expect( result[0][:player_type     ] ).to eq PlayerTypes::Pitcher
      expect( result[0][:rookie_season   ] ).to eq 3
      expect( result[0][:longevity       ] ).to eq 8
    end

    it 'should return multiple seasons if not given a season' do
      @db.execute 'insert into team_players_t values (18, 5, 1)'
      @db.execute 'insert into team_players_t values (18, 5, 3)'
      @db.execute 'insert into team_players_t values (18, 6, 2)'
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark}, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter}, 4, 10)"
      @db.execute "insert into players_t values (3, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter}, 5,  6)"

      result = @player_repository.get_players_by_team 18

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     3

      expect( result[0][:team_id         ] ).to eq 18
      expect( result[0][:season          ] ).to eq 5
      expect( result[0][:player_id       ] ).to eq 1

      expect( result[1][:team_id         ] ).to eq 18
      expect( result[1][:season          ] ).to eq 5
      expect( result[1][:player_id       ] ).to eq 3

      expect( result[2][:team_id         ] ).to eq 18
      expect( result[2][:season          ] ).to eq 6
      expect( result[2][:player_id       ] ).to eq 2
    end

    it 'return an empty array if no players are found' do
      result = @player_repository.get_players_by_team 18

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#get_players' do
    it 'return an array of all players in the database' do
      @db.execute "insert into players_t values (1, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6)"
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8)"
      @db.execute "insert into players_t values (3, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10)"

      result = @player_repository.get_players

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     3

      expect( result[0][:player_id       ] ).to eq 1
      expect( result[0][:first_name      ] ).to eq 'Jake'
      expect( result[0][:last_name       ] ).to eq 'Tom'
      expect( result[0][:first_phoenetic ] ).to eq 'JAY3K'
      expect( result[0][:last_phoenetic  ] ).to eq 'TAA4M'
      expect( result[0][:skin_tone       ] ).to eq SkinTones::Light
      expect( result[0][:handedness      ] ).to eq Handedness::Right
      expect( result[0][:player_type     ] ).to eq PlayerTypes::Batter
      expect( result[0][:rookie_season   ] ).to eq 5
      expect( result[0][:longevity       ] ).to eq 6

      expect( result[1][:player_id       ] ).to eq 2
      expect( result[1][:first_name      ] ).to eq 'Dave'
      expect( result[1][:last_name       ] ).to eq 'Bot'
      expect( result[1][:first_phoenetic ] ).to eq 'DAY3V'
      expect( result[1][:last_phoenetic  ] ).to eq 'BAA4T'
      expect( result[1][:skin_tone       ] ).to eq SkinTones::Dark
      expect( result[1][:handedness      ] ).to eq Handedness::Left
      expect( result[1][:player_type     ] ).to eq PlayerTypes::Pitcher
      expect( result[1][:rookie_season   ] ).to eq 3
      expect( result[1][:longevity       ] ).to eq 8

      expect( result[2][:player_id       ] ).to eq 3
      expect( result[2][:first_name      ] ).to eq 'Jim'
      expect( result[2][:last_name       ] ).to eq 'Poke'
      expect( result[2][:first_phoenetic ] ).to eq 'JIH4M'
      expect( result[2][:last_phoenetic  ] ).to eq 'POW3K'
      expect( result[2][:skin_tone       ] ).to eq SkinTones::Light
      expect( result[2][:handedness      ] ).to eq Handedness::Switch
      expect( result[2][:player_type     ] ).to eq PlayerTypes::Batter
      expect( result[2][:rookie_season   ] ).to eq 4
      expect( result[2][:longevity       ] ).to eq 10
    end

    it 'should return an empty array if there are no players' do
      result = @player_repository.get_players

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#get_players_by_rookie_season' do
    it 'should return an array of players for the given rookie season' do
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 5,  8)"
      @db.execute "insert into players_t values (3, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 5, 10)"

      result = @player_repository.get_players_by_rookie_season 5

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:player_id       ] ).to eq 2
      expect( result[0][:first_name      ] ).to eq 'Dave'
      expect( result[0][:last_name       ] ).to eq 'Bot'
      expect( result[0][:first_phoenetic ] ).to eq 'DAY3V'
      expect( result[0][:last_phoenetic  ] ).to eq 'BAA4T'
      expect( result[0][:skin_tone       ] ).to eq SkinTones::Dark
      expect( result[0][:handedness      ] ).to eq Handedness::Left
      expect( result[0][:player_type     ] ).to eq PlayerTypes::Pitcher
      expect( result[0][:rookie_season   ] ).to eq 5
      expect( result[0][:longevity       ] ).to eq 8

      expect( result[1][:player_id       ] ).to eq 3
      expect( result[1][:first_name      ] ).to eq 'Jim'
      expect( result[1][:last_name       ] ).to eq 'Poke'
      expect( result[1][:first_phoenetic ] ).to eq 'JIH4M'
      expect( result[1][:last_phoenetic  ] ).to eq 'POW3K'
      expect( result[1][:skin_tone       ] ).to eq SkinTones::Light
      expect( result[1][:handedness      ] ).to eq Handedness::Switch
      expect( result[1][:player_type     ] ).to eq PlayerTypes::Batter
      expect( result[1][:rookie_season   ] ).to eq 5
      expect( result[1][:longevity       ] ).to eq 10
    end

    it 'should return only players for the given rookie season' do
      @db.execute "insert into players_t values (1, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6)"
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 5,  8)"
      @db.execute "insert into players_t values (3, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 6, 10)"

      result = @player_repository.get_players_by_rookie_season 5

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:player_id       ] ).to eq 1
      expect( result[0][:first_name      ] ).to eq 'Jake'
      expect( result[0][:last_name       ] ).to eq 'Tom'
      expect( result[0][:first_phoenetic ] ).to eq 'JAY3K'
      expect( result[0][:last_phoenetic  ] ).to eq 'TAA4M'
      expect( result[0][:skin_tone       ] ).to eq SkinTones::Light
      expect( result[0][:handedness      ] ).to eq Handedness::Right
      expect( result[0][:player_type     ] ).to eq PlayerTypes::Batter
      expect( result[0][:rookie_season   ] ).to eq 5
      expect( result[0][:longevity       ] ).to eq 6

      expect( result[1][:player_id       ] ).to eq 2
      expect( result[1][:first_name      ] ).to eq 'Dave'
      expect( result[1][:last_name       ] ).to eq 'Bot'
      expect( result[1][:first_phoenetic ] ).to eq 'DAY3V'
      expect( result[1][:last_phoenetic  ] ).to eq 'BAA4T'
      expect( result[1][:skin_tone       ] ).to eq SkinTones::Dark
      expect( result[1][:handedness      ] ).to eq Handedness::Left
      expect( result[1][:player_type     ] ).to eq PlayerTypes::Pitcher
      expect( result[1][:rookie_season   ] ).to eq 5
      expect( result[1][:longevity       ] ).to eq 8
    end

    it 'should not return players that have been drafted' do
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 5,  8)"
      @db.execute "insert into players_t values (3, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 5, 10)"
      @db.execute 'insert into team_players_t values(1, 5, 3)'

      result = @player_repository.get_players_by_rookie_season 5

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id       ] ).to eq 2
      expect( result[0][:first_name      ] ).to eq 'Dave'
      expect( result[0][:last_name       ] ).to eq 'Bot'
      expect( result[0][:first_phoenetic ] ).to eq 'DAY3V'
      expect( result[0][:last_phoenetic  ] ).to eq 'BAA4T'
      expect( result[0][:skin_tone       ] ).to eq SkinTones::Dark
      expect( result[0][:handedness      ] ).to eq Handedness::Left
      expect( result[0][:player_type     ] ).to eq PlayerTypes::Pitcher
      expect( result[0][:rookie_season   ] ).to eq 5
      expect( result[0][:longevity       ] ).to eq 8
    end

    it 'should return an empty array if there are no players for the given rookie season' do
      @db.execute "insert into players_t values (1, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter}, 5,  6)"
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark}, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 5,  8)"
      @db.execute "insert into players_t values (3, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter}, 6, 10)"

      result = @player_repository.get_players_by_rookie_season 4

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#get_free_agents_by_season' do
    it 'should return an array of free agents in the given season' do
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8)"
      @db.execute "insert into players_t values (3, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10)"

      result = @player_repository.get_free_agents_by_season 2

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:player_id       ] ).to eq 2
      expect( result[0][:first_name      ] ).to eq 'Dave'
      expect( result[0][:last_name       ] ).to eq 'Bot'
      expect( result[0][:first_phoenetic ] ).to eq 'DAY3V'
      expect( result[0][:last_phoenetic  ] ).to eq 'BAA4T'
      expect( result[0][:skin_tone       ] ).to eq SkinTones::Dark
      expect( result[0][:handedness      ] ).to eq Handedness::Left
      expect( result[0][:player_type     ] ).to eq PlayerTypes::Pitcher
      expect( result[0][:rookie_season   ] ).to eq 3
      expect( result[0][:longevity       ] ).to eq 8

      expect( result[1][:player_id       ] ).to eq 3
      expect( result[1][:first_name      ] ).to eq 'Jim'
      expect( result[1][:last_name       ] ).to eq 'Poke'
      expect( result[1][:first_phoenetic ] ).to eq 'JIH4M'
      expect( result[1][:last_phoenetic  ] ).to eq 'POW3K'
      expect( result[1][:skin_tone       ] ).to eq SkinTones::Light
      expect( result[1][:handedness      ] ).to eq Handedness::Switch
      expect( result[1][:player_type     ] ).to eq PlayerTypes::Batter
      expect( result[1][:rookie_season   ] ).to eq 4
      expect( result[1][:longevity       ] ).to eq 10
    end

    it 'should return only free agents' do
      @db.execute 'insert into team_players_t values (18, 5, 1)'
      @db.execute 'insert into team_players_t values (18, 5, 2)'
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10)"
      @db.execute "insert into players_t values (3, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6)"

      result = @player_repository.get_free_agents_by_season 5

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id       ] ).to eq 3
      expect( result[0][:first_name      ] ).to eq 'Jake'
      expect( result[0][:last_name       ] ).to eq 'Tom'
      expect( result[0][:first_phoenetic ] ).to eq 'JAY3K'
      expect( result[0][:last_phoenetic  ] ).to eq 'TAA4M'
      expect( result[0][:skin_tone       ] ).to eq SkinTones::Light
      expect( result[0][:handedness      ] ).to eq Handedness::Right
      expect( result[0][:player_type     ] ).to eq PlayerTypes::Batter
      expect( result[0][:rookie_season   ] ).to eq 5
      expect( result[0][:longevity       ] ).to eq 6
    end

    it 'should return free agents only for the given season' do
      @db.execute 'insert into team_players_t values (18, 4, 1)'
      @db.execute 'insert into team_players_t values (18, 5, 2)'
      @db.execute 'insert into team_players_t values (18, 6, 3)'
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter}, 4, 10)"
      @db.execute "insert into players_t values (3, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter}, 5,  6)"

      result = @player_repository.get_free_agents_by_season 5

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:player_id       ] ).to eq 1
      expect( result[0][:first_name      ] ).to eq 'Dave'
      expect( result[0][:last_name       ] ).to eq 'Bot'
      expect( result[0][:first_phoenetic ] ).to eq 'DAY3V'
      expect( result[0][:last_phoenetic  ] ).to eq 'BAA4T'
      expect( result[0][:skin_tone       ] ).to eq SkinTones::Dark
      expect( result[0][:handedness      ] ).to eq Handedness::Left
      expect( result[0][:player_type     ] ).to eq PlayerTypes::Pitcher
      expect( result[0][:rookie_season   ] ).to eq 3
      expect( result[0][:longevity       ] ).to eq 8

      expect( result[1][:player_id       ] ).to eq 3
      expect( result[1][:first_name      ] ).to eq 'Jake'
      expect( result[1][:last_name       ] ).to eq 'Tom'
      expect( result[1][:first_phoenetic ] ).to eq 'JAY3K'
      expect( result[1][:last_phoenetic  ] ).to eq 'TAA4M'
      expect( result[1][:skin_tone       ] ).to eq SkinTones::Light
      expect( result[1][:handedness      ] ).to eq Handedness::Right
      expect( result[1][:player_type     ] ).to eq PlayerTypes::Batter
      expect( result[1][:rookie_season   ] ).to eq 5
      expect( result[1][:longevity       ] ).to eq 6
    end

    it 'should return an empty array if there are no free agents' do
      @db.execute 'insert into team_players_t values (18, 5, 1)'
      @db.execute 'insert into team_players_t values (18, 5, 2)'
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter}, 4, 10)"

      result = @player_repository.get_free_agents_by_season 5

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#get_player' do
    it 'should return a hash containing player information' do
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark}, #{Handedness::Left}, #{PlayerTypes::Pitcher}, 3,  8)"

      result = @player_repository.get_player 1

      expect( result                    ).to_not be_nil
      expect( result                    ).to     be_a    Hash
      expect( result[:player_id       ] ).to eq 1
      expect( result[:first_name      ] ).to eq 'Dave'
      expect( result[:last_name       ] ).to eq 'Bot'
      expect( result[:first_phoenetic ] ).to eq 'DAY3V'
      expect( result[:last_phoenetic  ] ).to eq 'BAA4T'
      expect( result[:skin_tone       ] ).to eq SkinTones::Dark
      expect( result[:handedness      ] ).to eq Handedness::Left
      expect( result[:player_type     ] ).to eq PlayerTypes::Pitcher
      expect( result[:rookie_season   ] ).to eq 3
      expect( result[:longevity       ] ).to eq 8
    end

    it 'return nil if the player is not found' do
      result = @player_repository.get_player 1

      expect( result ).to be_nil
    end
  end

  describe '#get_pitcher_details' do
    it 'should return a hash containing pitcher information' do
      @db.execute 'insert into pitchers_t values (1, 5, 3, 8, 7)'

      result = @player_repository.get_pitcher_details 1

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a    Hash
      expect( result[:player_id] ).to eq 1
      expect( result[:speed    ] ).to eq 5
      expect( result[:control  ] ).to eq 3
      expect( result[:bunt     ] ).to eq 8
      expect( result[:fatigue  ] ).to eq 7
    end

    it 'return nil if the pitcher is not found' do
      result = @player_repository.get_pitcher_details 1

      expect( result ).to be_nil
    end
  end

  describe '#get_batter_details' do
    it 'should return a hash containing batter information' do
      @db.execute "insert into batters_t values (1, 5, 3, 9, 7, 6, 4, #{Positions::RightFielder}, #{Positions::FirstBaseman})"

      result = @player_repository.get_batter_details 1

      expect( result                       ).to_not be_nil
      expect( result                       ).to     be_a    Hash
      expect( result[:player_id          ] ).to eq 1
      expect( result[:power              ] ).to eq 5
      expect( result[:hit_n_run          ] ).to eq 3
      expect( result[:bunt               ] ).to eq 9
      expect( result[:running            ] ).to eq 7
      expect( result[:range              ] ).to eq 6
      expect( result[:arm                ] ).to eq 4
      expect( result[:primary_position   ] ).to eq Positions::RightFielder
      expect( result[:secondary_position ] ).to eq Positions::FirstBaseman
    end

    it 'return nil if the batter is not found' do
      result = @player_repository.get_batter_details 1

      expect( result ).to be_nil
    end
  end

  describe '#get_pitcher_stats' do
    it 'should return a hash containing pitcher stats information' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 22, 13, 37, 5, 303, 1, 309, 150, 64, 100, 205)"

      result = @player_repository.get_pitcher_stats 1, 1, Phases::RegularSeason

      expect( result                 ).to_not be_nil
      expect( result                 ).to     be_a    Hash
      expect( result[:player_id    ] ).to eq 1
      expect( result[:season       ] ).to eq 1
      expect( result[:season_phase ] ).to eq Phases::RegularSeason
      expect( result[:wins         ] ).to eq 22
      expect( result[:losses       ] ).to eq 13
      expect( result[:games        ] ).to eq 37
      expect( result[:saves        ] ).to eq 5
      expect( result[:innings      ] ).to eq 303
      expect( result[:outs         ] ).to eq 1
      expect( result[:hits         ] ).to eq 309
      expect( result[:earned_runs  ] ).to eq 150
      expect( result[:home_runs    ] ).to eq 64
      expect( result[:walks        ] ).to eq 100
      expect( result[:strike_outs  ] ).to eq 205
    end

    it 'should return multiple phases if not given a phase' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 22, 13, 37, 5, 303, 1, 309, 150, 64, 100, 205)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::Playoff}, 5, 3, 12, 2, 108, 2, 80, 37, 17, 30, 94)"

      result = @player_repository.get_pitcher_stats 1, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:season_phase] ).to eq Phases::RegularSeason
      expect( result[1][:season_phase] ).to eq Phases::Playoff
    end

    it 'should return multiple seasons if not given a season' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::Playoff}, 22, 13, 37, 5, 303, 1, 309, 150, 64, 100, 205)"
      @db.execute "insert into pitcher_stats_t values (1, 2, #{Phases::Playoff}, 5, 3, 12, 2, 108, 2, 80, 37, 17, 30, 94)"

      result = @player_repository.get_pitcher_stats 1, nil, Phases::Playoff

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:season] ).to eq 1
      expect( result[1][:season] ).to eq 2
    end

    it 'should return multiple seasons and phases if only given a team id' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 22, 13, 37, 5, 303, 1, 309, 150, 64, 100, 205)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::Playoff}, 22, 13, 37, 5, 303, 1, 309, 150, 64, 100, 205)"
      @db.execute "insert into pitcher_stats_t values (1, 2, #{Phases::RegularSeason}, 5, 3, 12, 2, 108, 2, 80, 37, 17, 30, 94)"
      @db.execute "insert into pitcher_stats_t values (1, 2, #{Phases::Playoff}, 5, 3, 12, 2, 108, 2, 80, 37, 17, 30, 94)"

      result = @player_repository.get_pitcher_stats 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     4

      expect( result[0][:season      ] ).to eq 1
      expect( result[0][:season_phase] ).to eq Phases::RegularSeason
      expect( result[1][:season      ] ).to eq 1
      expect( result[1][:season_phase] ).to eq Phases::Playoff
      expect( result[2][:season      ] ).to eq 2
      expect( result[2][:season_phase] ).to eq Phases::RegularSeason
      expect( result[3][:season      ] ).to eq 2
      expect( result[3][:season_phase] ).to eq Phases::Playoff
    end

    it 'should return nil if the record is not found' do
      result = @player_repository.get_pitcher_stats 1, 1, Phases::RegularSeason

      expect( result ).to be_nil
    end

    it 'should return an empty array if no records are found' do
      result = @player_repository.get_pitcher_stats 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#get_batter_stats' do
    it 'should return a hash containing batter stats information' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 152, 602, 83, 153, 25, 2, 21, 77, 74, 95, 22, 5)"

      result = @player_repository.get_batter_stats 1, 1, Phases::RegularSeason

      expect( result                   ).to_not be_nil
      expect( result                   ).to     be_a    Hash
      expect( result[:player_id      ] ).to eq 1
      expect( result[:season         ] ).to eq 1
      expect( result[:season_phase   ] ).to eq Phases::RegularSeason
      expect( result[:games          ] ).to eq 152
      expect( result[:at_bats        ] ).to eq 602
      expect( result[:runs           ] ).to eq 83
      expect( result[:hits           ] ).to eq 153
      expect( result[:doubles        ] ).to eq 25
      expect( result[:triples        ] ).to eq 2
      expect( result[:home_runs      ] ).to eq 21
      expect( result[:runs_batted_in ] ).to eq 77
      expect( result[:walks          ] ).to eq 74
      expect( result[:strike_outs    ] ).to eq 95
      expect( result[:steals         ] ).to eq 22
      expect( result[:errors         ] ).to eq 5
    end

    it 'should return multiple phases if not given a phase' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 152, 602, 83, 153, 25, 2, 21, 77, 74, 95, 22, 5)"
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::Playoff}, 9, 13, 1, 3, 0, 0, 1, 3, 2, 1, 0, 0)"

      result = @player_repository.get_batter_stats 1, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:season_phase] ).to eq Phases::RegularSeason
      expect( result[1][:season_phase] ).to eq Phases::Playoff
    end

    it 'should return multiple seasons if not given a season' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::Playoff}, 152, 602, 83, 153, 25, 2, 21, 77, 74, 95, 22, 5)"
      @db.execute "insert into batter_stats_t values (1, 2, #{Phases::Playoff}, 9, 13, 1, 3, 0, 0, 1, 3, 2, 1, 0, 0)"

      result = @player_repository.get_batter_stats 1, nil, Phases::Playoff

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:season] ).to eq 1
      expect( result[1][:season] ).to eq 2
    end

    it 'should return multiple seasons and phases if only given a team id' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 152, 602, 83, 153, 25, 2, 21, 77, 74, 95, 22, 5)"
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::Playoff}, 9, 13, 1, 3, 0, 0, 1, 3, 2, 1, 0, 0)"
      @db.execute "insert into batter_stats_t values (1, 2, #{Phases::RegularSeason}, 152, 602, 83, 153, 25, 2, 21, 77, 74, 95, 22, 5)"
      @db.execute "insert into batter_stats_t values (1, 2, #{Phases::Playoff}, 9, 13, 1, 3, 0, 0, 1, 3, 2, 1, 0, 0)"

      result = @player_repository.get_batter_stats 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     4

      expect( result[0][:season      ] ).to eq 1
      expect( result[0][:season_phase] ).to eq Phases::RegularSeason
      expect( result[1][:season      ] ).to eq 1
      expect( result[1][:season_phase] ).to eq Phases::Playoff
      expect( result[2][:season      ] ).to eq 2
      expect( result[2][:season_phase] ).to eq Phases::RegularSeason
      expect( result[3][:season      ] ).to eq 2
      expect( result[3][:season_phase] ).to eq Phases::Playoff
    end

    it 'should return nil if the record is not found' do
      result = @player_repository.get_batter_stats 1, 1, Phases::RegularSeason

      expect( result ).to be_nil
    end

    it 'should return an empty array if no records are found' do
      result = @player_repository.get_batter_stats 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#save_player' do
    it 'should return an empty array on success' do
      player = {player_id: 1, first_name: 'Dave', last_name: 'Bot', first_phoenetic: 'DAY3V', last_phoenetic: 'BAA4T', skin_tone: SkinTones::Dark, handedness: Handedness::Left, player_type: PlayerTypes::Pitcher, rookie_season: 3, longevity: 8}

      result = @player_repository.save_player player

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should insert a players_t record' do
      player = {player_id: 1, first_name: 'Dave', last_name: 'Bot', first_phoenetic: 'DAY3V', last_phoenetic: 'BAA4T', skin_tone: SkinTones::Dark, handedness: Handedness::Left, player_type: PlayerTypes::Pitcher, rookie_season: 3, longevity: 8}

      @player_repository.save_player player

      result = @db.get_first_row 'select * from players_t where player_id = 1'

      expect( result                    ).to_not be_nil
      expect( result                    ).to     be_a   Hash
      expect( result['Player_Id'      ] ).to     eq     1
      expect( result['First_Name'     ] ).to     eq     'Dave'
      expect( result['Last_Name'      ] ).to     eq     'Bot'
      expect( result['First_Phoenetic'] ).to     eq     'DAY3V'
      expect( result['Last_Phoenetic' ] ).to     eq     'BAA4T'
      expect( result['Skin_Tone'      ] ).to     eq     SkinTones::Dark
      expect( result['Handedness'     ] ).to     eq     Handedness::Left
      expect( result['Player_Type'    ] ).to     eq     PlayerTypes::Pitcher
      expect( result['Rookie_Season'  ] ).to     eq     3
      expect( result['Longevity'      ] ).to     eq     8
    end

    it 'should raise an exception on failure' do
      player = {player_id: 1, first_name: 'Dave', last_name: 'Bot', first_phoenetic: 'DAY3V', last_phoenetic: 'BAA4T', skin_tone: SkinTones::Dark, handedness: Handedness::Left, player_type: PlayerTypes::Pitcher, rookie_season: 3, longevity: 8}

      @player_repository.save_player player

      expect { @player_repository.save_player player }.to raise_error SQLite3::ConstraintException
    end
  end

  describe '#save_pitcher' do
    it 'should return an empty array on success' do
      pitcher = {player_id: 1, speed: 5, control: 3, bunt: 9, fatigue: 7}

      result = @player_repository.save_pitcher pitcher

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should insert a pitchers_t record' do
      pitcher = {player_id: 1, speed: 5, control: 3, bunt: 8, fatigue: 7}

      @player_repository.save_pitcher pitcher

      result = @db.get_first_row 'select * from pitchers_t where player_id = 1'

      expect( result              ).to_not be_nil
      expect( result              ).to     be_a   Hash
      expect( result['Player_Id'] ).to     eq     1
      expect( result['Speed'    ] ).to     eq     5
      expect( result['Control'  ] ).to     eq     3
      expect( result['Bunt'     ] ).to     eq     8
      expect( result['Fatigue'  ] ).to     eq     7
    end

    it 'should raise an exception on failure' do
      pitcher = {player_id: 1, speed: 5, control: 3, bunt: 9, fatigue: 7}

      @player_repository.save_pitcher pitcher

      expect { @player_repository.save_pitcher pitcher }.to raise_error SQLite3::ConstraintException
    end
  end

  describe '#save_batter' do
    it 'should return an empty array on success' do
      batter = {player_id: 1, power: 5, hit_n_run: 3, bunt: 9, running: 7, range: 6, arm: 4, primary_position: Positions::RightFielder, secondary_position: Positions::FirstBaseman}

      result = @player_repository.save_batter batter

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should insert a batters_t record' do
      batter = {player_id: 1, power: 5, hit_n_run: 3, bunt: 9, running: 7, range: 6, arm: 4, primary_position: Positions::RightFielder, secondary_position: Positions::FirstBaseman}

      @player_repository.save_batter batter

      result = @db.get_first_row 'select * from batters_t where player_id = 1'

      expect( result                       ).to_not be_nil
      expect( result                       ).to     be_a   Hash
      expect( result['Player_Id'         ] ).to     eq     1
      expect( result['Power'             ] ).to     eq     5
      expect( result['Hit_N_Run'         ] ).to     eq     3
      expect( result['Bunt'              ] ).to     eq     9
      expect( result['Running'           ] ).to     eq     7
      expect( result['Range'             ] ).to     eq     6
      expect( result['Arm'               ] ).to     eq     4
      expect( result['Primary_Position'  ] ).to     eq     Positions::RightFielder
      expect( result['Secondary_Position'] ).to     eq     Positions::FirstBaseman
    end

    it 'should raise an exception on failure' do
      batter = {player_id: 1, power: 5, hit_n_run: 3, bunt: 9, running: 7, range: 6, arm: 4, primary_position: Positions::RightFielder, secondary_position: Positions::FirstBaseman}

      @player_repository.save_batter batter

      expect { @player_repository.save_batter batter }.to raise_error SQLite3::ConstraintException
    end
  end
end
