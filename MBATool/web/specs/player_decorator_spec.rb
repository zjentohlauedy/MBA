location = File.dirname __FILE__
$: << "#{location}"

require 'player_decorator'
require 'player_types'
require 'skin_tones'
require 'handedness'
require 'phases'

describe PlayerDecorator do
  before :each do
    @base_url       = 'http://www.mba.org/mba/resources'
    @player_decorator = PlayerDecorator.new @base_url
  end

  describe '#decorate_player' do
    it 'should return the given team player hash with links added' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      result = @player_decorator.decorate_player player

      expect( result                   ).to_not be_nil
      expect( result                   ).to     be_a   Hash
      expect( result[:player_id      ] ).to_not be_nil
      expect( result[:first_name     ] ).to_not be_nil
      expect( result[:last_name      ] ).to_not be_nil
      expect( result[:first_phoenetic] ).to_not be_nil
      expect( result[:last_phoenetic ] ).to_not be_nil
      expect( result[:skin_tone      ] ).to_not be_nil
      expect( result[:handedness     ] ).to_not be_nil
      expect( result[:player_type    ] ).to_not be_nil
      expect( result[:rookie_season  ] ).to_not be_nil
      expect( result[:longevity      ] ).to_not be_nil
      expect( result[:_links         ] ).to_not be_nil
    end

    it 'should add a self link' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      result = @player_decorator.decorate_player player

      self_link = result[:_links][:self]

      expect( self_link        ).to_not be_nil
      expect( self_link[:rel ] ).to     eq     'self'
      expect( self_link[:href] ).to     eq     "#{@base_url}/players/1"
      expect( self_link[:desc] ).to     eq     "This resource"
    end

    it 'should append season parameter to self link if given a season' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      result = @player_decorator.decorate_player player, 3

      expect( result[:_links][:self][:href] ).to eq "#{@base_url}/players/1?season=3"
    end

    it 'should append phase parameter to self link if given a phase' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      result = @player_decorator.decorate_player player, nil, Phases::RegularSeason

      expect( result[:_links][:self][:href] ).to eq "#{@base_url}/players/1?phase=#{Phases::RegularSeason}"
    end

    it 'should append season and phase parameters to self link if given season and phase' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      result = @player_decorator.decorate_player player, 3, Phases::RegularSeason

      expect( result[:_links][:self][:href] ).to eq "#{@base_url}/players/1?season=3&phase=#{Phases::RegularSeason}"
    end

    it 'should add a stats link' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      result = @player_decorator.decorate_player player

      stats_link = result[:_links][:stats]

      expect( stats_link        ).to_not be_nil
      expect( stats_link[:rel ] ).to     eq     'stats'
      expect( stats_link[:href] ).to     eq     "#{@base_url}/players/1/stats"
      expect( stats_link[:desc] ).to     eq     "Statistics for this player"
    end

    it 'should append season parameter to stats link if given a season' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      result = @player_decorator.decorate_player player, 3

      expect( result[:_links][:stats][:href] ).to eq "#{@base_url}/players/1/stats?season=3"
      expect( result[:_links][:stats][:desc] ).to eq "Season 3 statistics for this player"
    end

    it 'should append phase parameter to stats link if given a phase' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      result = @player_decorator.decorate_player player, nil, Phases::RegularSeason

      expect( result[:_links][:stats][:href] ).to eq "#{@base_url}/players/1/stats?phase=#{Phases::RegularSeason}"
      expect( result[:_links][:stats][:desc] ).to eq "Regular Season statistics for this player"
    end

    it 'should append season and phase parameters to stats link if given season and phase' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      result = @player_decorator.decorate_player player, 3, Phases::RegularSeason

      expect( result[:_links][:stats][:href] ).to eq "#{@base_url}/players/1/stats?season=3&phase=#{Phases::RegularSeason}"
      expect( result[:_links][:stats][:desc] ).to eq "Season 3 Regular Season statistics for this player"
    end

    it 'should add an accolades link' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      result = @player_decorator.decorate_player player

      accolades_link = result[:_links][:accolades]

      expect( accolades_link        ).to_not be_nil
      expect( accolades_link[:rel ] ).to     eq     'accolades'
      expect( accolades_link[:href] ).to     eq     "#{@base_url}/players/1/accolades"
      expect( accolades_link[:desc] ).to     eq     "Accolades for this player"
    end

    it 'should append season parameter to accolades link if given a season' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      result = @player_decorator.decorate_player player, 3

      expect( result[:_links][:accolades][:href] ).to eq "#{@base_url}/players/1/accolades?season=3"
      expect( result[:_links][:accolades][:desc] ).to eq "Season 3 accolades for this player"
    end

    it 'should add a player link if given a season' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      result = @player_decorator.decorate_player player, 3

      player_link = result[:_links][:player]

      expect( player_link        ).to_not be_nil
      expect( player_link[:rel ] ).to     eq     'player'
      expect( player_link[:href] ).to     eq     "#{@base_url}/players/1"
      expect( player_link[:desc] ).to     eq     "Information about this player"
    end

    it 'should add a player link if given a phase' do
      player = {player_id: 1, first_name: 'Firstname1', last_name: 'Lastname1', first_phoenetic: 'FN1', last_phoenetic: 'LN1', skin_tone: SkinTones::Light, handedness: Handedness::Switch, player_type: PlayerTypes::Batter, rookie_season: 5, longevity: 7}

      result = @player_decorator.decorate_player player, nil, Phases::RegularSeason

      player_link = result[:_links][:player]

      expect( player_link        ).to_not be_nil
      expect( player_link[:rel ] ).to     eq     'player'
      expect( player_link[:href] ).to     eq     "#{@base_url}/players/1"
      expect( player_link[:desc] ).to     eq     "Information about this player"
    end
  end

  describe '#decorate_player_stats' do
    it 'should return the given pitcher stats hash with links added' do
      pitcher_stats = {player_id: 1, season: 1, season_phase: 'Regular', wins: 22, losses: 13, games: 37, saves: 5, innings: 303, outs: 1, hits: 309, earned_runs: 150, home_runs: 64, walks: 100, strike_outs: 205}

      result = @player_decorator.decorate_player_stats pitcher_stats, Phases::RegularSeason

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
      expect( result[:_links       ] ).to_not be_nil
    end

    it 'should return the given batter stats hash with links added' do
      batter_stats = {player_id: 1, season: 1, season_phase: 'Regular', games: 152, at_bats: 602, runs: 83, hits: 153, doubles: 25, triples: 2, home_runs: 21, runs_batted_in: 77, walks: 74, strike_outs: 95, steals: 22, errors: 5}

      result = @player_decorator.decorate_player_stats batter_stats, Phases::RegularSeason

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
      expect( result[:_links        ] ).to_not be_nil
    end

    it 'should add a self link' do
      pitcher_stats = {player_id: 1, season: 1, season_phase: 'Regular', wins: 22, losses: 13, games: 37, saves: 5, innings: 303, outs: 1, hits: 309, earned_runs: 150, home_runs: 64, walks: 100, strike_outs: 205}

      result = @player_decorator.decorate_player_stats pitcher_stats, Phases::RegularSeason

      self_link = result[:_links][:self]

      expect( self_link        ).to_not be_nil
      expect( self_link[:rel ] ).to     eq     'self'
      expect( self_link[:href] ).to     eq     "#{@base_url}/players/1/stats?season=1&phase=#{Phases::RegularSeason}"
      expect( self_link[:desc] ).to     eq     "This resource"
    end

    it 'should add a player link' do
      pitcher_stats = {player_id: 1, season: 1, season_phase: 'Regular', wins: 22, losses: 13, games: 37, saves: 5, innings: 303, outs: 1, hits: 309, earned_runs: 150, home_runs: 64, walks: 100, strike_outs: 205}

      result = @player_decorator.decorate_player_stats pitcher_stats, Phases::RegularSeason

      player_link = result[:_links][:player]

      expect( player_link        ).to_not be_nil
      expect( player_link[:rel ] ).to     eq     'player'
      expect( player_link[:href] ).to     eq     "#{@base_url}/players/1"
      expect( player_link[:desc] ).to     eq     "Information about this player"
    end

    it 'should add a stats link' do
      pitcher_stats = {player_id: 1, season: 1, season_phase: 'Regular', wins: 22, losses: 13, games: 37, saves: 5, innings: 303, outs: 1, hits: 309, earned_runs: 150, home_runs: 64, walks: 100, strike_outs: 205}

      result = @player_decorator.decorate_player_stats pitcher_stats, Phases::RegularSeason

      stats_link = result[:_links][:stats]

      expect( stats_link        ).to_not be_nil
      expect( stats_link[:rel ] ).to     eq     'stats'
      expect( stats_link[:href] ).to     eq     "#{@base_url}/players/1/stats"
      expect( stats_link[:desc] ).to     eq     "All statistics for this player"
    end

    it 'should add an accolades link' do
      pitcher_stats = {player_id: 1, season: 1, season_phase: 'Regular', wins: 22, losses: 13, games: 37, saves: 5, innings: 303, outs: 1, hits: 309, earned_runs: 150, home_runs: 64, walks: 100, strike_outs: 205}

      result = @player_decorator.decorate_player_stats pitcher_stats, Phases::RegularSeason

      accolades_link = result[:_links][:accolades]

      expect( accolades_link        ).to_not be_nil
      expect( accolades_link[:rel ] ).to     eq     'accolades'
      expect( accolades_link[:href] ).to     eq     "#{@base_url}/players/1/accolades"
      expect( accolades_link[:desc] ).to     eq     "All accolades for this player"
    end
  end

  describe '#decorate_player_accolade' do
    it 'should return the given player accolade hash with links added' do
      player_accolade = {player_id: 1, season: 1, accolade: 'Some Accolade'}

      result = @player_decorator.decorate_player_accolade player_accolade

      expect( result             ).to_not be_nil
      expect( result             ).to     be_a   Hash
      expect( result[:player_id] ).to_not be_nil
      expect( result[:season   ] ).to_not be_nil
      expect( result[:accolade ] ).to_not be_nil
      expect( result[:_links   ] ).to_not be_nil
    end

    it 'should add a player link' do
      player_accolade = {player_id: 1, season: 1, accolade: 'Some Accolade'}

      result = @player_decorator.decorate_player_accolade player_accolade

      player_link = result[:_links][:player]

      expect( player_link        ).to_not be_nil
      expect( player_link[:rel ] ).to     eq     'player'
      expect( player_link[:href] ).to     eq     "#{@base_url}/players/1"
      expect( player_link[:desc] ).to     eq     "Information about this player"
    end

    it 'should add a stats link' do
      player_accolade = {player_id: 1, season: 1, accolade: 'Some Accolade'}

      result = @player_decorator.decorate_player_accolade player_accolade

      stats_link = result[:_links][:stats]

      expect( stats_link        ).to_not be_nil
      expect( stats_link[:rel ] ).to     eq     'stats'
      expect( stats_link[:href] ).to     eq     "#{@base_url}/players/1/stats"
      expect( stats_link[:desc] ).to     eq     "All statistics for this player"
    end

    it 'should add an accolades link' do
      player_accolade = {player_id: 1, season: 1, accolade: 'Some Accolade'}

      result = @player_decorator.decorate_player_accolade player_accolade

      accolades_link = result[:_links][:accolades]

      expect( accolades_link        ).to_not be_nil
      expect( accolades_link[:rel ] ).to     eq     'accolades'
      expect( accolades_link[:href] ).to     eq     "#{@base_url}/players/1/accolades"
      expect( accolades_link[:desc] ).to     eq     "All accolades for this player"
    end
  end
end
