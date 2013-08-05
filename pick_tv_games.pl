#!/usr/bin/perl
#
# Reads empty in schedule.csv file and calculates
# which games to televise for a balanced tv season.

( scalar @ARGV == 1 ) || die "Usage: pick_tv_games.pl <filename>\n";

my ($filename) = $ARGV[0];
my (%teams);
my (@picked_games);
my (@scheduled);
my (@televised);


sub contains {

    my (@array)  = @{ $_[0] };
    my ($search) =    $_[1];

    foreach $element (@array) {

        if ( $element eq $search ) {

            return 1;
        }
    }

    return 0;
}


sub balanced {

    my (%teams) = %{ $_[0] };

    foreach $team (keys %teams) {

        @record = @{ $teams{$team} };

        if ( $record[0] != 4  || $record[1] != 4 ) {

            return 0;
        }
    }

    return 1;
}


sub update_televised_games {

    foreach $entry (@picked_games) {

        my ($index) = $entry->[0] - 1;
        my (@game) = ( $entry->[1], $entry->[2] );

        push @{ $televised[$index] }, \@game;
    }
}


sub print_televised_games {

    foreach $tvday (@televised) {

        foreach $game (@{ $tvday }) {

            if ( ref($game) eq "ARRAY" ) {

                printf "%s @ %s, ", @{ $game }[0], @{ $game }[1];
            }
            else {

                print "Games for Day $game: ";
            }
        }

        print "\n";
    }
}


sub parse_schedule_file {

    my ($input);
    my ($first_record) = 1;
    my ($skip) = 0;
    my (@road_fields, @home_fields);
    my (@home_openers);

    open( $input, "<$filename" ) || die "Cannot open input file <$filename>\n";

    while ( <$input> ) {

        # skip first record which is only column numbers
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

                my (@gameday) = ( );
                my (@tvday)   = ( );

                push @gameday, $home_fields[0];
                push @tvday,   $home_fields[0];

                for ( $i = 1; $i < (scalar @home_fields); $i += 2 ) {

                    my (@game) = ( $road_fields[$i], $home_fields[$i] );

                    push @gameday, \@game;

                    if ( ! contains( \@home_openers, $home_fields[$i] ) ) {

                        push @tvday, \@game;
                        push @home_openers, $home_fields[$i];
                    }
                }

                push @scheduled, \@gameday;
                push @televised, \@tvday;

                @road_fields = ( );
                @home_fields = ( );
            }
        }
    }

    close( $input );
}


sub was_game_selected {

    my (@game)     = @{ $_[0] };
    my ($selected) = 0;

    if ( ! exists $teams{$game[0]} ) {

        if ( ! exists $teams{$game[1]} ) {

            $teams{$game[0]} = [ 1, 0 ];
            $teams{$game[1]} = [ 0, 1 ];
            $selected = 1;

            my @x = ( $index + 1, @game );

            push @picked_games, \@x;
        }
        else {

            $record = $teams{$game[1]};

            if ( $record->[1] < 4 ) {

                $teams{$game[0]} = [ 1, 0 ];
                $record->[1] += 1;
                $selected = 1;

                my @x = ( $index + 1, @game );

                push @picked_games, \@x;
            }
        }
    }
    else {

        $record = $teams{$game[0]};

        if ( $record->[0] < 4 ) {

            if ( ! exists $teams{$game[1]} ) {

                $teams{$game[1]} = [ 0, 1 ];
                $record->[0] += 1;
                $selected = 1;

                my @x = ( $index + 1, @game );

                push @picked_games, \@x;
            }
            else {

                $record2 = $teams{$game[1]};

                if ( $record2->[1] < 4 ) {

                    $record->[0] += 1;
                    $record2->[1] += 1;
                    $selected = 1;

                    my @x = ( $index + 1, @game );

                    push @picked_games, \@x;
                }
            }
        }
    }

    return $selected;
}


sub pick_tv_games {

    @picked_games = ( );

    for (keys %teams)
    {
        delete $teams{$_};
    }

    foreach $gameday (@scheduled) {

        $index = @{ $gameday }[0] - 1;

        if ( (scalar @{ $televised[$index] }) == 1 ) {

            $selected = 0;

            $random_pick = int(rand(scalar @{ $gameday }));

            for ( $i = 1; $i < (scalar @{ $gameday })  &&  ! $selected; $i += 1 ) {

                $random_pick += 1;

                if ( $random_pick >= (scalar @{ $gameday }) ) {

                    $random_pick = 1;
                }

                @game = @{ $gameday->[$random_pick] };

                $selected = was_game_selected( \@game );
            }
        }
    }
}


parse_schedule_file();

do {

    pick_tv_games();

} while ( ! balanced( \%teams ) );

update_televised_games();
print_televised_games();

exit;
