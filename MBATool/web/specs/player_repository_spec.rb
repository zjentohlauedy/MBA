location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'phases'
require 'player_types'
require 'skin_tones'
require 'handedness'
require 'positions'
require 'accolades'
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
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8,   12)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"

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
      expect( result[0][:retired_season  ] ).to eq 12

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
      expect( result[1][:retired_season  ] ).to eq nil
    end

    it 'should only include players on the given team in the given season' do
      @db.execute 'insert into team_players_t values (18, 6, 3)'
      @db.execute 'insert into team_players_t values (18, 5, 1)'
      @db.execute 'insert into team_players_t values (19, 5, 2)'
      @db.execute "insert into players_t values (3, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8,   12)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"

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
      expect( result[0][:retired_season  ] ).to eq 12
    end

    it 'should return multiple seasons if not given a season' do
      @db.execute 'insert into team_players_t values (18, 5, 1)'
      @db.execute 'insert into team_players_t values (18, 5, 3)'
      @db.execute 'insert into team_players_t values (18, 6, 2)'
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark}, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8,   12)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter}, 4, 10, null)"
      @db.execute "insert into players_t values (3, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter}, 5,  6, null)"

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
      @db.execute "insert into players_t values (1, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8,   12)"
      @db.execute "insert into players_t values (3, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"

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
      expect( result[0][:retired_season  ] ).to eq nil

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
      expect( result[1][:retired_season  ] ).to eq 12

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
      expect( result[2][:retired_season  ] ).to eq nil
    end

    it 'should return an empty array if there are no players' do
      result = @player_repository.get_players

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#search_players' do
    it 'should return an array of players from the database' do
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8, null)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"
      @db.execute "insert into players_t values (3, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"

      result = @player_repository.search_players nil, nil

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     3

      expect( result[0][:player_id       ] ).to eq 1
      expect( result[0][:first_name      ] ).to eq 'Dave'
      expect( result[0][:last_name       ] ).to eq 'Bot'

      expect( result[1][:player_id       ] ).to eq 2
      expect( result[1][:first_name      ] ).to eq 'Jim'
      expect( result[1][:last_name       ] ).to eq 'Poke'

      expect( result[2][:player_id       ] ).to eq 3
      expect( result[2][:first_name      ] ).to eq 'Jake'
      expect( result[2][:last_name       ] ).to eq 'Tom'
    end

    it 'should return only players matching the given last name prefix if not given first name prefix' do
      @db.execute "insert into players_t values (1, 'Jake', 'Tom', 'JAY3K', 'TAA4M',   #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T',   #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8, null)"
      @db.execute "insert into players_t values (3, 'Jim', 'Tori', 'JIH4M', 'TOH3REY', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"

      result = @player_repository.search_players 'to', nil

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:player_id       ] ).to eq 1
      expect( result[0][:first_name      ] ).to eq 'Jake'
      expect( result[0][:last_name       ] ).to eq 'Tom'

      expect( result[1][:player_id       ] ).to eq 3
      expect( result[1][:first_name      ] ).to eq 'Jim'
      expect( result[1][:last_name       ] ).to eq 'Tori'
    end

    it 'should return only players matching the given first name prefix if not given last name prefix' do
      @db.execute "insert into players_t values (1, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8, null)"
      @db.execute "insert into players_t values (3, 'Dan', 'Poke', 'DAH3N', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"

      result = @player_repository.search_players nil, 'da'

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:player_id       ] ).to eq 2
      expect( result[0][:first_name      ] ).to eq 'Dave'
      expect( result[0][:last_name       ] ).to eq 'Bot'

      expect( result[1][:player_id       ] ).to eq 3
      expect( result[1][:first_name      ] ).to eq 'Dan'
      expect( result[1][:last_name       ] ).to eq 'Poke'
    end

    it 'should return only players matching the given first and last name prefixes if given both' do
      @db.execute "insert into players_t values (1, 'Jeff', 'Jones', 'JEH3F', 'JOW3NZ', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"
      @db.execute "insert into players_t values (2, 'John', 'Jones', 'JAA3N', 'JOW3NZ', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8, null)"
      @db.execute "insert into players_t values (3, 'Joe',  'Jones', 'JOW3',  'JOW3NZ', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"

      result = @player_repository.search_players 'jones', 'joh'

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id       ] ).to eq 2
      expect( result[0][:first_name      ] ).to eq 'John'
      expect( result[0][:last_name       ] ).to eq 'Jones'
    end

    it 'should return players sorted by last name then first name' do
      @db.execute "insert into players_t values (1, 'Dave', 'Jones', 'DAY3V', 'JOW3NZ',   #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"
      @db.execute "insert into players_t values (2, 'Mike', 'Jonas', 'MAY3K', 'JOW3NAES', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8, null)"
      @db.execute "insert into players_t values (3, 'Art',  'Jones', 'AH3RT', 'JOW3NZ',   #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"

      result = @player_repository.search_players 'jon', nil

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     3

      expect( result[0][:player_id       ] ).to eq 2
      expect( result[0][:first_name      ] ).to eq 'Mike'
      expect( result[0][:last_name       ] ).to eq 'Jonas'

      expect( result[1][:player_id       ] ).to eq 3
      expect( result[1][:first_name      ] ).to eq 'Art'
      expect( result[1][:last_name       ] ).to eq 'Jones'

      expect( result[2][:player_id       ] ).to eq 1
      expect( result[2][:first_name      ] ).to eq 'Dave'
      expect( result[2][:last_name       ] ).to eq 'Jones'
    end

    it 'should return at most the number of players given as max if given a max player value' do
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8, null)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"
      @db.execute "insert into players_t values (3, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"
      @db.execute "insert into players_t values (4, 'Mike', 'Van', 'MAY3K', 'VAH4N', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"
      @db.execute "insert into players_t values (5, 'Art', 'Webb', 'AH3RT', 'WEH4B', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"

      result = @player_repository.search_players nil, nil, 3

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     3
    end

    it 'should return no more than 100 players if given max is greater than 100' do
      (1..120).each do |id|
        @db.execute "insert into players_t values (#{id}, 'A', 'A', '', '', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 1,  1, null)"
      end

      result = @player_repository.search_players nil, nil, 120

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     100
    end

    it 'should treat a given max players of less than 1 as not no max given' do
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8, null)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"
      @db.execute "insert into players_t values (3, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"
      @db.execute "insert into players_t values (4, 'Mike', 'Van', 'MAY3K', 'VAH4N', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"
      @db.execute "insert into players_t values (5, 'Art', 'Webb', 'AH3RT', 'WEH4B', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"

      result = @player_repository.search_players nil, nil, 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     5
    end

    it 'should return an empty array if no matching players are found' do
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8, null)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"
      @db.execute "insert into players_t values (3, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"

      result = @player_repository.search_players 'walker', 'mike'

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#get_players_by_rookie_season' do
    it 'should return an array of players for the given rookie season' do
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 5,  8, null)"
      @db.execute "insert into players_t values (3, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 5, 10, null)"

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
      expect( result[0][:retired_season  ] ).to eq nil

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
      expect( result[1][:retired_season  ] ).to eq nil
    end

    it 'should return only players for the given rookie season' do
      @db.execute "insert into players_t values (1, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 5,  8, null)"
      @db.execute "insert into players_t values (3, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 6, 10, null)"

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
      expect( result[0][:retired_season  ] ).to eq nil

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
      expect( result[1][:retired_season  ] ).to eq nil
    end

    it 'should not return players that have been drafted' do
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 5,  8, null)"
      @db.execute "insert into players_t values (3, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 5, 10, null)"
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
      expect( result[0][:retired_season  ] ).to eq nil
    end

    it 'should return an empty array if there are no players for the given rookie season' do
      @db.execute "insert into players_t values (1, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter}, 5,  6, null)"
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark}, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 5,  8, null)"
      @db.execute "insert into players_t values (3, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter}, 6, 10, null)"

      result = @player_repository.get_players_by_rookie_season 4

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#get_free_agents_by_season' do
    it 'should return an array of free agents in the given season' do
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8, null)"
      @db.execute "insert into players_t values (3, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"

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
      expect( result[0][:retired_season  ] ).to eq nil

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
      expect( result[1][:retired_season  ] ).to eq nil
    end

    it 'should return only free agents' do
      @db.execute 'insert into team_players_t values (18, 5, 1)'
      @db.execute 'insert into team_players_t values (18, 5, 2)'
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8, null)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"
      @db.execute "insert into players_t values (3, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"

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
      expect( result[0][:retired_season  ] ).to eq nil
    end

    it 'should return free agents only for the given season' do
      @db.execute 'insert into team_players_t values (18, 4, 1)'
      @db.execute 'insert into team_players_t values (18, 5, 2)'
      @db.execute 'insert into team_players_t values (18, 6, 3)'
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8, null)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"
      @db.execute "insert into players_t values (3, 'Jake', 'Tom', 'JAY3K', 'TAA4M', #{SkinTones::Light}, #{Handedness::Right }, #{PlayerTypes::Batter }, 5,  6, null)"

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
      expect( result[0][:retired_season  ] ).to eq nil

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
      expect( result[1][:retired_season  ] ).to eq nil
    end

    it 'should return an empty array if there are no free agents' do
      @db.execute 'insert into team_players_t values (18, 5, 1)'
      @db.execute 'insert into team_players_t values (18, 5, 2)'
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8, null)"
      @db.execute "insert into players_t values (2, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"

      result = @player_repository.get_free_agents_by_season 5

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should return only free agents that have not retired' do
      @db.execute "insert into players_t values (2, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark }, #{Handedness::Left  }, #{PlayerTypes::Pitcher}, 3,  8,   12)"
      @db.execute "insert into players_t values (3, 'Jim', 'Poke', 'JIH4M', 'POW3K', #{SkinTones::Light}, #{Handedness::Switch}, #{PlayerTypes::Batter }, 4, 10, null)"

      result = @player_repository.get_free_agents_by_season 2

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id       ] ).to eq 3
      expect( result[0][:first_name      ] ).to eq 'Jim'
      expect( result[0][:last_name       ] ).to eq 'Poke'
      expect( result[0][:first_phoenetic ] ).to eq 'JIH4M'
      expect( result[0][:last_phoenetic  ] ).to eq 'POW3K'
      expect( result[0][:skin_tone       ] ).to eq SkinTones::Light
      expect( result[0][:handedness      ] ).to eq Handedness::Switch
      expect( result[0][:player_type     ] ).to eq PlayerTypes::Batter
      expect( result[0][:rookie_season   ] ).to eq 4
      expect( result[0][:longevity       ] ).to eq 10
      expect( result[0][:retired_season  ] ).to eq nil
    end
  end

  describe '#get_player' do
    it 'should return a hash containing player information' do
      @db.execute "insert into players_t values (1, 'Dave', 'Bot', 'DAY3V', 'BAA4T', #{SkinTones::Dark}, #{Handedness::Left}, #{PlayerTypes::Pitcher}, 3,  8, 12)"

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
      expect( result[:retired_season  ] ).to eq 12
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

  describe '#get_pitcher_stats_by_highest' do
    it 'should return an array with at least one entry' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     be     >= 1
    end

    it 'should return the pitcher stats record with the highest value of the given stat' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should return multiple records if there is a tie' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2
    end

    it 'should work for any given stat' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 11, 11, 13, 12, 12, 10, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 1, #{Phases::RegularSeason}, 12, 13, 12, 11, 13, 12, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 1, #{Phases::RegularSeason}, 13, 12, 11, 10, 11, 11, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_pitcher_stats_by_highest 'wins',    'innings', 0)[0][:player_id] ).to eq 4
      expect( (@player_repository.get_pitcher_stats_by_highest 'losses',  'innings', 0)[0][:player_id] ).to eq 3
      expect( (@player_repository.get_pitcher_stats_by_highest 'games',   'innings', 0)[0][:player_id] ).to eq 2
      expect( (@player_repository.get_pitcher_stats_by_highest 'saves',   'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_pitcher_stats_by_highest 'innings', 'innings', 0)[0][:player_id] ).to eq 3
      expect( (@player_repository.get_pitcher_stats_by_highest 'outs',    'innings', 0)[0][:player_id] ).to eq 1
    end

    it 'should work for a calculation of stats' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10,  5, 10,  5, 10,  5, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason},  5, 10,  5, 10,  5, 10, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_pitcher_stats_by_highest 'wins + games',     'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_pitcher_stats_by_highest 'losses - innings', 'innings', 0)[0][:player_id] ).to eq 2
    end

    it 'should work for derived stats' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_pitcher_stats_by_highest 'earned_run_avg', 'innings', 0)[0][:player_id] ).to eq 1
    end

    it 'should only consider the given season' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_pitcher_stats_by_highest 'wins', 'innings', 0, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should only consider the given season and phase' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_pitcher_stats_by_highest 'wins', 'innings', 0, 1, Phases::Playoff

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should consider all records if no season or phase are given' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 3
    end

    it 'should only consider records that meet given qualifying criteria' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 50, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 50, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_pitcher_stats_by_highest 'wins', 'innings', 30

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 4
    end

    it 'should return an empty array if there are no qualifying stats records' do
      result = @player_repository.get_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should raise an error if an invalid stat is given' do
      expect { @player_repository.get_pitcher_stats_by_highest 'invalid', 'innings', 0 }.to raise_error SQLite3::SQLException
    end
  end

  describe '#get_starting_pitcher_stats_by_highest' do
    it 'should return an array with at least one entry' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     be     >= 1
    end

    it 'should return the pitcher stats record with the highest value of the given stat' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should return multiple records if there is a tie' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2
    end

    it 'should work for any given stat' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 11, 11, 13, 12, 12, 10, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 1, #{Phases::RegularSeason}, 12, 13, 12, 11, 13, 12, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 1, #{Phases::RegularSeason}, 13, 12, 11, 10, 11, 11, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_starting_pitcher_stats_by_highest 'wins',    'innings', 0)[0][:player_id] ).to eq 4
      expect( (@player_repository.get_starting_pitcher_stats_by_highest 'losses',  'innings', 0)[0][:player_id] ).to eq 3
      expect( (@player_repository.get_starting_pitcher_stats_by_highest 'games',   'innings', 0)[0][:player_id] ).to eq 2
      expect( (@player_repository.get_starting_pitcher_stats_by_highest 'saves',   'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_starting_pitcher_stats_by_highest 'innings', 'innings', 0)[0][:player_id] ).to eq 3
      expect( (@player_repository.get_starting_pitcher_stats_by_highest 'outs',    'innings', 0)[0][:player_id] ).to eq 1
    end

    it 'should work for a calculation of stats' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10,  5, 10,  5, 10,  5, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason},  5, 10,  5, 10,  5, 10, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_starting_pitcher_stats_by_highest 'wins + games',     'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_starting_pitcher_stats_by_highest 'losses - innings', 'innings', 0)[0][:player_id] ).to eq 2
    end

    it 'should work for derived stats' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_starting_pitcher_stats_by_highest 'earned_run_avg', 'innings', 0)[0][:player_id] ).to eq 1
    end

    it 'should only consider the given season' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_highest 'wins', 'innings', 0, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should only consider the given season and phase' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_highest 'wins', 'innings', 0, 1, Phases::Playoff

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should consider all records if no season or phase are given' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 3
    end

    it 'should only consider records that meet given qualifying criteria' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 50, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 50, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_highest 'wins', 'innings', 30

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 4
    end

    it 'should only consider starting pitchers' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 1, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_highest 'wins', 'innings', 0, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 4
    end

    it 'should return an empty array if there are no qualifying stats records' do
      result = @player_repository.get_starting_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should raise an error if an invalid stat is given' do
      expect { @player_repository.get_starting_pitcher_stats_by_highest 'invalid', 'innings', 0 }.to raise_error SQLite3::SQLException
    end
  end

  describe '#get_closing_pitcher_stats_by_highest' do
    it 'should return an array with at least one entry' do
      @db.execute "insert into pitchers_t values (1, 7, 7, 7, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     be     >= 1
    end

    it 'should return the pitcher stats record with the highest value of the given stat' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should return multiple records if there is a tie' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2
    end

    it 'should work for any given stat' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 11, 11, 13, 12, 12, 10, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 1, #{Phases::RegularSeason}, 12, 13, 12, 11, 13, 12, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 1, #{Phases::RegularSeason}, 13, 12, 11, 10, 11, 11, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_closing_pitcher_stats_by_highest 'wins',    'innings', 0)[0][:player_id] ).to eq 4
      expect( (@player_repository.get_closing_pitcher_stats_by_highest 'losses',  'innings', 0)[0][:player_id] ).to eq 3
      expect( (@player_repository.get_closing_pitcher_stats_by_highest 'games',   'innings', 0)[0][:player_id] ).to eq 2
      expect( (@player_repository.get_closing_pitcher_stats_by_highest 'saves',   'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_closing_pitcher_stats_by_highest 'innings', 'innings', 0)[0][:player_id] ).to eq 3
      expect( (@player_repository.get_closing_pitcher_stats_by_highest 'outs',    'innings', 0)[0][:player_id] ).to eq 1
    end

    it 'should work for a calculation of stats' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10,  5, 10,  5, 10,  5, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason},  5, 10,  5, 10,  5, 10, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_closing_pitcher_stats_by_highest 'wins + games',     'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_closing_pitcher_stats_by_highest 'losses - innings', 'innings', 0)[0][:player_id] ).to eq 2
    end

    it 'should work for derived stats' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_closing_pitcher_stats_by_highest 'earned_run_avg', 'innings', 0)[0][:player_id] ).to eq 1
    end

    it 'should only consider the given season' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_highest 'wins', 'innings', 0, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should only consider the given season and phase' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_highest 'wins', 'innings', 0, 1, Phases::Playoff

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should consider all records if no season or phase are given' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 3
    end

    it 'should only consider records that meet given qualifying criteria' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 50, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 50, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_highest 'wins', 'innings', 30

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 4
    end

    it 'should only consider closing pitchers' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 1, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_highest 'wins', 'innings', 0, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 4
    end

    it 'should return an empty array if there are no qualifying stats records' do
      result = @player_repository.get_closing_pitcher_stats_by_highest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should raise an error if an invalid stat is given' do
      expect { @player_repository.get_closing_pitcher_stats_by_highest 'invalid', 'innings', 0 }.to raise_error SQLite3::SQLException
    end
  end

  describe '#get_pitcher_stats_by_lowest' do
    it 'should return an array with at least one entry' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     be     >= 1
    end

    it 'should return the pitcher stats record with the lowest value of the given stat' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
    end

    it 'should return multiple records if there is a tie' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2
    end

    it 'should work for any given stat' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 11, 11, 13, 12, 12, 10, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 1, #{Phases::RegularSeason}, 12, 13, 12, 11, 13, 12, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 1, #{Phases::RegularSeason}, 13, 12, 11, 10, 11, 11, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_pitcher_stats_by_lowest 'wins',    'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_pitcher_stats_by_lowest 'losses',  'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_pitcher_stats_by_lowest 'games',   'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_pitcher_stats_by_lowest 'saves',   'innings', 0)[0][:player_id] ).to eq 4
      expect( (@player_repository.get_pitcher_stats_by_lowest 'innings', 'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_pitcher_stats_by_lowest 'outs',    'innings', 0)[0][:player_id] ).to eq 2
    end

    it 'should work for a calculation of stats' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10,  5, 10,  5, 10,  5, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason},  5, 10,  5, 10,  5, 10, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_pitcher_stats_by_lowest 'wins + games',     'innings', 0)[0][:player_id] ).to eq 2
      expect( (@player_repository.get_pitcher_stats_by_lowest 'losses - innings', 'innings', 0)[0][:player_id] ).to eq 1
    end

    it 'should work for derived stats' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_pitcher_stats_by_lowest 'earned_run_avg', 'innings', 0)[0][:player_id] ).to eq 1
    end

    it 'should only consider the given season' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_pitcher_stats_by_lowest 'wins', 'innings', 0, 2

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 4
    end

    it 'should only consider the given season and phase' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_pitcher_stats_by_lowest 'wins', 'innings', 0, 1, Phases::RegularSeason

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
    end

    it 'should consider all records if no season or phase are given' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should only consider records that meet given qualifying criteria' do
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 50, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 50, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_pitcher_stats_by_lowest 'wins', 'innings', 30

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
    end

    it 'should return an empty array if there are no qualifying stats records' do
      result = @player_repository.get_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should raise an error if an invalid stat is given' do
      expect { @player_repository.get_pitcher_stats_by_lowest 'invalid', 'innings', 0 }.to raise_error SQLite3::SQLException
    end
  end

  describe '#get_starting_pitcher_stats_by_lowest' do
    it 'should return an array with at least one entry' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     be     >= 1
    end

    it 'should return the pitcher stats record with the lowest value of the given stat' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
    end

    it 'should return multiple records if there is a tie' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2
    end

    it 'should work for any given stat' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 11, 11, 13, 12, 12, 10, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 1, #{Phases::RegularSeason}, 12, 13, 12, 11, 13, 12, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 1, #{Phases::RegularSeason}, 13, 12, 11, 10, 11, 11, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_starting_pitcher_stats_by_lowest 'wins',    'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_starting_pitcher_stats_by_lowest 'losses',  'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_starting_pitcher_stats_by_lowest 'games',   'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_starting_pitcher_stats_by_lowest 'saves',   'innings', 0)[0][:player_id] ).to eq 4
      expect( (@player_repository.get_starting_pitcher_stats_by_lowest 'innings', 'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_starting_pitcher_stats_by_lowest 'outs',    'innings', 0)[0][:player_id] ).to eq 2
    end

    it 'should work for a calculation of stats' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10,  5, 10,  5, 10,  5, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason},  5, 10,  5, 10,  5, 10, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_starting_pitcher_stats_by_lowest 'wins + games',     'innings', 0)[0][:player_id] ).to eq 2
      expect( (@player_repository.get_starting_pitcher_stats_by_lowest 'losses - innings', 'innings', 0)[0][:player_id] ).to eq 1
    end

    it 'should work for derived stats' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_starting_pitcher_stats_by_lowest 'earned_run_avg', 'innings', 0)[0][:player_id] ).to eq 1
    end

    it 'should only consider the given season' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_lowest 'wins', 'innings', 0, 2

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 4
    end

    it 'should only consider the given season and phase' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_lowest 'wins', 'innings', 0, 1, Phases::RegularSeason

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
    end

    it 'should consider all records if no season or phase are given' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should only consider records that meet given qualifying criteria' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 50, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 50, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_lowest 'wins', 'innings', 30

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
    end

    it 'should only consider starting pitchers' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 7)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_starting_pitcher_stats_by_lowest 'wins', 'innings', 0, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 4
    end

    it 'should return an empty array if there are no qualifying stats records' do
      result = @player_repository.get_starting_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should raise an error if an invalid stat is given' do
      expect { @player_repository.get_starting_pitcher_stats_by_lowest 'invalid', 'innings', 0 }.to raise_error SQLite3::SQLException
    end
  end

  describe '#get_closing_pitcher_stats_by_lowest' do
    it 'should return an array with at least one entry' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     be     >= 1
    end

    it 'should return the pitcher stats record with the lowest value of the given stat' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
    end

    it 'should return multiple records if there is a tie' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2
    end

    it 'should work for any given stat' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 11, 11, 13, 12, 12, 10, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 1, #{Phases::RegularSeason}, 12, 13, 12, 11, 13, 12, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 1, #{Phases::RegularSeason}, 13, 12, 11, 10, 11, 11, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_closing_pitcher_stats_by_lowest 'wins',    'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_closing_pitcher_stats_by_lowest 'losses',  'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_closing_pitcher_stats_by_lowest 'games',   'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_closing_pitcher_stats_by_lowest 'saves',   'innings', 0)[0][:player_id] ).to eq 4
      expect( (@player_repository.get_closing_pitcher_stats_by_lowest 'innings', 'innings', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_closing_pitcher_stats_by_lowest 'outs',    'innings', 0)[0][:player_id] ).to eq 2
    end

    it 'should work for a calculation of stats' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10,  5, 10,  5, 10,  5, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason},  5, 10,  5, 10,  5, 10, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_closing_pitcher_stats_by_lowest 'wins + games',     'innings', 0)[0][:player_id] ).to eq 2
      expect( (@player_repository.get_closing_pitcher_stats_by_lowest 'losses - innings', 'innings', 0)[0][:player_id] ).to eq 1
    end

    it 'should work for derived stats' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_closing_pitcher_stats_by_lowest 'earned_run_avg', 'innings', 0)[0][:player_id] ).to eq 1
    end

    it 'should only consider the given season' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_lowest 'wins', 'innings', 0, 2

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 4
    end

    it 'should only consider the given season and phase' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_lowest 'wins', 'innings', 0, 1, Phases::RegularSeason

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
    end

    it 'should consider all records if no season or phase are given' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should only consider records that meet given qualifying criteria' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 50, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 50, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_lowest 'wins', 'innings', 30

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
    end

    it 'should only consider starting pitchers' do
      @db.execute "insert into pitchers_t values (1, 5, 5, 5, 1)"
      @db.execute "insert into pitchers_t values (2, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (3, 5, 5, 5, 7)"
      @db.execute "insert into pitchers_t values (4, 5, 5, 5, 1)"
      @db.execute "insert into pitcher_stats_t values (1, 1, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (3, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into pitcher_stats_t values (4, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_closing_pitcher_stats_by_lowest 'wins', 'innings', 0, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 4
    end

    it 'should return an empty array if there are no qualifying stats records' do
      result = @player_repository.get_closing_pitcher_stats_by_lowest 'wins', 'innings', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should raise an error if an invalid stat is given' do
      expect { @player_repository.get_closing_pitcher_stats_by_lowest 'invalid', 'innings', 0 }.to raise_error SQLite3::SQLException
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

  describe '#get_batter_stats_by_highest' do
    it 'should return an array with at least one entry' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_batter_stats_by_highest 'games', 'at_bats', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     be     >= 1
    end

    it 'should return the batter stats record with the highest value of the given stat' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_batter_stats_by_highest 'games', 'at_bats', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should return multiple records if there is a tie' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_batter_stats_by_highest 'games', 'at_bats', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2
    end

    it 'should work for any given stat' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::RegularSeason}, 11, 11, 13, 12, 12, 10, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (3, 1, #{Phases::RegularSeason}, 12, 13, 12, 11, 13, 12, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (4, 1, #{Phases::RegularSeason}, 13, 12, 11, 10, 11, 11, 1, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_batter_stats_by_highest 'games',   'at_bats', 0)[0][:player_id] ).to eq 4
      expect( (@player_repository.get_batter_stats_by_highest 'at_bats', 'at_bats', 0)[0][:player_id] ).to eq 3
      expect( (@player_repository.get_batter_stats_by_highest 'runs',    'at_bats', 0)[0][:player_id] ).to eq 2
      expect( (@player_repository.get_batter_stats_by_highest 'hits',    'at_bats', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_batter_stats_by_highest 'doubles', 'at_bats', 0)[0][:player_id] ).to eq 3
      expect( (@player_repository.get_batter_stats_by_highest 'triples', 'at_bats', 0)[0][:player_id] ).to eq 1
    end

    it 'should work for a calculation of stats' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10,  5, 10,  5, 10,  5, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::RegularSeason},  5, 10,  5, 10,  5, 10, 1, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_batter_stats_by_highest 'games + runs',      'at_bats', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_batter_stats_by_highest 'at_bats - doubles', 'at_bats', 0)[0][:player_id] ).to eq 2
    end

    it 'should work for derived stats' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_batter_stats_by_highest 'batting_average', 'at_bats', 0)[0][:player_id] ).to eq 1
    end

    it 'should only consider the given season' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (4, 2, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_batter_stats_by_highest 'games', 'at_bats', 0, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should only consider the given season and phase' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_batter_stats_by_highest 'games', 'at_bats', 0, 1, Phases::Playoff

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should consider all records if no season or phase are given' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_batter_stats_by_highest 'games', 'at_bats', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 3
    end

    it 'should only consider records that meet given qualifying criteria' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::Playoff},        2, 50, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (4, 2, #{Phases::Playoff},        4, 50, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_batter_stats_by_highest 'games', 'at_bats', 30

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 4
    end

    it 'should return an empty array if there are no qualifying stats records' do
      result = @player_repository.get_batter_stats_by_highest 'games', 'at_bats', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should raise an error if an invalid stat is given' do
      expect { @player_repository.get_batter_stats_by_highest 'invalid', 'at_bats', 0 }.to raise_error SQLite3::SQLException
    end
  end

  describe '#get_batter_stats_by_lowest' do
    it 'should return an array with at least one entry' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_batter_stats_by_lowest 'games', 'at_bats', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     be     >= 1
    end

    it 'should return the batter stats record with the lowest value of the given stat' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_batter_stats_by_lowest 'games', 'at_bats', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
    end

    it 'should return multiple records if there is a tie' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_batter_stats_by_lowest 'games', 'at_bats', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2
    end

    it 'should work for any given stat' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::RegularSeason}, 11, 11, 13, 12, 12, 10, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (3, 1, #{Phases::RegularSeason}, 12, 13, 12, 11, 13, 12, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (4, 1, #{Phases::RegularSeason}, 13, 12, 11, 10, 11, 11, 1, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_batter_stats_by_lowest 'games',   'at_bats', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_batter_stats_by_lowest 'at_bats', 'at_bats', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_batter_stats_by_lowest 'runs',    'at_bats', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_batter_stats_by_lowest 'hits',    'at_bats', 0)[0][:player_id] ).to eq 4
      expect( (@player_repository.get_batter_stats_by_lowest 'doubles', 'at_bats', 0)[0][:player_id] ).to eq 1
      expect( (@player_repository.get_batter_stats_by_lowest 'triples', 'at_bats', 0)[0][:player_id] ).to eq 2
    end

    it 'should work for a calculation of stats' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10,  5, 10,  5, 10,  5, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::RegularSeason},  5, 10,  5, 10,  5, 10, 1, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_batter_stats_by_lowest 'games + runs',      'at_bats', 0)[0][:player_id] ).to eq 2
      expect( (@player_repository.get_batter_stats_by_lowest 'at_bats - doubles', 'at_bats', 0)[0][:player_id] ).to eq 1
    end

    it 'should work for derived stats' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 10, 10, 13, 10, 13, 1, 1, 1, 1, 1, 1)"

      expect( (@player_repository.get_batter_stats_by_lowest 'batting_average', 'at_bats', 0)[0][:player_id] ).to eq 1
    end

    it 'should only consider the given season' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (4, 2, #{Phases::RegularSeason}, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_batter_stats_by_lowest 'games', 'at_bats', 0, 2

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 4
    end

    it 'should only consider the given season and phase' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_batter_stats_by_lowest 'games', 'at_bats', 0, 1, Phases::RegularSeason

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
    end

    it 'should consider all records if no season or phase are given' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::Playoff},        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (4, 2, #{Phases::Playoff},        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_batter_stats_by_lowest 'games', 'at_bats', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 2
    end

    it 'should only consider records that meet given qualifying criteria' do
      @db.execute "insert into batter_stats_t values (1, 1, #{Phases::RegularSeason}, 10, 50, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (2, 1, #{Phases::Playoff},        2, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (3, 2, #{Phases::RegularSeason}, 20, 50, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"
      @db.execute "insert into batter_stats_t values (4, 2, #{Phases::Playoff},        4, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)"

      result = @player_repository.get_batter_stats_by_lowest 'games', 'at_bats', 30

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
    end

    it 'should return an empty array if there are no qualifying stats records' do
      result = @player_repository.get_batter_stats_by_lowest 'games', 'at_bats', 0

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should raise an error if an invalid stat is given' do
      expect { @player_repository.get_batter_stats_by_lowest 'invalid', 'at_bats', 0 }.to raise_error SQLite3::SQLException
    end
  end

  describe '#get_player_accolades_by_season' do
    it 'should return an array containing player accolades information' do
      @db.execute "insert into player_accolades_t values (1, 1, #{Accolades::Player::League_Title})"

      result = @player_repository.get_player_accolades_by_season 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Player::League_Title
    end

    it 'should return only records matching given season' do
      @db.execute "insert into player_accolades_t values (1, 1, #{Accolades::Player::League_Title})"
      @db.execute "insert into player_accolades_t values (1, 2, #{Accolades::Player::World_Title})"

      result = @player_repository.get_player_accolades_by_season 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Player::League_Title
    end

    it 'should return records for multiple players' do
      @db.execute "insert into player_accolades_t values (1, 1, #{Accolades::Player::League_Title})"
      @db.execute "insert into player_accolades_t values (2, 1, #{Accolades::Player::World_Title})"

      result = @player_repository.get_player_accolades_by_season 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Player::League_Title

      expect( result[1][:player_id] ).to eq 2
      expect( result[1][:season   ] ).to eq 1
      expect( result[1][:accolade ] ).to eq Accolades::Player::World_Title
    end

    it 'should return an empty array if no records are found' do
      result = @player_repository.get_player_accolades_by_season 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#get_player_accolades' do
    it 'should return an array containing player accolades information' do
      @db.execute "insert into player_accolades_t values (1, 1, #{Accolades::Player::League_Title})"

      result = @player_repository.get_player_accolades 1, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Player::League_Title
    end

    it 'should return only records matching given season' do
      @db.execute "insert into player_accolades_t values (1, 1, #{Accolades::Player::League_Title})"
      @db.execute "insert into player_accolades_t values (1, 2, #{Accolades::Player::League_Title})"

      result = @player_repository.get_player_accolades 1, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Player::League_Title
    end

    it 'should return multiple seasons if not given a season' do
      @db.execute "insert into player_accolades_t values (1, 1, #{Accolades::Player::League_Title})"
      @db.execute "insert into player_accolades_t values (1, 2, #{Accolades::Player::League_Title})"

      result = @player_repository.get_player_accolades 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Player::League_Title

      expect( result[1][:player_id] ).to eq 1
      expect( result[1][:season   ] ).to eq 2
      expect( result[1][:accolade ] ).to eq Accolades::Player::League_Title
    end

    it 'should return an empty array if no records are found' do
      result = @player_repository.get_player_accolades 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#get_player_accolade' do
    it 'should return a hash containing player accolade information' do
      @db.execute "insert into player_accolades_t values (1, 1, #{Accolades::Player::Division_Title})"

      result = @player_repository.get_player_accolade 1, 1, Accolades::Player::Division_Title

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a    Hash
      expect( result[:player_id] ).to eq 1
      expect( result[:season   ] ).to eq 1
      expect( result[:accolade ] ).to eq Accolades::Player::Division_Title
    end

    it 'return nil if the player accolade is not found' do
      result = @player_repository.get_player_accolade 1, 1, Accolades::Player::Division_Title

      expect( result ).to be_nil
    end
  end

  describe '#get_pitcher_accolades_by_season' do
    it 'should return an array containing pitcher accolades information' do
      @db.execute "insert into pitcher_accolades_t values (1, 1, #{Accolades::Pitching::Best_Earned_Run_Average})"

      result = @player_repository.get_pitcher_accolades_by_season 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Pitching::Best_Earned_Run_Average
    end

    it 'should return only records matching given season' do
      @db.execute "insert into pitcher_accolades_t values (1, 1, #{Accolades::Pitching::Best_Earned_Run_Average})"
      @db.execute "insert into pitcher_accolades_t values (1, 2, #{Accolades::Pitching::Most_Wins})"

      result = @player_repository.get_pitcher_accolades_by_season 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Pitching::Best_Earned_Run_Average
    end

    it 'should return records for multiple players' do
      @db.execute "insert into pitcher_accolades_t values (1, 1, #{Accolades::Pitching::Best_Earned_Run_Average})"
      @db.execute "insert into pitcher_accolades_t values (2, 1, #{Accolades::Pitching::Most_Wins})"

      result = @player_repository.get_pitcher_accolades_by_season 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Pitching::Best_Earned_Run_Average

      expect( result[1][:player_id] ).to eq 2
      expect( result[1][:season   ] ).to eq 1
      expect( result[1][:accolade ] ).to eq Accolades::Pitching::Most_Wins
    end

    it 'should return an empty array if no records are found' do
      result = @player_repository.get_pitcher_accolades_by_season 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#get_pitcher_accolades' do
    it 'should return an array containing pitcher accolades information' do
      @db.execute "insert into pitcher_accolades_t values (1, 1, #{Accolades::Pitching::Best_Earned_Run_Average})"

      result = @player_repository.get_pitcher_accolades 1, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Pitching::Best_Earned_Run_Average
    end

    it 'should return only records matching given season' do
      @db.execute "insert into pitcher_accolades_t values (1, 1, #{Accolades::Pitching::Best_Earned_Run_Average})"
      @db.execute "insert into pitcher_accolades_t values (1, 2, #{Accolades::Pitching::Best_Earned_Run_Average})"

      result = @player_repository.get_pitcher_accolades 1, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Pitching::Best_Earned_Run_Average
    end

    it 'should return multiple seasons if not given a season' do
      @db.execute "insert into pitcher_accolades_t values (1, 1, #{Accolades::Pitching::Best_Earned_Run_Average})"
      @db.execute "insert into pitcher_accolades_t values (1, 2, #{Accolades::Pitching::Best_Earned_Run_Average})"

      result = @player_repository.get_pitcher_accolades 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Pitching::Best_Earned_Run_Average

      expect( result[1][:player_id] ).to eq 1
      expect( result[1][:season   ] ).to eq 2
      expect( result[1][:accolade ] ).to eq Accolades::Pitching::Best_Earned_Run_Average
    end

    it 'should return an empty array if no records are found' do
      result = @player_repository.get_pitcher_accolades 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#get_pitcher_accolade' do
    it 'should return a hash containing pitcher accolade information' do
      @db.execute "insert into pitcher_accolades_t values (1, 1, #{Accolades::Pitching::Best_Earned_Run_Average})"

      result = @player_repository.get_pitcher_accolade 1, 1, Accolades::Pitching::Best_Earned_Run_Average

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a    Hash
      expect( result[:player_id] ).to eq 1
      expect( result[:season   ] ).to eq 1
      expect( result[:accolade ] ).to eq Accolades::Pitching::Best_Earned_Run_Average
    end

    it 'return nil if the pitcher accolade is not found' do
      result = @player_repository.get_pitcher_accolade 1, 1, Accolades::Pitching::Best_Earned_Run_Average

      expect( result ).to be_nil
    end
  end

  describe '#get_batter_accolades_by_season' do
    it 'should return an array containing batter accolades information' do
      @db.execute "insert into batter_accolades_t values (1, 1, #{Accolades::Batting::Best_Batting_Average})"

      result = @player_repository.get_batter_accolades_by_season 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Batting::Best_Batting_Average
    end

    it 'should return only records matching given season' do
      @db.execute "insert into batter_accolades_t values (1, 1, #{Accolades::Batting::Best_Batting_Average})"
      @db.execute "insert into batter_accolades_t values (1, 2, #{Accolades::Batting::Most_Home_Runs})"

      result = @player_repository.get_batter_accolades_by_season 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Batting::Best_Batting_Average
    end

    it 'should return records for multiple players' do
      @db.execute "insert into batter_accolades_t values (1, 1, #{Accolades::Batting::Best_Batting_Average})"
      @db.execute "insert into batter_accolades_t values (2, 1, #{Accolades::Batting::Most_Home_Runs})"

      result = @player_repository.get_batter_accolades_by_season 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Batting::Best_Batting_Average

      expect( result[1][:player_id] ).to eq 2
      expect( result[1][:season   ] ).to eq 1
      expect( result[1][:accolade ] ).to eq Accolades::Batting::Most_Home_Runs
    end

    it 'should return an empty array if no records are found' do
      result = @player_repository.get_batter_accolades_by_season 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#get_batter_accolades' do
    it 'should return an array containing batter accolades information' do
      @db.execute "insert into batter_accolades_t values (1, 1, #{Accolades::Batting::Most_Home_Runs})"

      result = @player_repository.get_batter_accolades 1, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Batting::Most_Home_Runs
    end

    it 'should return only records matching given season' do
      @db.execute "insert into batter_accolades_t values (1, 1, #{Accolades::Batting::Most_Home_Runs})"
      @db.execute "insert into batter_accolades_t values (1, 2, #{Accolades::Batting::Most_Home_Runs})"

      result = @player_repository.get_batter_accolades 1, 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     1

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Batting::Most_Home_Runs
    end

    it 'should return multiple seasons if not given a season' do
      @db.execute "insert into batter_accolades_t values (1, 1, #{Accolades::Batting::Most_Home_Runs})"
      @db.execute "insert into batter_accolades_t values (1, 2, #{Accolades::Batting::Most_Home_Runs})"

      result = @player_repository.get_batter_accolades 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     2

      expect( result[0][:player_id] ).to eq 1
      expect( result[0][:season   ] ).to eq 1
      expect( result[0][:accolade ] ).to eq Accolades::Batting::Most_Home_Runs

      expect( result[1][:player_id] ).to eq 1
      expect( result[1][:season   ] ).to eq 2
      expect( result[1][:accolade ] ).to eq Accolades::Batting::Most_Home_Runs
    end

    it 'should return an empty array if no records are found' do
      result = @player_repository.get_batter_accolades 1

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end
  end

  describe '#get_batter_accolade' do
    it 'should return a hash containing batter accolade information' do
      @db.execute "insert into batter_accolades_t values (1, 1, #{Accolades::Batting::Most_Doubles})"

      result = @player_repository.get_batter_accolade 1, 1, Accolades::Batting::Most_Doubles

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a    Hash
      expect( result[:player_id] ).to eq 1
      expect( result[:season   ] ).to eq 1
      expect( result[:accolade ] ).to eq Accolades::Batting::Most_Doubles
    end

    it 'return nil if the batter accolade is not found' do
      result = @player_repository.get_batter_accolade 1, 1, Accolades::Batting::Most_Doubles

      expect( result ).to be_nil
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
      expect( result['Retired_Season' ] ).to     eq     nil
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

  describe '#save_player_accolade' do
    it 'should return an empty array on success' do
      player_accolade = {player_id: 1, season: 3, accolade: Accolades::Player::Division_Title}

      result = @player_repository.save_player_accolade player_accolade

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should insert a player_accolades_t record' do
      player_accolade = {player_id: 1, season: 3, accolade: Accolades::Player::Division_Title}

      @player_repository.save_player_accolade player_accolade

      result = @db.get_first_row "select * from player_accolades_t where player_id = 1 and season = 3 and accolade = #{Accolades::Player::Division_Title}"

      expect( result              ).to_not be_nil
      expect( result              ).to     be_a   Hash
      expect( result['Player_Id'] ).to     eq     1
      expect( result['Season'   ] ).to     eq     3
      expect( result['Accolade' ] ).to     eq     Accolades::Player::Division_Title
    end

    it 'should be idempotent' do
      player_accolade = {player_id: 1, season: 3, accolade: Accolades::Player::Division_Title}

      @player_repository.save_player_accolade player_accolade
      @player_repository.save_player_accolade player_accolade
    end
  end

  describe '#save_pitcher_accolade' do
    it 'should return an empty array on success' do
      pitcher_accolade = {player_id: 1, season: 3, accolade: Accolades::Pitching::Most_Saves}

      result = @player_repository.save_pitcher_accolade pitcher_accolade

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should insert a pitcher_accolades_t record' do
      pitcher_accolade = {player_id: 1, season: 3, accolade: Accolades::Pitching::Most_Saves}

      @player_repository.save_pitcher_accolade pitcher_accolade

      result = @db.get_first_row "select * from pitcher_accolades_t where player_id = 1 and season = 3 and accolade = #{Accolades::Pitching::Most_Saves}"

      expect( result              ).to_not be_nil
      expect( result              ).to     be_a   Hash
      expect( result['Player_Id'] ).to     eq     1
      expect( result['Season'   ] ).to     eq     3
      expect( result['Accolade' ] ).to     eq     Accolades::Pitching::Most_Saves
    end

    it 'should be idempotent' do
      pitcher_accolade = {player_id: 1, season: 3, accolade: Accolades::Pitching::Most_Saves}

      @player_repository.save_pitcher_accolade pitcher_accolade
      @player_repository.save_pitcher_accolade pitcher_accolade
    end
  end

  describe '#save_batter_accolade' do
    it 'should return an empty array on success' do
      batter_accolade = {player_id: 1, season: 3, accolade: Accolades::Batting::Most_Runs_Batted_In}

      result = @player_repository.save_batter_accolade batter_accolade

      expect( result        ).to_not be_nil
      expect( result        ).to     be_a   Array
      expect( result.length ).to     eq     0
    end

    it 'should insert a batter_accolades_t record' do
      batter_accolade = {player_id: 1, season: 3, accolade: Accolades::Batting::Most_Runs_Batted_In}

      @player_repository.save_batter_accolade batter_accolade

      result = @db.get_first_row "select * from batter_accolades_t where player_id = 1 and season = 3 and accolade = #{Accolades::Batting::Most_Runs_Batted_In}"

      expect( result              ).to_not be_nil
      expect( result              ).to     be_a   Hash
      expect( result['Player_Id'] ).to     eq     1
      expect( result['Season'   ] ).to     eq     3
      expect( result['Accolade' ] ).to     eq     Accolades::Batting::Most_Runs_Batted_In
    end

    it 'should be idempotent' do
      batter_accolade = {player_id: 1, season: 3, accolade: Accolades::Batting::Most_Runs_Batted_In}

      @player_repository.save_batter_accolade batter_accolade
      @player_repository.save_batter_accolade batter_accolade
    end
  end
end
