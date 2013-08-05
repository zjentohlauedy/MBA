#!/usr/bin/perl
#
# Reads filled in schedule.csv file and calculates
# Overall, Division and Head-To-Head records as well
# as scoring totals.

( scalar @ARGV == 1 ) || die "Usage: calc_records.pl <filename>\n";

my ($filename) = $ARGV[0];
my ($first_record) = 1;
my ($skip) = 0;
my ($input);
my (@road_fields, @home_fields);
my (%teams);


my (%divisions) = ( 'Sabres'    => 1, 'Scorpions' => 2, 'Aces'      => 3, 'Eclipse'   => 4,
		    'Portsmen'  => 1, 'Lightning' => 2, 'Cyclone'   => 3, 'Legends'   => 4,
		    'Settlers'  => 1, 'Goblins'   => 2, 'Stormers'  => 3, 'Waves'     => 4,
		    'Kings'     => 1, 'Photons'   => 2, 'Express'   => 3, 'Horizon'   => 4,
		    'Voyagers'  => 1, 'Dragons'   => 2, 'Warriors'  => 3, 'Sharks'    => 4,
		    'Rockets'   => 1, 'Hammers'   => 2, 'Wanderers' => 3, 'Flames'    => 4,
		    'Knights'   => 1, 'Expos'     => 2, 'Thunder'   => 3, 'Techs'     => 4,
		    'Drizzle'   => 1, 'Dynamo'    => 2, 'Glory'     => 3, 'Quasars'   => 4 );

sub update_entry {

#   my (%records) = %{ $_[0] };
    my ($entry)   =    $_[1];
    my ($wins)    =    $_[2];
    my ($losses)  =    $_[3];
    my (@record)  = (0, 0);

    if   ( exists( ${ $_[0] }{$entry} ) ) {

	@record = @{ ${ $_[0] }{$entry} };
    }

    $record[0] += $wins;
    $record[1] += $losses;

    ${ $_[0] }{$entry} = [ @record ];
}


sub update_team {

    my ($team, $scored, $opponent, $allowed, $home) = @_;
    my (%records) = ( );
    my ($won) = ($scored > $allowed) ? 1 : 0;
    my ($wins, $losses);

    # Get the records hash from the teams hash or use the empty one that team
    # is not in the table yet
    if   ( exists( $teams{$team} ) ) {

	%records = %{ $teams{$team} };
    }

    if   ( $won ) { $wins = 1; $losses = 0; }
    else          { $wins = 0; $losses = 1; }

    update_entry( \%records, "Record", $wins,   $losses  );
    update_entry( \%records, "Score",  $scored, $allowed );

    if ( $divisions{$team} == $divisions{$opponent} ) {

	update_entry( \%records, "Division", $wins,   $losses  );
    }

    if   ( $home ) { update_entry( \%records, "Home", $wins,   $losses  ); }
    else           { update_entry( \%records, "Road", $wins,   $losses  ); }

    update_entry( \%records, $opponent, $wins,   $losses  );

    $teams{$team} = { %records };
}


sub process_games {

    my (@road) = (@{$_[0]});
    my (@home) = (@{$_[1]});

    for ( $i = 1; $i < (scalar @home); $i += 2 ) {

	update_team( $road[$i], $road[$i + 1], $home[$i], $home[$i + 1], 0 );
	update_team( $home[$i], $home[$i + 1], $road[$i], $road[$i + 1], 1 );
    }
}


open( $input, "<$filename" ) || die "Cannot open input file <$filename>\n";

while ( <$input> ) {

    if ( $first_record ) {

	$first_record = 0;
    }
    else {

	chomp;

	if ( scalar @road_fields == 0 ) {

	    @road_fields = split( "," );
	}
	elsif ( scalar @home_fields == 0 ) {

	    @home_fields = split( "," );
	}

	if ( scalar @road_fields > 0  &&
	     scalar @home_fields > 0     ) {

	    # make sure every game has been played for this day
	    $skip = 0;

	    foreach $field (@home_fields) {

		if ( $field eq "" ) { $skip = 1 };
	    }

	    if ( $skip == 0 ) {

		process_games( \@road_fields, \@home_fields );
	    }

	    @road_fields = ( );
	    @home_fields = ( );
	}
    }
}

close( $input );

foreach $team (sort keys %teams) {

    my (@rec) = @{ $teams{$team}{'Record'} };
    my ($count);

    printf "%-9s %3d-%3d  ", $team, $rec[0], $rec[1];

    @rec = @{ $teams{$team}{'Division'} };

    printf "%2d-%2d  ", $rec[0], $rec[1];

    @rec = @{ $teams{$team}{'Home'} };

    printf "%2d-%2d  ", $rec[0], $rec[1];

    @rec = @{ $teams{$team}{'Road'} };

    printf "%2d-%2d  ", $rec[0], $rec[1];

    @rec = @{ $teams{$team}{'Score'} };

    printf "%4d  %4d\n", $rec[0], $rec[1];

    $count = 0;

    foreach $record (sort keys %{ $teams{$team} }) {

	if ( $record ne "Record"   &&
	     $record ne "Division" &&
	     $record ne "Home"     &&
	     $record ne "Road"     &&
	     $record ne "Score"       ) {

	    my (@totals) = @{ $teams{$team}{$record} };

	    printf "   %9s %1d-%1d", $record, $totals[0], $totals[1];

	    $count++;

	    if ( $count == 4 ) {

		print "\n";

		$count = 0;
	    }
	}
    }

    if ( $count > 0 ) { print "\n"; }

    print "\n";
}
