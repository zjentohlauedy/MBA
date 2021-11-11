#!/bin/bash
#
set -e

BASE_PATH=/home/zjentohlauedy/Amiga/HD/MBA
DOS_PATH=/home/zjentohlauedy/DOS/earlw1
SEASON_DIR=/home/zjentohlauedy/Amiga/HD/MBA/Current
LAST_STAGE=''

if [[ -e $SEASON_DIR/.last_stage ]]
then
    LAST_STAGE=`cat $SEASON_DIR/.last_stage`
else
    LAST_STAGE='Z'
fi

if [[ "$LAST_STAGE" == 'Z' ]]
then
    printf "\e[97;1mStarting from beginning...\e[0m\n"
    LAST_STAGE='0'
    echo $LAST_STAGE > $SEASON_DIR/.last_stage
else
    printf "\e[97;1mRecovering from stage $LAST_STAGE\e[0m\n"
fi

printf "\e[97;1mSwitching to DOS...\e[0m\n"
cd $DOS_PATH

if [[ "$LAST_STAGE" == '0' ]]
then
    printf "\e[97;1mImporting data...\e[0m\n"

    ./import_amiga.sh

    LAST_STAGE='1'
    echo $LAST_STAGE > $SEASON_DIR/.last_stage
fi


PLAYOFFS=$(test $($BASE_PATH/next_game_day.rb $SEASON_DIR/schedule.csv 2>&1 >/dev/null | cut -c 15-) -gt 152)

if [[ $PLAYOFFS -eq 0 ]]
then
    echo "This is the playoffs, skipping steps..."
fi


if [[ "$LAST_STAGE" == '1' ]]
then
    if [[ ! $PLAYOFFS -eq 0 ]]
    then
        read -p "Ready to launch? " RESPONSE

        if [[ "$RESPONSE" != "y" ]]
        then
            exit 1
        fi
    fi

    LAST_STAGE='2'
    echo $LAST_STAGE > $SEASON_DIR/.last_stage
fi


if [[ "$LAST_STAGE" == '2' ]]
then
    if [[ ! $PLAYOFFS -eq 0 ]]
    then
        printf "\e[97;1mLaunching DOSBOX instances...\e[0m\n"
        ./launch_ewb.sh
    fi

    LAST_STAGE='3'
    echo $LAST_STAGE > $SEASON_DIR/.last_stage
fi


if [[ "$LAST_STAGE" == '3' ]]
then
    if [[ ! $PLAYOFFS -eq 0 ]]
    then
        RESPONSE=""

        read -p "Hit Enter when games have been played" RESPONSE
    fi

    LAST_STAGE='4'
    echo $LAST_STAGE > $SEASON_DIR/.last_stage
fi


if [[ "$LAST_STAGE" == '4' ]]
then
    if [[ ! $PLAYOFFS -eq 0 ]]
    then
        RESPONSE=""

        printf "\e[97;1mChecking games...\e[0m\n"
        GAMEDAY=`$BASE_PATH/next_game_day.rb $SEASON_DIR/schedule.csv 2>&1 >/dev/null | cut -c 15-`

        while [[ "$RESPONSE" != "y" ]]
        do
            ./check_games LEAGNAME.DAT $GAMEDAY

            read -p "Remaining games correct? " RESPONSE

            if [[ "$RESPONSE" == "x" ]]
            then
                exit 0
            fi
        done
    fi

    LAST_STAGE='5'
    echo $LAST_STAGE > $SEASON_DIR/.last_stage
fi


if [[ "$LAST_STAGE" == '5' ]]
then
    if [[ ! $PLAYOFFS -eq 0 ]]
    then
        printf "\e[97;1mKilling DOSBOX instances...\e[0m\n"
        killall dosbox
    fi

    LAST_STAGE='6'
    echo $LAST_STAGE > $SEASON_DIR/.last_stage
fi

printf "\e[97;1mSwitching to Amiga...\e[0m\n"
cd $SEASON_DIR

if [[ "$LAST_STAGE" == '6' ]]
then
    printf "\e[97;1mImporting data...\e[0m\n"

    ./import_dos.sh

    LAST_STAGE='7'
    echo $LAST_STAGE > $SEASON_DIR/.last_stage
fi


if [[ "$LAST_STAGE" == '7' ]]
then
    ls --color -F -T 0

    RESPONSE=""

    read -p "Ready to launch? " RESPONSE

    if [[ "$RESPONSE" != "y" ]]
    then
        exit 1
    fi

    LAST_STAGE='8'
    echo $LAST_STAGE > $SEASON_DIR/.last_stage
fi


if [[ "$LAST_STAGE" == '8' ]]
then
    printf "\e[97;1mLaunching UAE instance...\e[0m\n"
    ./launch_ewb.sh

    LAST_STAGE='9'
    echo $LAST_STAGE > $SEASON_DIR/.last_stage
fi


if [[ "$LAST_STAGE" == '9' ]]
then
    RESPONSE=""

    read -p "Hit Enter when games have been played" RESPONSE

    LAST_STAGE='A'
    echo $LAST_STAGE > $SEASON_DIR/.last_stage
fi


if [[ "$LAST_STAGE" == 'A' ]]
then
    printf "\e[97;1mArchiving boxscores...\e[0m\n"
    $BASE_PATH/archive_boxscores.sh

    LAST_STAGE='Z'
    echo $LAST_STAGE > $SEASON_DIR/.last_stage
fi


printf "\e[97;1mEnd of script.\e[0m\n"
exit 0
