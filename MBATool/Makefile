AR = ar
ARFLAGS = -rc
CC = cc
CFLAGS = -std=c99 -g -Werror-implicit-function-declaration
LDFLAGS = -L. -lm -lsqlite3

progs = \
   gen_player_ids \
   copy_player_ids \
   set_longevity \
   copy_longevity \
   format_amiga_to_dos \
   format_dos_to_amiga

tests = \
   test_gen_player_ids \
   test_copy_player_ids \
   test_set_longevity \
   test_copy_longevity \
   test_format_amiga_to_dos \
   test_format_dos_to_amiga \
   test_players_t \
   test_player_accolades_t \
   test_pitchers_t \
   test_pitcher_stats_t \
   test_pitcher_accolades_t \
   test_batters_t \
   test_batter_stats_t \
   test_batter_accolades_t \
   test_teams_t \
   test_team_players_t \
   test_team_stats_t \
   test_team_pitching_stats_t \
   test_team_batting_stats_t \
   test_team_accolades_t \
   test_divisions_t \
   test_division_teams_t \
   test_division_stats_t \
   test_division_accolades_t \
   test_leagues_t \
   test_league_divisions_t \
   test_league_stats_t \
   test_league_accolades_t \
   test_league_teams_t \
   test_player_service \
   test_team_service \
   test_division_service \
   test_league_service \
   test_org_service \
   test_file_utils \
   test_convert_org \
   test_convert_leagues \
   test_convert_divisions \
   test_convert_teams \
   test_convert_players \
   test_parse_csv \
   test_calc_records

ALL: PROGS TESTS

PROGS: $(progs) TAGS

gen_player_ids: gen_player_ids_main.o gen_player_ids.o file_utils.o data_list.o parse_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

copy_player_ids: copy_player_ids.o copy_player_ids_main.o file_utils.o data_list.o parse_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

set_longevity: set_longevity.o set_longevity_main.o file_utils.o data_list.o parse_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

copy_longevity: copy_longevity.o copy_longevity_main.o file_utils.o data_list.o parse_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

format_amiga_to_dos: format_amiga_to_dos.o format_amiga_to_dos_main.o file_utils.o data_list.o parse_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

format_dos_to_amiga: format_dos_to_amiga.o format_dos_to_amiga_main.o file_utils.o data_list.o parse_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

TESTS: $(tests) mba_test.db TAGS

test_gen_player_ids: test_gen_player_ids.o gen_player_ids.o file_utils.o data_list.o parse_csv.o libmbabuilders.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_copy_player_ids: test_copy_player_ids.o copy_player_ids.o file_utils.o data_list.o parse_csv.o libmbabuilders.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_set_longevity: test_set_longevity.o set_longevity.o file_utils.o data_list.o parse_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_copy_longevity: test_copy_longevity.o copy_longevity.o file_utils.o data_list.o parse_csv.o libmbabuilders.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_format_amiga_to_dos: test_format_amiga_to_dos.o format_amiga_to_dos.o file_utils.o data_list.o parse_csv.o libmbabuilders.a libmbacompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_format_dos_to_amiga: test_format_dos_to_amiga.o format_dos_to_amiga.o file_utils.o data_list.o parse_csv.o libmbabuilders.a libmbacompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_players_t: test_players_t.o players_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_player_accolades_t: test_player_accolades_t.o player_accolades_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_pitchers_t: test_pitchers_t.o pitchers_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_pitcher_stats_t: test_pitcher_stats_t.o pitcher_stats_t.o sql_query.o data_list.o libmbabuilders.a libmbacompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_pitcher_accolades_t: test_pitcher_accolades_t.o pitcher_accolades_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_batters_t: test_batters_t.o batters_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_batter_stats_t: test_batter_stats_t.o batter_stats_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_batter_accolades_t: test_batter_accolades_t.o batter_accolades_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_teams_t: test_teams_t.o teams_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_team_players_t: test_team_players_t.o team_players_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_team_stats_t: test_team_stats_t.o team_stats_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_team_pitching_stats_t: test_team_pitching_stats_t.o team_pitching_stats_t.o sql_query.o data_list.o libmbabuilders.a libmbacompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_team_batting_stats_t: test_team_batting_stats_t.o team_batting_stats_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_team_accolades_t: test_team_accolades_t.o team_accolades_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_divisions_t: test_divisions_t.o divisions_t.o sql_query.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_division_teams_t: test_division_teams_t.o division_teams_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_division_stats_t: test_division_stats_t.o division_stats_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_division_accolades_t: test_division_accolades_t.o division_accolades_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_leagues_t: test_leagues_t.o leagues_t.o sql_query.o data_list.o libmbabuilders.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_league_divisions_t: test_league_divisions_t.o league_divisions_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_league_stats_t: test_league_stats_t.o league_stats_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_league_accolades_t: test_league_accolades_t.o league_accolades_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_league_teams_t: test_league_teams_t.o league_teams_t.o sql_query.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_player_service: test_player_service.o libmbadata.a libmbabuilders.a libmbacompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_team_service: test_team_service.o libmbadata.a libmbabuilders.a libmbacompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_division_service: test_division_service.o libmbadata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_league_service: test_league_service.o libmbadata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_org_service: test_org_service.o libmbadata.a libmbabuilders.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_file_utils: test_file_utils.o file_utils.o data_list.o parse_csv.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_convert_org: test_convert_org.o data_list.o file_utils.o parse_csv.o libmbadata.a libmbabuilders.a libmbaconversion.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lmbadata -lmbabuilders -lmbaconversion

test_convert_leagues: test_convert_leagues.o data_list.o file_utils.o parse_csv.o libmbadata.a libmbabuilders.a libmbaconversion.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lmbadata -lmbabuilders -lmbaconversion

test_convert_divisions: test_convert_divisions.o data_list.o file_utils.o parse_csv.o libmbadata.a libmbabuilders.a libmbaconversion.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lmbadata -lmbabuilders -lmbaconversion

test_convert_teams: test_convert_teams.o data_list.o file_utils.o parse_csv.o libmbadata.a libmbabuilders.a libmbacompares.a libmbaconversion.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lmbadata -lmbabuilders -lmbacompares -lmbaconversion

test_convert_players: test_convert_players.o data_list.o file_utils.o parse_csv.o libmbadata.a libmbabuilders.a libmbaconversion.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lmbadata -lmbabuilders -lmbaconversion

test_parse_csv: test_parse_csv.o parse_csv.o data_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_calc_records: test_calc_records.o calc_records.o file_utils.o data_list.o parse_csv.o libmbabuilders.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

MBADATA_OBJECTS = \
 sql_query.o \
 data_list.o \
 batter_accolades_t.o \
 batter_stats_t.o \
 batters_t.o \
 division_accolades_t.o \
 division_stats_t.o \
 divisions_t.o \
 division_teams_t.o \
 league_accolades_t.o \
 league_divisions_t.o \
 league_stats_t.o \
 league_teams_t.o \
 leagues_t.o \
 pitcher_accolades_t.o \
 pitcher_stats_t.o \
 pitchers_t.o \
 player_accolades_t.o \
 players_t.o \
 team_accolades_t.o \
 team_batting_stats_t.o \
 team_pitching_stats_t.o \
 team_players_t.o \
 team_stats_t.o \
 teams_t.o \
 player_service_get.o \
 player_service_save.o \
 player_service_remove.o \
 player_service_free.o \
 team_service_get.o \
 team_service_save.o \
 team_service_remove.o \
 team_service_free.o \
 division_service.o \
 league_service.o \
 org_service.o

libmbadata.a: $(MBADATA_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

MBABUILDERS_OBJECTS = \
 build_league.o \
 build_league_team.o \
 build_league_division.o \
 build_division.o \
 build_division_team.o \
 build_team.o \
 build_team_pitching_stats.o \
 build_team_player.o \
 build_player.o \
 build_pitcher_stats.o \
 build_file_leagname.o \
 build_file_parks.o \
 build_file_players.o \
 build_schedule.o \
 build_records.o

libmbabuilders.a: $(MBABUILDERS_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

MBACOMPARES_OBJECTS = \
 compare_pitcher_stats.o \
 compare_team_pitching_stats.o \
 compare_team_batting_stats.o \
 compare_filebatting_stats.o

libmbacompares.a: $(MBACOMPARES_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

MBACONVERSION_OBJECTS = \
 convert_org.o \
 convert_leagues.o \
 convert_divisions.o \
 convert_teams.o \
 convert_players.o

libmbaconversion.a: $(MBACONVERSION_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c *.h
	$(CC) $(CFLAGS) -c $<

mba_test.db: schema.sql
	sqlite3 $@ < $<

TAGS: *.h *.c
	etags *.h *.c

.PHONY: clean
clean:
	rm $(tests) $(progs) *.o *.a
