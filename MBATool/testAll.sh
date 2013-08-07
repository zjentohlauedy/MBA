#!/usr/bin/env bash
#

rm mba.db
sqlite3 mba.db < schema.sql
make

./test_players_t
