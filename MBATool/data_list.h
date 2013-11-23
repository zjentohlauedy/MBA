#ifndef __INC_DATA_LIST_H__
#define __INC_DATA_LIST_H__

typedef struct
{
     int   __max;
     int   count;
     void *data;

} data_list_s;

typedef struct
{
     char value[ 30 ];

} data_field_s;


int add_to_data_list( data_list_s *data_list, const void *data_item, const int data_size, const int increment );
void clear_data_list( data_list_s *data_list );

#endif
