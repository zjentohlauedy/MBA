module ConsecutiveGameLeadersPrinter
  def self.print_leaders( streaks, suspicious_players, limit = 20 )
    last_value = nil
    count      = 0

    streaks.sort { |a,b| b[:games] <=> a[:games] }.each do |streak|
      break if count > limit and last_value != streak[:games]

      rank = (last_value == streak[:games]) ? ' - ' : sprintf( "%2d.", count + 1)

      printf "%s ?? %-19s  %s  %-10s  %s-%s %3d\n", rank, streak[:player_name], streak[:season], streak[:team_name], streak[:start_game], streak[:end_game], streak[:games]

      count      += 1
      last_value  = streak[:games]
    end

    puts ""
    puts ""

    unless suspicious_players.empty?
      puts "Suspicious Players:"
    end

    suspicious_players.sort { |a,b| "#{a[:season]}#{a[:team_name]}" <=> "#{b[:season]}#{b[:team_name]}" }.uniq.each do |player|
      printf "%s %-10s %-14s\n", player[:season], player[:team_name], player[:player_name]
    end
  end
end
