CC = cc
CFLAGS = -std=c99 -g
LDFLAGS = -lm

progs = test_sql

ALL: $(progs)


test_sql: test_sql.o players_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lsqlite3 -o $@ $^


%.o: %.c player.h sql_query.h
	$(CC) $(CFLAGS) -c $<


.PHONY: clean
clean:
	rm $(progs) *.o
