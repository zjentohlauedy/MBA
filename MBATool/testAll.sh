#!/usr/bin/env bash
#

rm mba.db
sqlite3 mba.db < schema.sql

if [ $? -ne 0 ]; then exit; fi

make

if [ $? -ne 0 ]; then exit; fi

echo
echo

echo test_players_t:
./test_players_t

if [ $? -ne 0 ]; then exit; fi

echo
echo "ALL TESTS COMPLETED SUCCESSFULLY"
echo

exit
