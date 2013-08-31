#!/usr/bin/env bash
#

DBFILE="mba_test.db"

FAIL=0

function runTestProg {

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

runTestProg test_players_t
runTestProg test_player_accolades_t
runTestProg test_pitchers_t
runTestProg test_pitcher_stats_t
runTestProg test_pitcher_accolades_t
runTestProg test_batters_t
runTestProg test_batter_stats_t
runTestProg test_batter_accolades_t

runTestProg test_teams_t

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
