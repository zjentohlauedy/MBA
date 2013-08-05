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


sub process_games {

    my (@road) = (@{$_[0]});
    my (@home) = (@{$_[1]});
    my ($team, $runs);

    for ( $i = 1; $i < (scalar @home); $i += 2 ) {

	$team = $home[$i];
	$runs = 0;

	if   ( exists( $teams{$team} ) ) {

	    $runs = $teams{$team};
	}

	$runs += $home[$i + 1] + $road[$i + 1];

	$teams{$team} = $runs;
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

    printf "%9s %4d\n", $team, $teams{$team};
}
