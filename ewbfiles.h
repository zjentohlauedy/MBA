#ifndef _EWBFILES_H_
#define _EWBFILES_H_

#define min( A, B ) ((A) < (B)) ? (A) : (B)

#define PLAYERS_PER_TEAM       25
#define TEAMS_PER_DIVISION      8
#define PLAYERS_PER_DIVISION    (PLAYERS_PER_TEAM * TEAMS_PER_DIVISION)
#define DIVISIONS_PER_LEAGUE    2
#define TEAMS_PER_LEAGUE        (TEAMS_PER_DIVISION * DIVISIONS_PER_LEAGUE)
#define PLAYERS_PER_LEAGUE      (PLAYERS_PER_DIVISION * DIVISIONS_PER_LEAGUE)
#define TOTAL_LEAGUES           2
#define TOTAL_TEAMS             (TEAMS_PER_LEAGUE * TOTAL_LEAGUES)
#define TOTAL_PLAYERS           (PLAYERS_PER_LEAGUE * TOTAL_LEAGUES)


typedef enum
{
     bl_False = 0,
     bl_True  = 1

} boolean_e;


typedef enum
{
     n_Low, // the 0 in A0
     n_High // the A in A0

} nibble_e;


typedef enum
{
     cl_LtBlue = 0,
     cl_Blue,
     cl_DkRed,
     cl_Red,
     cl_Gold,
     cl_Black,
     cl_Brown,
     cl_Green

} color_e;

typedef enum
{
     pcl_DkBlue = 0,
     pcl_Green,
     pcl_DkGrey,
     pcl_DkGreen,
     pcl_Grey,
     pcl_LtBlue,
     pcl_Red,
     pcl_DkRed

} park_color_e;

typedef enum
{
     pos_Pitcher      = 0,
     pos_Catcher,
     pos_FirstBase,
     pos_SecondBase,
     pos_ThirdBase,
     pos_ShortStop,
     pos_Infield,
     pos_LeftField,
     pos_CenterField,
     pos_RightField,
     pos_Outfield,
     pos_Unknown,
     pos_Designate

} position_e;


// ----  LEAGNAME.DAT  ----
typedef struct
{
     unsigned char name   [12];
     unsigned char color  [ 1];
     unsigned char wins   [ 1];
     unsigned char losses [ 1];
     unsigned char term   [ 1];

} fileteam_s;


typedef struct
{
     unsigned char header[52];
     unsigned char league1[12];
     unsigned char league2[12];
     unsigned char division1[12];
     unsigned char division2[12];
     unsigned char division3[12];
     unsigned char division4[12];
     fileteam_s    teams[32];

} fileleague_s;


// ----  PARKS.DAT  ----
typedef struct
{
     unsigned char text[16];

} park_name_s;

typedef struct
{
     unsigned char left_line[2];
     unsigned char left_center[2];
     unsigned char center[2];
     unsigned char right_center[2];
     unsigned char right_line[2];

} distances_s;

typedef struct
{
     unsigned char left1[1];
     unsigned char left2[1];
     unsigned char left3[1];
     unsigned char left4[1];
     unsigned char center[1];
     unsigned char right4[1];
     unsigned char right3[1];
     unsigned char right2[1];
     unsigned char right1[1];

} heights_s;

typedef struct
{
     signed char x[2];
     signed char y[2];
} coords_s;

typedef struct
{
     unsigned char details1[1];
     unsigned char bleachers[1];
     unsigned char details2[2];
     distances_s   distances;
     heights_s     heights;
     unsigned char empty[1];
     coords_s      outfield[10];
     coords_s      infield1[8];
     coords_s      infield2[8];

} park_details_s;

typedef struct
{
     unsigned char  filler[52];
     park_name_s    park_names[32];
     park_details_s park_details[32];

} fileparks_s;


// ----  PLAYERS.S  ----
typedef struct
{
     unsigned char acc_ab      [ 2];
     unsigned char acc_hits    [ 2];
     unsigned char acc_2b      [ 1];
     unsigned char acc_3b      [ 1];
     unsigned char acc_hr      [ 1];
     unsigned char acc_bb      [ 1];
     unsigned char acc_so      [ 1];
     unsigned char acc_games   [ 1];
     unsigned char acc_runs    [ 1];
     unsigned char acc_rbi     [ 1];
     unsigned char acc_sb      [ 1];
     unsigned char acc_err     [ 1];

} acc_bat_stats_s;


typedef struct
{
     unsigned char acc_wins    [ 1];
     unsigned char acc_losses  [ 1];
     unsigned char acc_starts  [ 1];
     unsigned char acc_saves   [ 1];
     unsigned char acc_innings [ 2];
     unsigned char acc_hits    [ 2];
     unsigned char acc_er      [ 2];
     unsigned char acc_hr      [ 1];
     unsigned char acc_bb      [ 1];
     unsigned char acc_so      [ 2];

} acc_pch_stats_s;


typedef struct
{
     /**/     char last_name   [10];
     /**/     char first_name  [ 8];
     /**/     char last_phoen  [14];
     /**/     char first_phoen [12];
     unsigned char real_avg    [ 2]; // era for pitchers, ba for batters
     unsigned char year        [ 1]; // added to 1845
     unsigned char position    [ 1]; // first nibble POS1; second nibble: batters = POS2 pitchers = throws R = 0/L = 1

     union
     {
	  struct batting_s
	  {
	       unsigned char ratings     [ 4]; // by nibble: bats R = 0/L = 1/S = 2,arm,runs,range,power,not used,bunt,h&r
	       unsigned char real_games  [ 1];
	       unsigned char real_runs   [ 1];
	       unsigned char real_rbi    [ 1];
	       unsigned char real_sb     [ 1];
	       unsigned char real_cs     [ 1];
	       unsigned char real_sh     [ 1];

	       unsigned char vl_ab       [ 2];
	       unsigned char vl_hits     [ 2];
	       unsigned char vl_2b       [ 1];
	       unsigned char vl_3b       [ 1];
	       unsigned char vl_hr       [ 1];
	       unsigned char vl_bb       [ 1];
	       unsigned char vl_so       [ 1];

	       unsigned char real_fa2    [ 1]; // FA2?

	       unsigned char vr_ab       [ 2];
	       unsigned char vr_hits     [ 2];
	       unsigned char vr_2b       [ 1];
	       unsigned char vr_3b       [ 1];
	       unsigned char vr_hr       [ 1];
	       unsigned char vr_bb       [ 1];
	       unsigned char vr_so       [ 1];

	       unsigned char real_err    [ 1];
	       unsigned char real_po     [ 2];
	       unsigned char real_as     [ 2];

	       unsigned char injury_days [ 1];
	       unsigned char color       [ 1]; // 00 for white, 40 for black

	       acc_bat_stats_s action;
	       acc_bat_stats_s simulated;

	  } filebatting;

	  struct pitching_s
	  {
	       unsigned char ratings     [ 2]; // by nibble: speed,control,fatigue,bunt
	       unsigned char real_wins   [ 1];
	       unsigned char real_losses [ 1];
	       unsigned char real_games  [ 1];
	       unsigned char real_starts [ 1];
	       unsigned char real_cg     [ 1];
	       unsigned char real_saves  [ 1];
	       unsigned char real_inn    [ 2];

	       unsigned char vl_ab       [ 2];
	       unsigned char vl_hits     [ 2];
	       unsigned char vl_hr       [ 1];
	       unsigned char vl_bb       [ 1];
	       unsigned char vl_so       [ 2];

	       unsigned char real_er     [ 1];
	       unsigned char real_fa     [ 1]; // FA?

	       unsigned char vr_ab       [ 2];
	       unsigned char vr_hits     [ 2];
	       unsigned char vr_hr       [ 1];
	       unsigned char vr_bb       [ 1];
	       unsigned char vr_so       [ 2];

	       unsigned char real_hb     [ 1];
	       unsigned char real_balk   [ 1];
	       unsigned char real_ab     [ 1]; // batting
	       unsigned char real_hits   [ 1]; // batting
	       unsigned char real_hr     [ 1]; // batting
	       unsigned char real_sh     [ 1]; // batting
	       unsigned char injury_days [ 1];

	       unsigned char color       [ 1]; // 00 for white, 40 for black

	       acc_pch_stats_s action;
	       acc_pch_stats_s simulated;

	  } filepitching;

     } filestats;

} fileplayer_s;


int           word2int(         unsigned char *word                                );
void          int2word(         unsigned char *word,     int           value       );
int           nibble(           unsigned char  byte,     nibble_e      position    );
void          termName(         unsigned char *buffer,   int           len         );
void          untermName(       unsigned char *buffer,   int           len         );
char         *getColorName(     unsigned int   color                               );
char         *getParkColorName( unsigned int   color );
char         *positionName(     int            position                            );
fileplayer_s *readPlayersFile(  char          *filename                            );
boolean_e     writePlayersFile( char          *filename, fileplayer_s *playersFile );
fileleague_s *readLeagueFile(   char          *filename                            );
boolean_e     writeLeagueFile(  char          *filename, fileleague_s *leagueFile  );
fileparks_s  *readParksFile(    char          *filename                            );
boolean_e     writeParksFile(   char          *filename, fileparks_s  *parksFile   );

#endif
