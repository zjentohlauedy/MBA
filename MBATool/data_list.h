#ifndef __INC_DATA_LIST_H__
#define __INC_DATA_LIST_H__

typedef struct
{
     int   __max;
     int   count;
     void *data;

} data_list_s;


int add_to_data_list( data_list_s *data_list, void *data_item, int data_size, int increment );

#endif
