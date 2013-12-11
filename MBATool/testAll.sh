#!/usr/bin/env bash
#

TESTARGS=""

DBFILE="mba_test.db"

FAIL=0
TESTS=0
PASSED=0
FAILED=0

function runTestProg {

    re="^.*Tests Run: ([0-9]+), Passed: ([0-9]+), Failed: ([0-9]+).*$"

    echo
    printf "%s:\n" $1
    RESULTS=$( $1 $TESTARGS )

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

make TESTS

if [ $? -ne 0 ]; then exit; fi

echo
echo

TESTARGS=$DBFILE

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
runTestProg test_league_accolades_t
runTestProg test_league_teams_t

runTestProg test_player_service
runTestProg test_team_service
runTestProg test_division_service
runTestProg test_league_service
runTestProg test_org_service

TESTARGS="."

runTestProg test_file_utils

TESTARGS=

runTestProg test_convert_org
runTestProg test_convert_leagues
runTestProg test_convert_divisions
runTestProg test_convert_teams
runTestProg test_convert_players
runTestProg test_gen_player_ids
runTestProg test_copy_player_ids
runTestProg test_set_longevity
runTestProg test_copy_longevity
runTestProg test_format_amiga_to_dos
runTestProg test_format_dos_to_amiga
runTestProg test_parse_csv
runTestProg test_calc_records
runTestProg test_generate_file_leagname
runTestProg test_generate_file_players

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
