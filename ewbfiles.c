#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "ewbfiles.h"



int word2int( const unsigned char *word )
{
     return (word[0]<<8) + word[1];
}


void int2word( unsigned char *word, const int value )
{
     word[0] = (value & 0x0000ff00)>>8;
     word[1] = (value & 0x000000ff);
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


char *getColorName( const unsigned int color )
{
     switch ( color )
     {
     case cl_LtBlue: return "Lt. Blue";
     case cl_Blue:   return "Blue";
     case cl_DkRed:  return "Dark Red";
     case cl_Red:    return "Red";
     case cl_Gold:   return "Gold";
     case cl_Black:  return "Black";
     case cl_Brown:  return "Brown";
     case cl_Green:  return "Green";
     default:        return "Unknown";
     }
}


char *getParkColorName( const unsigned int color )
{
     switch ( color )
     {
     case pcl_DkBlue:  return "Dk. Blue";
     case pcl_Green:   return "Green";
     case pcl_DkGrey:  return "Dk. Grey";
     case pcl_DkGreen: return "Dk. Green";
     case pcl_Grey:    return "Grey";
     case pcl_LtBlue:  return "Lt. Blue";
     case pcl_Red:     return "Red";
     case pcl_DkRed:   return "Dk. Red";
     default:          return "Unknown";
     }
}


char *positionName( const int position )
{
     switch ( position )
     {
     case  0: return "P";
     case  1: return "C";
     case  2: return "1B";
     case  3: return "2B";
     case  4: return "3B";
     case  5: return "SS";
     case  6: return "IF";
     case  7: return "LF";
     case  8: return "CF";
     case  9: return "RF";
     case 10: return "OF";
     case 11: return "XX";
     case 12: return "DH";
     default: return "??";
     }
}


fileplayer_s *readPlayersFile( const char *filename )
{
     fileplayer_s *playersFile;
     size_t        filesize;
     int           fd;


     filesize = sizeof(fileplayer_s) * TOTAL_PLAYERS;

     if ( (playersFile = malloc( filesize )) == NULL )
     {
	  printf( "Cannot allocate memory for players file.\n" );

	  return NULL;
     }

     if ( (fd = open( filename, O_RDONLY )) < 0 )
     {
	  printf( "Cannot open file <%s>: %s\n", filename, strerror(errno) );

	  free( playersFile );

	  return NULL;
     }

     if ( read( fd, playersFile, filesize ) < filesize )
     {
	  printf( "Unexpected end of file <%s>.\n", filename );

	  free( playersFile );

	  if ( close( fd ) < 0 )
	  {
	       printf( "Error closing file <%s>: %s\n", filename, strerror(errno) );
	  }

	  return NULL;
     }

     if ( close( fd ) < 0 )
     {
	  printf( "Error closing file <%s>: %s\n", filename, strerror(errno) );

          free( playersFile );

	  return NULL;
     }

     return playersFile;
}


boolean_e writePlayersFile( const char *filename, const fileplayer_s *playersFile )
{
     size_t        filesize;
     int           bytes_written;
     int           fd;


     filesize = sizeof(fileplayer_s) * TOTAL_PLAYERS;

     if ( (fd = creat( filename, S_IRUSR | S_IWUSR )) < 0 )
     {
	  printf( "Cannot open file <%s>: %s\n", filename, strerror(errno) );

	  return bl_False;
     }

     if ( (bytes_written = write( fd, playersFile, filesize )) < filesize )
     {
          if ( bytes_written < 0 )
          {
               printf( "Error writing to output file: %s\n", strerror( errno ) );

               if ( close( fd ) < 0 )
               {
                    printf( "Error closing output file: %s\n", strerror(errno) );
               }

               return bl_False;
          }

          printf( "Warning: incomplete buffer written to output file!\n" );
     }

     if ( close( fd ) < 0 )
     {
	  printf( "Error closing file <%s>: %s\n", filename, strerror(errno) );

	  return bl_False;
     }

     return bl_True;
}


fileleague_s *readLeagueFile( const char *filename )
{
     fileleague_s *leagueFile;
     size_t        filesize;
     int           fd;


     filesize = sizeof(fileleague_s);

     if ( (leagueFile = malloc( filesize )) == NULL )
     {
	  printf( "Cannot allocate memory for league file.\n" );

	  return NULL;
     }

     if ( (fd = open( filename, O_RDONLY )) < 0 )
     {
	  printf( "Cannot open file <%s>: %s\n", filename, strerror(errno) );

          free( leagueFile );

	  return NULL;
     }

     if ( read( fd, leagueFile, filesize ) < filesize )
     {
	  printf( "Unexpected end of file <%s>.\n", filename );

	  if ( close( fd ) < 0 )
	  {
	       printf( "Error closing file <%s>: %s\n", filename, strerror(errno) );
	  }

	  return NULL;
     }

     if ( close( fd ) < 0 )
     {
	  printf( "Error closing file <%s>: %s\n", filename, strerror(errno) );

	  return NULL;
     }

     return leagueFile;
}


boolean_e writeLeagueFile( const char *filename, const fileleague_s *leagueFile )
{
     size_t        filesize;
     int           bytes_written;
     int           fd;


     filesize = sizeof(fileleague_s);

     if ( (fd = creat( filename, S_IRUSR | S_IWUSR )) < 0 )
     {
	  printf( "Cannot open file <%s>: %s\n", filename, strerror(errno) );

	  return bl_False;
     }

     if ( (bytes_written = write( fd, leagueFile, filesize )) < filesize )
     {
          if ( bytes_written < 0 )
          {
               printf( "Error writing to output file: %s\n", strerror( errno ) );

               if ( close( fd ) < 0 )
               {
                    printf( "Error closing output file: %s\n", strerror(errno) );
               }

               return bl_False;
          }

          printf( "Warning: incomplete buffer written to output file!\n" );
     }

     if ( close( fd ) < 0 )
     {
	  printf( "Error closing file <%s>: %s\n", filename, strerror(errno) );

	  return bl_False;
     }

     return bl_True;
}


fileparks_s *readParksFile( const char *filename )
{
     fileparks_s *parksFile;
     size_t       filesize;
     int          fd;


     filesize = sizeof(fileparks_s);

     if ( (parksFile = malloc( filesize )) == NULL )
     {
	  printf( "Cannot allocate memory for parks file.\n" );

	  return NULL;
     }

     if ( (fd = open( filename, O_RDONLY )) < 0 )
     {
	  printf( "Cannot open file <%s>: %s\n", filename, strerror(errno) );

          free( parksFile );

	  return NULL;
     }

     if ( read( fd, parksFile, filesize ) < filesize )
     {
	  printf( "Unexpected end of file <%s>.\n", filename );

	  if ( close( fd ) < 0 )
	  {
	       printf( "Error closing file <%s>: %s\n", filename, strerror(errno) );
	  }

	  return NULL;
     }

     if ( close( fd ) < 0 )
     {
	  printf( "Error closing file <%s>: %s\n", filename, strerror(errno) );

	  return NULL;
     }

     return parksFile;
}


boolean_e writeParksFile( const char *filename, const fileparks_s *parksFile )
{
     size_t        filesize;
     int           bytes_written;
     int           fd;


     filesize = sizeof(fileleague_s);

     if ( (fd = creat( filename, S_IRUSR | S_IWUSR )) < 0 )
     {
	  printf( "Cannot open file <%s>: %s\n", filename, strerror(errno) );

	  return bl_False;
     }

     if ( (bytes_written = write( fd, parksFile, filesize )) < filesize )
     {
          if ( bytes_written < 0 )
          {
               printf( "Error writing to output file: %s\n", strerror( errno ) );

               if ( close( fd ) < 0 )
               {
                    printf( "Error closing output file: %s\n", strerror(errno) );
               }

               return bl_False;
          }

          printf( "Warning: incomplete buffer written to output file!\n" );
     }

     if ( close( fd ) < 0 )
     {
	  printf( "Error closing file <%s>: %s\n", filename, strerror(errno) );

	  return bl_False;
     }

     return bl_True;
}
