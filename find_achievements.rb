#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"

categories = {
  no_hitters:  { description: 'No-Hitters for Seven Innings',         incidents: [] },
  cg_shutouts: { description: 'Complete Game, Extra-Inning Shutouts', incidents: [] },
  twenty_so:   { description: 'Twenty Strike Outs in a Game',         incidents: [] },
  twelve_inn:  { description: 'Twelve Innings Pitched',               incidents: [] },

  cycles:      { description: 'Hits for the Cycle',                   incidents: [] },
  six_hits:    { description: 'Six Hits in a Game',                   incidents: [] },
  four_homers: { description: 'Four Home Runs in a Game',             incidents: [] },
  ten_rbi:     { description: 'Ten RBI in a Game',                    incidents: [] },
  four_steals: { description: 'Four Steals in a Game',                incidents: [] }
}



ARGV.each do |file|
  File::foreach(file) do |line|
    fields = line.strip.split %r{ +}

    next if fields[0] == 'TOTAL'

    if fields.size == 11
      innings     = fields[1].to_f
      hits        = fields[2].to_i
      runs        = fields[3].to_i
      strike_outs = fields[7].to_i

      if innings >= 7.0 and hits == 0 # no-hitters of 7 innings or more
        categories[:no_hitters][:incidents].push "#{file}: #{line.strip}"
      end

      if innings >= 10.0 and runs == 0 and innings == innings.to_i # cg x-inn shutouts
        categories[:cg_shutouts][:incidents].push "#{file}: #{line.strip}"
      end

      if strike_outs >= 20 # 20 strikeouts in a game
        categories[:twenty_so][:incidents].push "#{file}: #{line.strip}"
      end

      if innings >= 12.0 # 12 innings pitched
        categories[:twelve_inn][:incidents].push "#{file}: #{line.strip}"
      end
    end

    if fields.size == 15
      at_bats = fields[1].to_i
      hits    = fields[3].to_i
      doubles = fields[4].to_i
      triples = fields[5].to_i
      homers  = fields[6].to_i
      xbase   = doubles + triples + homers
      rbi     = fields[7].to_i
      steals  = fields[11].to_i

      if doubles > 0 and triples > 0 and homers > 0 and hits > xbase # hit for the cycle
        categories[:cycles][:incidents].push "#{file}: #{line.strip}"
      end

      if hits >= 6 # 6 hits in a game
        categories[:six_hits][:incidents].push "#{file}: #{line.strip}"
      end

      if homers >= 4 # 4 home runs in a game
        categories[:four_homers][:incidents].push "#{file}: #{line.strip}"
      end

      if rbi >= 10 # 10 RBI in a game
        categories[:ten_rbi][:incidents].push "#{file}: #{line.strip}"
      end

      if steals >= 4 # 4 steals in a game
        categories[:four_steals][:incidents].push "#{file}: #{line.strip}"
      end
    end
  end
end


categories.keys.each do |category|
  next if categories[category][:incidents].length == 0

  puts categories[category][:description]

  categories[category][:incidents].each do |entry|
    puts entry
  end

  puts ""
end
