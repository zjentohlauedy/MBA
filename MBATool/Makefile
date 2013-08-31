CC = cc
CFLAGS = -std=c99 -g
LDFLAGS = -lm

progs = \
   test_players_t \
   test_player_accolades_t \
   test_pitchers_t \
   test_pitcher_stats_t \
   test_pitcher_accolades_t \
   test_batters_t \
   test_batter_stats_t \
   test_batter_accolades_t \
   test_teams_t

ALL: $(progs) TAGS


test_players_t: test_players_t.o players_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 -o $@ $^

test_player_accolades_t: test_player_accolades_t.o player_accolades_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 -o $@ $^

test_pitchers_t: test_pitchers_t.o pitchers_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 -o $@ $^

test_pitcher_stats_t: test_pitcher_stats_t.o pitcher_stats_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 -o $@ $^

test_pitcher_accolades_t: test_pitcher_accolades_t.o pitcher_accolades_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 -o $@ $^

test_batters_t: test_batters_t.o batters_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 -o $@ $^

test_batter_stats_t: test_batter_stats_t.o batter_stats_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 -o $@ $^

test_batter_accolades_t: test_batter_accolades_t.o batter_accolades_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 -o $@ $^

test_teams_t: test_teams_t.o teams_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 -o $@ $^

%.o: %.c player.h team.h sql_query.h unit_test.h
	$(CC) $(CFLAGS) -c $<

TAGS: *.h *.c
	etags *.h *.c

.PHONY: clean
clean:
	rm $(progs) *.o
