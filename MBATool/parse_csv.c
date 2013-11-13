#include <stdio.h>
#include "file_formats.h"


#define IS_EOL( A ) ((A) == '\0' || (A) == '\n')


static boolean_e addField( data_list_s *list, const char *start, const char *end )
{
     data_field_s field = { 0 };

     snprintf( field.value, sizeof(field.value), "%.*s", end - start, start );

     if ( add_to_data_list( list, &field, sizeof(data_field_s), 50 ) < 0 ) return bl_False;

     return bl_True;
}

boolean_e parseCSV( data_list_s *list, const char *csv )
{
     const char *start;
     const char *end;

     boolean_e another_field = bl_True;

     for ( start = csv, end = csv; ! IS_EOL( *start ); start = end )
     {
          another_field = bl_False;

          while ( *end != ','  && ! IS_EOL( *end ) ) end++;

          if ( ! addField( list, start, end ) ) return bl_False;

          if ( *end == ',' )
          {
               another_field = bl_True;

               end++;
          }
     }

     if ( another_field )
     {
          if ( ! addField( list, start, end ) ) return bl_False;
     }

     return bl_True;
}
