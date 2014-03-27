#!/bin/bash
#

if [[ LEAGNAME.DAT -nt ~/Amiga/HD/MBA/S03/LeagName.Dat ||
      PARKS.DAT    -nt ~/Amiga/HD/MBA/S03/parks.dat    ||
      PLAYERS.DAT  -nt ~/Amiga/HD/MBA/S03/Players.S       ]];
then
    echo "WARNING: current files are newer than import files!";
    exit;
fi

soffice -env:UserInstallation=file://~/.libreoffice-alt --headless --convert-to csv --outdir ~/Amiga/HD/MBA ~/Amiga/HD/MBA/S03/schedule.ods

if [[ ~/Amiga/HD/MBA/setgames -nt ~/Amiga/HD/MBA/schedule.csv ]]
then
    echo "WARNING: schedule.csv hasn't been updated!";
    exit;
fi

echo "Parsing schedule..."

~/Amiga/HD/MBA/next_game_day.rb ~/Amiga/HD/MBA/schedule.csv > ~/Amiga/HD/MBA/setgames

source ~/Amiga/HD/MBA/setgames

echo "Copying files..."

~/Amiga/HD/MBA/ewbcopyleague ~/Amiga/HD/MBA/S03/LeagName.Dat LEAGNAME.DAT
cp ~/Amiga/HD/MBA/S03/parks.dat PARKS.DAT
~/Amiga/HD/MBA/ewbamiga2dos ~/Amiga/HD/MBA/S03/Players.S PLAYERS.DAT

