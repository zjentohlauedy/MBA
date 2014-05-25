#!/usr/bin/env bash
#
FILE_MATCHER="D[0-9][0-9][0-9]G[0-9][0-9].TXT"
BOXSCORES=($FILE_MATCHER)

echo "Removing uaem files"
rm *.uaem

if [[ "${BOXSCORES[0]}" == "$FILE_MATCHER" ]]
then
    echo "No boxscore files. Nothing to do here."
    exit
elif [[ ${#BOXSCORES[@]} -ne 16 ]]
then
    echo "Incorrect number of files: $BOXSCORE_COUNT."
    exit
fi

GAMEDAY=$((10#${BOXSCORES[0]:1:3}))

MONTH=$(($GAMEDAY/30+1))

if [[ $(($GAMEDAY%30)) -eq 0 ]]
then
    MONTH=$(($MONTH-1))
fi

echo "Processing Day $GAMEDAY into Month $MONTH..."

if [[ ! -d "M$MONTH" ]]
then
    mkdir "M$MONTH"
fi

chmod 444 $FILE_MATCHER
rename TXT txt $FILE_MATCHER

mv ${FILE_MATCHER//TXT/txt} "M$MONTH"

exit
