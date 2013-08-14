CC = cc
CFLAGS = -std=c99 -g
LDFLAGS = -lm

progs = \
   test_players_t \
   test_player_accolades_t \
   test_pitchers_t

ALL: $(progs)


test_players_t: test_players_t.o players_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 -o $@ $^

test_player_accolades_t: test_player_accolades_t.o player_accolades_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 -o $@ $^

test_pitchers_t: test_pitchers_t.o pitchers_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 -o $@ $^

%.o: %.c player.h sql_query.h unit_test.h
	$(CC) $(CFLAGS) -c $<


.PHONY: clean
clean:
	rm $(progs) *.o
