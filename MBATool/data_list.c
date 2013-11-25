#include <stdlib.h>
#include <string.h>
#include "data_list.h"

int add_to_data_list( data_list_s *data_list, const void *data_item, const int data_size, const int increment )
{
     if ( data_list->count == data_list->__max )
     {
          void *new_block = NULL;

          data_list->__max += increment;

          new_block = realloc( data_list->data, data_size * data_list->__max );

          if ( new_block == NULL )
          {
               free( data_list->data );

               data_list->count = 0;
               data_list->__max = 0;
               data_list->data  = NULL;

               return -1;
          }

          memset( new_block + (data_size * data_list->count), '\0', data_size * (data_list->__max - data_list->count) );

          data_list->data = new_block;
     }

     memcpy( data_list->data + (data_size * data_list->count), data_item, data_size );

     data_list->count++;

     return 0;
}

void clear_data_list( data_list_s *data_list )
{
     if ( data_list->data != NULL ) free( data_list->data );

     data_list->data  = NULL;
     data_list->__max = 0;
     data_list->count = 0;
}
