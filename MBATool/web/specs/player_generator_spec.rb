location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'name_manager'
require 'player_generator'

describe PlayerGenerator do
  before :each do
    @db = SQLite3::Database.new "test_mba.db"

    @db.results_as_hash  = true
    @db.type_translation = true

    @nm = double NameManager

    @player_generator = PlayerGenerator.new @db, @nm

    @db.transaction
  end

  after :each do
    @db.rollback
  end

  describe '#generate_pitcher' do
    it 'should return a pitcher object' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_pitcher 5

      expect( pitcher ).to_not be_nil
      expect( pitcher[:player_type]).to eq PlayerTypes::Pitcher
    end

    it 'should set the pitcher name to that given by the name manager' do
      @db.execute 'insert into players_t (player_id) values (1)'

      player_name = { first: 'Test', last: 'Player' }
      allow( @nm ).to receive( :get_name ).and_return player_name

      pitcher = @player_generator.generate_pitcher 5

      expect( pitcher ).to_not be_nil
      expect( pitcher[ :first_name ]).to eq player_name[ :first ]
      expect( pitcher[ :last_name  ]).to eq player_name[ :last  ]
    end

    it 'should set the player id to the one more than the highest value currently in the database' do
      @db.execute 'insert into players_t (player_id) values (37)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_pitcher 5

      expect( pitcher ).to_not be_nil
      expect( pitcher[:player_id] ).to eq 38
    end

    it 'should set the rookie season to the season passed to the method' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_pitcher 5

      expect( pitcher ).to_not be_nil
      expect( pitcher[:rookie_season] ).to eq 5
    end

    it 'should default the phoenetics to empty strings' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_pitcher 5

      expect( pitcher ).to_not be_nil
      expect( pitcher[ :first_phoenetic ] ).to eq ''
      expect( pitcher[ :last_phoenetic  ] ).to eq ''
    end

    it 'should set player skin tone' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_pitcher 5

      expect( pitcher ).to_not be_nil
      expect( [SkinTones::Dark, SkinTones::Light] ).to include pitcher[:skin_tone]
    end

    it 'should set player handedness' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_pitcher 5

      expect( pitcher ).to_not be_nil
      expect( [Handedness::Left, Handedness::Right] ).to include pitcher[:handedness]
    end

    it 'should set player longevity' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_pitcher 5

      expect( pitcher ).to_not be_nil
      expect( pitcher[:longevity] ).to be_between 1, 10
    end

    it 'should create player with pitcher details' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_pitcher 5

      expect( pitcher ).to_not be_nil
      expect( pitcher[:details] ).to_not be_nil

      pitcher_details = pitcher[:details]

      expect( pitcher_details[ :player_id ] ).to eq pitcher[:player_id]
      expect( pitcher_details[ :speed     ] ).to be_between 1, 10
      expect( pitcher_details[ :control   ] ).to be_between 1, 10
      expect( pitcher_details[ :bunt      ] ).to eq pitcher[:longevity]
      expect( pitcher_details[ :fatigue   ] ).to be_between 4, 10
    end
  end

  describe '#generate_closer' do
    it 'should return a pitcher object' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_closer 5

      expect( pitcher ).to_not be_nil
      expect( pitcher[:player_type]).to eq PlayerTypes::Pitcher
    end

    it 'should set the pitcher name to that given by the name manager' do
      @db.execute 'insert into players_t (player_id) values (1)'

      player_name = { first: 'Test', last: 'Player' }
      allow( @nm ).to receive( :get_name ).and_return player_name

      pitcher = @player_generator.generate_closer 5

      expect( pitcher ).to_not be_nil
      expect( pitcher[ :first_name ]).to eq player_name[ :first ]
      expect( pitcher[ :last_name  ]).to eq player_name[ :last  ]
    end

    it 'should set the player id to the one more than the highest value currently in the database' do
      @db.execute 'insert into players_t (player_id) values (37)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_closer 5

      expect( pitcher ).to_not be_nil
      expect( pitcher[:player_id] ).to eq 38
    end

    it 'should set the rookie season to the season passed to the method' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_closer 5

      expect( pitcher ).to_not be_nil
      expect( pitcher[:rookie_season] ).to eq 5
    end

    it 'should default the phoenetics to empty strings' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_closer 5

      expect( pitcher ).to_not be_nil
      expect( pitcher[ :first_phoenetic ] ).to eq ''
      expect( pitcher[ :last_phoenetic  ] ).to eq ''
    end

    it 'should set player skin tone' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_closer 5

      expect( pitcher ).to_not be_nil
      expect( [SkinTones::Dark, SkinTones::Light] ).to include pitcher[:skin_tone]
    end

    it 'should set player handedness' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_closer 5

      expect( pitcher ).to_not be_nil
      expect( [Handedness::Left, Handedness::Right] ).to include pitcher[:handedness]
    end

    it 'should set player longevity' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_closer 5

      expect( pitcher ).to_not be_nil
      expect( pitcher[:longevity] ).to be_between 1, 10
    end

    it 'should create player with pitcher details' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      pitcher = @player_generator.generate_closer 5

      expect( pitcher ).to_not be_nil
      expect( pitcher[:details] ).to_not be_nil

      pitcher_details = pitcher[:details]

      expect( pitcher_details[ :player_id ] ).to eq pitcher[:player_id]
      expect( pitcher_details[ :speed     ] ).to be_between 6, 10
      expect( pitcher_details[ :control   ] ).to be_between 6, 10
      expect( pitcher_details[ :bunt      ] ).to eq pitcher[:longevity]
      expect( pitcher_details[ :fatigue   ] ).to eq 1
    end
  end

  describe '#generate_batter' do
    it 'should return a batter object' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      batter = @player_generator.generate_batter 5

      expect( batter ).to_not be_nil
      expect( batter[:player_type]).to eq PlayerTypes::Batter
    end

    it 'should set the batter name to that given by the name manager' do
      @db.execute 'insert into players_t (player_id) values (1)'

      player_name = { first: 'Test', last: 'Player' }
      allow( @nm ).to receive( :get_name ).and_return player_name

      batter = @player_generator.generate_batter 5

      expect( batter ).to_not be_nil
      expect( batter[ :first_name ]).to eq player_name[ :first ]
      expect( batter[ :last_name  ]).to eq player_name[ :last  ]
    end

    it 'should set the player id to the one more than the highest value currently in the database' do
      @db.execute 'insert into players_t (player_id) values (37)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      batter = @player_generator.generate_batter 5

      expect( batter ).to_not be_nil
      expect( batter[:player_id] ).to eq 38
    end

    it 'should set the rookie season to the season passed to the method' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      batter = @player_generator.generate_batter 5

      expect( batter ).to_not be_nil
      expect( batter[:rookie_season] ).to eq 5
    end

    it 'should default the phoenetics to empty strings' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      batter = @player_generator.generate_batter 5

      expect( batter ).to_not be_nil
      expect( batter[ :first_phoenetic ] ).to eq ''
      expect( batter[ :last_phoenetic  ] ).to eq ''
    end

    it 'should set player skin tone' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      batter = @player_generator.generate_batter 5

      expect( batter ).to_not be_nil
      expect( [SkinTones::Dark, SkinTones::Light] ).to include batter[:skin_tone]
    end

    it 'should set player handedness' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      batter = @player_generator.generate_batter 5

      expect( batter ).to_not be_nil
      expect( [Handedness::Left, Handedness::Right, Handedness::Switch] ).to include batter[:handedness]
    end

    it 'should set player longevity' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      batter = @player_generator.generate_batter 5

      expect( batter ).to_not be_nil
      expect( batter[:longevity] ).to be_between 1, 10
    end

    it 'should create player with batter details' do
      @db.execute 'insert into players_t (player_id) values (1)'

      allow( @nm ).to receive( :get_name ).and_return Hash.new

      batter = @player_generator.generate_batter 5

      expect( batter ).to_not be_nil
      expect( batter[:details] ).to_not be_nil

      batter_details = batter[:details]

      expect( batter_details[ :player_id          ] ).to eq batter[:player_id]
      expect( batter_details[ :primary_position   ] ).to be_between 1,  8

      expect( [ Positions::Catcher,
                Positions::FirstBaseman,
                Positions::ThirdBaseman,
                Positions::Infielder,
                Positions::RightFielder,
                Positions::Outfielder   ] ).to include batter_details[ :secondary_position ]

      expect( batter_details[ :power              ] ).to be_between 1,  8
      expect( batter_details[ :hit_n_run          ] ).to be_between 1, 10
      expect( batter_details[ :bunt               ] ).to be_between 1, 10
      expect( batter_details[ :running            ] ).to be_between 1, 10
      expect( batter_details[ :range              ] ).to be_between 1, 10
      expect( batter_details[ :arm                ] ).to be_between 1, 10
    end
  end

  describe '#pick_pitcher_style' do
    it 'should return a pitcher style' do
      pitcher_style = @player_generator.pick_pitcher_style

      expect( [PitcherStyle::Starter, PitcherStyle::Closer] ).to include pitcher_style
    end
  end
end
