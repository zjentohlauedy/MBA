CC = cc
CFLAGS = -std=c99 -g
LDFLAGS = -lm

progs = \
calc_avg_park_size \
check_games draft \
clear_stats \
ewbcopyleague \
ewbfielding \
extract_data \
fixstats \
games_played \
gen_players \
injuries \
list_players \
lottery \
print_colors \
print_parks \
print_rosters \
print_team_totals \
rating_ratios \
readgames \
reset_injuries \
reset_closer_injuries \
reset_records \
reset_team_colors \
rest_players \
schedule \
sort_players \
standings \
updateewbcolors \
updateewbfielding \
updateewbpitching \
updateewbratings \
updateewbstats


ALL: $(progs)

calc_avg_park_size: calc_avg_park_size.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

check_games: check_games.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clear_stats: clear_stats.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

draft: draft.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

ewbcopyleague: ewbcopyleague.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

ewbfielding: ewbfielding.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

extract_data: extract_data.o ewbfiles.o convert_league.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

fixstats: fixstats.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

games_played: games_played.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

gen_players: gen_players.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

injuries: injuries.o ewbfiles.o convert_league.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

lottery: lottery.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

list_players: list_players.o ewbfiles.o convert_league.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

print_colors: print_colors.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

print_parks: print_parks.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

print_rosters: print_rosters.o ewbfiles.o convert_league.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

print_team_totals: print_team_totals.o ewbfiles.o convert_league.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

rating_ratios: rating_ratios.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

readgames: readgames.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

reset_injuries: reset_injuries.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

reset_closer_injuries: reset_closer_injuries.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

reset_records: reset_records.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

reset_team_colors: reset_team_colors.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

rest_players: rest_players.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

schedule: schedule.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

sort_players: sort_players.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

standings: standings.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

updateewbcolors: updateewbcolors.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

updateewbfielding: updateewbfielding.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

updateewbpitching: updateewbpitching.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

updateewbratings: updateewbratings.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

updateewbstats: updateewbstats.o ewbfiles.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

lottery.o: lottery.c
	$(CC) $(CFLAGS) -D_DEFAULT_SOURCE -c $<

%.o: %.c ewbfiles.h league.h
	$(CC) $(CFLAGS) -c $<


.PHONY: clean
clean:
	rm $(progs) *.o
