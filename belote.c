#include "neslib.h"
#include "include/drawing_small_cards.h"
#include "include/binsizes.h"
#include "include/shuffle.h"
#include "include/movement.h"
#include "include/fade.h"
#include "include/save.h"
#include "include/inplayachi.h"
#include "snesgss.h"


extern char music5[];
// For the in the play achievments
unsigned char player1tricksWon;

// End screen externs
extern unsigned char winLoseState, screenState;
extern unsigned long betAmount;
extern unsigned long winings;
extern unsigned long cash;

unsigned char rebeloteCards[2];
unsigned char beRebeCon;
unsigned char margin;

struct ply{
	unsigned char tablePosition;
	unsigned char team;
	unsigned char teamMate;
	unsigned char cards[9];
	unsigned char joypadNum;
	unsigned char cardNum;
	unsigned char spot;
};

typedef struct ply Ply;

Ply plys[4];

struct team
{
	unsigned short currentPoints;
	unsigned short totalPoints;
	unsigned char trump;
	unsigned char tricksWon;
	unsigned char lastTrick;
	unsigned char bonus;
	unsigned char bonus1;
	unsigned char bonus2;
	
};

struct team team1, team2;

unsigned char aiIndex;

unsigned char sts[4];
unsigned char cardSpots[4];
unsigned char cardSpots1[4];
unsigned char box1, box2;

unsigned char mt, leadingSuit;

unsigned char gpState;
unsigned char tpState;
unsigned char sp;
unsigned char vX, vY;
unsigned char auxSignNum;

unsigned char max, maxi;

unsigned char bli, bblink;
unsigned char canTrump;

extern unsigned char i, j, k; 

unsigned char xc;
char token[2];

unsigned char beloteState;
extern unsigned char allEntered;
extern unsigned char vramX, vramY;
extern unsigned char signNum;
extern unsigned char pl[4];
extern unsigned char cardsDeck[52];
extern unsigned char sX, sY;

extern unsigned long num1;
extern unsigned char bg3X, bg3Y;

unsigned char sW, sH; 
unsigned char joyNum;
unsigned char deckPos;
unsigned char trumpCard;
unsigned char trumpSuit;
unsigned char take, move;
unsigned char tpCount;

#pragma rodata-name(push, "RODATA")
#pragma data-name(push, "RODATA")
static unsigned short newPalette[] = 
{
0x7fff, 0x2156, 0x10ee, 0x125c, 0x331e, 0x2b39, 0x2bf9, 0x3f10, 0x3ff0, 0x1ec2, 0x15e2, 0x4a49, 0x5ea6, 0x454c, 0x3cab, 0x188b,
0x7fff, 0x295c, 0x463c, 0x2156, 0x10ee, 0x125c, 0x331e, 0x4a49, 0x7f89, 0x5ea6, 0x7f06, 0x454c, 0x3cab, 0x7f3f, 0x59ff, 0x188b,
0x7fff, 0x2156, 0x10ee, 0x125c, 0x16df, 0x331e, 0x133f, 0x2b39, 0x1bff, 0x1ec2, 0x4a49, 0x5ea6, 0x454c, 0x3cab, 0x188b, 0x0842,
0x0842, 0x2156, 0x10ee, 0x125c, 0x331e, 0x4a49, 0x7f89, 0x5ea6, 0x7f06, 0x454c, 0x3cab, 0x188b, 0x0842, 0x0842, 0x0842, 0x0842,
};

static unsigned short oldPalette[] = 
{
0x7fff, 0x1d36, 0x0ccd, 0x0e3c, 0x2f1e, 0x2739, 0x27f9, 0x3b0f, 0x3bef, 0x1ac0, 0x11c0, 0x4628, 0x5e85, 0x412b, 0x388a, 0x146a,
0x7fff, 0x253c, 0x421c, 0x1d36, 0x0ccd, 0x0e3c, 0x2f1e, 0x4628, 0x7f88, 0x5e85, 0x7f05, 0x412b, 0x388a, 0x7f3f, 0x59df, 0x146a,
0x7fff, 0x1d36, 0x0ccd, 0x0e3c, 0x12df, 0x2f1e, 0x0f3f, 0x2739, 0x17ff, 0x1ac0, 0x4628, 0x5e85, 0x412b, 0x388a, 0x146a, 0x0000,
0x0000, 0x1d36, 0x0ccd, 0x0e3c, 0x2f1e, 0x4628, 0x7f88, 0x5e85, 0x7f05, 0x412b, 0x388a, 0x146a, 0x0000, 0x0000, 0x0000, 0x0000,
};

#pragma data-name(pop)
#pragma rodata-name(pop)

extern unsigned char firstToPlay, lastToPlay;
unsigned char currentlyPlaying, firstToBid;

extern unsigned char spriteX, spriteY, spY, spX;

// colored selector externs

extern unsigned char availableSprid, cSelX, cSelY, selTile, tileUpdate, selTile1;
	
//unsigned char team1, team2;

void trampoline();
#pragma wrapped-call(push, trampoline, 0x90)
void belote();
void entering();
void pickTeam();
void detXY();
void loadTakePass();
void belote5Deal();
void findValidCard();
void showCards();
void belSel();
void moveBelSel();
void loadSetTrump();
void clearCard();
void moveBelSel1();
void belote3Deal();
void colBelSel();
void loadTrick();
void drawTrumpSuit();
void setTrumpDetermine();
void cardSelector();
void rearangeCards();
void rightEdge();
void leftEdge();
void baRightEdge();
void baLeftEdge();
void transformCards();
void sortSpots();
void calculatePoints();
void mustThrow();

void aiTakePass();
void aiSetTrump();
void findUnconected();
void conPl();
void showAiCards();
void aiTakeDecision();
void aiThrow();
void findHighNonT();
void trumpMin();
void aiRespond();
void lowestCard();
void printTeamPoints();
void dealEndPoints();
void checkEnd();
void resetBelote();
void loadTeamArrangement();
void arrangeTeams();
void beloteAchievments();
void belotePause();
static void myDelay(unsigned char d);
void checkBeRebe();
void declareBeRebe();
#pragma wrapped-call(pop)


#pragma wrapped-call(push, trampoline, 0x85)
void displayMessage(char *message);
#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x86)
void displayNumber();
#pragma wrapped-call(pop)

#undef BG1_NT(x, y)
#define BG1_NT(x, y) (0x2400 | ((y<<5) | (x)))
#define BG2_NT(x, y) (0x2800 | ((y<<5) | (x)))
#define BG3_NT(x, y) (0x2C00 | ((y<<5) | (x)))

#define printNumber(a, x, y) {\
								num1 = a; bg3X = x; bg3Y = y;\
								ppu_wait_nmi();\
								displayNumber();\
							}\

#define printMessage(x, y, mess){\
									ppu_wait_nmi();\
									vram_adr((0x2C00 | ((y << 5) | x)));\
									displayMessage(mess);\
								}\


#define firstLast(){\
						++firstToPlay;\
						if(firstToPlay == 4)\
						{\
							firstToPlay = 0;\
							lastToPlay = 3;\
						}\
						else\
						{\
							lastToPlay = firstToPlay - 1;\
						}\
					}\

#define getJoyNum(){\
						joyNum = sts[currentlyPlaying] - 1;\
					}\

#define getAiIndex()\
					{\
						aiIndex = sts[currentlyPlaying] - 4;\
					}\

#define moveCardSeq(a, b, c, d){\
								x0 = a;\
								x1 = b;\
								y0 = c;\
								y1 = d;\
								drMove();\
								spriteX = spX;\
								spriteY = spY;\
								ppu_wait_nmi();\
								signNum = trumpCard;\
								alternativePokSprite();\
}\

#define moveCardSeq1(a, b, c, d){\
								x0 = a;\
								x1 = b;\
								y0 = c;\
								y1 = d;\
								urMove();\
								spriteX = spX;\
								spriteY = spY;\
								ppu_wait_nmi();\
								alternativePokSprite();\
}\

#define cardClear(a, b){\
							vramX = a;\
							vramY = b;\
							clearCard();\
}\

#define clearMiddle(){\
						ppu_wait_nmi();\
						vram_adr(BG1_NT(14, 7));\
						vram_fill(4, 1600);\
					}\
					
#pragma code-name(push, "CODE8")
void belote()
{
	if(!beloteState)
	{
		selTile = 107;
		selTile1 = 99;
		token[1] = 'p';
		entering();
		pickTeam();
		firstToBid = 3;
		if((pad_trigger(0) & PAD_START) && (allEntered))
		{
			beloteState = 7;
			findUnconected();
			fadeout();
			
		}
	}
	else if(beloteState == 1)
	{
		deckPos = 0;
		shuffle();
		findValidCard();
		signNum = cardsDeck[deckPos];
		++deckPos;
		
		belote5Deal();
	
		loadTakePass();
		vramX = 10;
		vramY = 9;
		
		trumpCard = signNum;
//		trumpCard = 0;
		pokCardDisplay();
		sX = 119; sY = 72; sW = 42;
		belSel();
		
		firstLast();
		++firstToBid;
		if(firstToBid == 4)
		{
			firstToBid = 0;
		}
		currentlyPlaying = firstToBid;
		fadein();
		getJoyNum();
		if(joyNum < 4)
		{
			showCards();
		}
		else
		{
			getAiIndex();
			showAiCards();
		}
		++beloteState;
		
		
	}
	// TakePass part
	else if(beloteState == 2)
	{
		if(!tpState)
		{
			getJoyNum();
			if(joyNum > 3)
			{
				getAiIndex();
				aiTakePass();
			}
			else
			{
				moveBelSel();
			}
			signNum = trumpCard;
		//	moveCardSeq();
		}
		if(take)
		{
			if(currentlyPlaying == 0 || currentlyPlaying == 2)
			{
				team1.trump = 1;
			}
			else
			{
				team2.trump = 1;
			}
			if(joyNum < 4)
			plys[joyNum].cards[5] = trumpCard;
			else
			plys[aiIndex].cards[5] = trumpCard;
			belote3Deal();
			
			take = 0;
			spriteX = 80;
			spriteY = 72;
			alternativePokSprite();
			cardClear(10, 9);
			// add card to the current players cards, then deal rest of the cards
			
			trumpSuit = trumpCard >> 4;
			drawTrumpSuit();
			tpCount = 0;
			
		}
		
		if(move)
		{
			moveCardSeq(80, 144, 72, 160);
			if(arrived)
			{
				// card arived it is time to change state of the game
				beloteState = 5;
				arrived = 0;
				vramX = 18;
				vramY = 20;
				signNum = trumpCard;
				ppu_wait_nmi();
				
				pokCardDisplay();
			
				
				myDelay(15);
				getJoyNum();
			
				if(joyNum > 3)
				{
				
					vramX = 18;
					vramY = 20;
					ppu_wait_nmi();
					pokBa();
					oam_clear();
					myDelay(15);
				}
				fadeout();
				move = 0;
			
			}
		}
		
		if(tpCount == 4)
		{
		//	fadeout();
			tpCount = 0;
			beloteState =3;
			tpState = 0;
		}
	// player controls selector up down at all time
	// player presses B button
	// if card is taken move to the gameplay state
	// if not increment tpCount, if tpCount == 4 move to the setTrump state, reset tpCount
	// if tpCount is not == 4 update currentlyPlaying
	}
	else if(beloteState == 3)
	{
		loadSetTrump();
		beloteState = 4;
		vramX = 10;
		vramY = 9;
		signNum = trumpCard;
		pokCardDisplay();
		sX = 119; sY = 72; sW = 82;
		belSel();
		fadein();
		
		currentlyPlaying = firstToBid;
		getJoyNum();
		if(joyNum < 4)
		{
			showCards();
		}
		else
		{
			getAiIndex();
			showAiCards();
		}
	
		
	}
	else if(beloteState == 4)
	{
		if(!tpState)
		{
			getJoyNum();
			if(joyNum > 3)
			{
				getAiIndex();
				aiSetTrump();
			}
			else
			{
				moveBelSel1();
			}
			signNum = trumpCard;
		}
		if(take)
		{
			if(joyNum > 3)
			{
				plys[aiIndex].cards[5] = trumpCard;
			}
			else
			{
				getAiIndex();
				plys[joyNum].cards[5] = trumpCard;
			}
			belote3Deal();
			take = 0;
			spriteX = 80;
			spriteY = 72;
			alternativePokSprite();
			cardClear(10, 9);
			
			drawTrumpSuit();
			tpCount = 0;
			
		}
		
		if(move)
		{
			moveCardSeq(80, 144, 72, 160);
			if(arrived)
			{
				// card arived it is time to change state of the game
				beloteState = 5;
				arrived = 0;
				vramX = 18;
				vramY = 20;
				signNum = trumpCard;
				ppu_wait_nmi();
				pokCardDisplay();
				myDelay(15);
					getJoyNum();
			
				if(joyNum > 3)
				{
				
					vramX = 18;
					vramY = 20;
					ppu_wait_nmi();
					pokBa();
					oam_clear();
					myDelay(15);
				}
				fadeout();
				move = 0;
			}
		}
		
		if(tpCount == 4)
		{
			shuffle();
			belote5Deal();
			deckPos = 0;
			belote5Deal();
			tpState = 0;
			tpCount = 0;
			beloteState = 1;
			--firstToBid;
			if(firstToBid == 255)
			firstToBid = 3;
		}
	}
	else if(beloteState == 5)
	{
		// LOAD PLAY SCREEN
		
//		plys[0].cards[0] = 11;
//		plys[0].cards[1] = 12;
		checkBeRebe();
		loadTrick();
	//	fadein();
		beloteState = 6;
		margin = 82;
		gpState = 0;	
	}
	else if(beloteState == 6)
	{
		if(pad_trigger(0) & PAD_START)
		{
			belotePause();
		}
	
	//	fadein();
		if(!gpState)
		{
		
			getJoyNum();
		
			mustThrow();
			oam_clear();
			cSelX = 24;
			cSelY = 160;
			ppu_wait_nmi();
		//	pal_all(1, newPalette);
			vram_adr(BG1_NT(3, 20));
			vram_fill(4, 384);
			fadein();
			if(joyNum < 4)
			{
				showCards();
				colBelSel();
				signNum = plys[joyNum].cards[sp];
				vramX = 3;
				vramY = 20;
				pokCardDisplay();
			}
			else
			{
				getAiIndex();
				showAiCards();
			}
			gpState = 1;
			
		}
		else if(gpState == 1)
		{
			if(joyNum < 4)
			{
				colBelSel();
				cardSelector();
			}
			else
			{
				aiThrow();
			}
		}
		else if(gpState == 2)
		{
			moveCardSeq1(cSelX, vX << 3, cSelY, vY << 3);
			if(arrived)
			{
				arrived = 0;
				gpState = 0;
				
				vramX = vX;
				vramY = vY;
				ppu_wait_nmi();
				pokCardDisplay();
				
				rearangeCards();
				if(beRebeCon >= 2)
				printMessage(3, 19, "        ");
				if(lastToPlay == currentlyPlaying)
				{
					canTrump = 0;
					transformCards();
					sortSpots();
					calculatePoints();
					
					signNum = cardSpots1[maxi];
					if(maxi == 3)
					{
						spriteX = 80;
						spriteY = 80;
					}
					else if(maxi == 2)
					{
						spriteX = 112;
						spriteY = 104;
					}
					else if(maxi == 1)
					{
						spriteX = 144;
						spriteY = 80;
					}
					else 
					{
						spriteX = 112;
						spriteY = 56;
					}
					alternativePokSprite();
					
	
					if(joyNum > 3)
					{
						joyNum = aiIndex;
					}
					if(plys[joyNum].cardNum == 0)
					{
				//		printMessage(10, 10, "Next Deal");
						
					//	beloteState = 1;
						dealEndPoints();
						gpState = 3;
						tpState = 0;
						if(player1tricksWon >= 5)
						{
							trickster = 1;
							if(player1tricksWon == 8)
							{
								prestige = 1;
							}
						}
						player1tricksWon = 0;
						
					}
					else
					{
						currentlyPlaying = maxi;
						firstToPlay = maxi;
						if(!firstToPlay)
						{
							lastToPlay = 3;
						}
						else
						{
							lastToPlay = firstToPlay - 1;
						}
				
						gpState = 3;
					}
				}
				else
				{
					
					myDelay(20);
					fadeout();
					++currentlyPlaying;
					if(currentlyPlaying == 4)
					{
						currentlyPlaying = 0;
					}
					
				}
				sp = 0;
					
			}
			
		}
		else if(gpState == 3)
		{
			++bblink;	
			ppu_wait_nmi();
			if(bblink < 5)
			{
				pal_all(9, newPalette);
				pal_all(10, &newPalette[16]);
				pal_all(11, &newPalette[32]);
				pal_all(8, &newPalette[48]);
			}
			else if(bblink < 10)
			{
				pal_all(9, oldPalette);
				pal_all(10, &oldPalette[16]);
				pal_all(11, &oldPalette[32]);
				pal_all(8, &oldPalette[48]);
			}
			else
			{
				bblink = 0;
				++bli;
			}
			if(bli == 20)
			{
				fadeout();
				clearMiddle();
				bli = 0;
				gpState = 0;
				if(currentlyPlaying == lastToPlay && !plys[0].cardNum)
				{
					oam_clear();
					beloteState = 1;
					checkEnd();
				}
			
			}
			
		}
		else
		{
			
		}
	}
	else if(beloteState == 7)
	{
		loadTeamArrangement();
		arrangeTeams();
		
		fadein();
		myDelay(150);
		beloteState = 1;
		fadeout();
		
	}
	else if(beloteState == 8)
	{
	/*	++tileUpdate;
		if(tileUpdate == 80)
		tileUpdate = 0;
		if(tileUpdate < 40)
		{
			printMessage(3, 8, "Press Start");
		}
		else
		{
			printMessage(3, 8, "            ");
		}
		if(pad_trigger(0) & PAD_START)
		{
			beloteState = 9;
		}
		*/
	}
	// in setTrump state everything is the same except the trump can be chosen
	// tpCount again increments at every pass, if == 4, shuffle cards, reset everything and go to takePass state again
	
	// if gamePlay starts just load gameplay screen with the selected trump
}

void pickTeam()
{
	for(i = 0; i < 4; i++)
	{

		if(pad_trigger(i) & PAD_LEFT)
		{
			pl[i] = 1;
			if(!plys[i].tablePosition)
			{
					plys[i].tablePosition = 2;
					
					if(!sts[0])
					{
						plys[i].spot = 0;
						sts[0] = i + 1;
						detXY();
					}
					else if(!sts[2])
					{
						plys[i].spot = 2;
						sts[2] = i + 1;
						detXY();
					}
	
			}
			else if(plys[i].tablePosition == 1)
			{
				plys[i].tablePosition = 0;
				
				sts[plys[i].spot] = 0;
				plys[i].spot = 0;
				detXY();
			}
			
		}
		else if(pad_trigger(i) & PAD_RIGHT)
		{
			pl[i] = 1;
			if(!plys[i].tablePosition)
			{
					plys[i].tablePosition = 1; 
					
					if(!sts[1])
					{
						plys[i].spot = 1;
						sts[1] = i + 1;
						detXY();
					}
					else if(!sts[3])
					{
						plys[i].spot = 3;
						sts[3] = i + 1;
						detXY();
					}

			}
			else if(plys[i].tablePosition == 2)
			{
				plys[i].tablePosition = 0;
			
				sts[plys[i].spot] = 0;
				plys[i].spot = 0;
				detXY();
			}
		}
	}
	i = 0;
}

void detXY()
{
	if(i == 0)
	{
		++allEntered;
		token[0] = '1';
		if(plys[i].tablePosition == 0)
		{
			printMessage(3, 11, "  ");
			printMessage(24, 11, "  ");
			printMessage(12, 11, token);
			return;	
		}
		if(plys[i].tablePosition == 2)
		{
			printMessage(3, 11, token);
			printMessage(24, 11, "  ");
			printMessage(12, 11, "  ");
			return;	
		}
		if(plys[i].tablePosition == 1)
		{
			printMessage(3, 11, "  ");
			printMessage(24, 11, token);
			printMessage(12, 11, "  ");
			return;	
		}
	}
	if(i == 1)
	{
		token[0] = '2';
		if(plys[i].tablePosition == 0)
		{
			printMessage(6, 11, "  ");
			printMessage(27, 11, "  ");
			printMessage(18, 11, token);
			return;	
		}
		if(plys[i].tablePosition == 2)
		{
			printMessage(6, 11, token);
			printMessage(27, 11, "  ");
			printMessage(18, 11, "  ");
			return;	
		}
		if(plys[i].tablePosition == 1)
		{
			printMessage(6, 11, "  ");
			printMessage(27, 11, token);
			printMessage(18, 11, "  ");
			return;	
		}
	}
	if(i == 2)
	{
		token[0] = '3';
		if(plys[i].tablePosition == 0)
		{
			printMessage(3, 14, "  ");
			printMessage(24, 14, "  ");
			printMessage(12, 14, token);
			return;	
		}
		if(plys[i].tablePosition == 2)
		{
			printMessage(3, 14, token);
			printMessage(24, 14, "  ");
			printMessage(12, 14, "  ");
			return;	
		}
		if(plys[i].tablePosition == 1)
		{
			printMessage(3, 14, "  ");
			printMessage(24, 14, token);
			printMessage(12, 14, "  ");
			return;	
		}
	}
	if(i == 3)
	{
		token[0] = '4';
		if(plys[i].tablePosition == 0)
		{
			printMessage(6, 14, "  ");
			printMessage(27, 14, "  ");
			printMessage(18, 14, token);
			return;	
		}
		if(plys[i].tablePosition == 2)
		{
			printMessage(6, 14, token);
			printMessage(27, 14, "  ");
			printMessage(18, 14, "  ");
			return;	
		}
		if(plys[i].tablePosition == 1)
		{
			printMessage(6, 14, "  ");
			printMessage(27, 14, token);
			printMessage(18, 14, "  ");
			return;	
		}
	}
}

void loadTakePass()
{
	ppu_off();
	music_stop();
	vram_adr(0x2C00);
	vram_fill(0, 2048);	
	
	vram_adr(0x5000);
	vram_write_bank(CHR4_BANK, (char *) 0x8000 + 1120 + 1504 + 4000 + 1794 + 384 + 4480 + 1794, 5536);
	vram_adr(0x2800);
	vram_write_bank(CHR4_BANK, (char *) 0x8000 + 1120 + 1504 + 4000 + 1794 + 384 + 4480 + 1794 + 5536, 1794);
	
	
	vram_adr(0x3000);
	vram_write_bank(CHR5_BANK, (char *) 0x8000 + SOLITAIRE_TABLE_S + SOLITAIRE_TABLE_MAP_S, SOLITAIRE_CARDS_S);

	vram_adr(0x2400);
	vram_fill(4, 2048);
	scroll_bg3(0, 255);
	printTeamPoints();
	
	ppu_on_all();
	
}

void loadSetTrump()
{
	ppu_off();
	
	oam_clear();
	
	vram_adr(0x2C00);
	vram_fill(0, 2048);	
	
	vram_adr(0x5000);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + BEFORE_POKER_S + POKER_TABLE_S + POKER_TABLE_MAP_S + SMALL_SPADES_S + POKER_FIELDS_S + SLIDER_BAR_S + 5728 + 1794 + 352, 6368);
	vram_adr(0x2800);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + BEFORE_POKER_S + POKER_TABLE_S + POKER_TABLE_MAP_S + SMALL_SPADES_S + POKER_FIELDS_S + SLIDER_BAR_S + 5728 + 1794 + 352 + 6368, 1794);
	


	vram_adr(0x2400);
	vram_fill(4, 2048);
	
	printTeamPoints();
	scroll_bg3(0, 255);
	
	ppu_on_all();
	
}

void loadTrick()
{
	player1tricksWon = 0;
	ppu_off();
	
	oam_clear();
	
	vram_adr(0x2C00);
	vram_fill(0, 2048);	
	
	vram_adr(0x5000);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + BEFORE_POKER_S + POKER_TABLE_S + POKER_TABLE_MAP_S + SMALL_SPADES_S + POKER_FIELDS_S + SLIDER_BAR_S, 5728);
	
	vram_adr(0x2800);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + BEFORE_POKER_S + POKER_TABLE_S + POKER_TABLE_MAP_S + SMALL_SPADES_S + POKER_FIELDS_S + SLIDER_BAR_S + 5728, 1794);
	


	vram_adr(0x2400);
	vram_fill(4, 2048);
	
	drawTrumpSuit();
	
	printTeamPoints();
	scroll_bg3(0, 255);
	music_play(0x88, music5);
	ppu_on_all();
}

void loadTeamArrangement()
{
	ppu_off();
	
	vram_adr(0x2C00);
	vram_fill(0, 2048);	
	
	vram_adr(0x5000);
	vram_write_bank(CHR4_BANK, (char *) 0x8000 + 1120 + 1504 + 4000 + 1794 + 384 + 4480 + 1794 + 5536 + 1794, 5920);
	vram_adr(0x2800);
	vram_write_bank(CHR4_BANK, (char *) 0x8000 + 1120 + 1504 + 4000 + 1794 + 384 + 4480 + 1794 + 5536 + 1794 + 5920, 1794);

	vram_adr(0x2400);
	vram_fill(4, 2048);
	scroll_bg3(0, 255);
	ppu_on_all();
}

unsigned char jnOld;
void belote5Deal()
{
	getJoyNum();
	jnOld = joyNum;
	k = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 5; j++)
		{
			getJoyNum();
			findValidCard();
			if(joyNum > 3)
			{
				joyNum -= 3;
			}
			plys[joyNum].cards[j] = cardsDeck[deckPos];
			++deckPos;
			++currentlyPlaying;
			if(currentlyPlaying == 4)
			{
				currentlyPlaying = 0;
			}
		}
		plys[i].cards[j] = 255;
	}
	joyNum = jnOld;
	i = 0; j = 0; k = 0;
}

void belote3Deal()
{
	for(i = 0; i < 4; i++)
	{
			plys[i].cardNum = 5;
	}
	
	jnOld = joyNum;
	
	if(joyNum > 3)
	{
		getAiIndex();
		plys[aiIndex].cardNum = 6;
	}
	else
	{
		plys[joyNum].cardNum = 6;
	}

	
	for(i = 0; i < 11; ++i)
	{
		++currentlyPlaying;
		if(currentlyPlaying == 4)
		{
			currentlyPlaying = 0;
		}
		getJoyNum();
		findValidCard();
		if(joyNum > 3)
		{
			joyNum -= 3;
		}
		plys[joyNum].cards[plys[joyNum].cardNum] = cardsDeck[deckPos];
		++deckPos;
		++plys[joyNum].cardNum;
	}
	i = 0;
	
	plys[0].cards[8] = 255;
	plys[1].cards[8] = 255;
	plys[2].cards[8] = 255;
	plys[3].cards[8] = 255;
	
	plys[0].cardNum = 8;
	plys[1].cardNum = 8;
	plys[2].cardNum = 8;
	plys[3].cardNum = 8;
	
	joyNum = jnOld;
	
	++currentlyPlaying;
	if(currentlyPlaying == 4)
	{
		currentlyPlaying = 0;
	}
	
	firstToPlay = currentlyPlaying;
	if(currentlyPlaying)
	{
		lastToPlay = currentlyPlaying - 1;
	}
	else
	{
		lastToPlay = 3;
	}
		
	
//	printNumber(deckPos, 1, 1);
//	while(1);
}

void findValidCard()
{
	while((cardsDeck[deckPos] & 0x0F) < 6) 
	{
		
		if(!(cardsDeck[deckPos] & 0x0F))
		{
			break;
		}
		++deckPos;
	}
}

void showCards()
{
	vramX = 3;
	vramY = 20;
	j = vramX;
	j = 0;
	i = 0;

	for(xc = 0; xc < 9; xc++)
	{
		signNum = (unsigned char)plys[joyNum].cards[xc];
		
	//	signNum = 255;
		if(signNum == 255)
		break;
		ppu_wait_nmi();
		pokCardDisplay();

		vramX += 3;
		vramY = 20;
	}
	k = 0;
	j = 0;
}

void belSel()
{
	ppu_wait_nmi();
	oam_spr(sX, sY, 107, SPRITE_ATTR_FULL(5, 3, 0, 0, 1), 0);
	oam_spr(sX+sW, sY, 107, SPRITE_ATTR_FULL(5, 3, 0, 1, 1), 4);
	oam_spr(sX+sW, sY+8, 107, SPRITE_ATTR_FULL(5, 3, 1, 1, 1), 8);
	oam_spr(sX, sY+8, 107, SPRITE_ATTR_FULL(5, 3, 1, 0, 1), 12);
}

void cardSelector()
{
	if(pad_trigger(joyNum) & PAD_RIGHT)
			{
				if(sp < plys[joyNum].cardNum -1)
				{
					
					++sp;
					cSelX += 24;
					vramY = 20;
					vramX = cSelX >> 3;
					signNum = plys[joyNum].cards[sp];
					ppu_wait_nmi();
					pokCardDisplay();
					sfx_play(2);
				}
			}
			if(pad_trigger(joyNum) & PAD_LEFT)
			{
				if(sp)
				{
					--sp;
					cSelX -= 24;
					vramY = 20;
					vramX = cSelX >> 3;
					signNum = plys[joyNum].cards[sp];
					pokCardDisplay();
					sfx_play(2);
				}
			}
			else if(pad_trigger(joyNum) & PAD_B)
			{
				if(mt == 5 || ((plys[joyNum].cards[sp] >> 4) == mt)) //|| (plys[joyNum].cards[sp] >> 4 == trumpSuit && canTrump))
				{
					declareBeRebe();
					
					ppu_wait_nmi();
					oam_clear();
					spriteX = cSelX;
					spriteY = cSelY;
					alternativePokSprite();
					cardClear(cSelX >> 3, 20);
					vramX = (cSelX >> 3) + 3;
					vramY = 20;
					if(!sp)
					{
						auxSignNum = plys[joyNum].cards[sp + 1];
						if(plys[joyNum].cardNum != 1)
						leftEdge();
					}
					else if(plys[joyNum].cards[sp + 1] != 255)
					{
						ppu_wait_nmi();
						auxSignNum = plys[joyNum].cards[sp + 1];
						leftEdge();
						vramY -= 6;
						vramX -= 3;
						auxSignNum = plys[joyNum].cards[sp - 1];
						rightEdge();
					}
					else if(plys[joyNum].cardNum)
					{
					//	vramY -= 6;
						vramX -= 3;
						auxSignNum = plys[joyNum].cards[sp - 1];
						rightEdge();
					}
					gpState = 2;
					switch(currentlyPlaying)
					{
						case 0:
							vY = 7;
							vX = 14;
							cardSpots[0] = plys[joyNum].cards[sp];
							cardSpots1[0] = plys[joyNum].cards[sp];
						//	printNumber(cardSpots[0] & 0x0F, 5, 5);
							break;
						case 1:
							vY = 10;
							vX = 18;
							cardSpots[1] = plys[joyNum].cards[sp];
							cardSpots1[1] = plys[joyNum].cards[sp];
							break; 
						case 2:
							vY = 13;
							vX = 14;
							cardSpots[2] = plys[joyNum].cards[sp];
							cardSpots1[2] = plys[joyNum].cards[sp];
							break;
						case 3:
							vY = 10;
							vX = 10;
							cardSpots[3] = plys[joyNum].cards[sp];
							cardSpots1[3] = plys[joyNum].cards[sp];
							break;		
					}
					if(firstToPlay == currentlyPlaying)
					{
						leadingSuit = plys[joyNum].cards[sp] >> 4;
					}
					
					if(plys[joyNum].cards[sp] >> 4 == trumpSuit)
					{
						canTrump = 1;	
					}	
				}
			
			}
}

void moveBelSel()
{
	if(pad_trigger(joyNum) & PAD_DOWN)
	{
		if(sY == 72)
		sfx_play(2);
		sX = 119;
		sY = 96;
		belSel();
	}
	else if(pad_trigger(joyNum) & PAD_UP)
	{
		if(sY == 96)
		sfx_play(2);
		sX = 119;
		sY = 72;
		belSel();
	}
	else if(pad_trigger(joyNum) & PAD_B)
	{
		if(sY == 96)
		{
			++tpCount;
			++currentlyPlaying;
			if(currentlyPlaying == 4)
			{
				currentlyPlaying = 0;
			}
		}
		else
		{
			take = 1;
			move = 1;
			tpState = 1;
		}
		if(!take)
		{
			fadeout();
			ppu_off();
			oam_clear();
			vram_adr(BG1_NT(3, 20));
			vram_fill(4, 384);
		//	vram_put(4);
			sY = 72;
			belSel();
			ppu_on_all();
			if(tpCount != 4)
			fadein();
			
			
			
			getJoyNum();
			if(joyNum < 4)
			{
				showCards();
			}
			else
			{
				getAiIndex();
				showAiCards();
			}
		}
	}
}

void moveBelSel1()
{
	//printMessage(10, 10, "bs1");
	if(pad_trigger(joyNum) & PAD_DOWN)
	{
		sX = 119;
		sY += 16;
		if(sY >= 136)
		{
			sY = 136;
			sW = 42;
		}
		else
		{
			sfx_play(2);
		}
		belSel();
		setTrumpDetermine();
	}
	else if(pad_trigger(joyNum) & PAD_UP)
	{
		sX = 119;
		sY -= 16;
		sW = 82;
		if(sY < 72)
		{
			sY = 72;

		}
		else
		{
			sfx_play(2);
		}
		
		belSel();
		setTrumpDetermine();
	}
	else if(pad_trigger(joyNum) & PAD_B)
	{
		if(sY == 136)
		{
			++tpCount;
			++currentlyPlaying;
			if(currentlyPlaying == 4)
			{
				currentlyPlaying = 0;
			}
		}
		else
		{
			take = 1;
			move = 1;
		}
		if(!take)
		{
			fadeout();
			ppu_off();
			oam_clear();
			vram_adr(BG1_NT(3, 20));
			vram_fill(4, 384);
		//	vram_put(4);
			sY = 72;
			sW = 82;
			belSel();
			ppu_on_all();
			if(tpCount != 4)
			fadein();
			getJoyNum();
			if(joyNum < 4)
			{
				showCards();
			}
			else
			{
				getAiIndex();
				showAiCards();
			}
		}
	}
}


void clearCard()
{
	ppu_wait_nmi();
	vram_adr(BG1_NT(vramX, vramY));
	vram_fill(4, 8);
	vram_adr(BG1_NT(vramX, vramY+1));
	vram_fill(4, 8);
	vram_adr(BG1_NT(vramX, vramY+2));
	vram_fill(4, 8);
	vram_adr(BG1_NT(vramX, vramY+3));
	vram_fill(4, 8);
	vram_adr(BG1_NT(vramX, vramY+4));
	vram_fill(4, 8);
	vram_adr(BG1_NT(vramX, vramY+5));
	vram_fill(4, 8);
}

void colBelSel()
{
	availableSprid = 100;
	// corner tiles
	ppu_wait_nmi();
	availableSprid = oam_spr(cSelX, cSelY, selTile, SPRITE_ATTR_FULL(4, 3, 0, 0, 1), availableSprid);
	availableSprid = oam_spr(cSelX+24, cSelY, selTile, SPRITE_ATTR_FULL(4, 3, 0, 1, 1), availableSprid);
	
	++tileUpdate;
	if(tileUpdate > 50)
	{
		tileUpdate = 0;
		++selTile;
		if(selTile == 111)
		selTile = 107;
		++selTile1;
		if(selTile1 == 103)
		selTile1 = 99;
	}
}

#define ATTR 0X2C00

#define drawSpade(){\
						vram_adr(BG3_NT(15, 4));\
						vram_put(159 | ATTR);\
						vram_put(160 | ATTR);\
						vram_adr(BG3_NT(15, 5));\
						vram_put(168 | ATTR);\
						vram_put(169 | ATTR);\
						vram_adr(BG3_NT(15, 6));\
						vram_put(178 | ATTR);\
						vram_put(179 | ATTR);\
					}\

#define drawHeart(){\
						vram_adr(BG3_NT(15, 4));\
						vram_put(163 | ATTR);\
						vram_put(164 | ATTR);\
						vram_adr(BG3_NT(15, 5));\
						vram_put(172 | ATTR);\
						vram_put(173 | ATTR);\
						vram_adr(BG3_NT(15, 6));\
						vram_put(180 | ATTR);\
						vram_put(180 | 0x6C00);\
					}\

#define drawClover(){\
						vram_adr(BG3_NT(15, 4));\
						vram_put(161 | ATTR);\
						vram_put(162 | ATTR);\
						vram_adr(BG3_NT(15, 5));\
						vram_put(170 | ATTR);\
						vram_put(171 | ATTR);\
						vram_adr(BG3_NT(15, 6));\
						vram_put(178 | ATTR);\
						vram_put(179 | ATTR);\
					}\

#define drawDiamond(){\
						vram_adr(BG3_NT(15, 4));\
						vram_put(165 | ATTR);\
						vram_put(166 | ATTR);\
						vram_adr(BG3_NT(15, 5));\
						vram_put(174 | ATTR);\
						vram_put(175 | ATTR);\
						vram_adr(BG3_NT(15, 6));\
						vram_put(180 | ATTR);\
						vram_put(180 | 0x6C00);\
					}\

void drawTrumpSuit()
{
	ppu_wait_nmi();
	switch(trumpSuit)
	{
		case 0:
			drawClover();
			break;
		case 1:
			drawHeart();
			break;
		case 2:
			drawSpade();
			break;
		case 3:
			drawDiamond();
			break;			
	}
}

void setTrumpDetermine()
{
	if(sY == 72)
	{
		trumpSuit = 2;
	}
	else if(sY == 88)
	{
		trumpSuit = 0;
	}
	else if(sY == 104)
	{
		trumpSuit = 3;
	}
	else
	{
		trumpSuit = 1;
	}
}

void rearangeCards()
{
	//
	if(joyNum > 3)
	{
		joyNum = aiIndex;
	}
	for(i = sp; i < plys[joyNum].cardNum; i++)
	{
		plys[joyNum].cards[i] = plys[joyNum].cards[i+1];
	}
	i = 0;
	--plys[joyNum].cardNum;
	
}

void lSuit()
{
	if((cardSpots[i] >> 4) == leadingSuit)
	{
		cardSpots[i] &= 0x0F;
		cardSpots[i] |= 0x40;
	}
	else
	{
		cardSpots[i] &= 0x0F;
	}
}

void transformCards()
{
	
	for(i = 0; i < 4; i++)
	{
		if((cardSpots[i] >> 4) != trumpSuit)
		{
			if(!(cardSpots[i] & 0x0F))
			{
				lSuit();
				cardSpots[i] &= 0xF0;
				cardSpots[i] |= 11;
				
			}
			else if((cardSpots[i] & 0x0F) < 9)
			{
				lSuit();
				cardSpots[i] &= 0xF0;
				
			}
			else if((cardSpots[i] & 0x0F) == 9)
			{
				++cardSpots[i];
				lSuit();
				
			}
			else
			{
				cardSpots[i] -= 8;
				lSuit();
			}
		}
		else
		{
		//	printMessage(10, 10, "Trump Found");
			if((cardSpots[i] & 0x0F) == 10)
			{
				cardSpots[i] = 20;
				cardSpots[i] &= 0x1F;
				cardSpots[i] |= 0xC0;
			}
			else if((cardSpots[i] & 0x0F) == 8)
			{
				cardSpots[i] = 14;
				cardSpots[i] &= 0x0F;
				cardSpots[i] |= 0xC0;
			}
			else if(!(cardSpots[i] & 0x0F))
			{
				cardSpots[i] = 11;
				cardSpots[i] &= 0x0F;
				cardSpots[i] |= 0xC0;
			}
			else if((cardSpots[i] & 0x0F) < 8)
			{
				cardSpots[i] = 0;
				cardSpots[i] &= 0x0F;
				cardSpots[i] |= 0xC0;
			}
			else if((cardSpots[i] & 0x0F) == 9)
			{
				++cardSpots[i];
				cardSpots[i] &= 0x0F;
				cardSpots[i] |= 0xC0;
			}
			else
			{
				cardSpots[i] -= 8;
				cardSpots[i] &= 0x0F;
				cardSpots[i] |= 0xC0;
			}
			
			
		}
	}
	i = 0;
}

void sortSpots()
{
	max = cardSpots[0];
	maxi = 0;
	for(i = 1; i < 4; i++)
	{
		if(cardSpots[i] > max)
		{
			max = cardSpots[i];
			maxi = i;
		}
	}
	i = 0;
}

void calculatePoints()
{
/*	if(maxi == 0 || maxi == 2)
	{
		printMessage(20, 10, "Team 1 wins");
	}
	else
	{
		printMessage(20, 11, "Team 2 wins");
	}
*/	

	if(sts[maxi] == 1 )
	{
		++player1tricksWon;
	}
	for(i = 0; i < 4; i++)
	{
		
		if(maxi == 0 || maxi == 2)
		{
			team1.currentPoints += cardSpots[i] & 0x3F;
			printNumber(team1.currentPoints, 4, 5);
			if(plys[0].cardNum == 0 && team1.trump)
			team1.lastTrick = 1;
		}
		else
		{
			team2.currentPoints += cardSpots[i] & 0x3F;
			printNumber(team2.currentPoints, 25, 5);
			if(plys[1].cardNum == 0 && team2.trump)
			team2.lastTrick = 1;
		}
	}
	
	if(maxi == 0 || maxi == 2)
	{
		++team1.tricksWon;
	}
	else
	{
		++team2.tricksWon;
	}
	
	i = 0;
}


unsigned char hasSuit, hasTrump;

void mustThrow()
{
	hasSuit = 0; 
	hasTrump = 0;
	
	if(firstToPlay == currentlyPlaying)
	{
		mt = 5;
	
		return;
	}
	
	for(i = 0; i < plys[joyNum].cardNum; i++)
	{
		if(plys[joyNum].cards[i] >> 4 == leadingSuit)
		{
			hasSuit = 1;
			mt = leadingSuit;
		
			return;
		}
		if(plys[joyNum].cards[i] >> 4 == trumpSuit)
		{
			hasTrump = 1;
			
		}
	}
	if(hasTrump)
	{
		mt = trumpSuit;
	}
	else
	{
		mt = 5;
	
	}
	i = 0;
}

void aiTakePass()
{

		aiTakeDecision();
	//	delay(20);
		if(sY == 96)
		{
			belSel();
			++tpCount;
			++currentlyPlaying;
			if(currentlyPlaying == 4)
			{
				currentlyPlaying = 0;
			}
		}
		else
		{
			take = 1;
			move = 1;
			tpState = 1;
		}
		if(!take)
		{
			fadeout();
			ppu_off();
			oam_clear();
			vram_adr(BG1_NT(3, 20));
			vram_fill(4, 384);
		//	vram_put(4);
			sY = 72;
			belSel();
			ppu_on_all();
			if(tpCount != 4)
			fadein();
			getJoyNum();
			if(joyNum < 4)
			{
				showCards();
			}
			else
			{
				getAiIndex();
				showAiCards();
			}
		}
}

void aiTakeDecision()
{
	for(i = 0; i < 5; i++)
	{
		if((plys[aiIndex].cards[i] >> 4) == (trumpCard >> 4))
		{
			if(!(plys[aiIndex].cards[i] & 0x0F) || ((plys[aiIndex].cards[i] & 0x0F) == 10) || ((plys[aiIndex].cards[i] & 0x0F) == 8))
			{
				sY = 72;
				i = 0;
				return;
			}
		}
	}
	i = 0;
	sY = 96;
	take = 0;
	move = 0;
}

unsigned char ais;

void aiSetTrump()
{
/*	for(i = 0; i < 5; i++)
	{
		plys[0].cards[i] = 2;
		plys[1].cards[i] = 2;
		plys[2].cards[i] = 2;
		plys[3].cards[i] = 2;
	}
	
	plys[0].cards[5] = 255;
		plys[1].cards[5] = 255;
	//	plys[2].cards[4] = 0x10;
		plys[2].cards[5] = 255;
		plys[1].cards[4] = 0x30;
		plys[3].cards[5] = 255;
*/	
	sY = 96;
	for(i = 0; i < 5; i++)
	{
	
		
			if(!(plys[aiIndex].cards[i] & 0x0F) || ((plys[aiIndex].cards[i] & 0x0F) == 10) || ((plys[aiIndex].cards[i] & 0x0F) == 8))
			{
				
				trumpSuit = plys[aiIndex].cards[i] >> 4;
				switch(trumpSuit)
				{
					case 0:
						ais = 1;
						break;
					case 1:
						ais = 3;
						break;
					case 2:
						ais = 0;
						break;
					case 3:
						ais = 2;
						break;			
				}
				sY = 0;
				
			}
		
	}
		if(sY == 96)
		{
			ais = 4;
			++tpCount;
			++currentlyPlaying;
			if(currentlyPlaying == 4)
			{
				currentlyPlaying = 0;
			}
		}
		else
		{
			take = 1;
			move = 1;
			tpState = 1;
		}
		
		
//		printNumber(ais, 5, 10);
//		while(!(pad_trigger(0) & PAD_START));
		sY = 72;
		for(i = 0; i < ais; i++)
		{
			myDelay(5);
			sY += 16;
			
			if(sY == 136)
			sW = 42;
			belSel();
			myDelay(5); 
		}
		
		i = 0;
		
		
		
		if(!take)
		{
			fadeout();
			ppu_off();
			oam_clear();
			vram_adr(BG1_NT(3, 20));
			vram_fill(4, 384);
		//	vram_put(4);
			sY = 72;
			sW = 82;
			belSel();
			ppu_on_all();
			if(tpCount != 4)
			fadein();
			//
			getJoyNum();
			if(joyNum < 4)
			{
				showCards();
			}
			else
			{
				getAiIndex();
				showAiCards();
			}
		}
	i = 0;
//	while(!(pad_trigger(0) & PAD_START));
}

unsigned char conP;

void findUnconected()
{
	conP = 2;
	conPl();
	conP = 3;
	conPl();
	conP = 4;
	conPl();
	
}

void conPl()
{
	for(i = 0; i < 4; i++)
	{
		if(sts[i] == conP)
		{
			i = 10;
			break;
		}
		if(!sts[i])
		{
			j = i;
		}
	}
	if(i != 10)
	{
		sts[j] = conP + 3;
	}
//	printNumber(conP, 5, 10);
	
	
	i = 0;
	j = 0;
}

void showAiCards()
{
		vramX = 3;
	vramY = 20;
	j = vramX;
	j = 0;
	i = 0;

	for(xc = 0; xc < 9; xc++)
	{
		signNum = (unsigned char)plys[aiIndex].cards[xc];
		
	//	signNum = 255;
		if(signNum == 255)
		break;
		ppu_wait_nmi();
	//	pokCardDisplay();
		pokBa();

		vramX += 3;
		vramY = 20;
	}
	k = 0;
	j = 0;
//	printNumber(plys[joyNum])
}

void aiThrow()
{
	myDelay(10);
	if(firstToPlay == currentlyPlaying)
	{
		findHighNonT();
	//	printMessage(1,1, "fp");
	}
	else
	{
		printMessage(1,1, "  ");
		aiRespond();
	}
	vramX = 3 + snesmul(sp, 3);
	vramY = 20;
	cSelX = vramX << 3;
	signNum = plys[aiIndex].cards[sp];
	declareBeRebe();
	ppu_wait_nmi();
//	pokCardDisplay();
			 
		ppu_wait_nmi();
		oam_clear();
		spriteX = cSelX;
		spriteY = cSelY;
		alternativePokSprite();
		cardClear(cSelX >> 3, 20);
		vramX = (cSelX >> 3) + 3;
		vramY = 20;
		if(!sp)
		{
		//	auxSignNum = plys[aiIndex].cards[sp + 1];
			if(plys[aiIndex].cardNum != 1)
			baLeftEdge();
		}
		else if(plys[aiIndex].cards[sp + 1] != 255)
		{
			ppu_wait_nmi();
		//	auxSignNum = plys[aiIndex].cards[sp + 1];
			baLeftEdge();
			vramY -= 6;
			vramX -= 3;
		//	auxSignNum = plys[aiIndex].cards[sp - 1];
			baRightEdge();
		}
		else if(plys[aiIndex].cardNum)
		{
		//	vramY -= 6;
			vramX -= 3;
		//	auxSignNum = plys[aiIndex].cards[sp - 1];
			baRightEdge();
		}
		gpState = 2;
		switch(currentlyPlaying)
		{
			case 0:
				vY = 7;
				vX = 14;
				cardSpots[0] = plys[aiIndex].cards[sp];
				cardSpots1[0] = plys[aiIndex].cards[sp];
			//	printNumber(cardSpots[0] & 0x0F, 5, 5);
				break;
			case 1:
				vY = 10;
				vX = 18;
				cardSpots[1] = plys[aiIndex].cards[sp];
				cardSpots1[1] = plys[aiIndex].cards[sp];
				break; 
			case 2:
				vY = 13;
				vX = 14;
				cardSpots[2] = plys[aiIndex].cards[sp];
				cardSpots1[2] = plys[aiIndex].cards[sp];
				break;
			case 3:
				vY = 10;
				vX = 10;
				cardSpots[3] = plys[aiIndex].cards[sp];
				cardSpots1[3] = plys[aiIndex].cards[sp];
			break;		
		}
		if(firstToPlay == currentlyPlaying)
		{
			leadingSuit = plys[aiIndex].cards[sp] >> 4;
		}
					
		if(plys[aiIndex].cards[sp] >> 4 == trumpSuit)
		{
			canTrump = 1;	
		}	

}

unsigned char aiMax, aiMaxi, aiMin;
unsigned char aiMin1, aiMax1, aiMini;
unsigned char aiArray[9];

void findHighNonT()
{
	aiMax = 0;
	aiMaxi = 0;
	for(i = 0; i < plys[aiIndex].cardNum; i++)
	{
		if((plys[aiIndex].cards[i] >> 4) != trumpSuit)
		{
			if(!(plys[aiIndex].cards[i] & 0x0F) )
			{
				aiMaxi = i;
				aiMax = 1;
				break;
			}
			else if((plys[aiIndex].cards[i] & 0x0F) == 9)
			{
				aiMaxi = i;
				aiMax = 1;
				break;
			}
			else
			{
				if(aiMax < (plys[aiIndex].cards[i] & 0x0F))
				{
					aiMax = plys[aiIndex].cards[i] & 0x0F;
					aiMaxi = i;
				}
			}
		}
	}
	
	if(!aiMax)
	{
		trumpMin();
		sp = aiMini;
	}
	else
	{
		sp = aiMaxi;
	}
	i = 0;
}



void trumpMin()
{
	aiMin = 255;
	
	for(i = 0; i < plys[aiIndex].cardNum; i++)
	{
		aiMin1 = (plys[aiIndex].cards[i] & 0x0F);
		if((plys[aiIndex].cards[i] >> 4) == trumpSuit)
		{
			if((aiMin1 & 0x0f) == 0)
			{
				aiMin1 = 14;
			}
			else if((aiMin1 & 0x0f) == 8)
			{
				aiMin1 = 15;
			}
			else if((aiMin1 & 0x0f) == 10)
			{
				aiMin1 = 16;
			}
			else if((aiMin1 & 0x0f) == 9)
			{
				aiMin1 = 13;
			}
		
			if(aiMin1 < aiMin)
			{
				aiMin = aiMin1;
				aiMini = i;
			}
		}
	}
	i = 0;
}

void aiRespond()
{
	aiMax = 0;
	aiMaxi = 0;
//	while(!(pad_trigger(0) & PAD_START));
	for(i = 0; i < plys[aiIndex].cardNum; i++)
	{
		aiMax1 = (plys[aiIndex].cards[i] & 0x0F);
		if((plys[aiIndex].cards[i] >> 4) == leadingSuit)
		{
		//	printMessage(3, 15, "Leading Suit");
		//	printNumber(leadingSuit, 3, 16);
		//	printNumber(plys[aiIndex].cards[i] & 0x0F, 3, 17);
		//	while(!(pad_trigger(0) & PAD_START));
			if((aiMax1 & 0x0f) == 0)
			{
				aiMax1 = 14;
			}
			else if((aiMax1 & 0x0f) == 8)
			{
				if(leadingSuit == trumpSuit)
				{
					aiMax1 = 20;
				}
			}
			else if((aiMax1 & 0x0f) == 10)
			{
				if(leadingSuit == trumpSuit)
				{
					aiMax1 = 30;
				}
			}
			else if((aiMax1 & 0x0f) == 9)
			{
				aiMax1 = 13;
			}
		
			if(aiMax1 > aiMax)
			{
				aiMax = aiMax1;
				aiMaxi = i;
			}
		//	printNumber(aiMax, 3, 18);
		//	while(!(pad_trigger(0) & PAD_START));
		}
	}
	
	if(!aiMax)
	{
		trumpMin();
		if(aiMin != 255)
		{
			sp = aiMini;
		}
		else
		{
			lowestCard();
			sp = aiMini;
		}
	}
	else
	{
		sp = aiMaxi;
	}
	i = 0;
	//find lowest trump here
	
	// if not found find the lowest card possible
}

void lowestCard()
{
	aiMin = 255;
	aiMini = 0;
	
	for(i = 0; i < plys[aiIndex].cardNum; i++)
	{
		aiMin1 = (plys[aiIndex].cards[i] & 0x0F);
		if((aiMin1 & 0x0f) == 0)
		{
			aiMin1 = 14;
		}
	
		else if((aiMin1 & 0x0f) == 9)
		{
			aiMin1 = 13;
		}
		
		if(aiMin1 < aiMin)
		{
			aiMin = aiMin1;
			aiMini = i;
		}
	}
	i = 0;
}

#define printSymbols(a, b, c){\
							printMessage(a, b, "(");\
							if(c)\
							{\
								printNumber(team1.totalPoints, 5, 6);\
							}\
							else\
							{\
								printNumber(team2.totalPoints, 24, 6);\
							}\
							printMessage(a+2, b, "/");\
							printNumber(502, a+3, b);\
							printMessage(a+6, b, ")");\
						}\

void printTeamPoints()
{
	printNumber(team1.currentPoints, 4, 5);
	printNumber(team2.currentPoints, 25, 5);
//	printSymbols(2, 6, 1);
//	printSymbols(23, 6, 0);
	printNumber(team1.totalPoints, 4, 6);
	printNumber(team2.totalPoints, 25, 6);
//	printMessage(6, 6, "/");
//	printNumber(502, 6, 6);

}

void dealEndPoints()
{
	
	if(team1.lastTrick)
	{
		team1.currentPoints += 10;
		team1.bonus1 = 10;
	}
	if(team1.tricksWon == 8)
	{
		team1.currentPoints += 90;
		team1.bonus2 = 90;
	}
	
	if(team2.lastTrick)
	{
		team2.currentPoints += 10;
		team2.bonus1 = 10;
	}
	if(team2.tricksWon == 8)
	{
		team2.currentPoints += 90;
		team2.bonus2 = 90;
	}
	
	if(team1.trump && team1.currentPoints < margin)
	{
		team2.totalPoints += 152;
		team2.totalPoints += team2.bonus1 + team2.bonus2 + team2.bonus;
		team2.bonus1 = 0;
		team2.bonus2 = 0;
		team2.bonus = 0;
		
		team1.trump = 0;
		team2.trump = 0;
		
		team1.currentPoints = 0;
		team2.currentPoints = 0;
		
		team1.tricksWon = 0;
		team1.lastTrick = 0;
		
		team2.tricksWon = 0;
		team2.lastTrick = 0;
		
		team1.bonus = 0;
		team1.bonus1 = 0;
		team1.bonus2 = 0;
		
		return;
	}
	if(team2.trump && team2.currentPoints < margin)
	{
		team1.totalPoints += 152;
		team1.totalPoints += team2.bonus1 + team1.bonus2 + team1.bonus;
		team1.bonus1 = 0;
		team1.bonus2 = 0;
		team1.bonus = 0;
		
		team1.trump = 0;
		team2.trump = 0;
		
		team1.currentPoints = 0;
		team2.currentPoints = 0;
		
		team2.tricksWon = 0;
		team2.lastTrick = 0;
		
		team1.tricksWon = 0;
		team1.lastTrick = 0;
		
		team2.bonus = 0;
		team2.bonus1 = 0;
		team2.bonus2 = 0;
		return;
	}
	
	team1.totalPoints += team1.currentPoints;
	team2.totalPoints += team2.currentPoints;
	
	team1.totalPoints += team1.bonus;
	team2.totalPoints += team2.bonus;
	
	team1.bonus = 0;
	team2.bonus = 0;
	
		team1.trump = 0;
		team2.trump = 0;
		
		team1.currentPoints = 0;
		team2.currentPoints = 0;
		
		team1.tricksWon = 0;
		team2.tricksWon = 0;
		
		team1.lastTrick = 0;
		team2.lastTrick = 0;
		
		team2.bonus1 = 0;
		team2.bonus2 = 0;
		team1.bonus1 = 0;
		team1.bonus2 = 0;
}

void checkEnd()
{
//	team1.totalPoints = 600;
	if(team1.totalPoints >= 502 || team2.totalPoints >= 502)
	{
		if(team1.totalPoints > team2.totalPoints)
		{
			winLoseState = 1;
			if(sts[0] == 1 || sts[2] == 1)
			{
			//	player 1 wins
				beloteAchievments();
			}
			else
			{
				conBelWins = 0;
				savesram();
			}
		}
		else
		{
			winLoseState = 2;
			if(sts[1] == 1 || sts[3] == 1)
			{
				beloteAchievments();
			}
			else
			{
				conBelWins = 0;
				savesram();
			}
		}
		winings = betAmount + betAmount + snesdiv(betAmount, 4);
		screenState = 5;
		cash += winings;
		
	ppu_off();
	asm("lda #%b", 0b00110001);
	asm("sta $210B");
	//
	//lda #3
	//sta OBSEL 2101 reset sprite tilesets addresses
	asm("lda #3");
	asm("sta $2101");
	//
	// reset NTADDR
	asm("lda #0");
	asm("sta $2107");
	//
	asm("lda #5");
	asm("sta $210C");
	//lda #%00000100
	//sta $2108
	asm("lda #%b", 0b00000100);
	asm("sta $2108");
	// 2109 for bg3 map, start at $5400
	asm("lda #%b", 0b01010100);
	asm("sta $2109");
	resetBelote();
	}
}

void resetBelote()
{

margin = 82;
player1tricksWon = 0;
for(i = 0; i < 4; i++)
{
	pl[i] = 0;
	plys[i].tablePosition = 0;
	allEntered = 0;
	for(j = 0; j < 9; j++)
	{
		plys[i].cards[j] = 0;
	}
	plys[i].cardNum = 0;
}

i = 0;
j = 0;
	
	team1.currentPoints = 0;
	team1.totalPoints = 0;
	team1. trump = 0;
	team1.tricksWon = 0;
	team1.lastTrick = 0;
	
	team2.currentPoints = 0;
	team2.totalPoints = 0;
	team2. trump = 0;
	team2.tricksWon = 0;
	team2.lastTrick = 0;
	
	sts[0] = 0;
	sts[1] = 0;
	sts[2] = 0;
	sts[3] = 0;
	cardSpots[0] = 0;
	cardSpots[1] = 0;
	cardSpots[2] = 0;
	cardSpots[3] = 0;
	cardSpots1[4] = 0;
	
	box1 = 0; box2 = 0;

	mt = 0;
	leadingSuit = 0;

	gpState = 0;
	tpState = 0;
	sp = 0;
	vX = 0;
	vY = 0;
	auxSignNum = 0;

	max = 0; maxi = 0;

	bli = 0; bblink = 0;
	canTrump = 0;
	
	xc = 0;
//char token[2];
	token[0] = 0;
	token[1] = 0;
	beloteState = 0;

	sW = 0; sH = 0; 
	joyNum = 0;
	deckPos = 0;
	trumpCard = 0;
	
	trumpSuit = 0;
	take = 0; move = 0;
	tpCount = 0;
	currentlyPlaying = 0; firstToBid = 0;
}

extern unsigned char x, y;
void arrangeTeams()
{
	for(i = 0; i < 4; i++)
	{
		switch(i)
		{
			case 0:
				x = 15; y = 8;
				break;
			case 1:
				x = 19; y = 12;
				break;
			case 2:
				x = 15; y = 14;
				break;
			case 3:
				x = 10; y = 12;
				break;		
		}
		switch(sts[i])
		{
			case 1:
				printMessage(x, y, "1p");
				break;
			case 2:
				printMessage(x, y, "2p");
				break;
			case 3:
				printMessage(x, y, "3p");
				break;
			case 4:	
				printMessage(x, y, "4p");
				break;
			default:
				printMessage(x, y, "cpu");
				break;
		}
	}
	i = 0;
	x = 0; 
	y = 0;
}

void beloteAchievments()
{
	if(!(trumpCard >> 4))
	{
		++cloverWins;
		if(cloverWins > 10)
		{
			cloverWins = 10;
		}
		
	}
	else if((trumpCard >> 4) == 1)
	{
		++heartWins;
		if(heartWins > 10)
		{
			heartWins = 10;
		}
	}
	else if((trumpCard >> 4) == 2)
	{
		++diamondWins;
		if(diamondWins > 10)
		{
			diamondWins = 10;
		}
	}
	else if((trumpCard >> 4)== 3)
	{
		++spadeWins;
		if(spadeWins > 10)
		{
			spadeWins = 10;
		}
	}
	suitWins = spadeWins + diamondWins + heartWins + cloverWins;
	++conBelWins;
	if(conBelWins > curConBelWins)
	{
		curConBelWins = conBelWins;
		if(curConBelWins > 10)
		{
			curConBelWins = 10;
		}
	}
	savesram();
}
// 182, palette 3, diff 81
void belotePause()
{
	unsigned char ii, x, y;
	unsigned char rQ = 0;
	music_pause(1);
	ppu_wait_nmi();
	// upper line
	vram_adr((0x2C00 | ((8 << 5) | 10)));
	for(ii = 0; ii < 10 ; ii++)
	{
		vram_put(182 | 0x2C00);
	}
	// bottom line
	vram_adr((0x2C00 | ((14 << 5) | 10)));
	for(ii = 0; ii < 10 ; ii++)
	{
		vram_put(182 | 0xAC00);
	}
	
	y = 9;
	vram_adr((0x2C00 | ((8 << 5) | 9)));
	for(ii = 0; ii < 5 ; ii++)
	{
		vram_adr((0x2C00 | ((y << 5) | 9)));
		
		vram_put(183 | 0x2C00);
		vram_put(184 | 0x2C00);
		
		vram_adr((0x2C00 | ((y << 5) | 18)));
		vram_put(184 | 0x2C00);
		vram_put(184 | 0x2C00);
		vram_put(183 | 0x6C00);
		
		++y;
		
	}
	
	vram_adr((0x2C00 | ((10 << 5) | 11)));
	vram_put(185 | 0x2C00); // R
	vram_put(186 | 0x2C00); // e
	vram_put(187 | 0x2C00); // s
	vram_put(188 | 0x2C00); // u
	vram_put(189 | 0x2C00); // m
	vram_put(186 | 0x2C00); // e
	vram_put(184 | 0x2C00);
	vram_put(190 | 0x2C00);
	vram_put(191 | 0x2C00);
	
	vram_adr((0x2C00 | ((12 << 5) | 11)));
	vram_put(192 | 0x2C00); // Q
	vram_put(188 | 0x2C00); // u
	vram_put(193 | 0x2C00); // i
	vram_put(194 | 0x2C00); // t
	vram_put(184 | 0x2C00); // blank
	vram_put(184 | 0x2C00); // blank
	vram_put(184 | 0x2C00); // blank
	
	y = 9;
	x = 11;
	
	ppu_wait_nmi();
	for(ii = 0; ii < 7; ii++)
	{
		vram_adr((0x2C00 | ((y << 5) | (x + ii))));	
		vram_put(184 | 0x2C00); // blank
		vram_adr((0x2C00 | (((y+2) << 5) | (x + ii))));	
		vram_put(184 | 0x2C00); // blank
		vram_adr((0x2C00 | (((y+4) << 5) | (x + ii))));	
		vram_put(184 | 0x2C00); // blank
	}
	
	vram_adr((0x2C00 | ((8 << 5) | 9)));
	vram_put(181 | 0x2C00);
	vram_adr((0x2C00 | ((8 << 5) | 20)));
	vram_put(181 | 0x6C00);
	vram_adr((0x2C00 | ((14 << 5) | 9)));
	vram_put(181 | 0xAC00);
	vram_adr((0x2C00 | ((14 << 5) | 20)));
	vram_put(181 | 0xEC00);
	
	

	while(!(pad_trigger(0) & PAD_START))
	{
		if(pad_trigger(0) & PAD_UP)
		{
			vram_adr((0x2C00 | ((12 << 5) | 18)));
			vram_put(184 | 0x2C00);
			vram_put(184 | 0x2C00);
			
			vram_adr((0x2C00 | ((10 << 5) | 18)));
			vram_put( 190 | 0x2C00);
			vram_put( 191 | 0x2C00);
			rQ = 0;
		}
		else if(pad_trigger(0) & PAD_DOWN)
		{
			vram_adr((0x2C00 | ((10 << 5) | 18)));
			vram_put(184 | 0x2C00);
			vram_put(184 | 0x2C00);
			
			vram_adr((0x2C00 | ((12 << 5) | 18)));
			vram_put( 190 | 0x2C00);
			vram_put( 191 | 0x2C00);
			rQ = 1;
		}
	}
	if(rQ)
	{
	resetBelote();
	winLoseState = 0;
	screenState = 5;
	fadeout();	
	ppu_off();
	asm("lda #%b", 0b00110001);
	asm("sta $210B");
	//
	//lda #3
	//sta OBSEL 2101 reset sprite tilesets addresses
	asm("lda #3");
	asm("sta $2101");
	//
	// reset NTADDR
	asm("lda #0");
	asm("sta $2107");
	//
	asm("lda #5");
	asm("sta $210C");
	//lda #%00000100
	//sta $2108
	asm("lda #%b", 0b00000100);
	asm("sta $2108");
	// 2109 for bg3 map, start at $5400
	asm("lda #%b", 0b01010100);
	asm("sta $2109");
	
	}
	else
	{
		vram_adr((0x2C00 | ((8 << 5) | 9)));
		vram_fill(114, 430);
		music_pause(0);
	//	vram_adr((0x2C00 | ((12 << 5) | 9)));
	//	vram_fill(114, 24);
	}
}

static void myDelay(unsigned char d)
{
	for(i = 0; i < d; i++)
	{
		if(pad_trigger(0) & PAD_START)
		{
	//		belotePause();
		}
		ppu_wait_nmi();
	}
	i = 0;
}

void checkBeRebe()
{
	team1.bonus = 0;
	team2.bonus = 0;
	for(i = 0; i < 4; i++)
	{
		beRebeCon = 0;
		for(j = 0; j < 8; j++)
		{
			if((plys[i].cards[j] & 0xF0) == (trumpCard & 0xF0))
			{
				
				if((plys[i].cards[j] & 0x0F) >= 11)
				{
					++beRebeCon;
					if(beRebeCon == 1)
					{
						rebeloteCards[0] = plys[i].cards[j];
			
					}
					else
					{
						if(i == 0 || i == 2)
						{
							team1.bonus = 20;
							if(team2.trump)
							{
								margin = 92;
							}
						}
						else
						{
							team2.bonus = 20;
							if(team1.trump)
							{
								margin = 92;
							}
						}
						rebeloteCards[1] = plys[i].cards[j];
						return;
					}
				}
			}
		}
	}
	// FIND BELOTE TEAM, if NOT TRUMP TEAM INCREASE MARGIN TO 92
	// INITIALIZE BONUS TO THE TEAM STRUCT
	
	if(beRebeCon == 2)
	{
		printMessage(10, 10, "Found");
		while(1);
	}
	i = 0; j = 0;
}

void declareBeRebe()
{
	if(beRebeCon > 1 && (signNum == rebeloteCards[0]  || signNum == rebeloteCards[1]))
					{
						if(beRebeCon == 2)
						{
							printMessage(3, 19, "Belote");
							++beRebeCon;
						}
						else
						{
							printMessage(3, 19, "Rebelote");
						}
					}
}
#pragma code-name(pop)
