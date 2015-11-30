location = File.dirname __FILE__
$: << "#{location}"

require 'accolades'

describe Accolades do
  describe '::valid?' do
    it 'should return true if the given accolade value and type match an actual accolade' do
      expect( Accolades::valid? 'league',   Accolades::League::World_Title         ).to be true
      expect( Accolades::valid? 'division', Accolades::Division::League_Title      ).to be true
      expect( Accolades::valid? 'team',     Accolades::Team::Division_Title        ).to be true
      expect( Accolades::valid? 'player',   Accolades::Player::All_Star            ).to be true
      expect( Accolades::valid? 'pitching', Accolades::Pitching::Most_Strike_Outs  ).to be true
      expect( Accolades::valid? 'batting',  Accolades::Batting::Batter_of_the_Year ).to be true
    end

    it 'should return false if the given accolade value and type do not match an actual accolade' do
      expect( Accolades::valid? 'pitching', nil                            ).to be false
      expect( Accolades::valid? 'team',     11                             ).to be false
      expect( Accolades::valid? 'players',  Accolades::Player::Best_Record ).to be false
    end
  end

  describe '::get_accolade_name' do
    it 'should return the name of the accolade matching the given accolade value and type' do
      expect( Accolades::get_accolade_name 'league',   Accolades::League::World_Title         ).to eq "World Title"
      expect( Accolades::get_accolade_name 'division', Accolades::Division::League_Title      ).to eq "League Title"
      expect( Accolades::get_accolade_name 'team',     Accolades::Team::Division_Title        ).to eq "Division Title"
      expect( Accolades::get_accolade_name 'player',   Accolades::Player::All_Star            ).to eq "All Star"
      expect( Accolades::get_accolade_name 'pitching', Accolades::Pitching::Most_Strike_Outs  ).to eq "Most Strike Outs"
      expect( Accolades::get_accolade_name 'batting',  Accolades::Batting::Batter_of_the_Year ).to eq "MBA Batter of the Year"
    end

    it 'should return nil if no accolades match the given accolade value and type' do
      expect( Accolades::get_accolade_name 'pitching', nil                            ).to be_nil
      expect( Accolades::get_accolade_name 'team',     11                             ).to be_nil
      expect( Accolades::get_accolade_name 'players',  Accolades::Player::Best_Record ).to be_nil
    end
  end
end
