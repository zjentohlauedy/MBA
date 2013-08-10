#!/usr/bin/env bash
#

DBFILE="mba_test.db"

FAIL=0

function runtestprog {
    echo "$1:"
    $1 $DBFILE

    if [ $? -ne 0 ]; then
        FAIL=1
    fi
}

rm $DBFILE
sqlite3 $DBFILE < schema.sql

if [ $? -ne 0 ]; then exit; fi

make

if [ $? -ne 0 ]; then exit; fi

echo
echo

runtestprog test_players_t

if [ $FAIL -ne 0 ]; then

    echo
    echo "There were test failures..."
    echo

    exit
fi

echo
echo "ALL TESTS COMPLETED SUCCESSFULLY"
echo

exit
