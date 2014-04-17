#!/bin/bash
#

if [[ LeagName.Dat -nt ~/DOS/earlw1/LEAGNAME.DAT ||
      parks.dat    -nt ~/DOS/earlw1/PARKS.DAT    ||
      Players.S    -nt ~/DOS/earlw1/PLAYERS.DAT     ]];
then
    echo "WARNING: current files are newer than import files!";
    exit;
fi

echo "Copying files..."

~/Amiga/HD/MBA/ewbdos2amiga ~/DOS/earlw1/PLAYERS.DAT Players.S || exit
cp ~/DOS/earlw1/LEAGNAME.DAT LeagName.Dat
cp ~/DOS/earlw1/PARKS.DAT parks.dat

echo "Moving Boxscores..."

mv ~/DOS/earlw1/D*G*.TXT .
chmod 444 D*G*.TXT

echo "Creating Backups..."

cp Players.S .Players.S.bak
cp LeagName.Dat .LeagName.Dat.bak
cp parks.dat .parks.dat.bak
