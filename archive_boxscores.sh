#!/usr/bin/env bash
#
FILE_MATCHER="D[0-9][0-9][0-9]G[0-9][0-9].TXT"
BOXSCORES=($FILE_MATCHER)

n_uaem_files=$(ls -1 *.uaem 2>/dev/null | wc -l)
if [[ $n_uaem_files -gt 0 ]]
then
    echo "Removing uaem files"
    rm *.uaem
fi

if [[ "${BOXSCORES[0]}" == "$FILE_MATCHER" ]]
then
    echo "No boxscore files. Nothing to do here."
    exit
fi

GAMEDAY=$((10#${BOXSCORES[0]:1:3}))

MONTH=$(($GAMEDAY/30+1))

if [[ $(($GAMEDAY%30)) -eq 0 ]]
then
    MONTH=$(($MONTH-1))
fi

echo "Processing Day $GAMEDAY into Month $MONTH..."

if [[ $GAMEDAY -le 152 ]]
then
    if [[ ${#BOXSCORES[@]} -ne 16 ]]
    then
        echo "Incorrect number of boxscore files: ${#BOXSCORES[@]}."
        exit
    fi
fi

if [[ ! -d "M$MONTH" ]]
then
    mkdir "M$MONTH"
fi

chmod 444 $FILE_MATCHER
rename TXT txt $FILE_MATCHER

mv ${FILE_MATCHER//TXT/txt} "M$MONTH"

exit
