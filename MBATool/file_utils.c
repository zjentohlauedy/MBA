#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "file_formats.h"


static char error_message[999 + 1] = { 0 };


static void clearErrorMessage( void )
{
     error_message[0] = '\0';
}


static void *readFile( const char *filename, const size_t filesize )
{
     void *filedata;
     int   fd;

     clearErrorMessage();

     if ( (filedata = malloc( filesize )) == NULL )
     {
	  sprintf( error_message, "Cannot allocate memory for reading file <%s>.", filename );

	  return NULL;
     }

     if ( (fd = open( filename, O_RDONLY )) < 0 )
     {
	  sprintf( error_message, "Cannot open file <%s>: %s", filename, strerror(errno) );

	  free( filedata );

	  return NULL;
     }

     if ( read( fd, filedata, filesize ) < filesize )
     {
	  sprintf( error_message, "Unexpected end of file <%s>.", filename );

	  free( filedata );

	  if ( close( fd ) < 0 )
	  {
	       sprintf( error_message, "Error closing file <%s>: %s", filename, strerror(errno) );
	  }

	  return NULL;
     }

     if ( close( fd ) < 0 )
     {
	  sprintf( error_message, "Error closing file <%s>: %s", filename, strerror(errno) );

          free( filedata );

	  return NULL;
     }

     return filedata;
}


static boolean_e writeFile( const char *filename, const void *filedata, const size_t filesize )
{
     int bytes_written;
     int fd;

     clearErrorMessage();

     if ( (fd = creat( filename, S_IRUSR | S_IWUSR )) < 0 )
     {
	  sprintf( error_message, "Cannot open file <%s>: %s", filename, strerror(errno) );

	  return bl_False;
     }

     if ( (bytes_written = write( fd, filedata, filesize )) < filesize )
     {
          if ( bytes_written < 0 )
          {
               sprintf( error_message, "Error writing to output file: %s", strerror( errno ) );

               if ( close( fd ) < 0 )
               {
                    sprintf( error_message, "Error closing output file: %s", strerror(errno) );
               }

               return bl_False;
          }

          sprintf( error_message, "Warning: incomplete buffer written to output file!" );
     }

     if ( close( fd ) < 0 )
     {
	  sprintf( error_message, "Error closing file <%s>: %s", filename, strerror(errno) );

	  return bl_False;
     }

     return bl_True;
}


char *getFileUtilsError( void )
{
     return error_message;
}


int word2int( const unsigned char *word )
{
     return (word[0]<<8) + word[1];
}


void int2word( unsigned char *word, const int value )
{
     word[0] = (value & 0x0000ff00)>>8;
     word[1] = (value & 0x000000ff);
}


int byte2int( const unsigned char *byte )
{
     return byte[0];
}

void int2byte( unsigned char *byte, const int value )
{
     byte[0] = value;
}

int nibble( const unsigned char byte, const nibble_e position )
{
     if   ( position == n_Low ) return byte & 0x0F;
     else                       return byte >> 4;
}


void termName( unsigned char *buffer, const int len )
{
     if ( buffer[len - 1] == '\0' ) buffer[len - 1] = ' ';

     buffer[len - 1] |= 0x80;
}


void untermName( unsigned char *buffer, const int len )
{
     buffer[len - 1] &= 0x7F;

     if ( buffer[len - 1] == 0x20 ) buffer[len - 1] = 0;
}


void capStat( unsigned char *stat, unsigned char *overflow )
{
     if ( *stat < STAT_CAP_AMOUNT ) return;

     unsigned char x = *stat - STAT_CAP_AMOUNT;

     *overflow += x;
     *stat     -= x;
}


int getPlayerId( const fileplayer_s *player )
{
     return word2int( player->acc_stats.action.id_info.player_id );
}


unsigned char calcChecksum( int value )
{
     unsigned char checksum = 0;

     while ( value > 0 )
     {
          checksum += value % 10;

          value /= 10;
     }

     return checksum * 3;
}


fileplayer_s *findMatchingPlayer( const fileplayer_s *player, fileplayer_s *players )
{
     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          if ( memcmp( player->first_name, players[i].first_name, sizeof(player->first_name) ) == 0 &&
               memcmp( player->last_name,  players[i].last_name,  sizeof(player->last_name)  ) == 0    )
          {
               return &players[i];
          }
     }

     return NULL;
}


fileplayer_s *readPlayersFile( const char *filename )
{
     return readFile( filename, sizeof(fileplayer_s) * TOTAL_PLAYERS );
}


boolean_e writePlayersFile( const char *filename, const fileplayer_s *players_file )
{
     return writeFile( filename, players_file, sizeof(fileplayer_s) * TOTAL_PLAYERS );
}


fileleagname_s *readLeagueFile( const char *filename )
{
     return readFile( filename, sizeof(fileleagname_s) );
}


boolean_e writeLeagueFile( const char *filename, const fileleagname_s *league_file )
{
     return writeFile( filename, league_file, sizeof(fileleagname_s) );
}


fileparks_s *readParksFile( const char *filename )
{
     return readFile( filename, sizeof(fileparks_s) );
}


boolean_e writeParksFile( const char *filename, const fileparks_s *parks_file )
{
     return writeFile( filename, parks_file, sizeof(fileleagname_s) );
}
