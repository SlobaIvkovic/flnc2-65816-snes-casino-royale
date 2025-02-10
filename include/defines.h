#ifndef DEFINES_H
#define DEFINES_H

#define PROG1 0x81
#define PROG2 0x82
#define PROG3 0x83
#define PROG4 0x84
#define PROG5 0x85
#define PROG6 0x86

#define BANK1 "CODE1"
#define BANK2 "CODE2"
#define BANK3 "CODE3"
#define BANK4 "CODE4"
#define BANK5 "CODE5"
#define BANK6 "CODE6"

#define XSTART 160
#define YSTART 00
#define XEND  80 + 8
#define YEND  120 - 11

#define MOVE_STATE_0 15
#define MOVE_STATE MOVE_STATE_0 + 8
#define CALCULATE_STEPS 40
#define PAUSE_BETWEEN_CARDS 14


#define CLOVER  0
#define HEART   1
#define DIAMOND 2
#define SPADE   3

#define PX_PER_MOVE 1                        

#define BG3_NT_ADR(x, y) (0x5400 | ((y << 5) | x))
#define P_SCORE_X 7
#define P_SCORE_Y 15
#define D_SCORE_X 7
#define D_SCORE_Y 7

#define BETTING_SCREEN 3

#endif
