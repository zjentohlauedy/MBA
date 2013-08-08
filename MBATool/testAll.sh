#!/usr/bin/env bash
#

rm mba.db
sqlite3 mba.db < schema.sql
make

echo
echo

echo test_players_t:
./test_players_t
