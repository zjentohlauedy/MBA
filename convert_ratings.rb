#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'sqlite3'
require 'json'
require 'FileParser'


class CustomParser
  attr_reader :player_ratings

  def initialize()
    @rating_name    = ""
    @rating_value   = 0
    @player_ratings = {}
  end

  def parse( line )
    line = line.chomp

    if    line =~ /^[A-Za-z_]+:$/
      @rating_name = line[0..-2]
    elsif line =~ /^[1-9]0?:$/
      @rating_value = line[0..-2].to_i
    elsif line =~ /^[C123SLRP][ BSF] [A-Za-z.\']+, [A-Za-z.\']+$/
      fields = line.split /,? +/

      player_key = fields[2] + fields[1]

      ratings = @player_ratings.fetch player_key, {}

      if ratings.has_key? @rating_name
        raise "Player #{fields[2]}, #{fields[1]} already has a value for #{@rating_name}!"
      end

      ratings.store @rating_name, @rating_value

      @player_ratings.store player_key, ratings
    end
  end
end


class RatingConverter

  def initialize( player_ratings )
    @generator      = Random.new Time.new.usec
    @player_ratings = player_ratings

    #         rating:   0    1   2     3     4     5     6     7     8     9   10
    #     percentage:   0  .05  .9  3.75  12.5    50    25  6.25  1.25   0.2  0.1
    @rating_pbs     = [ 0,   4, 72,  300, 1000, 4000, 2000,  500,  100,   16,   8 ]

    #         rating:   0    1   2     3     4     5     6     7     8     9   10
    #     percentage:   0    0   0     0  1.25  12.5    50    25    10  0.95  0.3
    @fatigue_pbs    = [ 0,   0,  0,    0,  100, 1000, 4000, 2000,  800,   76,  24 ]

    #                    X     C    1B    2B    3B    SS    LF    CF    RF    IF    OF
    @secondary_pos  = [[ 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 ],  # X
                       [ 0,    0,   30,    0,   30,    0,    0,    0,   20,   10,   10 ],  # C
                       [ 0,   20,    0,    0,    0,    0,    0,    0,    0,   60,   20 ],  # 1B
                       [ 0,    5,    0,    0,    0,    0,    0,    0,    0,   85,   10 ],  # 2B
                       [ 0,   20,    0,    0,    0,    0,    0,    0,    0,   70,   10 ],  # 3B
                       [ 0,    5,    0,    0,    0,    0,    0,    0,    0,   75,   20 ],  # SS
                       [ 0,    5,    0,    0,    0,    0,    0,    0,    0,   10,   85 ],  # LF
                       [ 0,    5,    0,    0,    0,    0,    0,    0,    0,   20,   75 ],  # CF
                       [ 0,   20,    0,    0,    0,    0,    0,    0,    0,   10,   70 ]]; # RF
  end

  def roll_rating
    roll = (@generator.rand @rating_pbs.inject(:+)) + 1 # sum of elements

    @rating_pbs.each_with_index do |value, index|
      roll = roll - value

      unless roll > 0
        return index
      end
    end
  end

  def roll_fatigue
    roll = (@generator.rand @fatigue_pbs.inject(:+)) + 1 # sum of elements

    @fatigue_pbs.each_with_index do |value, index|
      roll = roll - value

      unless roll > 0
        return index
      end
    end
  end

  def roll_scnd_pos( pos )
    set = @secondary_pos[pos]

    roll = (@generator.rand set.inject(:+)) + 1

    set.each_with_index do |value, index|
      roll = roll - value

      unless roll > 0
        return index
      end
    end
  end

  def convert_ratings( player )
    player_key = player['First_Name'] + player['Last_Name']
    ratings    = @player_ratings[player_key]

    details = player['Details'];

    ratings.keys.each do |rating|
      if ! details.has_key? rating
        raise "Player #{player['First_Name']} #{player['Last_Name']} does not have a #{rating} rating!"
      end

      details[rating] = ratings[rating]
    end

    if player['Player_Type'] == 1
      details[ 'Bunt'    ] = roll_rating
      details[ 'Fatigue' ] = roll_fatigue

      player['Longevity'] =  details['Bunt']
    else
      details[ 'Bunt'  ] = roll_rating
      details[ 'Range' ] = roll_rating
      details[ 'Arm'   ] = roll_rating

      details[ 'Secondary_Position' ] = roll_scnd_pos details[ 'Primary_Position' ]

      player['Longevity'] = roll_rating
    end

    if player['Rookie_Season'] < 0
      roll = (@generator.rand player['Longevity']) + 1

      player['Rookie_Season'] = 0 - roll
    end
  end

end


cp = CustomParser.new
fp = FileParser.new cp

fp.process_file 'conversions.txt'

rc = RatingConverter.new cp.player_ratings

db = SQLite3::Database.new "mba.db"
db.results_as_hash = true
db.type_translation = true

db.transaction do |dbt|

  dbt.execute( "select * from players_t" ) do |row|
    player_id   = row[ 'Player_Id'   ]
    player_type = row[ 'Player_Type' ]

    if player_type == 1
      details = dbt.execute "select * from pitchers_t where player_id = ?", player_id

      row.store 'Details', details[0]

      rc.convert_ratings row

      dbt.execute '''update pitchers_t

                        set speed   = :Speed,
                            control = :Control,
                            bunt    = :Bunt,
                            fatigue = :Fatigue

                      where player_id = :Player_Id
                  ''', row['Details']
    else
      details = dbt.execute "select * from batters_t  where player_id = ?", player_id

      row.store 'Details', details[0]

      rc.convert_ratings row

      dbt.execute '''update batters_t

                        set power              = :Power,
                            hit_n_run          = :Hit_N_Run,
                            bunt               = :Bunt,
                            running            = :Running,
                            range              = :Range,
                            arm                = :Arm,
                            primary_position   = :Primary_Position,
                            secondary_position = :Secondary_Position

                      where player_id = :Player_Id
                  ''', row['Details']
    end

    dbt.execute '''update players_t

                      set rookie_season = :Rookie_Season,
                          longevity     = :Longevity

                    where player_id = :Player_Id
                ''', row.select { |k,v| ["Player_Id","Rookie_Season","Longevity"].index k }

  end
end

db.close
