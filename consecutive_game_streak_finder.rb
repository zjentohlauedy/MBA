class ConsecutiveGameStreakFinder
  attr_reader :suspicious_players

  def initialize( season_dir, streaks )
    @season_dir = season_dir
    @season     = @season_dir.split('/')[-1]

    @suspicious_players = []
    @streaks = streaks
  end

  def fix_duplicate_player_names( team )
    if dup = team[:players].detect { |x| team[:players].count { |y| y[:player_name] == x[:player_name] and y[:player_type] == x[:player_type] } > 1 }
      @suspicious_players.push( { season: @season, team_name: team[:name], player_name: dup[:player_name] } )
      dup[:player_name] = "#{dup[:player_name]}2"
    end
  end

  def find()
    files = %x(find #{@season_dir} -name "D[0-9][0-9][0-9]G[0-9][0-9].txt").split "\n"

    regular_season_files = files.select { |f| f.match %r{/D(0[0-9][0-9]|1([0-4][0-9]|5[012]))G[01][0-9]} }.sort

    regular_season_files.each do |file|
      begin
        teams = BoxscoreParser.parse file
      rescue Exception => e
        puts "Error processing file: #{file}"
        raise e
      end

      game = file.scan( %r[/D([0-9]+)G] )[0][0]

      teams.each_key do |team|
        fix_duplicate_player_names teams[team]
        teams[team][:players].each do |player|
          next if player[:player_type] == 1

          @streaks.update_streaks player, game
        end
      end
    end

    @streaks.compile_streaks
  end

  def print_results()
    @streaks.print_top_streaks

    puts ""
    puts ""

    unless @suspicious_players.empty?
      puts "Suspicious Players:"
    end

    @suspicious_players.sort { |a,b| a[:team_name] <=> b[:team_name] }.uniq.each do |player|
      printf "%s %-10s %-14s\n", player[:season], player[:team_name], player[:player_name]
    end
  end
end
