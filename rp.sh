#!/bin/bash
#
sed -E 's/ +/\t/g' $1 > /tmp/rate_players.txt

EXECPATH=$(dirname $0)

$EXECPATH/rate_players.rb /tmp/rate_players.txt $2 $3
