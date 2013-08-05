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


open( $input, "<$filename" ) || die "Cannot open input file <$filename>\n";

while ( <$input> ) {

    if ( $first_record ) {

	$first_record = 0;
    }
    elsif ( $skip == 0 ) {

	chomp;

	if ( scalar @road_fields == 0 ) {

	    @road_fields = split( "," );
	}
	elsif ( scalar @home_fields == 0 ) {

	    @home_fields = split( "," );
	}

	if ( scalar @road_fields > 0  &&
	     scalar @home_fields > 0     ) {

	    foreach $field (@home_fields) {

		if ( $field eq "" ) {

		    $skip = 1;
		};
	    }

	    if ( $skip == 0 ) {

		@road_fields = ( );
		@home_fields = ( );
	    }
	}
    }
}

close( $input );

printf STDERR "Next Gameday: %03d\n", $home_fields[0];

printf "export GAMES='";

for ( $i = 1; $i < (scalar @home_fields); $i += 2 ) {

    printf "%s %s ", $road_fields[$i], $home_fields[$i];
}

printf "'\n";
