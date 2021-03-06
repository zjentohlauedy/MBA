#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <math.h>
#include "org.h"


typedef struct
{
     int start;
     int range;

} random_s;

static const double ab_per_game =  3.85;

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

static const random_s cg_pct[10]        = { {   0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  5 }, {   0, 10 }, {   5, 25 }, {  20, 45 }, {  40, 60 } };
static const random_s so_per_9[10]      = { {  40, 10 }, {  45, 10 }, {  50, 10 }, {  55, 10 }, {  60, 15 }, {  70, 15 }, {  80, 15 }, {  90, 15 }, { 100, 15 }, { 110, 20 } };
static const random_s walks_per_9[10]   = { {  70, 15 }, {  60, 15 }, {  50, 15 }, {  40, 15 }, {  35, 10 }, {  30, 10 }, {  25, 10 }, {  20, 10 }, {  15, 10 }, {  10, 10 } };
static const random_s hr_per_9[10]      = { { 185, 15 }, { 165, 25 }, { 145, 25 }, { 125, 25 }, { 110, 20 }, { 100, 15 }, {  85, 20 }, {  65, 25 }, {  45, 25 }, {  25, 25 } };
static const random_s strikeout_avg[10] = { { 145,  5 }, { 140, 10 }, { 130, 20 }, { 125, 25 }, { 120, 30 }, { 110, 35 }, { 100, 40 }, {  90, 40 }, {  75, 50 }, {  50, 50 } };
static const random_s extra_hits[10]    = { {   0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  0 }, {   1,  2 }, {   2,  3 }, {   4,  4 }, {   7,  4 }, {  10,  5 } };
static const random_s steals[10]        = { {   0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  5 }, {   0, 10 }, {  10, 20 }, {  20, 30 }, {  40, 35 }, {  60, 40 } };
static const random_s doubles[10]       = { {  10,  8 }, {  10, 10 }, {  10, 12 }, {  12, 15 }, {  15, 15 }, {  17, 15 }, {  20, 15 }, {  20, 17 }, {  25, 20 }, {  30, 20 } };
static const random_s home_runs[10]     = { {   0,  5 }, {   0,  8 }, {   5,  7 }, {   8,  8 }, {  10, 10 }, {  15, 10 }, {  20, 12 }, {  25, 15 }, {  30, 20 }, {  35, 25 } };
static const random_s walks[10]         = { {  15, 10 }, {  18, 12 }, {  20, 15 }, {  25, 18 }, {  30, 20 }, {  35, 25 }, {  40, 30 }, {  50, 35 }, {  60, 40 }, {  75, 50 } };
static const random_s runs[10]          = { {  15, 10 }, {  18, 12 }, {  20, 15 }, {  25, 18 }, {  30, 20 }, {  35, 25 }, {  40, 30 }, {  50, 35 }, {  60, 40 }, {  75, 50 } };
static const random_s rbi[10]           = { {  15, 10 }, {  18, 12 }, {  20, 15 }, {  25, 18 }, {  30, 20 }, {  35, 25 }, {  40, 30 }, {  50, 35 }, {  60, 40 }, {  75, 50 } };


static int roll( const int start, const int range )
{
     return (rand() % (range + 1)) + start;
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

static int getFatigueFactor( const int fatigue )
{//                                 0  1  2  3    4    5    6    7    8    9   10
     static const int factors[] = { 0, 0, 0, 0, 550, 650, 750, 850, 900, 900, 900 };

     return factors[fatigue];
}

static filehand_e mapHandedness( const handedness_e handedness )
{
     switch ( handedness )
     {
     case hnd_Right:  return fh_Right;
     case hnd_Left:   return fh_Left;
     case hnd_Switch: return fh_Switch;
     }

     return fh_Right;
}

static fileposition_e mapPosition( const position_e position )
{
     switch ( position )
     {
     case pos_Catcher:     return fpos_Catcher;
     case pos_FirstBase:   return fpos_FirstBaseman;
     case pos_SecondBase:  return fpos_SecondBaseman;
     case pos_ThirdBase:   return fpos_ThirdBaseman;
     case pos_ShortStop:   return fpos_ShortStop;
     case pos_LeftField:   return fpos_LeftField;
     case pos_CenterField: return fpos_CenterField;
     case pos_RightField:  return fpos_RightField;
     case pos_Infield:     return fpos_Infield;
     case pos_Outfield:    return fpos_Outfield;
     }

     return fpos_DesignatedHitter;
}

int adjustRating( const int rating, const int season, const player_s *player )
{
     int age_adjustment = (season - player->rookie_season) - (player->longevity + 3);

     if (age_adjustment < 0 ) age_adjustment = 0;

     return MAX( rating - age_adjustment, 1 );
}

static int adjustFatigue( const int rating, const int season, const player_s *player )
{
     return MAX( adjustRating( rating, season, player ), 4 );
}

static void generateBatter( const player_s *player, const int season, fileplayer_s *players_file )
{
     batter_s      *batter  = player->details.batting;
     filebatting_s *batting = &(players_file->filestats.filebatting);

     filehand_e     hand      = mapHandedness( player->handedness );
     fileposition_e primary   = mapPosition( batter->primary_position   );
     fileposition_e secondary = mapPosition( batter->secondary_position );

     int arm       = adjustRating( batter->arm,       season, player );
     int running   = adjustRating( batter->running,   season, player );
     int range     = adjustRating( batter->range,     season, player );
     int power     = adjustRating( batter->power,     season, player );
     int bunt      = adjustRating( batter->bunt,      season, player );
     int hit_n_run = adjustRating( batter->hit_n_run, season, player );

     players_file->position[0] = (primary << 4) + secondary;

     batting->ratings[0] = (hand    << 4) +         arm;
     batting->ratings[1] = (running << 4) +         range;
     batting->ratings[2] = (power   << 4) + player->longevity;
     batting->ratings[3] = (bunt    << 4) +         hit_n_run;

     if   ( player->skin_tone == st_Dark ) int2byte( batting->color, fc_Dark  );
     else                                  int2byte( batting->color, fc_Light );

     int games = (rand() % 25) + 120;
     int ab    = (int)round( (double)games * ab_per_game );
     int po    = (int)round( (double)games * ((double)getValueForTwo( put_outs, primary, range ) / 100.0) );
     int as    = (int)round( (double)games * ((double)getValueForTwo( assists, primary, arm ) / 100.0) );
     int ba    = getValueForTwo( batting_avg, power, hit_n_run );
     int soa   = getValueForOne( strikeout_avg, hit_n_run );
     int fa    = getValueForTwo( fielding_avg, range, arm ) + 9000;
     int fa2   = roll( 9500, fa - 9500 );
     int hits  = (int)round( (double)ab * (double)ba  / 1000.0 );
     int so    = (int)round( (double)ab * (double)soa / 1000.0 );
     int err   = (int)round( (double)(po + as) / (double)fa * 10000.0 ) - (po + as);

     if ( err < 0 ) err = 0;

     hits += getValueForOne( extra_hits, running );

     int2word( players_file->real_avg, ba );

     int2byte( batting->real_games, games );
     int2byte( batting->real_sb,    getValueForOne( steals, running ) );
     int2word( batting->vl_ab,      ab   );
     int2word( batting->vl_hits,    hits );
     int2byte( batting->vl_2b,      getValueForOne( doubles, power ) );
     int2byte( batting->vl_3b,      getValueForTwo( triples, power, running ) );
     int2byte( batting->vl_hr,      getValueForOne( home_runs, power ) );
     int2byte( batting->vl_bb,      getValueForOne( walks, hit_n_run ) );
     int2byte( batting->vl_so,      so );
     int2byte( batting->real_runs,  batting->vl_hr[0] + getValueForOne( runs, running ) );
     int2byte( batting->real_rbi,   batting->vl_hr[0] + getValueForOne( rbi, power ) );
     int2word( batting->real_po,    po   );
     int2word( batting->real_as,    as   );
     int2byte( batting->real_err,   err );
     int2byte( batting->real_fa2,   (int)round( (10000.0 - (double)fa2) / 10.0 ) );
}

static void generateClosingPitcher( const player_s *player, const int season, fileplayer_s *players_file )
{
     pitcher_s      *pitcher  = player->details.pitching;
     filepitching_s *pitching = &(players_file->filestats.filepitching);

     filehand_e hand = mapHandedness( player->handedness );

     int speed   = adjustRating(  pitcher->speed,   season, player );
     int control = adjustRating(  pitcher->control, season, player );
     int fatigue =                pitcher->fatigue;

     players_file->position[0] = (fpos_Pitcher << 4) + hand;

     pitching->ratings[0] = (speed   << 4) +         control;
     pitching->ratings[1] = (fatigue << 4) + player->longevity;

     if   ( player->skin_tone == st_Dark ) int2byte( pitching->color, fc_Dark  );
     else                                  int2byte( pitching->color, fc_Light );

     int games = (rand() % 20) + 60;
     int era   = getValueForTwo( closer_earned_run_avg,   speed, control );
     int vsba  = getValueForTwo( closer_vs_batting_avg,   speed, control );
     int svpct = getValueForTwo( save_pct, speed, control );
     int innpg = (rand() % 30) + 100;
     int abpi  = getValueForTwo( at_bats_per_inn, speed, control );
     int sop9  = getValueForOne( so_per_9, speed );
     int bbp9  = getValueForOne( walks_per_9, control );
     int hrp9  = getValueForOne( hr_per_9, speed );

     int2word( players_file->real_avg, era );

     double dinn = (double)games * (double)innpg / 100.0;
     int    inn  = (int)floor( dinn ) * 10 + (rand() % 3);

     dinn  = (double)inn / 10.0;
     dinn += (((double)inn / 10.0) - dinn) / 3.0;

     int2byte( pitching->real_wins,   (rand() % 10) );
     int2byte( pitching->real_losses, (rand() % 10) );
     int2byte( pitching->real_games,  games         );
     int2byte( pitching->real_starts, 0             );
     int2byte( pitching->real_cg,     0             );
     int2byte( pitching->real_saves,  (int)round( (double)games * (double)svpct / 1000.0 ) );
     int2byte( pitching->real_fa,     1 );
     int2byte( pitching->real_er,     (int)round( dinn * (double)era / 900.0 ) ); // era is ERA * 100
     int2byte( pitching->vl_hr,       (int)round( dinn * (double)hrp9 / 900.0 ) ); // hrp9 is HR/9 * 100
     int2byte( pitching->vl_bb,       (int)round( dinn * (double)bbp9 / 90.0 ) ); // bbp9 is BB/9 * 10

     double dvsba = (double)vsba / 1000.0;
     int    ab    = (int)round( dinn * ((float)abpi / 100.0) );
     int    hits  = (int)round( dvsba * (float)ab );
     int    so    = (int)round( dinn * (double)sop9 / 90.0 ); // sop9 is SO/9 * 10

     int2word( pitching->real_inn,  inn  );
     int2word( pitching->vl_ab,     ab   );
     int2word( pitching->vl_hits,   hits );
     int2word( pitching->vl_so,     so   );
}

static void generateStartingPitcher( const player_s *player, const int season, fileplayer_s *players_file )
{
     pitcher_s      *pitcher  = player->details.pitching;
     filepitching_s *pitching = &(players_file->filestats.filepitching);

     filehand_e hand = mapHandedness( player->handedness );

     int speed   = adjustRating(  pitcher->speed,   season, player );
     int control = adjustRating(  pitcher->control, season, player );
     int fatigue = adjustFatigue( pitcher->fatigue, season, player );

     players_file->position[0] = (fpos_Pitcher << 4) + hand;

     pitching->ratings[0] = (speed   << 4) +         control;
     pitching->ratings[1] = (fatigue << 4) + player->longevity;

     if   ( player->skin_tone == st_Dark ) int2byte( pitching->color, fc_Dark  );
     else                                  int2byte( pitching->color, fc_Light );

     int games = (rand() % 15) + 30;
     int era   = getValueForTwo( earned_run_avg,   speed, control );
     int vsba  = getValueForTwo( vs_batting_avg,   speed, control );
     int innpg = getValueForTwo( innings_per_game, speed, control );
     int wpct  = getValueForTwo( win_pct,          speed, control );
     int abpi  = getValueForTwo( at_bats_per_inn,  speed, control );
     int dpct  = innpg + 200;
     int maxcg = (int)round( (double)((games * innpg) - (games * (innpg - 200))) / (double)(1100 - innpg) );
     int cpct  = getValueForOne( cg_pct, fatigue );
     int sop9  = getValueForOne( so_per_9, speed );
     int bbp9  = getValueForOne( walks_per_9, control );
     int hrp9  = getValueForOne( hr_per_9, speed );
     int decisions = (int)round((float)games * ((float)dpct / 1000.0) );
     int fat_fact = getFatigueFactor( fatigue );

     if ( fat_fact < innpg ) innpg = fat_fact;

     int2word( players_file->real_avg, era );

     double dinn = (double)games * (double)innpg / 100.0;
     int    inn  = (int)floor( dinn ) * 10 + (rand() % 3);

     dinn  = (double)inn / 10.0;
     dinn += (((double)inn / 10.0) - dinn) / 3.0;

     int2byte( pitching->real_wins,   (int)ceil( (double)decisions * (double)wpct / 1000.0 ) );
     int2byte( pitching->real_losses, (int)ceil( (double)decisions * (1.0 - (double)wpct / 1000.0) ) );
     int2byte( pitching->real_games,  games );
     int2byte( pitching->real_starts, games );
     int2byte( pitching->real_cg,     (int)round( (double)maxcg * (double)cpct / 100.0 ) );

     int2byte( pitching->real_fa, 1 );
     int2byte( pitching->real_er, (int)round( dinn * (double)era / 900.0 ) ); // era is ERA * 100
     int2byte( pitching->vl_hr,   (int)round( dinn * (double)hrp9 / 900.0 ) ); // hrp9 is HR/9 * 100
     int2byte( pitching->vl_bb,   (int)round( dinn * (double)bbp9 / 90.0 ) ); // bbp9 is BB/9 * 10

     double dvsba = (double)vsba / 1000.0;
     int    ab    = (int)round( dinn * ((float)abpi / 100.0) );
     int    hits  = (int)round( dvsba * (float)ab );
     int    so    = (int)round( dinn * (double)sop9 / 90.0 ); // sop9 is SO/9 * 10

     int2word( pitching->real_inn,  inn  );
     int2word( pitching->vl_ab,     ab   );
     int2word( pitching->vl_hits,   hits );
     int2word( pitching->vl_so,     so   );
}

static void generatePitcher( const player_s *player, const int season, fileplayer_s *players_file )
{
     if   ( player->details.pitching->fatigue > 1 ) { generateStartingPitcher( player, season, players_file ); }
     else                                           { generateClosingPitcher(  player, season, players_file ); }
}

static boolean_e generateFilePlayer( const player_s *player, const int season, const int player_idx, fileplayer_s *players_file )
{
     int2word( players_file[player_idx].acc_stats.action.id_info.player_id,               player->player_id   );
     int2byte( players_file[player_idx].acc_stats.action.id_info.checksum,  calcChecksum( player->player_id ) );

     memcpy( players_file[player_idx].first_name,  player->first_name,      sizeof(players_file[player_idx].first_name)  );
     memcpy( players_file[player_idx].last_name,   player->last_name,       sizeof(players_file[player_idx].last_name)   );
     memcpy( players_file[player_idx].first_phoen, player->first_phoenetic, sizeof(players_file[player_idx].first_phoen) );
     memcpy( players_file[player_idx].last_phoen,  player->last_phoenetic,  sizeof(players_file[player_idx].last_phoen)  );

     termName( players_file[player_idx].first_name,  sizeof(players_file[player_idx].first_name)  );
     termName( players_file[player_idx].last_name,   sizeof(players_file[player_idx].last_name)   );
     termName( players_file[player_idx].first_phoen, sizeof(players_file[player_idx].first_phoen) );
     termName( players_file[player_idx].last_phoen,  sizeof(players_file[player_idx].last_phoen)  );

     int2byte( players_file[player_idx].year, player->rookie_season + YEAR_SEASON_OFFSET );

     if   ( player->player_type == pt_Pitcher ) { generatePitcher( player, season, &(players_file[player_idx]) ); }
     else                                       { generateBatter(  player, season, &(players_file[player_idx]) ); }

     return bl_True;
}


static boolean_e iteratePlayers( const team_player_s *team_players, const int season, const int team_idx, fileplayer_s *players_file )
{
     for ( int l = 0; team_players[l].player != NULL; ++l )
     {
          player_s *player = team_players[l].player;

          if ( ! generateFilePlayer( player, season, (team_idx * PLAYERS_PER_TEAM) + l, players_file ) ) return bl_False;
     }

     return bl_True;
}


static boolean_e iterateTeams( const division_team_s *division_teams, const int season, const int division_idx, fileplayer_s *players_file )
{
     for ( int k = 0; division_teams[k].team != NULL; ++k )
     {
          team_s *team = division_teams[k].team;

          if ( ! iteratePlayers( team->players, season, (division_idx * TEAMS_PER_DIVISION) + k, players_file ) ) return bl_False;
     }

     return bl_True;
}


static boolean_e iterateDivisions( const league_division_s *league_divisions, const int season, const int league_idx, fileplayer_s *players_file )
{
     for ( int j = 0; league_divisions[j].division != NULL; ++j )
     {
          division_s *division = league_divisions[j].division;

          if ( ! iterateTeams( division->teams, season, (league_idx * DIVISIONS_PER_LEAGUE) + j, players_file ) ) return bl_False;
     }

     return bl_True;
}


static boolean_e iterateLeagues( const org_league_s *org_leagues, const int season, fileplayer_s *players_file )
{
     for ( int i = 0; org_leagues[i].league != NULL; ++i )
     {
          league_s *league = org_leagues[i].league;

          if ( ! iterateDivisions( league->divisions, season, i, players_file ) ) return bl_False;
     }

     return bl_True;
}


fileplayer_s *generateFilePlayers( const org_s *org, const int season )
{
     fileplayer_s *players_file = NULL;

     if ( (players_file = malloc( sizeof(fileplayer_s) * TOTAL_PLAYERS )) == NULL ) return NULL;

     memset( players_file, '\0', sizeof(fileplayer_s) * TOTAL_PLAYERS );

     if ( org->leagues == NULL ) return players_file;

     if ( ! iterateLeagues( org->leagues, season, players_file ) )
     {
          free( players_file );

          return NULL;
     }

     return players_file;
}
