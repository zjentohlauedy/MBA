#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "file_formats.h"
#include "unit_test.h"


static char *result;
static char  players_file_name [50];
static char  league_file_name  [50];
static char  parks_file_name   [50];


static char *word2int_ShouldReturnTheConvertedIntValue_GivenATwoByteAmigaWord()
{
     char word1[2] = {  0,  0 };
     char word2[2] = {  5,  0 };
     char word3[2] = {  0,  5 };
     char word4[2] = {  5,  5 };
     char word5[2] = { 28, 47 };

     assertEquals(    0, word2int( word1 ) );
     assertEquals( 1280, word2int( word2 ) );
     assertEquals(    5, word2int( word3 ) );
     assertEquals( 1285, word2int( word4 ) );
     assertEquals( 7215, word2int( word5 ) );

     return NULL;
}

static char *int2word_ShouldSetTheAmigaWordValue_GivenAnAmigaWordAndAnIntToConvert()
{
     char word[2] = { 0, 0 };

     int2word( word, 12345 );

     assertEquals( 48, word[0] );
     assertEquals( 57, word[1] );

     return NULL;
}

static char *byte2int_ShouldReturnTheIntValue_GivenAPointerToAnUnsignedChar()
{
     char byte[1] = { 0 };

     byte[0] = 123;

     assertEquals( 123, byte2int( byte ) );

     return NULL;
}

static char *int2byte_ShouldSetTheByteValue_GivenABytePointerAndAnIntToConvert()
{
     char byte[1] = { 0 };
     int  i       = 123;

     int2byte( byte, i );

     assertEquals( 123, byte[0] );

     return NULL;
}

static char *nibble_ShouldReturnTheValueOfTheLowNibble_GivenAByteAndLowPosition()
{
     assertEquals(  0, nibble( 0xF0, n_Low ) );
     assertEquals( 12, nibble( 0xCC, n_Low ) );
     assertEquals(  8, nibble( 0x08, n_Low ) );
     assertEquals( 11, nibble( 0xAB, n_Low ) );
     assertEquals(  4, nibble( 0x54, n_Low ) );

     return NULL;
}

static char *nibble_ShouldReturnTheValueOfTheHighNibble_GivenAByteAndHighPosition()
{
     assertEquals( 15, nibble( 0xF0, n_High ) );
     assertEquals( 12, nibble( 0xCC, n_High ) );
     assertEquals(  0, nibble( 0x08, n_High ) );
     assertEquals( 10, nibble( 0xAB, n_High ) );
     assertEquals(  5, nibble( 0x54, n_High ) );

     return NULL;
}

static char *termName_ShouldConvertTheLastByteToATerminator_GivenAPlayerNameBuffer()
{
     unsigned char player_name1[4] = { 'N', 'a', 'm',  'e' };
     unsigned char player_name2[4] = { 'N', 'a', 'm', '\0' };

     termName( player_name1, sizeof(player_name1) );
     termName( player_name2, sizeof(player_name2) );

     assertEquals( 0xE5, player_name1[3] );
     assertEquals( 0xA0, player_name2[3] );

     return NULL;
}

static char *untermName_ShouldRemoveTerminatorFromLastByte_GivenAPlayerNameBuffer()
{
     unsigned char player_name1[4] = { 'N', 'a', 'm', 0xE5 };
     unsigned char player_name2[4] = { 'N', 'a', 'm', 0xA0 };

     untermName( player_name1, sizeof(player_name1) );
     untermName( player_name2, sizeof(player_name2) );

     assertEquals(  'e', player_name1[3] );
     assertEquals( '\0', player_name2[3] );

     return NULL;
}

static char *capStat_ShouldMoveExcessAmountFromStatToOverflow_GivenAStatAndOverflow()
{
     unsigned char stat     = STAT_CAP_AMOUNT + 7;
     unsigned char overflow = 18;

     capStat( &stat, &overflow );

     assertEquals( STAT_CAP_AMOUNT, stat     );
     assertEquals( 25,              overflow );

     return NULL;
}

static char *calcChecksum_ShouldCalculateProperChecksum_GivenAPlayerId()
{
     assertEquals(   0, calcChecksum(     0 ) );
     assertEquals(   3, calcChecksum(     1 ) );
     assertEquals(  45, calcChecksum(    69 ) );
     assertEquals(  18, calcChecksum(   123 ) );
     assertEquals(  18, calcChecksum(   420 ) );
     assertEquals(  81, calcChecksum(   999 ) );
     assertEquals( 123, calcChecksum( 59999 ) );
     assertEquals(  72, calcChecksum( 65535 ) );

     return NULL;
}

static char *readPlayersFile_ShouldReturnAPointerToAFilePlayerObject_GivenAFilename()
{
     assertNotNull( readPlayersFile( players_file_name ) );

     return NULL;
}

static char *writePlayersFile_ShouldCreateAPlayersFile_GivenAPlayersObjectAndFilename()
{
     fileplayer_s players_file  [TOTAL_PLAYERS] = { 0 };
     char         temp_file_name[999 + 1]       = { 0 };

     sprintf( temp_file_name, "%s.tmp", players_file_name );

     assertEquals( bl_True, writePlayersFile( temp_file_name, players_file ) );

     assertEquals( 0, access( temp_file_name, F_OK ) );

     unlink( temp_file_name );

     return NULL;
}

static char *readLeagueFile_ShouldReturnAPointerToAFileLeagueObject_GivenAFilename()
{
     assertNotNull( readLeagueFile( league_file_name ) );

     return NULL;
}

static char *writeLeagueFile_ShouldCreateALeagueFile_GivenALeagueObjectAndFilename()
{
     fileleagname_s league_file             = { 0 };
     char           temp_file_name[999 + 1] = { 0 };

     sprintf( temp_file_name, "%s.tmp", league_file_name );

     assertEquals( bl_True, writeLeagueFile( temp_file_name, &league_file ) );

     assertEquals( 0, access( temp_file_name, F_OK ) );

     unlink( temp_file_name );

     return NULL;
}

static char *readParksFile_ShouldReturnAPointerToAFileParksObject_GivenAFilename()
{
     assertNotNull( readParksFile( parks_file_name ) );

     return NULL;
}

static char *writeParksFile_ShouldCreateAParksFile_GivenAParksObjectAndFilename()
{
     fileparks_s  parks_file              = { 0 };
     char         temp_file_name[999 + 1] = { 0 };

     sprintf( temp_file_name, "%s.tmp", parks_file_name );

     assertEquals( bl_True, writeParksFile( temp_file_name, &parks_file ) );

     assertEquals( 0, access( temp_file_name, F_OK ) );

     unlink( temp_file_name );

     return NULL;
}

static void check_file_utils_error()
{
     printf( "file utils error: %s\n", getFileUtilsError() );
}

static void run_all_tests()
{
     run_test( word2int_ShouldReturnTheConvertedIntValue_GivenATwoByteAmigaWord,       null );
     run_test( int2word_ShouldSetTheAmigaWordValue_GivenAnAmigaWordAndAnIntToConvert,  null );
     run_test( byte2int_ShouldReturnTheIntValue_GivenAPointerToAnUnsignedChar,         null );
     run_test( int2byte_ShouldSetTheByteValue_GivenABytePointerAndAnIntToConvert,      null );
     run_test( nibble_ShouldReturnTheValueOfTheLowNibble_GivenAByteAndLowPosition,     null );
     run_test( nibble_ShouldReturnTheValueOfTheHighNibble_GivenAByteAndHighPosition,   null );
     run_test( termName_ShouldConvertTheLastByteToATerminator_GivenAPlayerNameBuffer,  null );
     run_test( untermName_ShouldRemoveTerminatorFromLastByte_GivenAPlayerNameBuffer,   null );
     run_test( capStat_ShouldMoveExcessAmountFromStatToOverflow_GivenAStatAndOverflow, null );
     run_test( calcChecksum_ShouldCalculateProperChecksum_GivenAPlayerId,              null );

     run_test( readPlayersFile_ShouldReturnAPointerToAFilePlayerObject_GivenAFilename,   check_file_utils_error );
     run_test( writePlayersFile_ShouldCreateAPlayersFile_GivenAPlayersObjectAndFilename, check_file_utils_error );
     run_test( readLeagueFile_ShouldReturnAPointerToAFileLeagueObject_GivenAFilename,    check_file_utils_error );
     run_test( writeLeagueFile_ShouldCreateALeagueFile_GivenALeagueObjectAndFilename,    check_file_utils_error );
     run_test( readParksFile_ShouldReturnAPointerToAFileParksObject_GivenAFilename,      check_file_utils_error );
     run_test( writeParksFile_ShouldCreateAParksFile_GivenAParksObjectAndFilename,       check_file_utils_error );
}


int main( int argc, char *argv[] )
{
     if ( argc < 2 )
     {
          printf( "Must supply a path.\n" );

          return EXIT_FAILURE;
     }

     sprintf( players_file_name, "%s/%s", argv[1], "Players_test.S"    );
     sprintf( league_file_name,  "%s/%s", argv[1], "LeagName_test.Dat" );
     sprintf( parks_file_name,   "%s/%s", argv[1], "parks_test.dat"    );

     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
