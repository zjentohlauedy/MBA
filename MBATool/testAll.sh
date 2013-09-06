#!/usr/bin/env bash
#

DBFILE="mba_test.db"

FAIL=0
TESTS=0
PASSED=0
FAILED=0

function runTestProg {

    re="^.*Tests Run: ([0-9]+), Passed: ([0-9]+), Failed: ([0-9]+).*$"

    echo
    printf "%s:\n" $1
    RESULTS=$( $1 $DBFILE )

    if [ $? -ne 0 ]; then
        FAIL=1
    fi

    printf "$RESULTS"

    if [[ "$RESULTS" =~ $re ]]
    then
        TESTS=$[  TESTS  + BASH_REMATCH[1] ]
        PASSED=$[ PASSED + BASH_REMATCH[2] ]
        FAILED=$[ FAILED + BASH_REMATCH[3] ]
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
runTestProg test_team_players_t
runTestProg test_team_stats_t
runTestProg test_team_pitching_stats_t
runTestProg test_team_batting_stats_t
runTestProg test_team_accolades_t

runTestProg test_divisions_t
runTestProg test_division_teams_t
runTestProg test_division_stats_t
runTestProg test_division_accolades_t

runTestProg test_leagues_t
runTestProg test_league_divisions_t
runTestProg test_league_stats_t

if [ $FAIL -ne 0 ]; then

    echo
    printf "\033[1;31mThere were $FAILED test failures in $TESTS tests...\033[0m\n"
    echo

    exit
fi

echo
printf "\033[1;32m$TESTS TESTS COMPLETED SUCCESSFULLY\033[0m\n"
echo

exit
