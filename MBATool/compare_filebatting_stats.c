#include "file_formats.h"
#include "unit_test.h"

char *_compareBattingStats( acc_bat_stats_s *expected, acc_bat_stats_s *actual )
{
     assertEquals( word2int( expected->acc_ab    ), word2int( actual->acc_ab    ) );
     assertEquals( word2int( expected->acc_hits  ), word2int( actual->acc_hits  ) );
     assertEquals( byte2int( expected->acc_2b    ), byte2int( actual->acc_2b    ) );
     assertEquals( byte2int( expected->acc_3b    ), byte2int( actual->acc_3b    ) );
     assertEquals( byte2int( expected->acc_hr    ), byte2int( actual->acc_hr    ) );
     assertEquals( byte2int( expected->acc_bb    ), byte2int( actual->acc_bb    ) );
     assertEquals( byte2int( expected->acc_so    ), byte2int( actual->acc_so    ) );
     assertEquals( byte2int( expected->acc_games ), byte2int( actual->acc_games ) );
     assertEquals( byte2int( expected->acc_runs  ), byte2int( actual->acc_runs  ) );
     assertEquals( byte2int( expected->acc_rbi   ), byte2int( actual->acc_rbi   ) );
     assertEquals( byte2int( expected->acc_sb    ), byte2int( actual->acc_sb    ) );
     assertEquals( byte2int( expected->acc_err   ), byte2int( actual->acc_err   ) );

     return NULL;
}
