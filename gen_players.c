#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "ewbfiles.h"



typedef enum
{
     pos2_Pitcher      = 0,
     pos2_Catcher,
     pos2_FirstBase,
     pos2_SecondBase,
     pos2_ThirdBase,
     pos2_ShortStop,
     pos2_Infield,
     pos2_LeftField,
     pos2_CenterField,
     pos2_RightField,
     pos2_Outfield,
     pos2_Unknown,
     pos2_Designate,
     pos2_Closer

} position2_e;



typedef struct
{
     int start;
     int range;

} random_s;

typedef struct
{
     char first_name[ 30 ];
     char last_name [ 30 ];
     /**/
} names_s;

typedef struct
{
     char first_name[ 30 ];
     char last_name [ 30 ];
     int  color;
     int  hand;
     int  power;
     int  hit_n_run;
     int  bunt;
     int  speed;
     int  range;
     int  arm;
     /**/
} batter_attr_s;

typedef struct
{
     char first_name[ 30 ];
     char last_name [ 30 ];
     int  color;
     int  hand;
     int  speed;
     int  control;
     int  bunt;
     int  fatigue;
     /**/
} pitcher_attr_s;

static const double ab_per_game =  3.85;

static const position2_e positions[] = {
     pos2_Catcher,
     pos2_Catcher,
     pos2_FirstBase,
     pos2_FirstBase,
     pos2_SecondBase,
     pos2_SecondBase,
     pos2_ThirdBase,
     pos2_ThirdBase,
     pos2_ShortStop,
     pos2_LeftField,
     pos2_LeftField,
     pos2_CenterField,
     pos2_CenterField,
     pos2_RightField,
     pos2_Closer,
     pos2_Pitcher,
     pos2_Pitcher,
     pos2_Pitcher,
     pos2_Pitcher,
     pos2_Pitcher,
     pos2_Pitcher,
     pos2_Pitcher,
     pos2_Pitcher,
     pos2_Pitcher,
     pos2_Pitcher
};

static const int secondary_pos[10][6] = {
     //           C    1B    3B    IF    RF    OF
     /*  C */{    0,   30,   30,   10,   20,   10 },
     /* 1B */{   20,    0,    0,   60,    0,   20 },
     /* 2B */{    5,    0,    0,   85,    0,   10 },
     /* 3B */{   20,    0,    0,   70,    0,   10 },
     /* SS */{    5,    0,    0,   75,    0,   20 },
     /* XX */{    0,    0,    0,    0,    0,    0 },
     /* LF */{    5,    0,    0,   10,    0,   85 },
     /* CF */{    5,    0,    0,   20,    0,   75 },
     /* RF */{   20,    0,    0,   10,    0,   70 },
     /* XX */{    0,    0,    0,    0,    0,    0 }
};

// Rating Probabilities
#define RATING_PB_COUNT 11
//                                 0    1   2     3     4     5     6     7     8     9   10
//            percentage:          0  .05  .9  3.75  12.5    50    25  6.25  1.25   0.2  0.1
static const int rating_pbs[]  = { 0,   4, 72,  300, 1000, 4000, 2000,  500,  100,   16,   8 };
//                                 0    1   2     3     4     5     6     7     8     9   10
//            percentage:          0  .05  .9  3.75  12.5    50    25  6.25  1.25   0.2  0.1
static const int closer_pbs[]  = { 0,   0,  0,    0,    0,    0,  320, 4000, 2000, 1040, 640 };
//                                 0    1   2     3     4     5     6     7     8     9   10
//            percentage:          0  .05  .9  3.75  12.5    50    25  6.25  1.25   0.2  0.1
static const int power_pbs[]   = { 0,   4, 72,  300, 1000, 4000, 2000,  508,  116,    0,   0 };
//                                 0    1   2     3     4     5     6     7     8     9   10
//            percentage:          0    0   0     0  1.25  12.5    50    25    10  0.95  0.3
static const int fatigue_pbs[] = { 0,   0,  0,    0,  100, 1000, 4000, 2000,  800,   76,  24 };

static const random_s batting_avg[10][10] = {
     { { 265,  7 }, { 266,  7 }, { 267,  7 }, { 268,  7 }, { 269,  7 }, { 269,  7 }, { 270,  7 }, { 271,  7 }, { 272,  7 }, { 273,  7 } },
     { { 270, 10 }, { 272, 10 }, { 273, 10 }, { 274, 10 }, { 275, 10 }, { 275, 10 }, { 276, 10 }, { 277, 10 }, { 278, 10 }, { 280, 10 } },
     { { 275, 12 }, { 277, 12 }, { 279, 12 }, { 280, 12 }, { 281, 12 }, { 282, 12 }, { 283, 12 }, { 284, 12 }, { 286, 12 }, { 288, 12 } },
     { { 280, 15 }, { 282, 15 }, { 284, 15 }, { 286, 15 }, { 287, 15 }, { 288, 15 }, { 289, 15 }, { 291, 15 }, { 293, 15 }, { 295, 15 } },
     { { 290, 15 }, { 292, 15 }, { 294, 15 }, { 296, 15 }, { 297, 15 }, { 298, 15 }, { 299, 15 }, { 301, 15 }, { 303, 15 }, { 305, 15 } },
     { { 300, 15 }, { 302, 15 }, { 304, 15 }, { 306, 15 }, { 307, 15 }, { 308, 15 }, { 309, 15 }, { 311, 15 }, { 313, 15 }, { 315, 15 } },
     { { 310, 20 }, { 312, 20 }, { 315, 20 }, { 317, 20 }, { 320, 20 }, { 320, 20 }, { 322, 20 }, { 325, 20 }, { 328, 20 }, { 330, 20 } },
     { { 320, 20 }, { 322, 20 }, { 325, 20 }, { 327, 20 }, { 330, 20 }, { 330, 20 }, { 332, 20 }, { 335, 20 }, { 338, 20 }, { 340, 20 } },
     { { 330, 20 }, { 332, 20 }, { 335, 20 }, { 337, 20 }, { 340, 20 }, { 340, 20 }, { 342, 20 }, { 345, 20 }, { 348, 20 }, { 350, 20 } },
     { { 350, 25 }, { 355, 25 }, { 360, 25 }, { 365, 25 }, { 370, 25 }, { 370, 25 }, { 370, 25 }, { 370, 25 }, { 375, 25 }, { 375, 25 } }
};

static const random_s triples[10][10] = {
     { {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  2 }, {  0,  4 }, {  0,  6 }, {  0,  7 }, {  0, 10 } },
     { {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  3 }, {  0,  4 }, {  0,  6 }, {  0,  8 }, {  2,  8 }, {  3,  8 } },
     { {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  3 }, {  0,  5 }, {  0,  7 }, {  0,  8 }, {  2,  8 }, {  4,  8 }, {  7,  7 } },
     { {  0,  0 }, {  0,  0 }, {  0,  3 }, {  0,  6 }, {  0,  7 }, {  0,  9 }, {  3,  7 }, {  5,  7 }, {  7,  8 }, {  9,  9 } },
     { {  0,  0 }, {  0,  3 }, {  0,  5 }, {  0,  7 }, {  0, 10 }, {  3,  7 }, {  5,  7 }, {  8,  7 }, { 10,  8 }, { 10, 10 } },
     { {  0,  2 }, {  0,  4 }, {  0,  7 }, {  0,  9 }, {  3,  7 }, {  6,  6 }, {  8,  8 }, { 10,  9 }, { 11,  9 }, { 12,  8 } },
     { {  0,  4 }, {  0,  6 }, {  0,  8 }, {  3,  7 }, {  5,  7 }, {  8,  8 }, { 10, 10 }, { 11, 10 }, { 13,  7 }, { 14,  9 } },
     { {  0,  6 }, {  0,  8 }, {  2,  8 }, {  5,  7 }, {  8,  7 }, { 10,  9 }, { 11, 10 }, { 13,  8 }, { 15,  8 }, { 16,  9 } },
     { {  0,  7 }, {  2,  8 }, {  4,  8 }, {  7,  8 }, { 10,  8 }, { 11,  9 }, { 13,  7 }, { 15,  8 }, { 18,  7 }, { 18,  9 } },
     { {  0, 10 }, {  3,  8 }, {  7,  7 }, {  9,  9 }, { 10, 10 }, { 12,  8 }, { 14,  9 }, { 16,  9 }, { 18,  9 }, { 20, 10 } }
};

static const random_s fielding_avg[10][10] = {
     { { 750, 100 }, { 765, 100 }, { 780, 100 }, { 790, 100 }, { 800, 100 }, { 805, 100 }, { 810, 100 }, { 815, 100 }, { 820, 100 }, { 825, 100 } },
     { { 765, 100 }, { 780, 100 }, { 790, 100 }, { 800, 100 }, { 805, 100 }, { 810, 100 }, { 815, 100 }, { 820, 100 }, { 825, 100 }, { 830, 100 } },
     { { 780, 100 }, { 790, 100 }, { 800, 100 }, { 805, 100 }, { 810, 100 }, { 815, 100 }, { 820, 100 }, { 825, 100 }, { 830, 100 }, { 835, 100 } },
     { { 790, 100 }, { 800, 100 }, { 805, 100 }, { 810, 100 }, { 815, 100 }, { 820, 100 }, { 825, 100 }, { 830, 100 }, { 835, 100 }, { 840, 100 } },
     { { 800, 100 }, { 805, 100 }, { 810, 100 }, { 815, 100 }, { 820, 100 }, { 825, 100 }, { 830, 100 }, { 835, 100 }, { 840, 100 }, { 845, 100 } },
     { { 805, 100 }, { 810, 100 }, { 815, 100 }, { 820, 100 }, { 825, 100 }, { 830, 100 }, { 835, 100 }, { 840, 100 }, { 845, 100 }, { 850, 100 } },
     { { 810, 100 }, { 815, 100 }, { 820, 100 }, { 825, 100 }, { 830, 100 }, { 835, 100 }, { 840, 100 }, { 845, 100 }, { 850, 100 }, { 860, 100 } },
     { { 815, 100 }, { 820, 100 }, { 825, 100 }, { 830, 100 }, { 835, 100 }, { 840, 100 }, { 845, 100 }, { 850, 100 }, { 860, 100 }, { 870, 100 } },
     { { 820, 100 }, { 825, 100 }, { 830, 100 }, { 835, 100 }, { 840, 100 }, { 845, 100 }, { 850, 100 }, { 860, 100 }, { 870, 100 }, { 885, 100 } },
     { { 825, 100 }, { 830, 100 }, { 835, 100 }, { 840, 100 }, { 845, 100 }, { 850, 100 }, { 860, 100 }, { 870, 100 }, { 885, 100 }, { 900, 100 } }
};

static const random_s vs_batting_avg[10][10] = {
     { { 250, 35 }, { 235, 20 }, { 225, 15 }, { 215, 15 }, { 205, 15 }, { 200, 10 }, { 195, 10 }, { 190, 10 }, { 185, 10 }, { 182,  8 } },
     { { 235, 20 }, { 225, 15 }, { 215, 15 }, { 205, 15 }, { 200, 10 }, { 195, 10 }, { 190, 10 }, { 185, 10 }, { 182,  8 }, { 180,  7 } },
     { { 225, 15 }, { 215, 15 }, { 205, 15 }, { 200, 10 }, { 195, 10 }, { 190, 10 }, { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 } },
     { { 215, 15 }, { 205, 15 }, { 200, 10 }, { 195, 10 }, { 190, 10 }, { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 170, 10 } },
     { { 205, 15 }, { 200, 10 }, { 195, 10 }, { 190, 10 }, { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 170, 10 }, { 165, 10 } },
     { { 200, 10 }, { 195, 10 }, { 190, 10 }, { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 170, 10 }, { 165, 10 }, { 160, 10 } },
     { { 195, 10 }, { 190, 10 }, { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 170, 10 }, { 165, 10 }, { 160, 10 }, { 150, 15 } },
     { { 190, 10 }, { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 170, 10 }, { 165, 10 }, { 160, 10 }, { 150, 15 }, { 140, 15 } },
     { { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 170, 10 }, { 165, 10 }, { 160, 10 }, { 150, 15 }, { 140, 15 }, { 130, 15 } },
     { { 182,  8 }, { 180,  7 }, { 175, 10 }, { 170, 10 }, { 165, 10 }, { 160, 10 }, { 150, 15 }, { 140, 15 }, { 130, 15 }, { 115, 20 } }
};

static const random_s earned_run_avg[10][10] = {
     { { 290, 35 }, { 265, 35 }, { 245, 30 }, { 230, 25 }, { 220, 20 }, { 210, 20 }, { 200, 20 }, { 190, 20 }, { 185, 15 }, { 182,  8 } },
     { { 265, 30 }, { 245, 30 }, { 230, 25 }, { 220, 20 }, { 210, 20 }, { 200, 20 }, { 190, 20 }, { 185, 15 }, { 182,  8 }, { 180,  7 } },
     { { 245, 25 }, { 230, 25 }, { 220, 20 }, { 210, 20 }, { 200, 20 }, { 190, 20 }, { 185, 15 }, { 182,  8 }, { 180,  7 }, { 175, 10 } },
     { { 230, 20 }, { 220, 20 }, { 210, 20 }, { 200, 20 }, { 190, 20 }, { 185, 15 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 165, 15 } },
     { { 220, 15 }, { 210, 20 }, { 200, 20 }, { 190, 20 }, { 185, 15 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 165, 15 }, { 155, 15 } },
     { { 210, 15 }, { 200, 20 }, { 190, 20 }, { 185, 15 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 165, 15 }, { 155, 15 }, { 145, 15 } },
     { { 200, 15 }, { 190, 20 }, { 185, 15 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 165, 15 }, { 155, 15 }, { 145, 15 }, { 135, 15 } },
     { { 190, 15 }, { 185, 15 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 165, 15 }, { 155, 15 }, { 145, 15 }, { 135, 15 }, { 120, 20 } },
     { { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 165, 15 }, { 155, 15 }, { 145, 15 }, { 135, 15 }, { 120, 20 }, { 100, 25 } },
     { { 182,  8 }, { 180,  7 }, { 175, 10 }, { 165, 15 }, { 155, 15 }, { 145, 15 }, { 135, 15 }, { 120, 20 }, { 100, 25 }, {  75, 30 } }
};

static const random_s innings_per_game[10][10] = {
     { { 500, 30 }, { 525, 25 }, { 545, 25 }, { 565, 20 }, { 580, 20 }, { 595, 15 }, { 605, 15 }, { 615, 15 }, { 625, 12 }, { 632, 13 } },
     { { 525, 25 }, { 545, 25 }, { 565, 20 }, { 580, 20 }, { 595, 15 }, { 605, 15 }, { 615, 15 }, { 625, 12 }, { 632, 13 }, { 640, 15 } },
     { { 545, 25 }, { 565, 20 }, { 580, 20 }, { 595, 15 }, { 605, 15 }, { 615, 15 }, { 625, 12 }, { 632, 13 }, { 640, 15 }, { 650, 15 } },
     { { 565, 20 }, { 580, 20 }, { 595, 15 }, { 605, 15 }, { 615, 15 }, { 625, 12 }, { 632, 13 }, { 640, 15 }, { 650, 15 }, { 660, 15 } },
     { { 580, 20 }, { 595, 15 }, { 605, 15 }, { 615, 15 }, { 625, 12 }, { 632, 13 }, { 640, 15 }, { 650, 15 }, { 660, 15 }, { 670, 20 } },
     { { 595, 15 }, { 605, 15 }, { 615, 15 }, { 625, 12 }, { 632, 13 }, { 640, 15 }, { 650, 15 }, { 660, 15 }, { 670, 20 }, { 685, 20 } },
     { { 605, 15 }, { 615, 15 }, { 625, 12 }, { 632, 13 }, { 640, 15 }, { 650, 15 }, { 660, 15 }, { 670, 20 }, { 685, 20 }, { 700, 25 } },
     { { 615, 15 }, { 625, 12 }, { 632, 13 }, { 640, 15 }, { 650, 15 }, { 660, 15 }, { 670, 20 }, { 685, 20 }, { 700, 25 }, { 720, 25 } },
     { { 625, 12 }, { 632, 13 }, { 640, 15 }, { 650, 15 }, { 660, 15 }, { 670, 20 }, { 685, 20 }, { 700, 25 }, { 720, 25 }, { 740, 30 } },
     { { 632, 13 }, { 640, 15 }, { 650, 15 }, { 660, 15 }, { 670, 20 }, { 685, 20 }, { 700, 25 }, { 720, 25 }, { 740, 30 }, { 765, 35 } }
};

static const random_s win_pct[10][10] = {
     { { 500, 35 }, { 530, 35 }, { 560, 30 }, { 585, 30 }, { 610, 30 }, { 635, 30 }, { 660, 30 }, { 685, 30 }, { 710, 30 }, { 735, 30 } },
     { { 530, 35 }, { 560, 30 }, { 585, 30 }, { 610, 30 }, { 635, 30 }, { 660, 30 }, { 685, 30 }, { 710, 30 }, { 735, 30 }, { 760, 30 } },
     { { 560, 30 }, { 585, 30 }, { 610, 30 }, { 635, 30 }, { 660, 30 }, { 685, 30 }, { 710, 30 }, { 735, 30 }, { 760, 30 }, { 785, 30 } },
     { { 585, 30 }, { 610, 30 }, { 635, 30 }, { 660, 30 }, { 685, 30 }, { 710, 30 }, { 735, 30 }, { 760, 30 }, { 785, 30 }, { 810, 30 } },
     { { 610, 30 }, { 635, 30 }, { 660, 30 }, { 685, 30 }, { 710, 30 }, { 735, 30 }, { 760, 30 }, { 785, 30 }, { 810, 30 }, { 835, 30 } },
     { { 635, 30 }, { 660, 30 }, { 685, 30 }, { 710, 30 }, { 735, 30 }, { 760, 30 }, { 785, 30 }, { 810, 30 }, { 835, 30 }, { 860, 30 } },
     { { 660, 30 }, { 685, 30 }, { 710, 30 }, { 735, 30 }, { 760, 30 }, { 785, 30 }, { 810, 30 }, { 835, 30 }, { 860, 30 }, { 885, 30 } },
     { { 685, 30 }, { 710, 30 }, { 735, 30 }, { 760, 30 }, { 785, 30 }, { 810, 30 }, { 835, 30 }, { 860, 30 }, { 885, 30 }, { 910, 30 } },
     { { 710, 30 }, { 735, 30 }, { 760, 30 }, { 785, 30 }, { 810, 30 }, { 835, 30 }, { 860, 30 }, { 885, 30 }, { 910, 30 }, { 935, 35 } },
     { { 735, 30 }, { 760, 30 }, { 785, 30 }, { 810, 30 }, { 835, 30 }, { 860, 30 }, { 885, 30 }, { 910, 30 }, { 935, 35 }, { 965, 35 } }
};
/*
static const random_s save_pct[10][10] = {
     { {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
     { {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
     { {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
     { {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
     { {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
     { {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
     { {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
     { {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
     { {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
     { {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } }
};
*/
static const random_s closer_vs_batting_avg[10][10] = {
     { { 235, 35 }, { 220, 20 }, { 210, 15 }, { 200, 15 }, { 190, 15 }, { 185, 10 }, { 180, 10 }, { 175, 10 }, { 170, 10 }, { 167,  8 } },
     { { 220, 20 }, { 210, 15 }, { 200, 15 }, { 190, 15 }, { 185, 10 }, { 180, 10 }, { 175, 10 }, { 170, 10 }, { 167,  8 }, { 165,  7 } },
     { { 210, 15 }, { 200, 15 }, { 190, 15 }, { 185, 10 }, { 180, 10 }, { 175, 10 }, { 170, 10 }, { 167,  8 }, { 165,  7 }, { 160, 10 } },
     { { 200, 15 }, { 190, 15 }, { 185, 10 }, { 180, 10 }, { 175, 10 }, { 170, 10 }, { 167,  8 }, { 165,  7 }, { 160, 10 }, { 155, 10 } },
     { { 190, 15 }, { 185, 10 }, { 180, 10 }, { 175, 10 }, { 170, 10 }, { 167,  8 }, { 165,  7 }, { 160, 10 }, { 155, 10 }, { 150, 10 } },
     { { 185, 10 }, { 180, 10 }, { 175, 10 }, { 170, 10 }, { 167,  8 }, { 165,  7 }, { 160, 10 }, { 155, 10 }, { 150, 10 }, { 145, 10 } },
     { { 180, 10 }, { 175, 10 }, { 170, 10 }, { 167,  8 }, { 165,  7 }, { 160, 10 }, { 155, 10 }, { 150, 10 }, { 145, 10 }, { 135, 15 } },
     { { 175, 10 }, { 170, 10 }, { 167,  8 }, { 165,  7 }, { 160, 10 }, { 155, 10 }, { 150, 10 }, { 145, 10 }, { 135, 15 }, { 125, 15 } },
     { { 170, 10 }, { 167,  8 }, { 165,  7 }, { 160, 10 }, { 155, 10 }, { 150, 10 }, { 145, 10 }, { 135, 15 }, { 125, 15 }, { 115, 15 } },
     { { 167,  8 }, { 165,  7 }, { 160, 10 }, { 155, 10 }, { 150, 10 }, { 145, 10 }, { 135, 15 }, { 125, 15 }, { 115, 15 }, { 100, 20 } }
};

static const random_s closer_earned_run_avg[10][10] = {
     { { 265, 35 }, { 240, 35 }, { 220, 30 }, { 205, 25 }, { 195, 20 }, { 185, 20 }, { 175, 20 }, { 165, 20 }, { 160, 15 }, { 157,  8 } },
     { { 240, 30 }, { 220, 30 }, { 205, 25 }, { 195, 20 }, { 185, 20 }, { 175, 20 }, { 165, 20 }, { 160, 15 }, { 157,  8 }, { 155,  7 } },
     { { 220, 25 }, { 205, 25 }, { 195, 20 }, { 185, 20 }, { 175, 20 }, { 165, 20 }, { 160, 15 }, { 157,  8 }, { 155,  7 }, { 150, 10 } },
     { { 205, 20 }, { 195, 20 }, { 185, 20 }, { 175, 20 }, { 165, 20 }, { 160, 15 }, { 157,  8 }, { 155,  7 }, { 150, 10 }, { 140, 15 } },
     { { 195, 15 }, { 185, 20 }, { 175, 20 }, { 165, 20 }, { 160, 15 }, { 157,  8 }, { 155,  7 }, { 150, 10 }, { 140, 15 }, { 130, 15 } },
     { { 185, 15 }, { 175, 20 }, { 165, 20 }, { 160, 15 }, { 157,  8 }, { 155,  7 }, { 150, 10 }, { 140, 15 }, { 130, 15 }, { 120, 15 } },
     { { 175, 15 }, { 165, 20 }, { 160, 15 }, { 157,  8 }, { 155,  7 }, { 150, 10 }, { 140, 15 }, { 130, 15 }, { 120, 15 }, { 110, 15 } },
     { { 165, 15 }, { 160, 15 }, { 157,  8 }, { 155,  7 }, { 150, 10 }, { 140, 15 }, { 130, 15 }, { 120, 15 }, { 110, 15 }, {  95, 20 } },
     { { 160, 10 }, { 157,  8 }, { 155,  7 }, { 150, 10 }, { 140, 15 }, { 130, 15 }, { 120, 15 }, { 110, 15 }, {  95, 20 }, {  75, 25 } },
     { { 157,  8 }, { 155,  7 }, { 150, 10 }, { 140, 15 }, { 130, 15 }, { 120, 15 }, { 110, 15 }, {  95, 20 }, {  75, 25 }, {  50, 30 } }
};

static const random_s save_pct[10][10] = {
     { { 430,  20 }, { 440,  25 }, { 450,  25 }, { 460,  30 }, { 470,  30 }, { 480,  40 }, { 490,  45 }, { 500,  45 }, { 515,  50 }, { 530,  50 } },
     { { 440,  25 }, { 450,  25 }, { 460,  30 }, { 470,  30 }, { 480,  40 }, { 490,  45 }, { 500,  45 }, { 515,  50 }, { 530,  50 }, { 545,  60 } },
     { { 450,  25 }, { 460,  30 }, { 470,  30 }, { 480,  40 }, { 490,  45 }, { 500,  45 }, { 515,  50 }, { 530,  50 }, { 545,  60 }, { 560,  60 } },
     { { 460,  30 }, { 470,  30 }, { 480,  40 }, { 490,  45 }, { 500,  45 }, { 515,  50 }, { 530,  50 }, { 545,  60 }, { 560,  60 }, { 575,  75 } },
     { { 470,  30 }, { 480,  40 }, { 490,  45 }, { 500,  45 }, { 515,  50 }, { 530,  50 }, { 545,  60 }, { 560,  60 }, { 575,  75 }, { 600,  75 } },
     { { 480,  40 }, { 490,  45 }, { 500,  45 }, { 515,  50 }, { 530,  50 }, { 545,  60 }, { 560,  60 }, { 575,  75 }, { 600,  75 }, { 625,  75 } },
     { { 490,  45 }, { 500,  45 }, { 515,  50 }, { 530,  50 }, { 545,  60 }, { 560,  60 }, { 575,  75 }, { 600,  75 }, { 625,  75 }, { 650, 100 } },
     { { 500,  45 }, { 515,  50 }, { 530,  50 }, { 545,  60 }, { 560,  60 }, { 575,  75 }, { 600,  75 }, { 625,  75 }, { 650, 100 }, { 675, 100 } },
     { { 515,  50 }, { 530,  50 }, { 545,  60 }, { 560,  60 }, { 575,  75 }, { 600,  75 }, { 625,  75 }, { 650, 100 }, { 675, 100 }, { 700, 100 } },
     { { 530,  50 }, { 545,  60 }, { 560,  60 }, { 575,  75 }, { 600,  75 }, { 625,  75 }, { 650, 100 }, { 675, 100 }, { 700, 100 }, { 750, 150 } }
};

static const random_s at_bats_per_inn[10][10] = {
     //            1            2            3            4            5            6            7            8            9           10
     /*  1 */{ { 435, 15 }, { 425, 15 }, { 415, 15 }, { 405, 15 }, { 395, 15 }, { 385, 15 }, { 380, 10 }, { 375, 10 }, { 370, 10 }, { 367,  8 } },
     /*  2 */{ { 425, 15 }, { 415, 15 }, { 405, 15 }, { 395, 15 }, { 385, 15 }, { 380, 10 }, { 375, 10 }, { 370, 10 }, { 367,  8 }, { 365,  7 } },
     /*  3 */{ { 415, 15 }, { 405, 15 }, { 395, 15 }, { 385, 15 }, { 380, 10 }, { 375, 10 }, { 370, 10 }, { 367,  8 }, { 365,  7 }, { 360, 10 } },
     /*  4 */{ { 405, 15 }, { 395, 15 }, { 385, 15 }, { 380, 10 }, { 375, 10 }, { 370, 10 }, { 367,  8 }, { 365,  7 }, { 360, 10 }, { 355, 10 } },
     /*  5 */{ { 395, 15 }, { 385, 15 }, { 380, 10 }, { 375, 10 }, { 370, 10 }, { 367,  8 }, { 365,  7 }, { 360, 10 }, { 355, 10 }, { 350, 10 } },
     /*  6 */{ { 385, 15 }, { 380, 10 }, { 375, 10 }, { 370, 10 }, { 367,  8 }, { 365,  7 }, { 360, 10 }, { 355, 10 }, { 350, 10 }, { 340, 15 } },
     /*  7 */{ { 380, 10 }, { 375, 10 }, { 370, 10 }, { 367,  8 }, { 365,  7 }, { 360, 10 }, { 355, 10 }, { 350, 10 }, { 340, 15 }, { 330, 15 } },
     /*  8 */{ { 375, 10 }, { 370, 10 }, { 367,  8 }, { 365,  7 }, { 360, 10 }, { 355, 10 }, { 350, 10 }, { 340, 15 }, { 330, 15 }, { 320, 15 } },
     /*  9 */{ { 370, 10 }, { 367,  8 }, { 365,  7 }, { 360, 10 }, { 355, 10 }, { 350, 10 }, { 340, 15 }, { 330, 15 }, { 320, 15 }, { 310, 15 } },
     /* 10 */{ { 367,  8 }, { 365,  7 }, { 360, 10 }, { 355, 10 }, { 350, 10 }, { 340, 15 }, { 330, 15 }, { 320, 15 }, { 310, 15 }, { 300, 15 } }
};

static const random_s put_outs[10][10] = {
     /*  C */{ { 300, 200 }, { 325, 200 }, { 350, 200 }, { 375, 200 }, { 390, 200 }, { 410, 200 }, { 425, 200 }, { 450, 200 }, { 475, 200 }, { 500, 200 } },
     /* 1B */{ { 400, 200 }, { 433, 200 }, { 466, 200 }, { 500, 200 }, { 533, 200 }, { 566, 200 }, { 600, 200 }, { 633, 200 }, { 666, 200 }, { 700, 200 } },
     /* 2B */{ {  50, 100 }, {  65, 100 }, {  80, 100 }, {  90, 100 }, {  97, 100 }, { 103, 100 }, { 110, 100 }, { 120, 100 }, { 135, 100 }, { 150, 100 } },
     /* 3B */{ {  20,  40 }, {  25,  40 }, {  30,  40 }, {  35,  40 }, {  38,  40 }, {  42,  40 }, {  45,  40 }, {  50,  40 }, {  55,  40 }, {  60,  40 } },
     /* SS */{ {  50,  70 }, {  75,  70 }, { 100,  70 }, { 110,  70 }, { 120,  70 }, { 130,  70 }, { 140,  70 }, { 150,  70 }, { 175,  70 }, { 130,  70 } },
     /* XX */{ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
     /* LF */{ {  50, 100 }, {  65, 100 }, {  80, 100 }, {  90, 100 }, {  97, 100 }, { 103, 100 }, { 110, 100 }, { 120, 100 }, { 135, 100 }, { 150, 100 } },
     /* CF */{ {  50, 100 }, {  70, 100 }, {  90, 100 }, { 110, 100 }, { 120, 100 }, { 130, 100 }, { 140, 100 }, { 160, 100 }, { 180, 100 }, { 200, 100 } },
     /* RF */{ {  50, 100 }, {  65, 100 }, {  80, 100 }, {  90, 100 }, {  97, 100 }, { 103, 100 }, { 110, 100 }, { 120, 100 }, { 135, 100 }, { 150, 100 } },
     /* XX */{ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } }
};

static const random_s assists[10][10] = {
     /*  C */{ {  20,  20 }, {  23,  20 }, {  26,  20 }, {  28,  20 }, {  30,  20 }, {  30,  20 }, {  32,  20 }, {  34,  20 }, {  37,  20 }, {  40,  20 } },
     /* 1B */{ {  20,  40 }, {  25,  40 }, {  30,  40 }, {  35,  40 }, {  38,  40 }, {  42,  40 }, {  45,  40 }, {  50,  40 }, {  55,  40 }, {  60,  40 } },
     /* 2B */{ {  50, 150 }, {  70, 150 }, {  90, 150 }, { 110, 150 }, { 120, 150 }, { 130, 150 }, { 140, 150 }, { 160, 150 }, { 180, 150 }, { 200, 150 } },
     /* 3B */{ {  50, 100 }, {  65, 100 }, {  80, 100 }, {  90, 100 }, {  97, 100 }, { 103, 100 }, { 110, 100 }, { 120, 100 }, { 135, 100 }, { 150, 100 } },
     /* SS */{ { 100, 150 }, { 120, 150 }, { 140, 150 }, { 160, 150 }, { 170, 150 }, { 180, 150 }, { 190, 150 }, { 210, 150 }, { 230, 150 }, { 250, 150 } },
     /* XX */{ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
     /* LF */{ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   1 }, {   0,   3 }, {   0,   6 }, {   2,   6 }, {   3,   8 }, {   5,   8 }, {   8,  10 } },
     /* CF */{ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   1 }, {   0,   3 }, {   0,   6 }, {   2,   6 }, {   3,   8 }, {   5,   8 }, {   8,  10 } },
     /* RF */{ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   1 }, {   0,   3 }, {   0,   6 }, {   2,   6 }, {   3,   8 }, {   5,   8 }, {   8,  10 } },
     /* XX */{ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } }
};

static const random_s extra_hits[10]    = { {   0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  0 }, {   1,  2 }, {   2,  3 }, {   4,  4 }, {   7,  4 }, {  10,  5 } };
static const random_s strikeout_avg[10] = { { 145,  5 }, { 140, 10 }, { 130, 20 }, { 125, 25 }, { 120, 30 }, { 110, 35 }, { 100, 40 }, {  90, 40 }, {  75, 50 }, {  50, 50 } };
static const random_s home_runs[10]     = { {   0,  5 }, {   0,  8 }, {   5,  7 }, {   8,  8 }, {  10, 10 }, {  15, 10 }, {  20, 12 }, {  25, 15 }, {  30, 20 }, {  35, 25 } };
static const random_s runs[10]          = { {  15, 10 }, {  18, 12 }, {  20, 15 }, {  25, 18 }, {  30, 20 }, {  35, 25 }, {  40, 30 }, {  50, 35 }, {  60, 40 }, {  75, 50 } };
static const random_s rbi[10]           = { {  15, 10 }, {  18, 12 }, {  20, 15 }, {  25, 18 }, {  30, 20 }, {  35, 25 }, {  40, 30 }, {  50, 35 }, {  60, 40 }, {  75, 50 } };
static const random_s walks[10]         = { {  15, 10 }, {  18, 12 }, {  20, 15 }, {  25, 18 }, {  30, 20 }, {  35, 25 }, {  40, 30 }, {  50, 35 }, {  60, 40 }, {  75, 50 } };
static const random_s steals[10]        = { {   0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  5 }, {   0, 10 }, {  10, 20 }, {  20, 30 }, {  40, 35 }, {  60, 40 } };
static const random_s doubles[10]       = { {  10,  8 }, {  10, 10 }, {  10, 12 }, {  12, 15 }, {  15, 15 }, {  17, 15 }, {  20, 15 }, {  20, 17 }, {  25, 20 }, {  30, 20 } };
static const random_s so_per_9[10]      = { {  40, 10 }, {  45, 10 }, {  50, 10 }, {  55, 10 }, {  60, 15 }, {  70, 15 }, {  80, 15 }, {  90, 15 }, { 100, 15 }, { 110, 20 } };
static const random_s walks_per_9[10]   = { {  70, 15 }, {  60, 15 }, {  50, 15 }, {  40, 15 }, {  35, 10 }, {  30, 10 }, {  25, 10 }, {  20, 10 }, {  15, 10 }, {  10, 10 } };
static const random_s hr_per_9[10]      = { { 185, 15 }, { 165, 25 }, { 145, 25 }, { 125, 25 }, { 110, 20 }, { 100, 15 }, {  85, 20 }, {  65, 25 }, {  45, 25 }, {  25, 25 } };
static const random_s cg_pct[10]        = { {   0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  5 }, {   0, 10 }, {   5, 25 }, {  20, 45 }, {  40, 60 } };

static int roll( const int start, const int range )
{
     return (rand() % (range + 1)) + start;
     //return (range / 2) + start;
}


static int getValueForOne( const random_s *array, const int value )
{
     const random_s *rnd = &(array[value - 1]);

     return roll( rnd->start, rnd->range );
}


static int getValueForTwo( const random_s (*array)[10], const int value1, const int value2 )
{
     const random_s *rnd = &(array[value1 - 1][value2 - 1]);

     return roll( rnd->start, rnd->range );
}


static int probabilityRoll( const int probabilities[], const int count )
{
     int total = 0;

     for ( int i = 0; i < count; ++i )
     {
          total += probabilities[i];
     }

     int value = roll( 1, total - 1 );

     for ( int i = 0; i < count; ++i )
     {
          if ( (value -= probabilities[i]) <= 0 ) return i;
     }

     return 0;
}


static int getFatigueFactor( const int fatigue )
{//                                 0  1  2  3    4    5    6    7    8    9   10
     static const int factors[] = { 0, 0, 0, 0, 550, 650, 750, 850, 900, 900, 900 };

     return factors[fatigue];
}


static position2_e getSecondaryPosition( const position2_e position )
{
     int value = roll( 0, 100 );

     for ( int i = 0; i < 6; ++i )
     {
          if ( (value -= secondary_pos[position - 1][i]) <= 0 )
          {
               switch ( i )
               {
               case 0: return pos2_Catcher;
               case 1: return pos2_FirstBase;
               case 2: return pos2_ThirdBase;
               case 3: return pos2_Infield;
               case 4: return pos2_RightField;
               case 5: return pos2_Outfield;
               }
          }
     }

     return pos2_Designate;
}


static void setBatterRatings( char ratings[], const batter_attr_s *attr )
{
     ratings[0] = (attr->hand  << 4) + attr->arm;
     ratings[1] = (attr->speed << 4) + attr->range;
     ratings[2] = (attr->power << 4);
     ratings[3] = (attr->bunt  << 4) + attr->hit_n_run;
}


static void setPitcherRatings( char ratings[], const pitcher_attr_s *attr )
{
     ratings[0] = attr->speed   * 0x10 + attr->control;
     ratings[1] = attr->fatigue * 0x10 + attr->bunt;
}


static names_s *readNamesFile( const char *filename )
{
     FILE    *pNamesFile;
     names_s *names;
     char     fn[50];
     char     ln[50];
     int      scanned;
     int      list_size;
     int      entries;

     if ( (pNamesFile = fopen( filename, "r" )) == NULL )
     {
          printf( "Cannot open file <%s>: %s\n", filename, strerror(errno) );

          return NULL;
     }

     names     = NULL;
     entries   = 0;
     list_size = 0;

     while ( (scanned = fscanf( pNamesFile, "%s %s\n", fn, ln )) != EOF )
     {
          if ( scanned != 2 )
          {
               printf( "Incorrect number of words.\n" );

               return NULL;
          }

          if ( names == NULL )
          {
               list_size = 100;

               if ( (names = (names_s *)malloc( sizeof(names_s) * list_size )) == NULL )
               {
                    printf( "Unable to allocate memory for names list.\n" );

                    return NULL;
               }
          }

          if ( entries == list_size )
          {
               names_s *tmp_names;

               list_size *= 2;

               if ( (tmp_names = (names_s *)realloc( names, sizeof(names_s) * list_size )) == NULL )
               {
                    printf( "Unable to allocate memory for names list.\n" );

                    free( names );

                    return NULL;
               }

               names = tmp_names;
          }

          strncpy( names[entries].first_name, fn, sizeof(names[entries].first_name) );
          strncpy( names[entries].last_name,  ln, sizeof(names[entries].last_name)  );

          entries++;
     }

     return names;
}


static void rollBatterAttributes( batter_attr_s *attributes )
{
     attributes->color       = ((rand() % 100) >= 50) ? '\0' : '@';
     attributes->hand        = ((rand() % 100) >= 50) ? 0 : 1;

     attributes->power       = probabilityRoll( power_pbs,  RATING_PB_COUNT );
     attributes->hit_n_run   = probabilityRoll( rating_pbs, RATING_PB_COUNT );
     attributes->bunt        = probabilityRoll( rating_pbs, RATING_PB_COUNT );
     attributes->speed       = probabilityRoll( rating_pbs, RATING_PB_COUNT );
     attributes->range       = probabilityRoll( rating_pbs, RATING_PB_COUNT );
     attributes->arm         = probabilityRoll( rating_pbs, RATING_PB_COUNT );
}


static void rollPitcherAttributes( pitcher_attr_s *attributes )
{
     attributes->color       = ((rand() % 100) >= 50) ? '\0' : '@';
     attributes->hand        = ((rand() % 100) >= 50) ? 0 : 1;

     attributes->speed       = probabilityRoll( rating_pbs,  RATING_PB_COUNT );
     attributes->control     = probabilityRoll( rating_pbs,  RATING_PB_COUNT );
     attributes->fatigue     = probabilityRoll( fatigue_pbs, RATING_PB_COUNT );
     attributes->bunt        = probabilityRoll( rating_pbs,  RATING_PB_COUNT );
}


static void rollCloserAttributes( pitcher_attr_s *attributes )
{
     attributes->color       = ((rand() % 100) >= 50) ? '\0' : '@';
     attributes->hand        = ((rand() % 100) >= 50) ? 0 : 1;

     attributes->speed       = probabilityRoll( closer_pbs,  RATING_PB_COUNT );
     attributes->control     = probabilityRoll( closer_pbs,  RATING_PB_COUNT );
     attributes->fatigue     = 1;
     attributes->bunt        = probabilityRoll( rating_pbs,  RATING_PB_COUNT );
}


static void generatePitcher( fileplayer_s *player, const names_s *name )
{
     pitcher_attr_s attributes = { 0 };

     rollPitcherAttributes( &attributes );

     strcpy( attributes.last_name,  name->last_name  );
     strcpy( attributes.first_name, name->first_name );

     int games = (rand() % 15) + 30;
     int era   = getValueForTwo( earned_run_avg,   attributes.speed, attributes.control );
     int vsba  = getValueForTwo( vs_batting_avg,   attributes.speed, attributes.control );
     int innpg = getValueForTwo( innings_per_game, attributes.speed, attributes.control );
     int wpct  = getValueForTwo( win_pct,          attributes.speed, attributes.control );
     int abpi  = getValueForTwo( at_bats_per_inn,  attributes.speed, attributes.control );
     int dpct  = innpg + 200;
     int maxcg = (int)round( (double)((games * innpg) - (games * (innpg - 200))) / (double)(1100 - innpg) );
     int cpct  = getValueForOne( cg_pct, attributes.fatigue );
     int sop9  = getValueForOne( so_per_9, attributes.speed );
     int bbp9  = getValueForOne( walks_per_9, attributes.control );
     int hrp9  = getValueForOne( hr_per_9, attributes.speed );
     int decisions = (int)round((float)games * ((float)dpct / 1000.0) );
     int fat_fact = getFatigueFactor( attributes.fatigue );

     if ( fat_fact < innpg ) innpg = fat_fact;

     memset( player, '\0', sizeof(fileplayer_s) );

     strncpy( player->last_name,  attributes.last_name,  sizeof(player->last_name)  );
     strncpy( player->first_name, attributes.first_name, sizeof(player->first_name) );

     termName( player->last_name,  sizeof(player->last_name)  );
     termName( player->first_name, sizeof(player->first_name) );

     int2word( player->real_avg, era );
     player->year[0]     = 0x9D;
     player->position[0] = attributes.hand;

     struct pitching_s *pitching = &(player->filestats.filepitching);

     setPitcherRatings( pitching->ratings, &attributes );

     double dinn      = (double)games * (double)innpg / 100.0;
     int    inn       = (int)floor( dinn ) * 10 + (rand() % 3);

     dinn  = (double)inn / 10.0;
     dinn += (((double)inn / 10.0) - dinn) / 3.0;

     pitching->real_wins[0]   = (int)ceil( (double)decisions * (double)wpct / 1000.0 );
     pitching->real_losses[0] = (int)ceil( (double)decisions * (1.0 - (double)wpct / 1000.0) );
     pitching->real_games[0]  = games;
     pitching->real_starts[0] = games;
     pitching->real_cg[0]     = (int)round( (double)maxcg * (double)cpct / 100.0 );

     pitching->color[0]       = attributes.color;
     pitching->real_fa[0]     = 1;
     pitching->real_er[0]     = (int)round( dinn * (double)era / 900.0 ); // era is ERA * 100
     pitching->vl_hr[0]       = (int)round( dinn * (double)hrp9 / 900.0 ); // hrp9 is HR/9 * 100
     pitching->vl_bb[0]       = (int)round( dinn * (double)bbp9 / 90.0 ); // bbp9 is BB/9 * 10

     double dvsba = (double)vsba / 1000.0;
     int    ab    = (int)round( dinn * ((float)abpi / 100.0) );
     int    hits  = (int)round( dvsba * (float)ab );
     int    so    = (int)round( dinn * (double)sop9 / 90.0 ); // sop9 is SO/9 * 10

     int2word( pitching->real_inn,  inn  );
     int2word( pitching->vl_ab,     ab   );
     int2word( pitching->vl_hits,   hits );
     int2word( pitching->vl_so,     so   );
}


static void generateCloser( fileplayer_s *player, const names_s *name )
{
     pitcher_attr_s attributes = { 0 };

     rollCloserAttributes( &attributes );

     strcpy( attributes.last_name,  name->last_name  );
     strcpy( attributes.first_name, name->first_name );

     int games = (rand() % 20) + 60;
     int era   = getValueForTwo( closer_earned_run_avg,   attributes.speed, attributes.control );
     int vsba  = getValueForTwo( closer_vs_batting_avg,   attributes.speed, attributes.control );
     int svpct = getValueForTwo( save_pct,         attributes.speed, attributes.control );
     int innpg = (rand() % 30) + 100;
     int abpi  = getValueForTwo( at_bats_per_inn,  attributes.speed, attributes.control );
     int sop9  = getValueForOne( so_per_9, attributes.speed );
     int bbp9  = getValueForOne( walks_per_9, attributes.control );
     int hrp9  = getValueForOne( hr_per_9, attributes.speed );

     memset( player, '\0', sizeof(fileplayer_s) );

     strncpy( player->last_name,  attributes.last_name,  sizeof(player->last_name)  );
     strncpy( player->first_name, attributes.first_name, sizeof(player->first_name) );

     termName( player->last_name,  sizeof(player->last_name)  );
     termName( player->first_name, sizeof(player->first_name) );

     int2word( player->real_avg, era );
     player->year[0]     = 0x9D;
     player->position[0] = attributes.hand;

     struct pitching_s *pitching = &(player->filestats.filepitching);

     setPitcherRatings( pitching->ratings, &attributes );

     double dinn      = (double)games * (double)innpg / 100.0;
     int    inn       = (int)floor( dinn ) * 10 + (rand() % 3);

     dinn  = (double)inn / 10.0;
     dinn += (((double)inn / 10.0) - dinn) / 3.0;

     pitching->real_wins[0]   = (rand() % 10);
     pitching->real_losses[0] = (rand() % 10);
     pitching->real_games[0]  = games;
     pitching->real_starts[0] = 0;
     pitching->real_cg[0]     = 0;
     pitching->real_saves[0]  = (int)round( (double)games * (double)svpct / 1000.0 );

     pitching->color[0]       = attributes.color;
     pitching->real_fa[0]     = 1;
     pitching->real_er[0]     = (int)round( dinn * (double)era / 900.0 ); // era is ERA * 100
     pitching->vl_hr[0]       = (int)round( dinn * (double)hrp9 / 900.0 ); // hrp9 is HR/9 * 100
     pitching->vl_bb[0]       = (int)round( dinn * (double)bbp9 / 90.0 ); // bbp9 is BB/9 * 10

     double dvsba = (double)vsba / 1000.0;
     int    ab    = (int)round( dinn * ((float)abpi / 100.0) );
     int    hits  = (int)round( dvsba * (float)ab );
     int    so    = (int)round( dinn * (double)sop9 / 90.0 ); // sop9 is SO/9 * 10

     int2word( pitching->real_inn,  inn  );
     int2word( pitching->vl_ab,     ab   );
     int2word( pitching->vl_hits,   hits );
     int2word( pitching->vl_so,     so   );
}


static void generateBatter( fileplayer_s *player, const names_s *name, const position2_e position )
{
     batter_attr_s attributes = { 0 };

     rollBatterAttributes( &attributes );

     strcpy( attributes.last_name,  name->last_name  );
     strcpy( attributes.first_name, name->first_name );

     int games = (rand() % 25) + 120;
     int ab    = (int)round( (double)games * ab_per_game );
     int po    = (int)round( (double)games * ((double)getValueForTwo( put_outs, position, attributes.range ) / 100.0) );
     int as    = (int)round( (double)games * ((double)getValueForTwo( assists, position, attributes.arm ) / 100.0) );
     int ba    = getValueForTwo( batting_avg, attributes.power, attributes.hit_n_run );
     int soa   = getValueForOne( strikeout_avg, attributes.hit_n_run );
     int fa    = getValueForTwo( fielding_avg, attributes.range, attributes.arm ) + 9000;
     int fa2   = roll( 9500, fa - 9500 );
     int hits  = (int)round( (double)ab * (double)ba  / 1000.0 );
     int so    = (int)round( (double)ab * (double)soa / 1000.0 );
     int err   = (int)round( (double)(po + as) / (double)fa * 10000.0 ) - (po + as);

     if ( err < 0 ) err = 0;

     hits += getValueForOne( extra_hits, attributes.speed );

     memset( player, '\0', sizeof(fileplayer_s) );

     strncpy( player->last_name,  attributes.last_name,  sizeof(player->last_name)  );
     strncpy( player->first_name, attributes.first_name, sizeof(player->first_name) );

     termName( player->last_name,  sizeof(player->last_name)  );
     termName( player->first_name, sizeof(player->first_name) );

     int2word( player->real_avg, ba );
     player->year[0]     = 0x9D;

     player->position[0] = (position * 0x10) + getSecondaryPosition( position );

     struct batting_s *batting = &(player->filestats.filebatting);

     setBatterRatings( batting->ratings, &attributes );

     batting->real_games[0] = games;
     batting->real_sb[0]    = getValueForOne( steals, attributes.speed );
     batting->vl_2b[0]      = getValueForOne( doubles, attributes.power );
     batting->vl_3b[0]      = getValueForTwo( triples, attributes.power, attributes.speed );
     batting->vl_hr[0]      = getValueForOne( home_runs, attributes.power );
     batting->vl_bb[0]      = getValueForOne( walks, attributes.hit_n_run );
     batting->vl_so[0]      = so;
     batting->real_runs[0]  = batting->vl_hr[0] + getValueForOne( runs, attributes.speed );
     batting->real_rbi[0]   = batting->vl_hr[0] + getValueForOne( rbi, attributes.power );
     batting->real_err[0]   = err;
     batting->real_fa2[0]   = (int)round( (10000.0 - (double)fa2) / 10.0 );
     batting->color[0]      = attributes.color;

     int2word( batting->vl_ab,   ab   );
     int2word( batting->vl_hits, hits );
     int2word( batting->real_po, po   );
     int2word( batting->real_as, as   );
}


int main( const int argc, const char *argv[] )
{
     fileplayer_s   *playersFile;
     names_s        *nameList;
     int             next_name;


     srand( time( NULL ) );

     if ( argc < 4 )
     {
	  printf( "Usage: %s <names file> <input file> <output file>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     if ( (nameList = readNamesFile( argv[1] )) == NULL )
     {
          printf( "Unable to load names file <%s>.\n", argv[1] );

          return EXIT_SUCCESS;
     }

     if ( (playersFile = readPlayersFile( argv[2] )) == NULL )
     {
          printf( "Unable to load players file <%s>.\n", argv[2] );

          free( nameList );

          return EXIT_SUCCESS;
     }

     srand( time( NULL ) );

     next_name = 0;

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          for ( int j = 0; j < PLAYERS_PER_TEAM; ++j )
          {
               position2_e pos = positions[j];
               int         idx = (i * PLAYERS_PER_TEAM) + j;

               if      ( pos == pos2_Pitcher ) generatePitcher( &playersFile[idx], &nameList[next_name]      );
               else if ( pos == pos2_Closer  ) generateCloser(  &playersFile[idx], &nameList[next_name]      );
               else                            generateBatter(  &playersFile[idx], &nameList[next_name], pos );

               next_name++;
          }
     }

     if ( ! writePlayersFile( argv[3], playersFile ) )
     {
          printf( "Cannot save player changes.\n" );

          free( nameList );
          free( playersFile );

          return EXIT_FAILURE;
     }

     free( nameList );
     free( playersFile );

     return EXIT_SUCCESS;
}
