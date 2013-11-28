#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "file_formats.h"
#include "org.h"
#include "unit_test.h"

static char *result;


static char *calculateRecords_ShouldReturnARecordsObject_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     return NULL;
}

static char *calculateRecords_ShouldReturnNullIfScheduleIsEmpty_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s      schedule    = { 0 };

     records_s *records = calculateRecords( &schedule, league_file, 4, sp_Regular );

     assertNull( records );

     return NULL;
}

static char *calculateRecords_ShouldReturnNullIfRoadTeamNotFound_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     strcpy( schedule->days[0].games[0].road.name, "Invalid" );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNull( records );

     return NULL;
}

static char *calculateRecords_ShouldReturnNullIfHomeTeamNotFound_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     strcpy( schedule->days[0].games[0].home.name, "Invalid" );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNull( records );

     return NULL;
}

static char *calculateRecords_ShouldSetTeamRecords_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals(  0, records->teams[ 0].wins   );
     assertEquals( 31, records->teams[ 0].losses );
     assertEquals(  1, records->teams[ 1].wins   );
     assertEquals( 30, records->teams[ 1].losses );
     assertEquals(  2, records->teams[ 2].wins   );
     assertEquals( 29, records->teams[ 2].losses );
     assertEquals(  3, records->teams[ 3].wins   );
     assertEquals( 28, records->teams[ 3].losses );
     assertEquals(  4, records->teams[ 4].wins   );
     assertEquals( 27, records->teams[ 4].losses );
     assertEquals(  5, records->teams[ 5].wins   );
     assertEquals( 26, records->teams[ 5].losses );
     assertEquals(  6, records->teams[ 6].wins   );
     assertEquals( 25, records->teams[ 6].losses );
     assertEquals(  7, records->teams[ 7].wins   );
     assertEquals( 24, records->teams[ 7].losses );
     assertEquals(  8, records->teams[ 8].wins   );
     assertEquals( 23, records->teams[ 8].losses );
     assertEquals(  9, records->teams[ 9].wins   );
     assertEquals( 22, records->teams[ 9].losses );
     assertEquals( 10, records->teams[10].wins   );
     assertEquals( 21, records->teams[10].losses );
     assertEquals( 11, records->teams[11].wins   );
     assertEquals( 20, records->teams[11].losses );
     assertEquals( 12, records->teams[12].wins   );
     assertEquals( 19, records->teams[12].losses );
     assertEquals( 13, records->teams[13].wins   );
     assertEquals( 18, records->teams[13].losses );
     assertEquals( 14, records->teams[14].wins   );
     assertEquals( 17, records->teams[14].losses );
     assertEquals( 15, records->teams[15].wins   );
     assertEquals( 16, records->teams[15].losses );
     assertEquals( 16, records->teams[16].wins   );
     assertEquals( 15, records->teams[16].losses );
     assertEquals( 17, records->teams[17].wins   );
     assertEquals( 14, records->teams[17].losses );
     assertEquals( 18, records->teams[18].wins   );
     assertEquals( 13, records->teams[18].losses );
     assertEquals( 19, records->teams[19].wins   );
     assertEquals( 12, records->teams[19].losses );
     assertEquals( 20, records->teams[20].wins   );
     assertEquals( 11, records->teams[20].losses );
     assertEquals( 21, records->teams[21].wins   );
     assertEquals( 10, records->teams[21].losses );
     assertEquals( 22, records->teams[22].wins   );
     assertEquals(  9, records->teams[22].losses );
     assertEquals( 23, records->teams[23].wins   );
     assertEquals(  8, records->teams[23].losses );
     assertEquals( 24, records->teams[24].wins   );
     assertEquals(  7, records->teams[24].losses );
     assertEquals( 25, records->teams[25].wins   );
     assertEquals(  6, records->teams[25].losses );
     assertEquals( 26, records->teams[26].wins   );
     assertEquals(  5, records->teams[26].losses );
     assertEquals( 27, records->teams[27].wins   );
     assertEquals(  4, records->teams[27].losses );
     assertEquals( 28, records->teams[28].wins   );
     assertEquals(  3, records->teams[28].losses );
     assertEquals( 29, records->teams[29].wins   );
     assertEquals(  2, records->teams[29].losses );
     assertEquals( 30, records->teams[30].wins   );
     assertEquals(  1, records->teams[30].losses );
     assertEquals( 31, records->teams[31].wins   );
     assertEquals(  0, records->teams[31].losses );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetTeamHomeRecords_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals(  0, records->teams[ 0].home_wins   );
     assertEquals( 15, records->teams[ 0].home_losses );
     assertEquals(  1, records->teams[ 1].home_wins   );
     assertEquals( 14, records->teams[ 1].home_losses );
     assertEquals(  1, records->teams[ 2].home_wins   );
     assertEquals( 14, records->teams[ 2].home_losses );
     assertEquals(  2, records->teams[ 3].home_wins   );
     assertEquals( 13, records->teams[ 3].home_losses );
     assertEquals(  2, records->teams[ 4].home_wins   );
     assertEquals( 13, records->teams[ 4].home_losses );
     assertEquals(  3, records->teams[ 5].home_wins   );
     assertEquals( 12, records->teams[ 5].home_losses );
     assertEquals(  3, records->teams[ 6].home_wins   );
     assertEquals( 12, records->teams[ 6].home_losses );
     assertEquals(  4, records->teams[ 7].home_wins   );
     assertEquals( 11, records->teams[ 7].home_losses );
     assertEquals(  4, records->teams[ 8].home_wins   );
     assertEquals( 11, records->teams[ 8].home_losses );
     assertEquals(  5, records->teams[ 9].home_wins   );
     assertEquals( 10, records->teams[ 9].home_losses );
     assertEquals(  5, records->teams[10].home_wins   );
     assertEquals( 10, records->teams[10].home_losses );
     assertEquals(  6, records->teams[11].home_wins   );
     assertEquals(  9, records->teams[11].home_losses );
     assertEquals(  6, records->teams[12].home_wins   );
     assertEquals(  9, records->teams[12].home_losses );
     assertEquals(  7, records->teams[13].home_wins   );
     assertEquals(  8, records->teams[13].home_losses );
     assertEquals(  7, records->teams[14].home_wins   );
     assertEquals(  8, records->teams[14].home_losses );
     assertEquals(  8, records->teams[15].home_wins   );
     assertEquals(  7, records->teams[15].home_losses );
     assertEquals(  8, records->teams[16].home_wins   );
     assertEquals(  7, records->teams[16].home_losses );
     assertEquals(  9, records->teams[17].home_wins   );
     assertEquals(  6, records->teams[17].home_losses );
     assertEquals(  9, records->teams[18].home_wins   );
     assertEquals(  6, records->teams[18].home_losses );
     assertEquals( 10, records->teams[19].home_wins   );
     assertEquals(  5, records->teams[19].home_losses );
     assertEquals( 10, records->teams[20].home_wins   );
     assertEquals(  5, records->teams[20].home_losses );
     assertEquals( 11, records->teams[21].home_wins   );
     assertEquals(  4, records->teams[21].home_losses );
     assertEquals( 11, records->teams[22].home_wins   );
     assertEquals(  4, records->teams[22].home_losses );
     assertEquals( 12, records->teams[23].home_wins   );
     assertEquals(  3, records->teams[23].home_losses );
     assertEquals( 12, records->teams[24].home_wins   );
     assertEquals(  3, records->teams[24].home_losses );
     assertEquals( 13, records->teams[25].home_wins   );
     assertEquals(  2, records->teams[25].home_losses );
     assertEquals( 13, records->teams[26].home_wins   );
     assertEquals(  2, records->teams[26].home_losses );
     assertEquals( 14, records->teams[27].home_wins   );
     assertEquals(  1, records->teams[27].home_losses );
     assertEquals( 14, records->teams[28].home_wins   );
     assertEquals(  1, records->teams[28].home_losses );
     assertEquals( 15, records->teams[29].home_wins   );
     assertEquals(  0, records->teams[29].home_losses );
     assertEquals( 15, records->teams[30].home_wins   );
     assertEquals(  0, records->teams[30].home_losses );
     assertEquals( 31, records->teams[31].home_wins   );
     assertEquals(  0, records->teams[31].home_losses );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetTeamRoadRecords_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals(  0, records->teams[ 0].road_wins   );
     assertEquals( 16, records->teams[ 0].road_losses );
     assertEquals(  0, records->teams[ 1].road_wins   );
     assertEquals( 16, records->teams[ 1].road_losses );
     assertEquals(  1, records->teams[ 2].road_wins   );
     assertEquals( 15, records->teams[ 2].road_losses );
     assertEquals(  1, records->teams[ 3].road_wins   );
     assertEquals( 15, records->teams[ 3].road_losses );
     assertEquals(  2, records->teams[ 4].road_wins   );
     assertEquals( 14, records->teams[ 4].road_losses );
     assertEquals(  2, records->teams[ 5].road_wins   );
     assertEquals( 14, records->teams[ 5].road_losses );
     assertEquals(  3, records->teams[ 6].road_wins   );
     assertEquals( 13, records->teams[ 6].road_losses );
     assertEquals(  3, records->teams[ 7].road_wins   );
     assertEquals( 13, records->teams[ 7].road_losses );
     assertEquals(  4, records->teams[ 8].road_wins   );
     assertEquals( 12, records->teams[ 8].road_losses );
     assertEquals(  4, records->teams[ 9].road_wins   );
     assertEquals( 12, records->teams[ 9].road_losses );
     assertEquals(  5, records->teams[10].road_wins   );
     assertEquals( 11, records->teams[10].road_losses );
     assertEquals(  5, records->teams[11].road_wins   );
     assertEquals( 11, records->teams[11].road_losses );
     assertEquals(  6, records->teams[12].road_wins   );
     assertEquals( 10, records->teams[12].road_losses );
     assertEquals(  6, records->teams[13].road_wins   );
     assertEquals( 10, records->teams[13].road_losses );
     assertEquals(  7, records->teams[14].road_wins   );
     assertEquals(  9, records->teams[14].road_losses );
     assertEquals(  7, records->teams[15].road_wins   );
     assertEquals(  9, records->teams[15].road_losses );
     assertEquals(  8, records->teams[16].road_wins   );
     assertEquals(  8, records->teams[16].road_losses );
     assertEquals(  8, records->teams[17].road_wins   );
     assertEquals(  8, records->teams[17].road_losses );
     assertEquals(  9, records->teams[18].road_wins   );
     assertEquals(  7, records->teams[18].road_losses );
     assertEquals(  9, records->teams[19].road_wins   );
     assertEquals(  7, records->teams[19].road_losses );
     assertEquals( 10, records->teams[20].road_wins   );
     assertEquals(  6, records->teams[20].road_losses );
     assertEquals( 10, records->teams[21].road_wins   );
     assertEquals(  6, records->teams[21].road_losses );
     assertEquals( 11, records->teams[22].road_wins   );
     assertEquals(  5, records->teams[22].road_losses );
     assertEquals( 11, records->teams[23].road_wins   );
     assertEquals(  5, records->teams[23].road_losses );
     assertEquals( 12, records->teams[24].road_wins   );
     assertEquals(  4, records->teams[24].road_losses );
     assertEquals( 12, records->teams[25].road_wins   );
     assertEquals(  4, records->teams[25].road_losses );
     assertEquals( 13, records->teams[26].road_wins   );
     assertEquals(  3, records->teams[26].road_losses );
     assertEquals( 13, records->teams[27].road_wins   );
     assertEquals(  3, records->teams[27].road_losses );
     assertEquals( 14, records->teams[28].road_wins   );
     assertEquals(  2, records->teams[28].road_losses );
     assertEquals( 14, records->teams[29].road_wins   );
     assertEquals(  2, records->teams[29].road_losses );
     assertEquals( 15, records->teams[30].road_wins   );
     assertEquals(  1, records->teams[30].road_losses );
     assertEquals(  0, records->teams[31].road_wins   );
     assertEquals(  0, records->teams[31].road_losses );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetTeamDivisionRecords_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals( 0, records->teams[ 0].division_wins   );
     assertEquals( 7, records->teams[ 0].division_losses );
     assertEquals( 1, records->teams[ 1].division_wins   );
     assertEquals( 6, records->teams[ 1].division_losses );
     assertEquals( 2, records->teams[ 2].division_wins   );
     assertEquals( 5, records->teams[ 2].division_losses );
     assertEquals( 3, records->teams[ 3].division_wins   );
     assertEquals( 4, records->teams[ 3].division_losses );
     assertEquals( 4, records->teams[ 4].division_wins   );
     assertEquals( 3, records->teams[ 4].division_losses );
     assertEquals( 5, records->teams[ 5].division_wins   );
     assertEquals( 2, records->teams[ 5].division_losses );
     assertEquals( 6, records->teams[ 6].division_wins   );
     assertEquals( 1, records->teams[ 6].division_losses );
     assertEquals( 7, records->teams[ 7].division_wins   );
     assertEquals( 0, records->teams[ 7].division_losses );
     assertEquals( 0, records->teams[ 8].division_wins   );
     assertEquals( 7, records->teams[ 8].division_losses );
     assertEquals( 1, records->teams[ 9].division_wins   );
     assertEquals( 6, records->teams[ 9].division_losses );
     assertEquals( 2, records->teams[10].division_wins   );
     assertEquals( 5, records->teams[10].division_losses );
     assertEquals( 3, records->teams[11].division_wins   );
     assertEquals( 4, records->teams[11].division_losses );
     assertEquals( 4, records->teams[12].division_wins   );
     assertEquals( 3, records->teams[12].division_losses );
     assertEquals( 5, records->teams[13].division_wins   );
     assertEquals( 2, records->teams[13].division_losses );
     assertEquals( 6, records->teams[14].division_wins   );
     assertEquals( 1, records->teams[14].division_losses );
     assertEquals( 7, records->teams[15].division_wins   );
     assertEquals( 0, records->teams[15].division_losses );
     assertEquals( 0, records->teams[16].division_wins   );
     assertEquals( 7, records->teams[16].division_losses );
     assertEquals( 1, records->teams[17].division_wins   );
     assertEquals( 6, records->teams[17].division_losses );
     assertEquals( 2, records->teams[18].division_wins   );
     assertEquals( 5, records->teams[18].division_losses );
     assertEquals( 3, records->teams[19].division_wins   );
     assertEquals( 4, records->teams[19].division_losses );
     assertEquals( 4, records->teams[20].division_wins   );
     assertEquals( 3, records->teams[20].division_losses );
     assertEquals( 5, records->teams[21].division_wins   );
     assertEquals( 2, records->teams[21].division_losses );
     assertEquals( 6, records->teams[22].division_wins   );
     assertEquals( 1, records->teams[22].division_losses );
     assertEquals( 7, records->teams[23].division_wins   );
     assertEquals( 0, records->teams[23].division_losses );
     assertEquals( 0, records->teams[24].division_wins   );
     assertEquals( 7, records->teams[24].division_losses );
     assertEquals( 1, records->teams[25].division_wins   );
     assertEquals( 6, records->teams[25].division_losses );
     assertEquals( 2, records->teams[26].division_wins   );
     assertEquals( 5, records->teams[26].division_losses );
     assertEquals( 3, records->teams[27].division_wins   );
     assertEquals( 4, records->teams[27].division_losses );
     assertEquals( 4, records->teams[28].division_wins   );
     assertEquals( 3, records->teams[28].division_losses );
     assertEquals( 5, records->teams[29].division_wins   );
     assertEquals( 2, records->teams[29].division_losses );
     assertEquals( 6, records->teams[30].division_wins   );
     assertEquals( 1, records->teams[30].division_losses );
     assertEquals( 7, records->teams[31].division_wins   );
     assertEquals( 0, records->teams[31].division_losses );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetTeamLeagueRecords_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals(  0, records->teams[ 0].league_wins   );
     assertEquals( 15, records->teams[ 0].league_losses );
     assertEquals(  1, records->teams[ 1].league_wins   );
     assertEquals( 14, records->teams[ 1].league_losses );
     assertEquals(  2, records->teams[ 2].league_wins   );
     assertEquals( 13, records->teams[ 2].league_losses );
     assertEquals(  3, records->teams[ 3].league_wins   );
     assertEquals( 12, records->teams[ 3].league_losses );
     assertEquals(  4, records->teams[ 4].league_wins   );
     assertEquals( 11, records->teams[ 4].league_losses );
     assertEquals(  5, records->teams[ 5].league_wins   );
     assertEquals( 10, records->teams[ 5].league_losses );
     assertEquals(  6, records->teams[ 6].league_wins   );
     assertEquals(  9, records->teams[ 6].league_losses );
     assertEquals(  7, records->teams[ 7].league_wins   );
     assertEquals(  8, records->teams[ 7].league_losses );
     assertEquals(  8, records->teams[ 8].league_wins   );
     assertEquals(  7, records->teams[ 8].league_losses );
     assertEquals(  9, records->teams[ 9].league_wins   );
     assertEquals(  6, records->teams[ 9].league_losses );
     assertEquals( 10, records->teams[10].league_wins   );
     assertEquals(  5, records->teams[10].league_losses );
     assertEquals( 11, records->teams[11].league_wins   );
     assertEquals(  4, records->teams[11].league_losses );
     assertEquals( 12, records->teams[12].league_wins   );
     assertEquals(  3, records->teams[12].league_losses );
     assertEquals( 13, records->teams[13].league_wins   );
     assertEquals(  2, records->teams[13].league_losses );
     assertEquals( 14, records->teams[14].league_wins   );
     assertEquals(  1, records->teams[14].league_losses );
     assertEquals( 15, records->teams[15].league_wins   );
     assertEquals(  0, records->teams[15].league_losses );
     assertEquals(  0, records->teams[16].league_wins   );
     assertEquals( 15, records->teams[16].league_losses );
     assertEquals(  1, records->teams[17].league_wins   );
     assertEquals( 14, records->teams[17].league_losses );
     assertEquals(  2, records->teams[18].league_wins   );
     assertEquals( 13, records->teams[18].league_losses );
     assertEquals(  3, records->teams[19].league_wins   );
     assertEquals( 12, records->teams[19].league_losses );
     assertEquals(  4, records->teams[20].league_wins   );
     assertEquals( 11, records->teams[20].league_losses );
     assertEquals(  5, records->teams[21].league_wins   );
     assertEquals( 10, records->teams[21].league_losses );
     assertEquals(  6, records->teams[22].league_wins   );
     assertEquals(  9, records->teams[22].league_losses );
     assertEquals(  7, records->teams[23].league_wins   );
     assertEquals(  8, records->teams[23].league_losses );
     assertEquals(  8, records->teams[24].league_wins   );
     assertEquals(  7, records->teams[24].league_losses );
     assertEquals(  9, records->teams[25].league_wins   );
     assertEquals(  6, records->teams[25].league_losses );
     assertEquals( 10, records->teams[26].league_wins   );
     assertEquals(  5, records->teams[26].league_losses );
     assertEquals( 11, records->teams[27].league_wins   );
     assertEquals(  4, records->teams[27].league_losses );
     assertEquals( 12, records->teams[28].league_wins   );
     assertEquals(  3, records->teams[28].league_losses );
     assertEquals( 13, records->teams[29].league_wins   );
     assertEquals(  2, records->teams[29].league_losses );
     assertEquals( 14, records->teams[30].league_wins   );
     assertEquals(  1, records->teams[30].league_losses );
     assertEquals( 15, records->teams[31].league_wins   );
     assertEquals(  0, records->teams[31].league_losses );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetTeamRunsScoredAndAllowed_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals(   0, records->teams[ 0].runs_scored  );
     assertEquals( 496, records->teams[ 0].runs_allowed );
     assertEquals(  31, records->teams[ 1].runs_scored  );
     assertEquals( 495, records->teams[ 1].runs_allowed );
     assertEquals(  62, records->teams[ 2].runs_scored  );
     assertEquals( 494, records->teams[ 2].runs_allowed );
     assertEquals(  93, records->teams[ 3].runs_scored  );
     assertEquals( 493, records->teams[ 3].runs_allowed );
     assertEquals( 124, records->teams[ 4].runs_scored  );
     assertEquals( 492, records->teams[ 4].runs_allowed );
     assertEquals( 155, records->teams[ 5].runs_scored  );
     assertEquals( 491, records->teams[ 5].runs_allowed );
     assertEquals( 186, records->teams[ 6].runs_scored  );
     assertEquals( 490, records->teams[ 6].runs_allowed );
     assertEquals( 217, records->teams[ 7].runs_scored  );
     assertEquals( 489, records->teams[ 7].runs_allowed );
     assertEquals( 248, records->teams[ 8].runs_scored  );
     assertEquals( 488, records->teams[ 8].runs_allowed );
     assertEquals( 279, records->teams[ 9].runs_scored  );
     assertEquals( 487, records->teams[ 9].runs_allowed );
     assertEquals( 310, records->teams[10].runs_scored  );
     assertEquals( 486, records->teams[10].runs_allowed );
     assertEquals( 341, records->teams[11].runs_scored  );
     assertEquals( 485, records->teams[11].runs_allowed );
     assertEquals( 372, records->teams[12].runs_scored  );
     assertEquals( 484, records->teams[12].runs_allowed );
     assertEquals( 403, records->teams[13].runs_scored  );
     assertEquals( 483, records->teams[13].runs_allowed );
     assertEquals( 434, records->teams[14].runs_scored  );
     assertEquals( 482, records->teams[14].runs_allowed );
     assertEquals( 465, records->teams[15].runs_scored  );
     assertEquals( 481, records->teams[15].runs_allowed );
     assertEquals( 496, records->teams[16].runs_scored  );
     assertEquals( 480, records->teams[16].runs_allowed );
     assertEquals( 527, records->teams[17].runs_scored  );
     assertEquals( 479, records->teams[17].runs_allowed );
     assertEquals( 558, records->teams[18].runs_scored  );
     assertEquals( 478, records->teams[18].runs_allowed );
     assertEquals( 589, records->teams[19].runs_scored  );
     assertEquals( 477, records->teams[19].runs_allowed );
     assertEquals( 620, records->teams[20].runs_scored  );
     assertEquals( 476, records->teams[20].runs_allowed );
     assertEquals( 651, records->teams[21].runs_scored  );
     assertEquals( 475, records->teams[21].runs_allowed );
     assertEquals( 682, records->teams[22].runs_scored  );
     assertEquals( 474, records->teams[22].runs_allowed );
     assertEquals( 713, records->teams[23].runs_scored  );
     assertEquals( 473, records->teams[23].runs_allowed );
     assertEquals( 744, records->teams[24].runs_scored  );
     assertEquals( 472, records->teams[24].runs_allowed );
     assertEquals( 775, records->teams[25].runs_scored  );
     assertEquals( 471, records->teams[25].runs_allowed );
     assertEquals( 806, records->teams[26].runs_scored  );
     assertEquals( 470, records->teams[26].runs_allowed );
     assertEquals( 837, records->teams[27].runs_scored  );
     assertEquals( 469, records->teams[27].runs_allowed );
     assertEquals( 868, records->teams[28].runs_scored  );
     assertEquals( 468, records->teams[28].runs_allowed );
     assertEquals( 899, records->teams[29].runs_scored  );
     assertEquals( 467, records->teams[29].runs_allowed );
     assertEquals( 930, records->teams[30].runs_scored  );
     assertEquals( 466, records->teams[30].runs_allowed );
     assertEquals( 961, records->teams[31].runs_scored  );
     assertEquals( 465, records->teams[31].runs_allowed );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetTheTeamIdSeasonAndPhase_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          assertEquals( i + 1,      records->teams[i].team_id      );
          assertEquals( 4,          records->teams[i].season       );
          assertEquals( sp_Regular, records->teams[i].season_phase );
     }

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetDivisionRecords_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals(   0, records->divisions[0].wins   );
     assertEquals( 192, records->divisions[0].losses );
     assertEquals(  64, records->divisions[1].wins   );
     assertEquals( 128, records->divisions[1].losses );
     assertEquals( 128, records->divisions[2].wins   );
     assertEquals(  64, records->divisions[2].losses );
     assertEquals( 192, records->divisions[3].wins   );
     assertEquals(   0, records->divisions[3].losses );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetDivisionHomeRecords_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals(   0, records->divisions[0].home_wins   );
     assertEquals(  92, records->divisions[0].home_losses );
     assertEquals(  32, records->divisions[1].home_wins   );
     assertEquals(  60, records->divisions[1].home_losses );
     assertEquals(  64, records->divisions[2].home_wins   );
     assertEquals(  28, records->divisions[2].home_losses );
     assertEquals( 108, records->divisions[3].home_wins   );
     assertEquals(   0, records->divisions[3].home_losses );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetDivisionRoadRecords_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals(   0, records->divisions[0].road_wins   );
     assertEquals( 100, records->divisions[0].road_losses );
     assertEquals(  32, records->divisions[1].road_wins   );
     assertEquals(  68, records->divisions[1].road_losses );
     assertEquals(  64, records->divisions[2].road_wins   );
     assertEquals(  36, records->divisions[2].road_losses );
     assertEquals(  84, records->divisions[3].road_wins   );
     assertEquals(   0, records->divisions[3].road_losses );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetDivisionLeagueRecords_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals(   0, records->divisions[0].league_wins   );
     assertEquals(  64, records->divisions[0].league_losses );
     assertEquals(  64, records->divisions[1].league_wins   );
     assertEquals(   0, records->divisions[1].league_losses );
     assertEquals(   0, records->divisions[2].league_wins   );
     assertEquals(  64, records->divisions[2].league_losses );
     assertEquals(  64, records->divisions[3].league_wins   );
     assertEquals(   0, records->divisions[3].league_losses );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetDivisionRunsScoredAndAllowed_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals(  672, records->divisions[0].runs_scored  );
     assertEquals( 3744, records->divisions[0].runs_allowed );
     assertEquals( 2208, records->divisions[1].runs_scored  );
     assertEquals( 3232, records->divisions[1].runs_allowed );
     assertEquals( 3744, records->divisions[2].runs_scored  );
     assertEquals( 2720, records->divisions[2].runs_allowed );
     assertEquals( 5280, records->divisions[3].runs_scored  );
     assertEquals( 2208, records->divisions[3].runs_allowed );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetTheDivisionIdSeasonAndPhase_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     for ( int i = 0; i < TOTAL_DIVISIONS; ++i )
     {
          assertEquals( i + 1,      records->divisions[i].division_id  );
          assertEquals( 4,          records->divisions[i].season       );
          assertEquals( sp_Regular, records->divisions[i].season_phase );
     }

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetLeagueRecords_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals(   0, records->leagues[0].wins   );
     assertEquals( 256, records->leagues[0].losses );
     assertEquals( 256, records->leagues[1].wins   );
     assertEquals(   0, records->leagues[1].losses );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetLeagueHomeRecords_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals(   0, records->leagues[0].home_wins   );
     assertEquals( 120, records->leagues[0].home_losses );
     assertEquals( 136, records->leagues[1].home_wins   );
     assertEquals(   0, records->leagues[1].home_losses );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetLeagueRoadRecords_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals(   0, records->leagues[0].road_wins   );
     assertEquals( 136, records->leagues[0].road_losses );
     assertEquals( 120, records->leagues[1].road_wins   );
     assertEquals(   0, records->leagues[1].road_losses );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetLeagueRunsScoredAndAllowed_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     assertEquals( 1920, records->leagues[0].runs_scored  );
     assertEquals( 6016, records->leagues[0].runs_allowed );
     assertEquals( 6016, records->leagues[1].runs_scored  );
     assertEquals( 1920, records->leagues[1].runs_allowed );

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldSetTheLeagueIdSeasonAndPhase_GivenAScheduleAndLeagueFile()
{
     fileleagname_s *league_file = buildFileLeagName();
     schedule_s     *schedule    = buildSchedule( league_file );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          assertEquals( i + 1,      records->leagues[i].league_id    );
          assertEquals( 4,          records->leagues[i].season       );
          assertEquals( sp_Regular, records->leagues[i].season_phase );
     }

     freeRecords(  records  );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldOnlyCalculateRegularSeasonRecords_GivenRegularSeasonPhase()
{
     fileleagname_s *league_file = readLeagueFile( "LeagName_test.Dat" );
     schedule_s     *schedule    = readScheduleCSV( "schedule_test.csv" );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Regular );

     assertNotNull( records );

     // Portsmen 83 - 69
     assertEquals( 83, records->teams[ 1].wins   );
     assertEquals( 69, records->teams[ 1].losses );

     // Dragons 92 - 60
     assertEquals( 92, records->teams[12].wins   );
     assertEquals( 60, records->teams[12].losses );

     // Thunder 83 - 69
     assertEquals( 83, records->teams[22].wins   );
     assertEquals( 69, records->teams[22].losses );

     // Waves 97 - 55
     assertEquals( 97, records->teams[26].wins   );
     assertEquals( 55, records->teams[26].losses );

     free( league_file );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldOnlyCalculatePlayoffRecords_GivenPlayoffPhase()
{
     fileleagname_s *league_file = readLeagueFile( "LeagName_test.Dat" );
     schedule_s     *schedule    = readScheduleCSV( "schedule_test.csv" );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Playoff );

     assertNotNull( records );

     // Portsmen 3 - 4
     assertEquals( 3, records->teams[ 1].wins   );
     assertEquals( 4, records->teams[ 1].losses );

     // Dragons 9 - 7
     assertEquals( 9, records->teams[12].wins   );
     assertEquals( 7, records->teams[12].losses );

     // Thunder 9 - 7
     assertEquals( 9, records->teams[22].wins   );
     assertEquals( 7, records->teams[22].losses );

     // Waves 2 - 4
     assertEquals( 2, records->teams[26].wins   );
     assertEquals( 4, records->teams[26].losses );

     // Quasars 0 - 0
     assertEquals( 0, records->teams[31].wins   );
     assertEquals( 0, records->teams[31].losses );

     free( league_file );
     freeSchedule( schedule );

     return NULL;
}

static char *calculateRecords_ShouldOnlyCalculateAllstarRecords_GivenAllstarPhase()
{
     fileleagname_s *league_file = readLeagueFile( "LeagName_allstar_test.Dat" );
     schedule_s     *schedule    = readScheduleCSV( "schedule_test.csv" );

     records_s *records = calculateRecords( schedule, league_file, 4, sp_Allstar );

     assertNotNull( records );

     // World 0 - 3
     assertEquals( 0, records->teams[ 0].wins   );
     assertEquals( 3, records->teams[ 0].losses );

     // Should be empty
     assertEquals( 0, records->teams[ 8].wins   );
     assertEquals( 0, records->teams[ 8].losses );

     // Global 3 - 0
     assertEquals( 3, records->teams[16].wins   );
     assertEquals( 0, records->teams[16].losses );

     // Should be empty
     assertEquals( 0, records->teams[24].wins   );
     assertEquals( 0, records->teams[24].losses );

     free( league_file );
     freeSchedule( schedule );

     return NULL;
}

static void get_error_message()
{
     printf( "calculateRecords error message: %s\n", getCalculateRecordsError() );
}

static void run_all_tests()
{
     run_test( calculateRecords_ShouldReturnARecordsObject_GivenAScheduleAndLeagueFile,         get_error_message );
     run_test( calculateRecords_ShouldReturnNullIfScheduleIsEmpty_GivenAScheduleAndLeagueFile,  get_error_message );
     run_test( calculateRecords_ShouldReturnNullIfRoadTeamNotFound_GivenAScheduleAndLeagueFile, get_error_message );
     run_test( calculateRecords_ShouldReturnNullIfHomeTeamNotFound_GivenAScheduleAndLeagueFile, get_error_message );

     // team records
     run_test( calculateRecords_ShouldSetTeamRecords_GivenAScheduleAndLeagueFile,              get_error_message );
     run_test( calculateRecords_ShouldSetTeamHomeRecords_GivenAScheduleAndLeagueFile,          get_error_message );
     run_test( calculateRecords_ShouldSetTeamRoadRecords_GivenAScheduleAndLeagueFile,          get_error_message );
     run_test( calculateRecords_ShouldSetTeamDivisionRecords_GivenAScheduleAndLeagueFile,      get_error_message );
     run_test( calculateRecords_ShouldSetTeamLeagueRecords_GivenAScheduleAndLeagueFile,        get_error_message );
     run_test( calculateRecords_ShouldSetTeamRunsScoredAndAllowed_GivenAScheduleAndLeagueFile, get_error_message );
     run_test( calculateRecords_ShouldSetTheTeamIdSeasonAndPhase_GivenAScheduleAndLeagueFile,  get_error_message );

     // division records
     run_test( calculateRecords_ShouldSetDivisionRecords_GivenAScheduleAndLeagueFile,              get_error_message );
     run_test( calculateRecords_ShouldSetDivisionHomeRecords_GivenAScheduleAndLeagueFile,          get_error_message );
     run_test( calculateRecords_ShouldSetDivisionRoadRecords_GivenAScheduleAndLeagueFile,          get_error_message );
     run_test( calculateRecords_ShouldSetDivisionLeagueRecords_GivenAScheduleAndLeagueFile,        get_error_message );
     run_test( calculateRecords_ShouldSetDivisionRunsScoredAndAllowed_GivenAScheduleAndLeagueFile, get_error_message );
     run_test( calculateRecords_ShouldSetTheDivisionIdSeasonAndPhase_GivenAScheduleAndLeagueFile,  get_error_message );

     // league records
     run_test( calculateRecords_ShouldSetLeagueRecords_GivenAScheduleAndLeagueFile,              get_error_message );
     run_test( calculateRecords_ShouldSetLeagueHomeRecords_GivenAScheduleAndLeagueFile,          get_error_message );
     run_test( calculateRecords_ShouldSetLeagueRoadRecords_GivenAScheduleAndLeagueFile,          get_error_message );
     run_test( calculateRecords_ShouldSetLeagueRunsScoredAndAllowed_GivenAScheduleAndLeagueFile, get_error_message );
     run_test( calculateRecords_ShouldSetTheLeagueIdSeasonAndPhase_GivenAScheduleAndLeagueFile,  get_error_message );

     // phases
     run_test( calculateRecords_ShouldOnlyCalculateRegularSeasonRecords_GivenRegularSeasonPhase, get_error_message );
     run_test( calculateRecords_ShouldOnlyCalculatePlayoffRecords_GivenPlayoffPhase,             get_error_message );
     run_test( calculateRecords_ShouldOnlyCalculateAllstarRecords_GivenAllstarPhase,             get_error_message );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
