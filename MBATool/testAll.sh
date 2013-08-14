#!/usr/bin/env bash
#

DBFILE="mba_test.db"

FAIL=0

function runtestprog {

    echo
    printf "%s:\n" $1
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
runtestprog test_player_accolades_t
runtestprog test_pitchers_t

if [ $FAIL -ne 0 ]; then

    echo
    printf "\033[1;31mThere were test failures...\033[0m\n"
    echo

    exit
fi

echo
printf "\033[1;32mALL TESTS COMPLETED SUCCESSFULLY\033[0m\n"
echo

exit
