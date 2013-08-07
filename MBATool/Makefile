CC = cc
CFLAGS = -std=c99 -g
LDFLAGS = -lm

progs = test_players_t

ALL: $(progs)


test_players_t: test_players_t.o players_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 -o $@ $^


%.o: %.c player.h sql_query.h
	$(CC) $(CFLAGS) -c $<


.PHONY: clean
clean:
	rm $(progs) *.o
