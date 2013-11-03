#ifndef _EWBFILES_H_
#define _EWBFILES_H_


#define TOTAL_PARKS            32
#define PLAYERS_PER_TEAM       25
#define TEAMS_PER_DIVISION      8
#define PLAYERS_PER_DIVISION    (PLAYERS_PER_TEAM * TEAMS_PER_DIVISION)
#define DIVISIONS_PER_LEAGUE    2
#define TEAMS_PER_LEAGUE        (TEAMS_PER_DIVISION * DIVISIONS_PER_LEAGUE)
#define PLAYERS_PER_LEAGUE      (PLAYERS_PER_DIVISION * DIVISIONS_PER_LEAGUE)
#define TOTAL_LEAGUES           2
#define TOTAL_DIVISIONS         (DIVISIONS_PER_LEAGUE * TOTAL_LEAGUES)
#define TOTAL_TEAMS             (TEAMS_PER_LEAGUE * TOTAL_LEAGUES)
#define TOTAL_PLAYERS           (PLAYERS_PER_LEAGUE * TOTAL_LEAGUES)

#define YEAR_SEASON_OFFSET      (2000 - 1845)


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
     fpos_Pitcher          =  0,
     fpos_Catcher          =  1,
     fpos_FirstBaseman     =  2,
     fpos_SecondBaseman    =  3,
     fpos_ThirdBaseman     =  4,
     fpos_ShortStop        =  5,
     fpos_Infield          =  6,
     fpos_LeftField        =  7,
     fpos_CenterField      =  8,
     fpos_RightField       =  9,
     fpos_Outfield         = 10,
     fpos_DesignatedHitter = 12

} fileposition_e;


typedef enum
{
     fh_Right = 0,
     fh_Left  = 1

} filehand_e;

typedef enum
{
     fc_Light = 0,
     fc_Dark  = 64

} filecolor_e;


// ----  LEAGNAME.DAT  ----
typedef struct
{
     unsigned char name [12];

} fileleague_s;

typedef struct
{
     unsigned char name [12];

} filedivision_s;

typedef struct
{
     unsigned char name   [12];
     unsigned char color  [ 1];
     unsigned char wins   [ 1];
     unsigned char losses [ 1];
     unsigned char term   [ 1]; // possible team id

} fileteam_s;


typedef struct
{
     unsigned char  header    [ 52              ];
     fileleague_s   leagues   [ TOTAL_LEAGUES   ];
     filedivision_s divisions [ TOTAL_DIVISIONS ];
     fileteam_s     teams     [ TOTAL_TEAMS     ];

} fileleagname_s;


// ----  PARKS.DAT  ----
typedef struct
{
     unsigned char text[16];

} park_name_s;

typedef struct
{
     unsigned char left_line    [2];
     unsigned char left_center  [2];
     unsigned char center       [2];
     unsigned char right_center [2];
     unsigned char right_line   [2];

} distances_s;

typedef struct
{
     unsigned char left1  [1];
     unsigned char left2  [1];
     unsigned char left3  [1];
     unsigned char left4  [1];
     unsigned char center [1];
     unsigned char right4 [1];
     unsigned char right3 [1];
     unsigned char right2 [1];
     unsigned char right1 [1];

} heights_s;

typedef struct
{
     signed char x[2];
     signed char y[2];
} coords_s;

typedef struct
{
     unsigned char details1  [1];
     unsigned char bleachers [1];
     unsigned char details2  [2];
     distances_s   distances;
     heights_s     heights;
     unsigned char empty     [1];
     coords_s      outfield  [10];
     coords_s      infield1  [8];
     coords_s      infield2  [8];

} park_details_s;

typedef struct
{
     unsigned char  filler       [ 52          ];
     park_name_s    park_names   [ TOTAL_PARKS ];
     park_details_s park_details [ TOTAL_PARKS ];

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
     unsigned char player_id   [ 2];
     unsigned char checksum    [ 1];
     unsigned char filler      [11];

} acc_player_id_s;


typedef struct
{
     unsigned char ratings     [ 4]; // by nibble: bats R = 0/L = 1/S = 2,arm,runs,range,power,longevity,bunt,h&r
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

} filebatting_s;


typedef struct
{
     unsigned char ratings     [ 2]; // by nibble: speed,control,fatigue,bunt/longevity
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

} filepitching_s;


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
          filebatting_s  filebatting;
          filepitching_s filepitching;

     } filestats;

} fileplayer_s;


int             word2int(           const unsigned char *word                                           );
void            int2word(                 unsigned char *word,         const int           value        );
int             nibble(             const unsigned char  byte,         const nibble_e      position     );
void            termName(                 unsigned char *buffer,       const int           len          );
void            untermName(               unsigned char *buffer,       const int           len          );
int             getPlayerId(        const fileplayer_s  *players_file                                   );
fileplayer_s   *findMatchingPlayer( const fileplayer_s  *player,             fileplayer_s *players      );
unsigned char   calcChecksum(             int            value                                          );
char           *getFileUtilsError(        void                                                          );
fileplayer_s   *readPlayersFile(    const char          *filename                                       );
boolean_e       writePlayersFile(   const char          *filename,     const fileplayer_s *playersFile  );
fileleagname_s *readLeagueFile(     const char          *filename                                       );
boolean_e       writeLeagueFile(    const char          *filename,     const fileleagname_s *leagueFile );
fileparks_s    *readParksFile(      const char          *filename                                       );
boolean_e       writeParksFile(     const char          *filename,     const fileparks_s  *parksFile    );

// Utility program methods:
boolean_e genPlayerIds(  fileplayer_s *players_file                               );
boolean_e copyPlayerIds( fileplayer_s *players_file1, fileplayer_s *players_file2 );
void      setLongevity(  fileplayer_s *players_file                               );
boolean_e copyLongevity( fileplayer_s *players_file1, fileplayer_s *players_file2 );

#endif
