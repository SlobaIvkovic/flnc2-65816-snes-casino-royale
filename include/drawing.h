#ifndef DRAWING_H
#define DRAWING_H

#include "defines.h"

void trampoline();

#pragma wrapped-call(push, trampoline, 0x85)
void displayCard();
void displayCloverCard();
void createCloverBase();
void insertCloverMiddle();
void displayHeartCard();
void createHeartBase();
void insertHeartMiddle();
void displayDiamondCard();
void createDiamondBase();
void insertDiamondMiddle();
void displaySpadeCard();
void createSpadeBase();
void insertSpadeMiddle();




void drawBackCardStatic();


void moveCard();
void moveCardLD();
void moveCardDL();


void displayLetter(char letter);
void displayText(const char text[]);

void displaySelector();
void drawBigSelectorSprite();
void drawCash();

void displayPlayerCash();

/********************************************************************************************************

	SMALL CARD DRAWING ROUTINES

********************************************************************************************************/

#pragma wrapped-call(push, trampoline, 0x91)
void calculateScore();
void displayDscore();
void displayPscore();
void openDealersCard();
#pragma wrapped-call(pop)


#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x94)
void drawSpriteCard1();
void drawSpriteCard2();
void drawSpriteCard3();
void drawSpriteCard4();
void drawCardBackSprite();
#pragma wrapped-call(pop)


//unsigned char endCorner;



#define letterDisplay(x, y, l) {\
									vram_adr(0x5400 + ((y<<5) | (x)));\
									vram_put((l - 97 + 30) | 0x2000);\
								}\

#define textDisplay(x, y, t) {\
									vram_adr(0x5400 + ((y<<5) | (x)));\
									displayText(t);\
								  }\



#define td(x, y, t) {\
						i = 0;\
						vram_adr(0x5400 | ((y<<5) | (x)));\
						for(i = 0; t[i]; i++)\
						{\
							if(t[i] == ' ')\
							{\
								vram_put(63 | 0b0011110000000000);\
							}\
							else\
							{\
								vram_put((unsigned short)(t[i] - 97 + 30) | 0b0011110000000000);\
							}\
						}\
						i = 0;\
					}\

#define selectorDisplay(x, y) {\
									vram_adr(0x5400 + ((y<<5) | (x)));\
									vram_put(56 | 0b0011110000000000);\
									vram_adr(0x5400 + (((y+1)<<5) | (x)));\
									vram_put(58 | 0b0011110000000000);\
									\
									vram_adr(0x5400 + (((y)<<5) | (x + 6)));\
									vram_put(57 | 0b0011110000000000);\
									vram_adr(0x5400 + (((y+1)<<5) | (x + 6)));\
									vram_put(59 | 0b0011110000000000);\
								}


#define clearSelector(x, y) {\
									vram_adr(0x5400 + ((y<<5) | (x)));\
									vram_put(60);\
									vram_adr(0x5400 + (((y+1)<<5) | (x)));\
									vram_put(60);\
									\
									vram_adr(0x5400 + (((y)<<5) | (x + 6)));\
									vram_put(60);\
									vram_adr(0x5400 + (((y+1)<<5) | (x + 6)));\
									vram_put(60);\
								}

#define dD()  	createDiamondBase();\
                insertDiamondMiddle();\
                
#define dC()    createCloverBase();\
                insertCloverMiddle();\
                
#define dS()    createSpadeBase();\
                insertSpadeMiddle();\
				
#define dH()	createHeartBase();\
                insertHeartMiddle();\

#define heartBase(a, b) {	k=0;\
							vramX = a;\
							vramY = b;\
                      		for(i = 0; i < 7; i++)\
							{\
								vram_adr(NTADR_A(vramX, vramY));\
								for(j = 0; j < 5; j++)\
								{\
									vram_put(mmHe[k]);\
									++k;\
								}\
								++vramY;\
							}\
							vramX += 5;\
						}\
// At the end of this code sequence vramX and Y values shall be updated
#define heartMiddle(sN) {\
							vram_adr(NTADR_A(vramX - 3, vramY - 5));\
							vram_put(heNumbers[(sN &0x0f) *5]);\
							vram_adr(NTADR_A(vramX - 4, vramY - 4));\
							vram_put(heNumbers[(sN & 0x0f) * 5 + 3]);\
							vram_adr(NTADR_A(vramX - 3, vramY -3));\
							vram_put(heNumbers[(sN & 0x0F) * 5 + 4]);\
							}\

#define heartCard(x, y, sn) {\
                             	heartBase(x,y);\
                             	heartMiddle(sn);\
                            }\


#define drawSpriteCard(sN){\
								signNum = sN;\
								if(sN >> 4 == CLOVER)\
								{\
								drawSpriteCard1();\
								}\
								else if(sN >> 4 == HEART)\
								{\
								drawSpriteCard2();\
								}\
								else if(sN >> 4 == DIAMOND)\
								{\
								drawSpriteCard3();\
								}\
								else if(sN >> 4 == SPADE)\
								{\
								drawSpriteCard4();\
								}\
							}\

#define dealerSum() {\
					 score = dealerScore;\
					 calculateScore();\
					 dealerScore = score;\
					}\
					
#define playerSum() {\
					 score = playerScore;\
					 calculateScore();\
					 playerScore = score;\
					 }\

#define bigSelectorSprite(x, y) {\
									spX = x;\
									spY = y;\
									drawBigSelectorSprite();\
								}\


// Possibly never use drawBackCardStatic function and relax the memory usage								
#define drawCardBackStatic(x, y) {\
									vramX = x;\
									vramY = y;\
									k = 0;\
									for(i = 0; i < 7; i++)\
									{\
										vram_adr(NTADR_A(vramX, vramY));\
										for(j = 0; j < 5; j++)\
										{\
											vram_put(mmBc[k]);\
											++k;\
										}\
										++vramY;\
									}\
									vramX += 5;\
								}

extern unsigned char availableSprid;
/*								
#define drawCardTip(x, y, sN, nS) {\
							spX = x;\
							spY = y;\
							availableSprid = nS;\
							if(sN >> 4 == CLOVER)\
							{\
								availableSprid = oam_spr(spX + 0, spY, (unsigned char)mmClSp[0], (unsigned char)(mmClSp[0] >>8), availableSprid);\
								availableSprid = oam_spr(x + 8, spY, (unsigned char)mmClSp[1], (unsigned char)(mmClSp[1] >>8), availableSprid);\
								availableSprid = oam_spr(x + 16, spY, (unsigned char)mmClSp[2], (unsigned char)(mmClSp[2] >>8), availableSprid);\
								availableSprid = oam_spr(x + 24, spY, (unsigned char)mmClSp[3], (unsigned char)(mmClSp[3] >>8), availableSprid);\
								availableSprid = oam_spr(x + 32, spY, (unsigned char)mmClSp[4], (unsigned char)(mmClSp[4] >>8), availableSprid);\
							}\
							else if(sN >> 4 == HEART)\
							{\
								availableSprid = oam_spr(spX + 0, spY, (unsigned char)mmHeSp[0], (unsigned char)(mmHeSp[0] >>8), availableSprid);\
								availableSprid = oam_spr(x + 8, spY, (unsigned char)mmHeSp[1], (unsigned char)(mmHeSp[1] >>8), availableSprid);\
								availableSprid = oam_spr(x + 16, spY, (unsigned char)mmHeSp[2], (unsigned char)(mmHeSp[2] >>8), availableSprid);\
								availableSprid = oam_spr(x + 24, spY, (unsigned char)mmHeSp[3], (unsigned char)(mmHeSp[3] >>8), availableSprid);\
								availableSprid = oam_spr(x + 32, spY, (unsigned char)mmHeSp[4], (unsigned char)(mmHeSp[4] >>8), availableSprid);\
							}\
							else if(sN >> 4 == DIAMOND)\
							{\
								availableSprid = oam_spr(spX + 0, spY, (unsigned char)mmDiSp[0], (unsigned char)(mmDiSp[0] >>8), availableSprid);\
								availableSprid = oam_spr(x + 8, spY, (unsigned char)mmDiSp[1], (unsigned char)(mmDiSp[1] >>8), availableSprid);\
								availableSprid = oam_spr(x + 16, spY, (unsigned char)mmDiSp[2], (unsigned char)(mmDiSp[2] >>8), availableSprid);\
								availableSprid = oam_spr(x + 24, spY, (unsigned char)mmDiSp[3], (unsigned char)(mmDiSp[3] >>8), availableSprid);\
								availableSprid = oam_spr(x + 32, spY, (unsigned char)mmDiSp[4], (unsigned char)(mmDiSp[4] >>8), availableSprid);\
							}\
							else if(sN >> 4 == SPADE)\
							{\
								availableSprid = oam_spr(spX + 0, spY, (unsigned char)mmSpSp[0], (unsigned char)(mmSpSp[0] >>8), availableSprid);\
								availableSprid = oam_spr(x + 8, spY, (unsigned char)mmSpSp[1], (unsigned char)(mmSpSp[1] >>8), availableSprid);\
								availableSprid = oam_spr(x + 16, spY, (unsigned char)mmSpSp[2], (unsigned char)(mmSpSp[2] >>8), availableSprid);\
								availableSprid = oam_spr(x + 24, spY, (unsigned char)mmSpSp[3], (unsigned char)(mmSpSp[3] >>8), availableSprid);\
								availableSprid = oam_spr(x + 32, spY, (unsigned char)mmSpSp[4], (unsigned char)(mmSpSp[4] >>8), availableSprid);\
							}\
}\
*/
void clearColumnMess();

#define messClear(x, y, h) {\
								vramX = x;\
								vramY = y;\
								messHeight = h;\
								clearColumnMess();\
								vramX = x;\
								vramY = y;\
							}\

#endif							
