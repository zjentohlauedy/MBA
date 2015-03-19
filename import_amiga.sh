#!/bin/bash
#
SEASON_DIR=S04

if [[ LEAGNAME.DAT -nt ~/Amiga/HD/MBA/$SEASON_DIR/LeagName.Dat ||
      PARKS.DAT    -nt ~/Amiga/HD/MBA/$SEASON_DIR/parks.dat    ||
      PLAYERS.DAT  -nt ~/Amiga/HD/MBA/$SEASON_DIR/Players.S       ]];
then
    echo "WARNING: current files are newer than import files!";
    exit;
fi

soffice -env:UserInstallation=file://~/.libreoffice-alt --headless --convert-to csv --outdir ~/Amiga/HD/MBA ~/Amiga/HD/MBA/$SEASON_DIR/schedule.ods

if [[ ~/Amiga/HD/MBA/setgames -nt ~/Amiga/HD/MBA/schedule.csv ]]
then
    echo "WARNING: schedule.csv hasn't been updated!";
    exit;
fi

echo "Parsing schedule..."

~/Amiga/HD/MBA/next_game_day.rb ~/Amiga/HD/MBA/schedule.csv > ~/Amiga/HD/MBA/setgames

source ~/Amiga/HD/MBA/setgames

echo "Copying files..."

~/Amiga/HD/MBA/ewbcopyleague ~/Amiga/HD/MBA/$SEASON_DIR/LeagName.Dat LEAGNAME.DAT || exit
cp ~/Amiga/HD/MBA/$SEASON_DIR/parks.dat PARKS.DAT
~/Amiga/HD/MBA/ewbamiga2dos ~/Amiga/HD/MBA/$SEASON_DIR/Players.S PLAYERS.DAT || exit

echo "Making Backups..."

cp LEAGNAME.DAT .LEAGNAME.DAT.bak
cp PARKS.DAT .PARKS.DAT.bak
cp PLAYERS.DAT .PLAYERS.DAT.bak


n_boxscores=$(ls -1 D*G*.TXT 2>/dev/null | wc -l)
if [[ $n_boxscores -gt 0 ]]
then
    echo "Removing Boxscores..."

    rm D*G*.TXT
fi
