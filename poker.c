#include "neslib.h"
#include "include/drawing_small_cards.h"
#include "include/defines.h"
#include "snesgss.h"
#include "include/shuffle.h"
#include "include/movement.h"
#include "include/joypad.h"

#define LARGE_BET   11
#define MEDIUM_BET  10
#define SMALL_BET   9
#define FOLD        8

#define X_START 112
#define Y_START 16

#define PLAYER1_1_X 24
#define PLAYER1_1_Y 16
#define PLAYER1_2_X 56
#define PLAYER1_2_Y 16

#define PLAYER2_1_X 168
#define PLAYER2_1_Y 16
#define PLAYER2_2_X 200
#define PLAYER2_2_Y 16

#define PLAYER3_1_X 168
#define PLAYER3_1_Y 144
#define PLAYER3_2_X 200
#define PLAYER3_2_Y 144

#define PLAYER4_1_X 24
#define PLAYER4_1_Y 144
#define PLAYER4_2_X 56
#define PLAYER4_2_Y 144

extern unsigned long winings;

unsigned char delete1;
extern unsigned long pokerCash;

extern unsigned long alreadyIn;
extern unsigned long aiBet;

extern unsigned char alowSprite, spX, spY, cardArived, tileUpdate, currentInput, playersJoypad;
extern unsigned char deckSize;
unsigned char playState;
unsigned char selState;
unsigned char selPos;
unsigned char possibilities;
unsigned long currentBet;
unsigned char bg3X;
unsigned char bg3Y;
unsigned long minBetRaise;
unsigned char stepWidth;
unsigned char maxSteps;
unsigned char restRaise;
unsigned char stepCount;
unsigned long num1, num2;
unsigned long potentialBet;
unsigned long toCall;
unsigned char dealStates;
unsigned char sB;
unsigned char forbidRaise;
unsigned char communityCards[5];
unsigned char cantRaise;
unsigned char whoPlaysGuard;
unsigned char flopFinished;
unsigned char riverFinished;
unsigned char inPlay;

unsigned char uu;

unsigned char dealingRound;
unsigned char xend, yend;
unsigned char saveNP, saveLP;

extern unsigned char combinationSize;
unsigned char communitySize;

#define SEL_ANIMATION_TIME 14	

unsigned long int all;

struct player{
	
	unsigned long int cash;
	unsigned long int notCalling; // used for reraises, player can't call his own raise
	unsigned long int inThePot;
	unsigned char joyPadNum;
	unsigned char combination[5];
	unsigned char combinationStrength;
	unsigned char pocket[2];
	union
	{
		unsigned char status;
		struct
		{
			unsigned char atTheSeet : 1;
			unsigned char inPlay    : 1;
			unsigned char folded    : 1;
		}flags;
		
		
	}flags;
};

typedef struct player Player;

Player players[4];

unsigned char button;
unsigned char lastPlayed;
unsigned char lastToPlay;
unsigned char firstToPlay;
unsigned char nextToPlay;

unsigned char inTheHand;
unsigned char atTheTable;
unsigned char stageEnded;
unsigned int raise;
unsigned int bigBlind, buyIn;

extern unsigned char cSelX, cSelY;

unsigned char selTile;
unsigned char selTile1;
unsigned char selTile2;

#define PREFLOP  0
#define FLOP     1
#define TURN     2
#define RIVER    3
#define SHOWDOWN 8

#define PREFLOP_SUB_0  0
#define PREFLOP_SUB_1  1
#define FLOP_SUB_0     2
#define FLOP_SUB_1     3
#define TURN_SUB_0     4
#define TURN_SUB_1     5
#define RIVER_SUB_0    6
#define RIVER_SUB_1;   7


unsigned char pokState;
unsigned char subState;
unsigned char initPoker;
unsigned char universalPokerStates;


extern unsigned char vramX, vramY, signNum, i, j, k, availableSprid, spriteX, spriteY; 
extern unsigned char cardsDeck[52];
extern unsigned char fix[15];
#pragma wrapped-call(push, trampoline, 0x86)

void initializePoker();
void playerPlays();
void drawField();
void drawSlider();
void moveSlider();
void dealCards();
void dealOneCard();
void coloredSelector1();
void drawSlider();
void handPlay();
void whoPlays();
void openCards();
void drawFields();
void lowerSelector();
void upperSelector();
void sliderBar();
void displayNumber();
void buttonBbSb();
void closeCards();
void whoPlaysLast();
void checkCallFold();
void pressStart();
void displayInThePot();
void clearFields();
void clearSlider();
void clearCards();
void dealFlop();
void dealTurnRiver();
void openAllCards();
void clearBG3();
void calculateXYend();
void fixCards();
void findMaxSteps();
void checkAllStrengths();


// combination check functions
void checkOfAkind();
void checkCombination();
void preflopStrength();

// showdown functions
void showdown();
void compareHands();
void findHighCard();

// ai functions

void aiRaise();
void initDecisionParams();

#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x90)
void aiDecision();
#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x94)
// side pot
void solveSidePots();
//if everybody folded
void payWinnings();
// hand end
void prepareNextHand();
// showdown box
void showBox();
void boxNumber();
void boxMessage(char *message);
void clearBox();
void boxStart();
void showButton();
#pragma wrapped-call(pop)

// SHOWDOWN VARIABLES
unsigned char withMoney[4];
unsigned char withMoneyNum;

unsigned char withoutMoney[4];
unsigned char withoutMoneyNum;

unsigned long currentPot;

unsigned char sf1, sf2, f1, f2;

unsigned char draw1, draw2, draw3;

unsigned char toCompare[4];
unsigned char toCompareSize;

unsigned char index1, index2;
// SHOWDOWN VARIABLES END

extern unsigned char combination1[5];
extern unsigned char combination[7];
extern unsigned char combinationStrength;

unsigned char stage;

// Ai player variables
extern unsigned char decision;
extern unsigned char sng;
unsigned char aiTime;

extern unsigned char aiCommunity[5];
extern unsigned char aiCommSize;

extern unsigned char aiPocket[2];

extern unsigned char seats[4];

unsigned char winner;

extern unsigned char winLoseState, screenState;

#pragma wrapped-call(push, trampoline, 0x85)
void displayMessage(char *message);
#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x91)
void resetPoker();
void pokerPause();
void myDelay(unsigned char d);
void playChips();
void playCall();
void playAiChips();
void playAllIn();
#pragma wrapped-call(pop)

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

#define printBoxNumber(a, x, y) {\
								num1 = a; bg3X = x; bg3Y = y;\
								boxNumber();\
							}\
							
#define printBoxMessage(x, y, mess){\
									vram_adr((0x2C00 | ((y << 5) | x)));\
									boxMessage(mess);\
								}\
								
#define checkStrengths(combSize, comuSize){\
									combinationSize = combSize;\
									communitySize   = comuSize;\
									checkAllStrengths();\
								}\
																
#pragma code-name(push, "CODE5")

void initializePoker()
{
	
	for(i = 0; i < 4; i++)
	{	
		if(players[i].flags.flags.atTheSeet)
		{
			players[i].flags.status = 0x03;
		}
	}
	i = 0;
	dealStates = 0;
	currentInput = 10;    // just to be different from last input
//	bigBlind = 100;
	inPlay = atTheTable;
	currentBet = bigBlind;
	minBetRaise = bigBlind;	
	deckSize = 52;
	selTile = 107;
	selTile1 = 99;
	selTile2 = 103;
	inTheHand = atTheTable; //
	stageEnded = 0;
//	button = 2;
//	nextToPlay = button + 2;
	
//	players[1].flags.flags.atTheSeet = 0;
//	players[2].flags.flags.atTheSeet = 0;
	buttonBbSb();
	whoPlaysLast();
	
	players[0].joyPadNum = seats[0] - 2;
	players[1].joyPadNum = seats[1] - 2;
	players[2].joyPadNum = seats[2] - 2;
	players[3].joyPadNum = seats[3] - 2;
	
}

void buttonBbSb()
{
	while(!(players[button].flags.flags.atTheSeet))
	{
		++button;
		if(button == 4)
		{
			button = 0;
		}
	}
	lastPlayed = button + 1;
//	printNumber(button, 20, 25);
	// ADD 1 so player nums start at 1
//	vram_adr((0x2C00 | ((1 << 5) | 29)));
//	displayNumber(button + 1);
	
	if(atTheTable == 2)
	{
		sB = button;
		nextToPlay = button;
		if(players[button].cash > (bigBlind >> 1))
		{
			players[button].cash -= bigBlind >> 1;
			players[button].inThePot = bigBlind >> 1;
			players[button].notCalling = players[button].inThePot;
		}
		else
		{
			players[button].inThePot = players[button].cash;
			players[button].cash = 0;
			players[button].flags.flags.inPlay = 0;
			--inPlay;
			
		}
		displayInThePot();
	//	lastPlayed = button + 1;
		whoPlays();
		
//		vram_adr((0x2C00 | ((3 << 5) | 29)));
//		displayNumber(nextToPlay + 1);
		if(players[nextToPlay].cash >= bigBlind)
		{
			players[nextToPlay].cash -= bigBlind;
			players[nextToPlay].inThePot = bigBlind;
			players[nextToPlay].notCalling = bigBlind;
		}
		else
		{
			players[nextToPlay].inThePot = players[nextToPlay].cash;
			players[nextToPlay].cash = 0;
			players[nextToPlay].flags.flags.inPlay = 0;
			--inPlay;
			
		}
		displayInThePot();
		
		nextToPlay = button;
		lastPlayed = button+1;
		return;
	}
	// We have the button
	// find the SB
	whoPlays();
//	vram_adr((0x2C00 | ((2 << 5) | 29)));
//	displayMessage("sB");
//	displayNumber(nextToPlay + 1);
	sB = nextToPlay;
	if(players[nextToPlay].cash > (bigBlind >> 1))
		{
			players[nextToPlay].cash -= (bigBlind >> 1);
			players[nextToPlay].inThePot = (bigBlind >> 1);
			players[nextToPlay].notCalling = players[nextToPlay].inThePot;
		}
		else
		{
			players[nextToPlay].inThePot = players[nextToPlay].cash;
			players[nextToPlay].cash = 0;
			players[nextToPlay].flags.flags.inPlay = 0;
			--inPlay;
			
		}
		displayInThePot();
	// take sB, update the player
	// Now find the bB
	whoPlays();
//	vram_adr((0x2C00 | ((3 << 5) | 29)));
//	displayMessage("bB");
//	displayNumber(nextToPlay + 1);
	if(players[nextToPlay].cash > bigBlind)
		{
			players[nextToPlay].cash -= bigBlind;
			players[nextToPlay].inThePot = bigBlind;
			players[nextToPlay].notCalling = bigBlind;
		}
		else
		{
			players[nextToPlay].inThePot = players[nextToPlay].cash;
			players[nextToPlay].cash = 0;
			players[nextToPlay].flags.flags.inPlay = 0;
			--inPlay;
			
		}
		displayInThePot();
		whoPlays();

	
//	lastToPlay = whoPlaysLast();
}

extern char music2[];

void poker()
{	
	vramX = 0;
	vramY = 0;
	if(pad_trigger(0) & PAD_START)
	{
		pokerPause();
		if(screenState == 5)
		{
			return;
		}
	}
	if(universalPokerStates < 5)
	{
		switch(universalPokerStates)
		{
			case(0):
				stage = 0;
				initializePoker();
				saveNP = nextToPlay;
				saveLP = lastToPlay;
	
				nextToPlay = sB;
				lastPlayed = sB+1;
				whoPlaysLast();
				
			//	fixCards();
			//	showButton();
				++universalPokerStates;
				break;
			case(1):
	
					// if next to play at the table
					// deal first one card to player next to play
					// pack changeCardPoss and deal one card into one function
					// calculate xend yend possitions based on the player index(0 1 2 3) and dealing round (0 1)
					// if the player last to play card dealt, increment dealing round, if dealing round == 2 dealing is finished
					
					// next to play -> deal one card, if it is also last player to play increment dealing round, if dealing round == 2 dealing finished
					// until dealing round < 2, deal one card next to play, check if last to play also
					// use next to deal and last to deal if necessary instead of next to play and last to play
					if(dealingRound < 2)
					{
						
						//	calculate x start and y end, deal one card, card arived, check if it was last to deal and update dealing round if necessary
						if(players[nextToPlay].flags.flags.atTheSeet)
						{
							
							
							players[nextToPlay].pocket[dealingRound] = cardsDeck[deckSize-1];
							calculateXYend();
							changeCardPoss(X_START, xend, Y_START, yend);
							// when this one finishes new next to play is calculated and last to play is checked
							// if last to play == next to play dealing round increments
							dealOneCard();
							
						}
					}
					else
					{
					//	openAllCards();
						// calculate preflop strengths
						for(uu = 0; uu < 4; uu++)
						{
							combination[0] = players[uu].pocket[0];
							combination[1] = players[uu].pocket[1];
							preflopStrength();
							players[uu].combinationStrength = combinationStrength;
						//	printMessage(25, 10 + uu, "Sng");
						//	printNumber(players[uu].combinationStrength, 28, 10 + uu);
						//	printNumber(uu, 25, 10 + i);
						}
						uu = 0;
						universalPokerStates = 9;
						nextToPlay = saveNP;
						lastToPlay = saveLP;
						lastPlayed = nextToPlay + 1;
					}
			//	printNumber(playState, 10, 10);
					break;
			}
	}
	else if(universalPokerStates == 9)
	{
		handPlay();
	}
	else if(universalPokerStates == 10)
	{
		music_play(0x88, music2);
		for(i = 0; i < 4; i++)
		{
			players[i].flags.status = 0x03;
			players[i].cash = pokerCash;
		}
		players[0].cash = buyIn;
		players[1].cash = buyIn;
		players[2].cash = buyIn;
		players[3].cash = buyIn;
	/*	players[0].cash = 50;
		players[1].cash = 50;
		players[2].cash = 50;
		players[3].cash = 50;*/
		atTheTable = 4;
		universalPokerStates = 0;
	}
	else if(universalPokerStates == 11)
	{
		// reset all variables
		// go to end screen
		resetPoker();
	//	winLoseState = 1;
		screenState = 5;
	//	printMessage(10, 10, "The winner is");
	//	printNumber(winner, 10, 11);
	}
	
	
//	coloredSelector1();
}

void whoPlays()
{
	
	if(lastPlayed == 4)
	{
		nextToPlay = 0;
	}
	else
	{
		nextToPlay = lastPlayed;      // because lastToPlay start at 1 and nextToPlay start at 0
	}
	whoPlaysGuard = 0;
	while((players[nextToPlay].flags.status & 0x07) != 3)
	{
	//	printf("uslo");
		if(nextToPlay == 3)
		{
			nextToPlay = 0;
		}
		else
		{
			++nextToPlay;
		}
		++whoPlaysGuard;
		if(whoPlaysGuard == 4)
		{
			
			break;
			
		}
	}
	
//	printf("who plays part aux %d lastPlayed %d\n", aux, lastPlayed);
	lastPlayed = nextToPlay + 1;
	
}

void whoPlaysLast()
{
	if(lastPlayed == 1)
	{
		lastToPlay = 3;	
	}
	else
	{
		lastToPlay = lastPlayed - 2;
	}
	while((players[lastToPlay].flags.status & 0x07) != 3)
	{
		if(lastToPlay == 0)
		{
			lastToPlay = 3;
		}
		else
		{
			--lastToPlay;
		}
	}
	
}

void handPlay()
{
	
	if(!playState)
	{	
		ppu_wait_nmi();
		switch(nextToPlay)
		{
			case(0):
				vram_adr((0x2C00 | ((4 << 5) | 1)));
				bg3X = 13;
				bg3Y = 4;
				break;
			case(1):
				vram_adr((0x2C00 | ((4 << 5) | 19)));
				bg3X = 31;
				bg3Y = 4;
				break;
			case(2):
				vram_adr((0x2C00 | ((20 << 5) | 19)));
				bg3X = 31;
				bg3Y = 20;
				break;
			case(3):
				vram_adr((0x2C00 | ((20 << 5) | 1)));
				bg3X = 13;
				bg3Y = 20;
				break;	
			
		}
		//displayMessage("Press Start ");
		toCall = currentBet - players[nextToPlay].inThePot;

		if(players[nextToPlay].inThePot < currentBet)
		{
			possibilities = 1;
		}
		else
		{
			possibilities = 0;
		}
		
		if(players[nextToPlay].cash <= toCall)
		{
			cantRaise = 1;
		}
		else
		{
			cantRaise = 0;
		}
/*		if((nextToPlay == lastToPlay) && forbidRaise)
		{
			cantRaise = 1;
		}
*/		
		if(inPlay == 1)
		{
			cantRaise = 1;
		//	printMessage(10, 10, "ONLY ONE IN PLAY");
		//	printNumber(cantRaise+1, 10, 11);
		}
		if(players[nextToPlay].joyPadNum <= 3)
		{
			pressStart();
			if(pad_trigger(players[nextToPlay].joyPadNum) & PAD_START)
			{
				ppu_wait_nmi();
				displayMessage("            ");
				++playState;
			}
		}
		else
		{
			++playState;
		}
	}
	if(playState == 1 && players[nextToPlay].joyPadNum < 4)
	{
		bg3X -= 12;
		vram_adr((0x2C00 | ((bg3Y << 5) | bg3X)));
		displayMessage("            ");

		openCards();
		drawFields();
		
		
		if(possibilities)
		{
			potentialBet = toCall + minBetRaise;
			vram_adr((0x2C00 | ((26 << 5) | 6)));
			displayMessage("CALL");
	
			vram_adr((0x2C00 | ((26 << 5) | 13)));
			if((players[nextToPlay].cash > potentialBet) && (cantRaise != 1))  // cantRaise was inPlay
			displayMessage("RAISE");
			else if((players[nextToPlay].cash > toCall) && (cantRaise != 1))
			displayMessage("ALLIN");
			else
			{
				// clear middle field;
				ppu_wait_nmi();
				vram_adr(0x2400 | ((25 << 5) | 13));
				vram_fill(4, 12);
				vram_adr(0x2400 | ((26 << 5) | 13));
				vram_fill(4, 12);
				// set condition for the colored selector
			}
		}
		else
		{
			potentialBet = toCall + bigBlind;	
			vram_adr((0x2C00 | ((26 << 5) | 6)));
			displayMessage("CHECK");
	
			vram_adr((0x2C00 | ((26 << 5) | 13)));
			if(players[nextToPlay].cash > potentialBet)
			displayMessage("BET");
			else if(players[nextToPlay].cash > toCall)
			displayMessage("ALLIN");
			else
			{
				
			}
		//	displayMessage("BET");
			
		}
		vram_adr((0x2C00 | ((26 << 5) | 20)));
		displayMessage("FOLD");
			
		cSelX = 48;
		cSelY = 200;
		++playState;
	}
	else if(players[nextToPlay].joyPadNum >= 4 && playState == 1)
	{
		++playState;
	}
	if(playState == 2)
	{
		if(!selState)
		{
			lowerSelector();
		}
		else
		{
			upperSelector();
		}
/*		bg3X = 2;
	bg3Y = 25;
	num1 = inPlay;
	displayNumber();*/
	}
	else if(playState == 3)
	{
	//	dealStates = 1;
//	ppu_wait_nmi();
//		displayMessage("GUARD");
		dealFlop();
		
		//This is deal three cards begining of the flop stage
		// new stage is to begin, calculate np and lp
		// switch playState to repeat preflop play
	}
	else if(playState == 4)
	{
		dealTurnRiver();
	}
	else if(playState == 5)
	{
	
		if(dealStates < 4)
		{
		
			dealFlop();
			
			playState = 5;
			
			
		}
		else if(dealStates < 8)
		{
		
			dealTurnRiver();
			playState = 5;
			
		}
		else
		{
			clearBG3();
			openAllCards();
			
			for(uu=0; uu<4; uu++)
			{
			//	if(players[uu].flags.flags.folded)
			//	continue;
				combination[0] = players[uu].pocket[0];
				combination[1] = players[uu].pocket[1];
				combination[2] = communityCards[0];
				combination[3] = communityCards[1];
				combination[4] = communityCards[2];
				combination[5] = communityCards[3];
				combination[6] = communityCards[4];
				combinationSize = 7;
				communitySize = 5;
				checkCombination();
				players[uu].combinationStrength = combinationStrength;
				for(i = 0; i < 5; i++)
				{
					players[uu].combination[i] = combination1[i];
				}
				i = 0;
		/*		for(delete1 = 0; delete1 < 5; delete1++)
				{
					if(((players[uu].combination[delete1] & 0x0f) == 0) || ((players[uu].combination[delete1] & 0x0f)  == 13))
					{
						printMessage(11+delete1, 19+uu, "A");
					}
					else if((players[uu].combination[delete1] & 0x0f) == 12)
					{
						printMessage(11+delete1, 19+uu, "K");
					}
					else if((players[uu].combination[delete1] & 0x0f) == 11)
					{
						printMessage(11+delete1, 19+uu, "Q");
					}
					else if((players[uu].combination[delete1] & 0x0f) == 10)
					{
						printMessage(11+delete1, 19+uu, "J");
					}
					else if((players[uu].combination[delete1] & 0x0f) == 9)
					{
						printMessage(11+delete1, 19+uu, "T");
					}
					else
					{
						printNumber((players[uu].combination[delete1] & 0x0f)+1, 11+delete1, 19+uu);
					}
				}
		
		*/	
		//	printMessage(0, 12 + uu, "Sng");
		//	printNumber(players[uu].combinationStrength, 3, 12+uu);	
			
			}	
		//	printNumber(combination[uu], 5, 5 + uu);
		//	printMessage(10, 10, "SHOWDOWN FINISH");
			
		showdown();	
		
	//	playState = 6;
	
		}
		
	}
	else if(playState == 6)
	{
		printMessage(10, 10, "STATE 6");
	}
	
	// When START HAS BEEN PRESSED MOVE to the next preflop state
	// open player cards, calcute vram addr and draw static cards
	
	// calculate possible options then draw fields and selector
	// depending on where the selector is call upperSelector and lowerSelector functions
	
	// if B pressed, confirm choice update player reset preflop state or exit preflop
	
	
	
	
	// player to play 
	// player or AI makes the play
	// check if it was the last player to play
	// if it was last and didnt raise finish preflop stage
	// calculate who plays next
	// calculate who plays last
	// close the cards
}

void lowerSelector()
{
	// if Ai plays delay short period of time, then change selPos and selector coordinates based on Ai decision, make another even shorter delay
	// then if Ai decision is not to raise call checkCallFold, otherwise call AiRaise function
	if(players[nextToPlay].joyPadNum >= 4)
	{
			sng = players[nextToPlay].combinationStrength;
			myDelay(50);			
			aiTime = 0;
			playState = 0;
		//	closeCards();
			alreadyIn = players[nextToPlay].inThePot;
			initDecisionParams();
			aiDecision();
		//	decision = 2;
		//	selPos = decision;
		//	decision = 1;
		//	aiBet = 2000;
			if(decision == 1 && !cantRaise)
			{
				aiRaise();
			//	playChips();
			
			}
			else
			{
				// ai wants to raise but it can't so make decision call
				if(decision == 1)
				{
					decision = 0;
				}
				selPos = decision;
				checkCallFold();
				oam_clear();
				if(nextToPlay == lastToPlay && stage == 3)
				{
					playState = 5;
				}
				if(inTheHand == 1)
				{
					playState = 0;
				}
				displayInThePot();
				whoPlays();
			}
			
		
			return;
	}
	
	else if(pad_trigger(players[nextToPlay].joyPadNum) & PAD_RIGHT)
	{
		if(selPos == 2)
		{
			selPos = 0;
			cSelX = 48;
			cSelY = 200;
		}
		else
		{
			++selPos;
			cSelX += 56;
			if(cantRaise)
			{	
				++selPos;
				cSelX += 56;
			}
		}
		
		
	}
	else if(pad_trigger(players[nextToPlay].joyPadNum) & PAD_LEFT)
	{
		if(selPos == 0)
		{
			selPos = 2;
			cSelX = 160;
			cSelY = 200;
		}
		else
		{
			--selPos;
			cSelX -= 56;
			if(cantRaise)
			{
				--selPos;
				cSelX -= 56;
			}
		}
		
	}
	else if(pad_trigger(players[nextToPlay].joyPadNum) & PAD_B)
	{
		
		if(selPos == 1)
		{
			
			if(players[nextToPlay].cash > potentialBet)
			{
					forbidRaise = 0;
					oam_clear();
					selState = 1;
					cSelX = 90;
					cSelY = 180;
					vramX = 11;
					vramY = 22;
					drawSlider();
				
					num1 = potentialBet;
					printNumber(potentialBet, 11, 22);
				
				// maxSteps should be equal to second highest cash to avoid side pot in which only chip leader gets his extra chips
				// find other players cashes and find the highest one, then check if that value is greater than current players(nestToPlay) cash
			//	findMaxSteps();
					maxSteps = (players[nextToPlay].cash - potentialBet)/bigBlind;
					stepWidth = 73/maxSteps;
					stepCount = 0;
					playersJoypad = players[nextToPlay].joyPadNum;
			//	}
			}
			else if(players[nextToPlay].cash > toCall)
			{
				all = players[nextToPlay].cash;
				playAllIn();
				forbidRaise = 1;
				bg3X = 11;
				bg3Y = 20;
				ppu_wait_nmi();
				players[nextToPlay].inThePot += players[nextToPlay].cash;
				currentBet += players[nextToPlay].cash - toCall;
				players[nextToPlay].cash = 0;
				players[nextToPlay].flags.flags.inPlay = 0;
				--inPlay;
			
		
				selPos = 0;
				playState = 0;
				displayInThePot();
				closeCards();
				clearFields();
				whoPlaysLast();
				whoPlays();
				
			//	displayMessage("ALL IN");
			}
			
		}
		else
		{
			playState = 0;
			closeCards();
			checkCallFold();
			displayInThePot();
			clearFields();
			if(nextToPlay == lastToPlay && stage == 3)
			{
				playState = 5;
			}
			if(inTheHand == 1)
			{
				playState = 0;
			}
			whoPlays();
		}	
		return;
	}
/*	else if(pad_trigger(players[nextToPlay].joyPadNum) & PAD_UP)
	{
		oam_clear();
		selState = 1;
		cSelX = 106;
		cSelY = 184;
		playersJoypad = players[nextToPlay].joyPadNum;
		return;
	}
*/	
	coloredSelector1();
}


void upperSelector()
{
	if(pad_trigger(players[nextToPlay].joyPadNum) & PAD_DOWN)
	{
		oam_clear();
		selState = 0;
		selPos = 0;
		cSelX = 48;
		cSelY = 200;
		potentialBet = minBetRaise + bigBlind;
		clearSlider();
		return;
	}
	// divide cash with minBetRaise calculate max possible number of steps
	// divide 78px by max number of steps and calculate step width
	
	scanJoypad();

	if(players[nextToPlay].cash >= potentialBet)
	{
	
		if(stepCount < maxSteps || stepCount == 255)
		{
			if(currentInput == 2)
			{
				if(stepCount == 255)
				stepCount = 0;
				cSelX += stepWidth;
				++stepCount;

				bg3X = 11;
				bg3Y = 22;
	
				num1 = players[nextToPlay].cash;
			
				potentialBet += bigBlind;
				printNumber(potentialBet, 11, 22);
		
			}
		}
		else if((players[nextToPlay].cash - potentialBet) && stepCount == maxSteps)
		{
			if(currentInput == 2)
			{
				cSelX = 163;
				restRaise = players[nextToPlay].cash - potentialBet;
				potentialBet += players[nextToPlay].cash - potentialBet;
				printNumber(potentialBet, 11, 22);
				++stepCount;
				
			}
		}
		if(stepCount > 0 && stepCount != 255)
		{	
			if(currentInput == 1)
			{
				if(stepCount == maxSteps + 1)
				{
					potentialBet -= restRaise;
				}
				else
				{
					potentialBet -= bigBlind;
				}
				cSelX -= stepWidth;
				--stepCount;
				bg3X = 11;
				bg3Y = 22;
		
				
				num1 = potentialBet;
				printNumber(potentialBet, 11, 22);
	
			}
		}
/*		else if((players[nextToPlay].cash - potentialBet > minBetRaise + toCall) && stepCount == 0)
		{
			
			if(currentInput == 1)
			{
				--stepCount;
				cSelX = 90;
				potentialBet = minBetRaise + toCall;
				printNumber(potentialBet, 10, 22);
			}
		}
*/		
	}
	if(pad_trigger(players[nextToPlay].joyPadNum) & PAD_B)
	{
		playChips();
		selState = 0;

		players[nextToPlay].cash -= potentialBet;
		players[nextToPlay].inThePot += potentialBet;
		if(players[nextToPlay].cash == 0)
		{
			--inPlay;
			players[nextToPlay].flags.flags.inPlay = 0;
		}
		
		minBetRaise = potentialBet - toCall;
		currentBet += minBetRaise;
		players[nextToPlay].notCalling = minBetRaise;
		whoPlaysLast();

	//	printNumber(lastToPlay,20, 11);
		
		selPos = 0;
		playState = 0;
		displayInThePot();
		closeCards();
		printMessage(10, 22, "          ");
		
		whoPlays();

	//	printNumber(lastToPlay,20, 13);
		clearFields();
		clearSlider();
		possibilities = 1;
		return;
	}
	sliderBar();
	
}

void checkCallFold()
{
	if(selPos == 2)
	{
		players[nextToPlay].flags.flags.folded = 1;
		players[nextToPlay].flags.flags.inPlay = 0;
		--inTheHand;
		--inPlay;
		ppu_wait_nmi();
		clearCards();
		clearFields();
		sfx_play(FOLD);
	//	possibilities = 0;
	}
	if(inTheHand == 1)
	{
		ppu_wait_nmi();
		vram_adr(0x2C00 | ((15 << 5) | 15));
		payWinnings();
		prepareNextHand();
		selPos = 0;
		
		return;
	}
	if(selPos == 0)
	{

		if(possibilities)
		{
			if(players[nextToPlay].cash >= (currentBet - players[nextToPlay].inThePot))
			{
				players[nextToPlay].cash -= (currentBet - players[nextToPlay].inThePot);
				players[nextToPlay].inThePot += (currentBet - players[nextToPlay].inThePot);
				players[nextToPlay].notCalling = 0; // player must call everything in case of reraise
				if(players[nextToPlay].cash == 0)
				{
					players[nextToPlay].flags.flags.inPlay = 0;
					--inPlay;
				}
			}
			else
			{
				players[nextToPlay].inThePot += players[nextToPlay].cash;
				players[nextToPlay].cash = 0;
				players[nextToPlay].flags.flags.inPlay = 0;
				--inPlay;
			}
			playCall();
		}
	}
	if(nextToPlay == lastToPlay)
	{
		if(dealStates == 4 || dealStates == 6)
		{
			playState = 4;
		}
		else
		{
			playState = 3;
			vram_adr(0x2C00 | ((15 << 5) | 15));
		
		}
		++dealStates;
	}
	selPos = 0;
	
	
}

void aiRaise()
{
	//	selState = 0;

		// if ai wants to call but has money only to call (complete call or incomplete call) THIS IS NOT A RAISE
		if(players[nextToPlay].cash <= toCall)
		{
			players[nextToPlay].inThePot += players[nextToPlay].cash;
			players[nextToPlay].cash = 0;
			--inPlay;
			players[nextToPlay].flags.flags.inPlay = 0;
			// who plays next
			printMessage(10, 10, "CANT RAISE, CALL");
			displayInThePot();
			
			if(nextToPlay == lastToPlay)
			{
				if(dealStates == 4 || dealStates == 6)
				{
					playState = 4;
				}
				else
				{
					playState = 3;
					vram_adr(0x2C00 | ((15 << 5) | 15));
		
				}
				++dealStates;
			}
			selPos = 0;
			
			whoPlays();
			while(!(pad_trigger(0) & PAD_START));
		}
		else // THIS IS RAISE
		{
			// check if ai has enough cash to make souch a bet
			if(players[nextToPlay].cash > aiBet)
			{
				players[nextToPlay].inThePot += aiBet;
				players[nextToPlay].cash -= aiBet;
				
				minBetRaise = aiBet - toCall;
				playAiChips();
				
			}
			else // go all in
			{
				minBetRaise = players[nextToPlay].cash - toCall;
				players[nextToPlay].inThePot += players[nextToPlay].cash;
				players[nextToPlay].cash = 0;
				--inPlay;
				players[nextToPlay].flags.flags.inPlay = 0;
				all = players[nextToPlay].cash;
				playAllIn();
				
			//	minBetRaise = minBetRaise - (minBetRaise % bigBlind); complete raise sets new minBetRaise which is not divisable by BB
				
			}
			// find who plays next and who plays last and set possibilities
			currentBet += minBetRaise;
			displayInThePot();
			whoPlaysLast();
			selPos = 0;
			playState = 0;
			whoPlays();
			possibilities = 1;
		//	printNumber(aiBet, 14, 22);
		//	while(!(pad_trigger(0) & PAD_START));
		}
		
	//	displayInThePot();
}


void initDecisionParams()
{
	aiPocket[0] = players[nextToPlay].pocket[0];
	aiPocket[1] = players[nextToPlay].pocket[1];
	if(stage == 1)
	{
		aiCommSize = 3;
	}
	else if(stage == 2)
	{
		aiCommSize = 4;
	}
	else
	{
		aiCommSize = 5;
	}
	for(i = 0; i < aiCommSize; i++)
	{
		aiCommunity[i] = communityCards[i];
	}
	i = 0;
}

void findMaxSteps()
{
	unsigned long maxBet;
	for(i = 0; i < 4; i++)
	{
		if(i != nextToPlay)
		{
			if((players[i].flags.status & 0x07) == 3)
			{
				if((players[i].cash - players[i].notCalling) >= maxBet)
				{
					maxBet = players[i].cash + players[i].notCalling;
				}
				
			}	
		}
	}
	if(maxBet > players[nextToPlay].cash)
	{
		maxBet = players[nextToPlay].cash;
	}
	else if(maxBet == 0 )
	{
		cantRaise = 1;
	}
	else if(maxBet < potentialBet)
	{
		maxBet = potentialBet;
	}
	
	maxSteps = (maxBet - potentialBet)/bigBlind;
}

// Allways set combinationSize before calling this functions
void checkAllStrengths()
{
	for(uu = 0; uu < 4; uu++)
	{
		combination[0] = players[uu].pocket[0];
		combination[1] = players[uu].pocket[1];
		combination[2] = communityCards[0];
		combination[3] = communityCards[1];
		combination[4] = communityCards[2];
		combination[5] = communityCards[3];
		combination[6] = communityCards[4];
		checkCombination();
		players[uu].combinationStrength = combinationStrength;
	//	printNumber(combinationStrength, 1, 10+uu);
	}
	uu = 0;
	i = 0;
	j = 0;
	k = 0;
}

void openCards()
{
	ppu_wait_nmi();

	switch(nextToPlay)
	{
		case(0):
			vramY = 2;
			vramX = 3;
			signNum = players[nextToPlay].pocket[0];
			pokCardDisplay();

			signNum = players[nextToPlay].pocket[1];
			vramY = 2;
			vramX = 7;
			ppu_wait_nmi();
			pokCardDisplay();
			break;
		case(1):
			vramY = 2;
			vramX = 21;
			signNum = players[nextToPlay].pocket[0];
			pokCardDisplay();
			vramY = 2;
			vramX = 25;
			signNum = players[nextToPlay].pocket[1];
			ppu_wait_nmi();
			pokCardDisplay();
			break;
		case(2):
			ppu_wait_nmi();
			vramY = 18;
			vramX = 21;
		//	players[nextToPlay].pocket[0] = cardsDeck[deckSize-1];
			signNum = players[nextToPlay].pocket[0];
			pokCardDisplay();
			ppu_wait_nmi();
			vramY = 18;
			vramX = 25;
			signNum = players[nextToPlay].pocket[1];
			ppu_wait_nmi();
			pokCardDisplay();
			break;
		case(3):
			vramY = 18;
			vramX = 3;
			signNum = players[nextToPlay].pocket[0];
			pokCardDisplay();
			vramY = 18;
			vramX = 7;
			signNum = players[nextToPlay].pocket[1];
			ppu_wait_nmi();
			pokCardDisplay();
			break;		
	}
}

void closeCards()
{
	switch(nextToPlay)
	{
		case(0):
			vramY = 2;
			vramX = 3;
			ppu_wait_nmi();
			pokBa();
	
			vramY = 2;
			vramX = 7;
			ppu_wait_nmi();
			pokBa();
			break;
		case(1):
			vramY = 2;
			vramX = 21;
			ppu_wait_nmi();
			pokBa();
	
			vramY = 2;
			vramX = 25;
			ppu_wait_nmi();
			pokBa();
			break;
		case(2):
			vramY = 18;
			vramX = 21;
			ppu_wait_nmi();
			pokBa();
	
			vramY = 18;
			vramX = 25;
			ppu_wait_nmi();
			pokBa();
			
			break;
		case(3):
			vramY = 18;
			vramX = 3;
			ppu_wait_nmi();
			pokBa();
	
			vramY = 18;
			vramX = 7;
			ppu_wait_nmi();
			pokBa();
			break;			
	}	

}

void playerPlays()
{
	
}

void drawFields()
{
	ppu_wait_nmi();
	vramX = 6;
	vramY = 25;
	drawField();
	
	vramX = 13;
	vramY = 25;
	drawField();
	
	vramX = 20;
	vramY = 25;
	drawField();
}

void drawField()
{
	ppu_wait_nmi();
	vram_adr(BG1_NT(vramX, vramY));
	vram_put(315 | 0b0001000000000000);
	vram_put(316 | 0b0001000000000000);
	vram_put(316 | 0b0001000000000000);
	vram_put(316 | 0b0001000000000000);
	vram_put(316 | 0b0001000000000000);
	vram_put(315 | 0b0101000000000000);
	++vramY;
	vram_adr(BG1_NT(vramX, vramY));
	vram_put(322 | 0b0001000000000000);
	vram_put(323 | 0b0001000000000000);
	vram_put(323 | 0b0001000000000000);
	vram_put(323 | 0b0001000000000000);
	vram_put(323 | 0b0001000000000000);
	vram_put(322 | 0b0101000000000000);
	
}

void drawSlider()
{
	ppu_wait_nmi();
	vram_adr(BG1_NT(vramX, vramY));
	vram_put(317 | 0b0001000000000000);
	vram_put(318 | 0b0001000000000000);
	vram_put(319 | 0b0001000000000000);
	vram_put(320 | 0b0001000000000000);
	vram_put(320 | 0b0001000000000000);
	vram_put(318 | 0b0101000000000000);
	vram_put(318 | 0b0101000000000000);
	vram_put(318 | 0b0101000000000000);
	vram_put(318 | 0b0101000000000000);
	vram_put(321 | 0b0001000000000000);
	++vramY;
	vram_adr(BG1_NT(vramX, vramY));
	vram_put(324 | 0b0001000000000000);
	vram_put(325 | 0b0001000000000000);
	vram_put(325 | 0b0001000000000000);
	vram_put(325 | 0b0001000000000000);
	vram_put(325 | 0b0001000000000000);
	vram_put(325 | 0b0001000000000000);
	vram_put(325 | 0b0001000000000000);
	vram_put(325 | 0b0001000000000000);
	vram_put(325 | 0b0001000000000000);
	vram_put(326 | 0b0001000000000000);
}

void moveSlider()
{
	
}

void dealCards()
{
	
}

void calculateXYend()
{
	switch(nextToPlay)
	{
		case(0):
			if(!dealingRound)
			{
				xend = PLAYER1_1_X;
				yend = PLAYER1_1_Y;
			}
			else
			{
				xend = PLAYER1_2_X;
				yend = PLAYER1_2_Y;
			}
			break;
		case(1):
			if(!dealingRound)
			{
				xend = PLAYER2_1_X;
				yend = PLAYER2_1_Y;
			}
			else
			{
				xend = PLAYER2_2_X;
				yend = PLAYER2_2_Y;
			}
			break;
		case(2):
			if(!dealingRound)
			{
				xend = PLAYER3_1_X;
				yend = PLAYER3_1_Y;
			}
			else
			{
				xend = PLAYER3_2_X;
				yend = PLAYER3_2_Y;
			}
			break;
		case(3):
			if(!dealingRound)
			{
				xend = PLAYER4_1_X;
				yend = PLAYER4_1_Y;
			}
			else
			{
				xend = PLAYER4_2_X;
				yend = PLAYER4_2_Y;
			}
			break;			
	}
}

void dealOneCard()
{
	if(alowSprite)
				{
					spriteX = spX;
					spriteY = spY;
					
					ppu_wait_nmi();
					if(dealStates)
					{
						signNum = cardsDeck[deckSize - 1];
						alternativePokSprite();
					}
					else
					{
						smallBackSprite();
					}
				}
				if(cardArived)
				{
					vramX = spX >> 3;
					vramY = spY >> 3;
					ppu_wait_nmi();
					if(dealStates)
					{
						signNum = cardsDeck[deckSize - 1];
						pokCardDisplay();
						
						++dealStates;
					}
					else
					{
						pokBa();
					//	++universalPokerStates;
						if(nextToPlay == lastToPlay)
						{
							++dealingRound;
						}
					//	whoPlays();
						do
						{
							++nextToPlay;
							if(nextToPlay == 4)
							nextToPlay = 0;
						}while(!players[nextToPlay].flags.flags.atTheSeet);

					}
					
					oam_clear();
					cardArived = 0;
					--deckSize;
					
				}
}

void dealFlop()
{
		
	switch(dealStates)
	{
		case(1):
				communityCards[0] = cardsDeck[deckSize - 1];
				changeCardPoss(X_START, 48, Y_START, 80);
				dealOneCard();
				
				break;
		case(2):
				communityCards[1] = cardsDeck[deckSize - 1];
				changeCardPoss(X_START, 80, Y_START, 80);
				dealOneCard();
				
				break;
		case(3):
				communityCards[2] = cardsDeck[deckSize - 1];
				changeCardPoss(X_START, 112, Y_START, 80);
				dealOneCard();
				
			
				break;
		case(4):
			if(inPlay <= 1)
			{
				playState = 5;
				break;
			}
			lastPlayed = button + 1;			
			whoPlays();
			whoPlaysLast();	
			if(atTheTable == 2)
			{
				whoPlays();
				whoPlaysLast();
			}			
			playState = 0;
			
			flopFinished = 1;
			stage = 1;    // it is now flop stage of the hand
			
	//	combinationSize = 5;
		//	openAllCards();
			checkStrengths(5, 3);
			
		/*	for(uu = 0; uu < 4; uu++)
			{
				printNumber(players[uu].combinationStrength, 28, 10 + uu);
				printMessage(25, 10 + uu, "Sng");
			}
		*/	
			uu = 0;
			break;

	}
}

void dealTurnRiver()
{
	switch(dealStates)
	{
		case(4):
			++dealStates;
			break;
		case(5):
				communityCards[3] = cardsDeck[deckSize - 1];
				changeCardPoss(X_START, 144, Y_START, 80);
				dealOneCard();
				
				break;
		case(6):
				if((inPlay <= 1) || (playState == 5))
				{
					++dealStates;
					playState = 5;
				//	printMessage(10, 15, "Turn 1 player state 5");
					break;
					
				}
				
			/*	if(playState == 5)
				{
					++dealStates;
				}*/
				else
				{
					lastPlayed = button + 1;			
					whoPlays();
					whoPlaysLast();
					if(atTheTable == 2)
					{
						whoPlays();
						whoPlaysLast();
					}				
					playState = 0;
				//	openAllCards();
					checkStrengths(6, 4);
			
		/*	for(uu = 0; uu < 4; uu++)
			{
				printNumber(players[uu].combinationStrength, 28, 10 + uu);
				printMessage(25, 10 + uu, "Sng");
			}
			uu = 0;*/
				}
				stage = 2; // it is turn stage of the hand play
				break;
		case(7):
				communityCards[4] = cardsDeck[deckSize - 1];
				changeCardPoss(X_START, 176, Y_START, 80);
				dealOneCard();
				
				break;
		case(8):
			if((inPlay <= 1) || (playState == 5))
			{
			//	++dealStates;
				playState = 5;
			//	printMessage(10, 15, "RIVER 1 player state 5");
					break;
					
			}
			else
		//	if((playState != 5) && !possibilities)
			{
				playState = 0;
			
				lastPlayed = button + 1;			
				whoPlays();
				whoPlaysLast();
				if(atTheTable == 2)
				{
					whoPlays();
					whoPlaysLast();
				}
				riverFinished = 1;
				stage = 3;
				checkStrengths(7, 5);
				break;
			}
			//	playState = 5;
				riverFinished = 1;
		
			
			break;
	}
}

void coloredSelector1()
{
	availableSprid = 100;
	// corner tiles
	ppu_wait_nmi();
	availableSprid = oam_spr(cSelX, cSelY, selTile, SPRITE_ATTR_FULL(4, 3, 0, 0, 1), availableSprid);
	availableSprid = oam_spr(cSelX+40, cSelY, selTile, SPRITE_ATTR_FULL(4, 3, 0, 1, 1), availableSprid);
	availableSprid = oam_spr(cSelX, cSelY+8, selTile, SPRITE_ATTR_FULL(4, 3, 1, 0, 1), availableSprid);
	availableSprid = oam_spr(cSelX+40, cSelY+8, selTile, SPRITE_ATTR_FULL(4, 3, 1, 1, 1), availableSprid);
	availableSprid = oam_spr(cSelX + 16, cSelY, selTile1, SPRITE_ATTR_FULL(4, 3, 0, 0, 1), availableSprid);
	availableSprid = oam_spr(cSelX + 16, cSelY + 8, selTile1, SPRITE_ATTR_FULL(4, 3, 1, 0, 1), availableSprid);
	availableSprid = oam_spr(cSelX + 24, cSelY, selTile1, SPRITE_ATTR_FULL(4, 3, 0, 1, 1), availableSprid);
	availableSprid = oam_spr(cSelX + 24, cSelY + 8, selTile1, SPRITE_ATTR_FULL(4, 3, 1, 1, 1), availableSprid);
	// Corner tiles end
	// middle tiles
	/*
	availableSprid = oam_spr(cSelX, cSelY, tile, SPRITE_ATTR_FULL(4, 3, 0, 0, 0), availableSprid);
	availableSprid oam_spr(cSelX+24, cSelY, tile, SPRITE_ATTR_FULL(4, 3, 0, 1, 0), availableSprid);
	availableSprid = oam_spr(cSelX, cSelY, tile, SPRITE_ATTR_FULL(4, 3, 0, 0, 0), availableSprid);
	availableSprid oam_spr(cSelX+24, cSelY, tile, SPRITE_ATTR_FULL(4, 3, 0, 1, 0), availableSprid);
	*/
	
	++tileUpdate;
	if(tileUpdate > SEL_ANIMATION_TIME)
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

void sliderBar()
{
	availableSprid = 100;
	// corner tiles
	availableSprid = oam_spr(cSelX, cSelY, selTile2, SPRITE_ATTR_FULL(4, 3, 0, 0, 1), availableSprid);
	++tileUpdate;
	if(tileUpdate > SEL_ANIMATION_TIME)
	{
		tileUpdate = 0;
		++selTile2;
		if(selTile2 == 107)
		{
			selTile2 = 103;
		}
	}
}
/*
void displayNumber(unsigned char num)
{
	vram_put((((unsigned int)num) + 16) | 0x2000);
}
*/
void pressStart()
{
	++tileUpdate;
	if(tileUpdate == 50)
	tileUpdate = 0;
	if(tileUpdate < 25)
	{
		displayMessage("Press Start ");
	}
	else
	{
		displayMessage("            ");
	}
}

void displayNumber()
{
	num2 = num1;
	
	k = 0;
/*	
	if(!num1)
	{
		vram_adr(0x2C00 | ((bg3Y << 5) | bg3X + k));
		vram_put((111) | 0x2400);
		return;
	}
*/	
	while(num1)
	{
		++k;
		num1 = snesdiv(num1, 10);
	}

	ppu_wait_nmi();
	vram_adr(0x2C00 | ((bg3Y << 5) | bg3X + k));
	displayMessage("      ");

	if(k)
	--k;
	
	num1 = num2;
	do
	{
		num2 = snesmod(num1, 10);
	//	ppu_wait_nmi();
		vram_adr(0x2C00 | ((bg3Y << 5) | bg3X + k));
		vram_put((((unsigned int)num2) + 16) | 0x2000);
		num1 = snesdiv(num1, 10);
		--bg3X;
	}while(num1);
	
	k = 0;
}

void displayInThePot()
{
	switch(nextToPlay)
	{
		case(0):
			bg3X = 3;
			bg3Y = 2;
			break;
		case(1):
			bg3X = 21;
			bg3Y = 2;
			break;
		case(2):
			bg3X = 21;
			bg3Y = 18;
			break;
		case(3):
			bg3X = 3;
			bg3Y = 18;
			break;			
	}
	vramX = bg3X;
	ppu_wait_nmi();
	vram_adr(0x2C00 |((bg3Y << 5) | bg3X));
	num1 = players[nextToPlay].inThePot;
	displayNumber();
	bg3Y += 6;
	bg3X = vramX;
	ppu_wait_nmi();
	vram_adr(0x2C00 |((bg3Y << 5) | bg3X));
	num1 = players[nextToPlay].cash;
	displayNumber();
}

void clearFields()
{
	ppu_wait_nmi();
	oam_clear();
	
	vram_adr(0x2400 | ((25 << 5) | 5));
	vram_fill(4, 128);
	
	vram_adr(0x2C00 | ((25 << 5) | 5));
	vram_fill(0, 128);
	
}

void clearSlider()
{
	ppu_wait_nmi();
	oam_clear();
	vram_adr(0x2400 | ((22 << 5) | 11));
	vram_fill(4, 20);
	vram_adr(0x2400 | ((23 << 5) | 11));
	vram_fill(4, 20);
	vram_adr(0x2C00 | ((22 << 5) | 10));
	vram_fill(10, 20);
	
}

void clearCards()
{
	switch(nextToPlay)
	{
		case(0):
			vramX = 3;
			vramY = 2;
			break;
		case(1):
			vramX = 21;
			vramY = 2;
			break;
		case(2):
			vramX = 21;
			vramY = 18;
			break;
		case(3):
			vramX = 3;
			vramY = 18;
			break;			
	}

	k = 0;
	for(i = 0; i < 6; i++)
	{
		vram_adr(BG1_NT(vramX, vramY));
		for(j = 0; j < 8; j++)
		{
			vram_put(4);        // here every vram write increments x
			++k;		
		}
		++vramY;
	}
//	vramX += 4;
	vramY -= 6;

}


void openAllCards()
{
	uu = 0;
	for(uu = 0; uu < 4; uu++)
	{
		if(players[uu].flags.flags.folded || !players[uu].flags.flags.atTheSeet)
		continue;
		
		ppu_wait_nmi();
		switch(uu)
		{
			case(0):
			
			vramY = 2;
			vramX = 3;
			signNum = players[0].pocket[0];
			pokCardDisplay();

			signNum = players[0].pocket[1];
			vramY = 2;
			vramX = 7;
			ppu_wait_nmi();
			pokCardDisplay();
			break;
		case(1):
			vramY = 2;
			vramX = 21;
			signNum = players[1].pocket[0];
			ppu_wait_nmi();
			pokCardDisplay();
			vramY = 2;
			vramX = 25;
			signNum = players[1].pocket[1];
			
			ppu_wait_nmi();
			pokCardDisplay();
			break;
		case(2):
			ppu_wait_nmi();
			vramY = 18;
			vramX = 21;
			signNum = players[2].pocket[0];
			pokCardDisplay();
			ppu_wait_nmi();
			vramY = 18;
			vramX = 25;
			signNum = players[2].pocket[1];
			ppu_wait_nmi();
			pokCardDisplay();
			break;
		case(3):
			vramY = 18;
			vramX = 3;
			signNum = players[3].pocket[0];
			ppu_wait_nmi();
			pokCardDisplay();
			vramY = 18;
			vramX = 7;
			signNum = players[3].pocket[1];
			ppu_wait_nmi();
			pokCardDisplay();
			break;		
		}
	}
	uu = 0;
}

void clearBG3()
{
	ppu_wait_nmi();
	vram_adr(0x2c00);
	vram_fill(10, 1024);
	
	ppu_wait_nmi();
	vram_adr(0x2c00 | (13<<5) | 0);
	vram_fill(10, 1024);
}

void fixCards()
{
	for( i = 1; i < 14; i++)
	{
		cardsDeck[deckSize-i] = fix[i-1];
	}
	i = 0;
}

/****************************************************************************************************

	SHOWDOWN PART, DECIDING THE WINNER, SPLIT POTS, SIDE POTS
	
*****************************************************************************************************/	
	
unsigned char withMoney[4];
unsigned char withMoneyNum;

unsigned char withoutMoney[4];
unsigned char withoutMoneyNum;


unsigned char sf1, sf2, f1, f2;

unsigned char draw1, draw2, draw3;

unsigned char toCompare[4];
unsigned char toCompareSize;

unsigned char index1, index2;

char text0[2];
unsigned char indexToPrint;

#define BOX_X 6
#define BOX_Y 3

#define TEXT0_X BOX_X + 1
#define TEXT1_X TEXT0_X + 1
#define TEXT2_X TEXT1_X + 2

#define TEXT0 "P"
#define TEXT2 "wins"

#define WGS  TEXT1_X + 7

#define FIRST  4
#define SECOND FIRST + 1
#define THIRD  FIRST + 2
#define FOURTH FIRST + 3
#define FIFTH  FIRST + 4

#define PS

void showdown()
{
	i = 0;
	j = 0;
	k = 0;
	
	sf1 = 0;
	sf2 = 0;
	f1 = 0;
	f2 = 0;
	
	draw1 = 0;
	draw2 = 0;
	draw3 = 0;
	
	currentPot = 0;
	toCompareSize = 0;
	withoutMoneyNum = 0;
	withMoneyNum = 0;
	
	showBox();
	
	// First find all players that didn't go all in
	// player must be at the table, in the play and not folded
	
	// displayMessage main
	for(i = 0; i < 4; i++)
	{
		if(players[i].flags.status == 0x03)
		{
			withMoney[withMoneyNum] = i;
			++withMoneyNum;
			toCompare[toCompareSize] = i+1;
			++toCompareSize;
		// display players nums	
		}
		currentPot += players[i].inThePot;
	}
	for(i = withMoneyNum; i < 4; i++)
	{
		toCompare[i] = 0;  // spots with the 0 value will automaticaly lose sf or f
	}
	
	// Now count players that went all in
	// player must be at the seet, not folded and not in play, 0 0 1
	
	// displayMessage side
	for(i = 0; i < 4; i++)
	{
		if(players[i].flags.status == 0x01)
		{
			withoutMoney[withoutMoneyNum] = i;
			++withoutMoneyNum;
			// this array should be sorted so the the player with greatest inThePot ocupies index 0
		// display players nums	
		}
	}
	text0[0] = 'P';
	text0[1] = '\0';
	// Case where all players are inPlay, case when there are side pots
	// All in play
	if(!withoutMoneyNum)
	{
		// if there arent side pots
		// use toCopmare array to decide winner or winners and distribute winnings
		sf1 = 0;
		sf2 = 1;
		compareHands();
		sf1 = 2;
		sf2 = 3;
		compareHands();
		sf1 = f1;
		sf2 = f2;
		compareHands();
		
		while(!(pad_trigger(0) & PAD_START))
		{
			boxStart();	
			if(pad_trigger(0) & PAD_R)
			{
				ppu_wait_nmi();
			//	asm("lda #%b", 0b00000001);
			//	asm("sta $2105");
				asm("lda $2105");
				asm("eor #$08");
				asm("sta $2105");
			}
		}	
	
	}
	else
	{
		// Solve side pots, though it is possible that one player went all in but there aren't side pots
		// if all in value equals max bet
		solveSidePots();
	}
	
	
	// Remove players without cash from the table, update atThe table and inPlay variables
	// set folded flag to 0 and inPlay flag to 1 for all players that are at the table
	// Completely clear Bg1 and Bg3
	// shuffle cards, reset card deck size
	
	prepareNextHand();
	


}

void compareHands()
{
	if(toCompare[sf1] == 0)
	{
		// this must be second semifinal hence f2 gets sf2 value
		// player at sf1 can't have 0 value in the first semi because into toCompare array first go active players(not folded at the table)
		// and there must be atleast 2 of them at showdown time
		f2 = sf2;
	}
	// IN THIS CASE PLAYER AT INDEX sf1 has beaten player at sf2
	else if(toCompare[sf2] == 0)
	{
		// was this the final
		if(f2 != 0)
		{
			ppu_wait_nmi();
			if(draw1)
			{
					// split pot
					
					
					if(players[toCompare[0] - 1].joyPadNum < 4)
					{
						text0[0] = 'P';
						indexToPrint = players[toCompare[0] - 1].joyPadNum + 1;
					}
					else
					{
						text0[0] = 'C';
						indexToPrint = players[toCompare[0] - 1].joyPadNum - 3;
					}
					printBoxMessage(TEXT0_X, FIRST, text0);
					printBoxNumber(indexToPrint, TEXT1_X, FIRST);
					
					if(players[toCompare[1] - 1].joyPadNum < 4)
					{
						text0[0] = 'P';
						indexToPrint = players[toCompare[1] - 1].joyPadNum + 1;
					}
					else
					{
						text0[0] = 'C';
						indexToPrint = players[toCompare[1] - 1].joyPadNum - 3;
					}
					
					printBoxMessage(TEXT0_X, SECOND, TEXT0);
					printBoxNumber(indexToPrint, TEXT1_X, SECOND);
					
					printBoxMessage(TEXT2_X, FIRST, TEXT2);
					printBoxMessage(TEXT2_X, SECOND, TEXT2);
					printBoxNumber(currentPot >> 1, WGS, FIRST);
					printBoxNumber(currentPot >> 1, WGS, SECOND);
					players[toCompare[0] - 1].cash += currentPot >> 1;
					players[toCompare[1] - 1].cash += currentPot >> 1;
			//		printBoxMessage(11, 3, "Split Pot");
					
			}
			else
			{
				// whole pot goes to player at index sf1
				if(players[toCompare[sf1] - 1].joyPadNum < 4)
				{
					text0[0] = 'P';
					indexToPrint = players[toCompare[sf1] - 1].joyPadNum + 1;
				}
				else
				{
					text0[0] = 'C';
					indexToPrint = players[toCompare[sf1] - 1].joyPadNum - 3;
				}
				printBoxMessage(TEXT0_X, FIRST, text0);
				printBoxNumber(indexToPrint, TEXT1_X, FIRST);
				printBoxMessage(TEXT2_X, FIRST, TEXT2);
				printBoxNumber(currentPot, WGS, FIRST);
				players[toCompare[sf1] - 1].cash += currentPot;
			//	printBoxMessage(11, 3, "Main Pot");
			}
		}
		else
		{
			// this is the second semifinal
			f2 = sf1; // first finalist is player at index sf1 in the toCompare array
		} 
	}
		
	
		
	
	// Above are the cases in which players play against the player who folded or isnt at the table anymore
	// so the other player wins automaticaly
	// it is possible that both players at sf1 and sf2 indexes have values 0, in that case one of them will advance to the final
	// but again will automaticaly lose from other finalist because the other two players in the second semifinal doesn't have 0 values
	// in the toCompare array
	
	// NOW iF BOTH PLAYERS ARE ELIGIBILE TO PLAY
	else
	{
		// first compare hand strengths
		// if equal compare player combinations
		// if equal check which semi is it and set corresponding draw, or if it is a final set draw3, and set sf1 at f1
		// meaning that sf1 will play in the final and if he wins and the draw is set pot will be split between him and sf2
		// if it was final check for draws and split pots if necessary
		
		// if not equal, check if it was final or semifinal and update f1 f2 or cashes
		index1 = toCompare[sf1] - 1;
		index2 = toCompare[sf2] - 1;
		if(players[index1].combinationStrength == players[index2].combinationStrength)
		{
			findHighCard();
		
		}
		else if(players[index1].combinationStrength > players[index2].combinationStrength)
		{
			// index1 has won
			// check final semifinal
			if(f2 != 0)
			{
				ppu_wait_nmi();
				// check draw1
				if(draw1)
				{
					if(players[toCompare[0] - 1].joyPadNum < 4)
					{
						text0[0] = 'P';
						indexToPrint =  players[toCompare[0] - 1].joyPadNum + 1;
					}
					else
					{
						text0[0] = 'C';
						indexToPrint =  players[toCompare[0] - 1].joyPadNum -3;
					}
					printBoxMessage(TEXT0_X, FIRST, text0);
					printBoxNumber(indexToPrint, TEXT1_X, FIRST);
					
					if(players[toCompare[1] - 1].joyPadNum < 4)
					{
						text0[0] = 'P';
						indexToPrint =  players[toCompare[1] - 1].joyPadNum + 1;
					}
					else
					{
						text0[0] = 'C';
						indexToPrint =  players[toCompare[1] - 1].joyPadNum -3;
					}
					
					printBoxMessage(TEXT0_X, SECOND, text0);
					printBoxNumber(indexToPrint, TEXT1_X, SECOND);
					printBoxMessage(TEXT2_X, FIRST, TEXT2);
					printBoxMessage(TEXT2_X, SECOND, TEXT2);
					printBoxNumber(currentPot >> 1, WGS, FIRST);
					printBoxNumber(currentPot >> 1, WGS, SECOND);
					players[0].cash += currentPot >> 1;
					players[1].cash += currentPot >> 1;
				//	printBoxMessage(11, 3, "Split Pot");
				
					
				/*	players[0].cash += currentPot >> 1;
					players[1].cash += currentPot >> 1;
					printNumber(toCompare[0], 1, 1);
					printNumber(toCompare[1], 3, 1);
					printMessage(6, 1, "SPLIT POT");
					printMessage(6, 2, "DRAW 1");
					printNumber(currentPot, 17, 1);
				*/	
				}
				else
				{
					if(players[index1].joyPadNum < 4)
					{
						text0[0] = 'P';
						indexToPrint = players[index1].joyPadNum + 1;
					}
					else
					{
						text0[0] = 'C';
						indexToPrint = players[index1].joyPadNum  -3;
					}
					printBoxMessage(TEXT0_X, FIRST, text0);
					printBoxNumber(indexToPrint, TEXT1_X, FIRST);
					printBoxMessage(TEXT2_X, FIRST, TEXT2);
					printBoxNumber(currentPot, WGS, FIRST);
					players[index1].cash += currentPot;
				//	printBoxMessage(11, 3, "Main Pot");
				/*	
					players[index1].cash += currentPot;
					printNumber(index1 + 1, 1, 1);
					printMessage(4, 1, "Wins");
					printNumber(currentPot, 8, 1);
				*/
				}
				// if draw1 players 0 and 1 split pot
				// else player at playerIndex1 takes whole pot
			}
			// was this first semifinal
			else if(sf1 < 2)
			{
				f1 = sf1;       // first semifinalist has won and advances to the final as first finalist
			}
			else
			{
				f2 = sf1;      // first semifinalist has won and advanced to the final as second finalist
			}
		}
		else
		{
			// index2 has won,
			// was it final
			if(f2 != 0)
			{
				ppu_wait_nmi();
				// in the final index2 can only be 2 or 3, so draw2 (second semifinal) has to be checked
				if(draw2)
				{
					if(players[toCompare[2] - 1].joyPadNum < 4)
					{
						text0[0] = 'P';
						indexToPrint = players[toCompare[2] - 1].joyPadNum + 1;
					}
					else
					{
						text0[0] = 'C';
						indexToPrint = players[toCompare[2] - 1].joyPadNum - 3;
					}
					printBoxMessage(TEXT0_X, FIRST, text0);
					printBoxNumber(indexToPrint, TEXT1_X, FIRST);
					
					if(players[toCompare[3] - 1].joyPadNum < 4)
					{
						text0[0] = 'P';
						indexToPrint = players[toCompare[3] - 1].joyPadNum + 1;
					}
					else
					{
						text0[0] = 'C';
						indexToPrint = players[toCompare[3] - 1].joyPadNum - 3;
					}
					
					printBoxMessage(TEXT0_X, SECOND, text0);
					printBoxNumber(indexToPrint, TEXT1_X, SECOND);
					printBoxMessage(TEXT2_X, FIRST, TEXT2);
					printBoxMessage(TEXT2_X, SECOND, TEXT2);
					printBoxNumber(currentPot >> 1, WGS, FIRST);
					printBoxNumber(currentPot >> 1, WGS, SECOND);
					players[2].cash += currentPot >> 1;
					players[3].cash += currentPot >> 1;
				//	printBoxMessage(11, 3, "Split Pot");
					
				/*	
					// players at 2 and 3 split pot
					players[2].cash += currentPot >> 1;  //
					players[3].cash += currentPot >> 1;
					// display message players indexes split pot and current pot amount
					printNumber(toCompare[2], 1, 1);
					printNumber(toCompare[3], 3, 1);
					printMessage(6, 1, "SPLIT POT");
					printNumber(currentPot, 17, 1);
				*/	 
				}
				else
				{
					if(players[index2].joyPadNum < 4)
					{
						text0[0] = 'P';
						indexToPrint = players[index2].joyPadNum + 1;
					}
					else
					{
						text0[0] = 'C';
						indexToPrint = players[index2].joyPadNum  -3;
					}
					printBoxMessage(TEXT0_X, FIRST, text0);
					printBoxNumber(indexToPrint, TEXT1_X, FIRST);
					printBoxMessage(TEXT2_X, FIRST, TEXT2);
					printBoxNumber(currentPot, WGS, FIRST);
					players[index2].cash += currentPot;
				//	printBoxMessage(11, 3, "Main Pot");
				/*	
					// player at index2 takes whole pot
					players[index2].cash += currentPot;
					printNumber(index2 + 1, 1, 1);
					printMessage(4, 1, "Wins");
					printNumber(currentPot, 8, 1);
				*/	
				}
			}
			// if not final, was this first semifinal
			else if(sf1 < 2)
			{
				f1 = sf2;    // player at index sf2 advances to the final
			}
			// or it was second semifinal
			else
			{
				f2 = sf2;
			}
		}
	}
}

void findHighCard()
{
	for(i = 0; i < 5; i++)
	{
		// player at sf1 won
		if((players[index1].combination[i] & 0x0f) > (players[index2].combination[i] & 0x0f))
		{
			if(f2 != 0)
			{
				ppu_wait_nmi();
				if(draw1)
				{
				//	printBoxMessage(TEXT0_X, FIRST, "SPLIT POT");
				
					if(players[toCompare[0] - 1].joyPadNum < 4)
					{
						text0[0] = 'P';
						indexToPrint = players[toCompare[0] - 1].joyPadNum + 1;
					}
					else
					{
						text0[0] = 'C';
						indexToPrint = players[toCompare[0] - 1].joyPadNum - 3;
					}
					printBoxMessage(TEXT0_X, FIRST, text0);
					printBoxNumber(indexToPrint, TEXT1_X, FIRST);
					
					if(players[toCompare[1] - 1].joyPadNum < 4)
					{
						text0[0] = 'P';
						indexToPrint = players[toCompare[1] - 1].joyPadNum + 1;
					}
					else
					{
						text0[0] = 'C';
						indexToPrint = players[toCompare[1] - 1].joyPadNum - 3;
					}
					
					printBoxMessage(TEXT0_X, SECOND, text0);
					printBoxNumber(indexToPrint, TEXT1_X, SECOND);
					
					
					printBoxMessage(TEXT2_X, SECOND, TEXT2);
					printBoxMessage(TEXT2_X, THIRD, TEXT2);
					printBoxNumber(currentPot >> 1, WGS, FIRST);
					printBoxNumber(currentPot >> 1, WGS, SECOND);
					
					players[0].cash += currentPot >> 1;
					players[1].cash += currentPot >> 1;
				}
				else
				{
					if(players[index1].joyPadNum < 4)
					{
						text0[0] = 'P';
						indexToPrint = players[index1].joyPadNum + 1;
					}
					else
					{
						text0[0] = 'C';
						indexToPrint = players[index1].joyPadNum  -3;
					}
					printBoxMessage(TEXT0_X, FIRST, TEXT0);
					printBoxNumber(index1 + 1, TEXT1_X, FIRST);
					printBoxMessage(TEXT2_X, FIRST, TEXT2);
					printBoxNumber(currentPot, WGS, FIRST);
					players[index1].cash += currentPot;
			
				}
			}
			else if(sf1 < 2)
			{
				f1 = sf1;
			}
			else
			{
				f2 = sf1;
			}
			return;
		}
		else if((players[index2].combination[i] & 0x0f) > (players[index1].combination[i] & 0x0f))
		{
			if(f2 != 0)
			{
				ppu_wait_nmi();
				if(draw2)
				{
				//	printBoxMessage(TEXT0_X, FIRST, "SPLIT POT");
					if(players[toCompare[2] - 1].joyPadNum < 4)
					{
						text0[0] = 'P';
						indexToPrint = players[toCompare[2] - 1].joyPadNum + 1;
					}
					else
					{
						text0[0] = 'C';
						indexToPrint = players[toCompare[2] - 1].joyPadNum - 3;
					}
					printBoxMessage(TEXT0_X, FIRST, text0);
					printBoxNumber(indexToPrint, TEXT1_X, FIRST);	
					
					if(players[toCompare[3] - 1].joyPadNum < 4)
					{
						text0[0] = 'P';
						indexToPrint = players[toCompare[3] - 1].joyPadNum + 1;
					}
					else
					{
						text0[0] = 'C';
						indexToPrint = players[toCompare[3] - 1].joyPadNum - 3;
					}
					
					printBoxMessage(TEXT0_X, SECOND, text0);
					printBoxNumber(indexToPrint, TEXT1_X, SECOND);
					
					printBoxMessage(TEXT2_X, FIRST, TEXT2);
					printBoxMessage(TEXT2_X, SECOND, TEXT2);
					printBoxNumber(currentPot >> 1, WGS, FIRST);
					printBoxNumber(currentPot >> 1, WGS, SECOND);
					players[2].cash += currentPot >> 1;
					players[3].cash += currentPot >> 1;
			//		printBoxMessage(11, 3, "Split Pot");
					
				/*	
					players[2].cash += currentPot >> 1;
					players[3].cash += currentPot >> 1;
					printNumber(index1 + 1, 1, 1);
					printNumber(index2 + 1, 3, 1);
					printMessage(6, 1, "SPLIT POT");
					printMessage(6, 2, "DRAW 2");
					printNumber(currentPot, 17, 1);
				*/	 
				}
				else
				{
					if(players[index2].joyPadNum < 4)
					{
						text0[0] = 'P';
						indexToPrint = players[index2].joyPadNum + 1;
					}
					else
					{
						text0[0] = 'C';
						indexToPrint = players[index2].joyPadNum  -3;
					}
					printBoxMessage(TEXT0_X, FIRST, TEXT0);
					printBoxNumber(index2 + 1, TEXT1_X, FIRST);
					printBoxMessage(TEXT2_X, FIRST, TEXT2);
					printBoxNumber(currentPot, WGS, FIRST);
					players[index2].cash += currentPot;
			
				}
			}
			else if(sf1 < 2)
			{
				f1 = sf2;
			}
			else
			{
				f2 = sf2;
			}
			return;
		}
	}
	
	
	
/*	ppu_wait_nmi();
	printNumber(toCompare[0], TEXT0_X, FIFTH + 3);
	printNumber(toCompare[1], TEXT0_X + 1, FIFTH + 3);
	printNumber(toCompare[2], TEXT0_X + 2, FIFTH + 3);
	printNumber(toCompare[3], TEXT0_X + 3, FIFTH + 3);*/
	// Players played a draw
	if(f2 != 0)
	{
		ppu_wait_nmi();
	//	printBoxMessage(TEXT0_X, FIRST, "SPLIT POT");
		// if there was a draw in the final, check both semifinal draws
		if(draw1 & draw2)
		{
			// all 4 players split pot
			
			players[0].cash += currentPot >> 2;
			players[1].cash += currentPot >> 2; 
			players[2].cash += currentPot >> 2;
			players[3].cash += currentPot >> 2;
			
			
			ppu_wait_nmi();
			
			
			// joypadnum instead of hardcoded values
			if(players[0].joyPadNum < 4)
			{
				text0[0] = 'P';
				indexToPrint = players[0].joyPadNum + 1;
			}
			else
			{
				text0[0] = 'C';
				indexToPrint = players[0].joyPadNum - 3;
			}
			printBoxMessage(TEXT0_X, SECOND, text0);
			printBoxNumber(indexToPrint, TEXT1_X, SECOND);
			printBoxMessage(TEXT2_X, SECOND, TEXT2);
			printBoxNumber(currentPot >> 2, WGS, SECOND);
			
			if(players[1].joyPadNum < 4)
			{
				text0[0] = 'P';
				indexToPrint = players[1].joyPadNum + 1;
			}
			else
			{
				text0[0] = 'C';
				indexToPrint = players[1].joyPadNum - 3;
			}
			
			printBoxMessage(TEXT0_X, THIRD, text0);
			printBoxNumber(indexToPrint, TEXT1_X, THIRD);
			printBoxMessage(TEXT2_X, THIRD, TEXT2);
			printBoxNumber(currentPot >> 2, WGS, THIRD);
			
			if(players[2].joyPadNum < 4)
			{
				text0[0] = 'P';
				indexToPrint = players[2].joyPadNum + 1;
			}
			else
			{
				text0[0] = 'C';
				indexToPrint = players[2].joyPadNum - 3;
			}
			
			printBoxMessage(TEXT0_X, FOURTH, text0);
			printBoxNumber(indexToPrint, TEXT1_X, FOURTH);
			printBoxMessage(TEXT2_X, FOURTH, TEXT2);
			printBoxNumber(currentPot >> 2, WGS, FOURTH);
			
			if(players[3].joyPadNum < 4)
			{
				text0[0] = 'P';
				indexToPrint = players[3].joyPadNum + 1;
			}
			else
			{
				text0[0] = 'C';
				indexToPrint = players[3].joyPadNum - 3;
			}
			
			printBoxMessage(TEXT0_X, FIFTH, text0);
			printBoxNumber(indexToPrint, TEXT1_X, FIFTH);
			printBoxMessage(TEXT2_X, FIFTH, TEXT2);
			printBoxNumber(currentPot >> 2, WGS, FIFTH);
			
		//	printBoxMessage(TEXT0_X, FIFTH + 1, "COMB");
		//	printBoxMessage(TEXT0_X, FIFTH + 2, "PRESS START");
			 
		}
		else if(draw1)
		{
			players[index1].cash += snesdiv(currentPot, 3);
			players[index2].cash += snesdiv(currentPot, 3); 
			players[toCompare[1]].cash += snesdiv(currentPot, 3); // this one played a draw in sf, but 0 index went in final
			
			ppu_wait_nmi();
			
			
			if(players[toCompare[1] - 1].joyPadNum < 4)
			{
				text0[0] = 'P';
				indexToPrint = players[toCompare[1] - 1].joyPadNum + 1;
			}
			else
			{
				text0[0] = 'C';
				indexToPrint = players[toCompare[1] - 1].joyPadNum - 3;
				
			}
			
			printBoxMessage(TEXT0_X, SECOND, text0);
			printBoxNumber(indexToPrint, TEXT1_X, SECOND);
			printBoxMessage(TEXT2_X, SECOND, TEXT2);
			printBoxNumber(snesdiv(currentPot, 3), WGS, SECOND);
			// joypadnum instead of hardcoded values
			if(players[index1].joyPadNum < 4)
			{
				text0[0] = 'P';
				indexToPrint = players[index1].joyPadNum + 1;
			}
			else
			{
				text0[0] = 'C';
				indexToPrint = players[index1].joyPadNum - 3;
			}
			printBoxMessage(TEXT0_X, THIRD, text0);
			printBoxNumber(indexToPrint, TEXT1_X, THIRD);
			printBoxMessage(TEXT2_X, THIRD, TEXT2);
			printBoxNumber(snesdiv(currentPot, 3), WGS, THIRD);
			
			if(players[index2].joyPadNum < 4)
			{
				text0[0] = 'P';
				indexToPrint = players[index2].joyPadNum + 1;
			}
			else
			{
				text0[0] = 'C';
				indexToPrint = players[index2].joyPadNum - 3;
			}
			
			printBoxMessage(TEXT0_X, FOURTH, TEXT0);
			printBoxNumber(index2 + 1, TEXT1_X, FOURTH);
			printBoxMessage(TEXT2_X, FOURTH, TEXT2);
			printBoxNumber(snesdiv(currentPot, 3), WGS, FOURTH);
			
		}
		else if(draw2)
		{
			players[index2].cash += snesdiv(currentPot, 3);
			players[toCompare[3] - 1].cash += snesdiv(currentPot, 3); 
			players[index1].cash += snesdiv(currentPot, 3);
			
			ppu_wait_nmi();
			
			// joypadnum instead of hardcoded values
			if(players[toCompare[3] - 1].joyPadNum < 4)
			{
				text0[0] = 'P';
				indexToPrint = players[toCompare[3] - 1].joyPadNum + 1;
			}
			else
			{
				text0[0] = 'C';
				indexToPrint = players[toCompare[3] - 1].joyPadNum - 3;	
			}
			printBoxMessage(TEXT0_X, SECOND, text0);
			printBoxNumber(indexToPrint, TEXT1_X, SECOND);
			printBoxMessage(TEXT2_X, SECOND, TEXT2);
			printBoxNumber(snesdiv(currentPot, 3), WGS, SECOND);
			
			if(players[index2].joyPadNum < 4)
			{
				text0[0] = 'P';
				indexToPrint = players[index2].joyPadNum + 1;
			}
			else
			{
				text0[0] = 'C';
				indexToPrint = players[index2].joyPadNum - 3;
			}
			
			printBoxMessage(TEXT0_X, THIRD, text0);
			printBoxNumber(indexToPrint, TEXT1_X, THIRD);
			printBoxMessage(TEXT2_X, THIRD, TEXT2);
			printBoxNumber(snesdiv(currentPot, 3), WGS, THIRD);
			
			if(players[index1].joyPadNum < 4)
			{
				text0[0] = 'P';
				indexToPrint = players[index1].joyPadNum + 1;
			}
			else
			{
				text0[0] = 'C';
				indexToPrint = players[index1].joyPadNum - 3;
			}
			
			printBoxMessage(TEXT0_X, FOURTH, text0);
			printBoxNumber(indexToPrint, TEXT1_X, FOURTH);
			printBoxMessage(TEXT2_X, FOURTH, TEXT2);
			printBoxNumber(snesdiv(currentPot, 3), WGS, FOURTH);
		}
		else
		{
			players[index1].cash += currentPot >> 1;
			players[index2].cash += currentPot >> 1; 
			
			ppu_wait_nmi();
			
			if(players[index1].joyPadNum < 4)
			{
				text0[0] = 'P';
				indexToPrint = players[index1].joyPadNum + 1;
			}
			else
			{
				text0[0] = 'C';
				indexToPrint = players[index1].joyPadNum - 3;
			}
			
			printBoxMessage(TEXT0_X, SECOND, text0);
			printBoxNumber(indexToPrint, TEXT1_X, SECOND);
			printBoxMessage(TEXT2_X, SECOND, TEXT2);
			printBoxNumber(snesdiv(currentPot, 2), WGS, SECOND);
			
			if(players[index2].joyPadNum < 4)
			{
				text0[0] = 'P';
				indexToPrint = players[index2].joyPadNum + 1;
			}
			else
			{
				text0[0] = 'C';
				indexToPrint = players[index2].joyPadNum - 3;
			}
			
			printBoxMessage(TEXT0_X, THIRD, text0);
			printBoxNumber(indexToPrint, TEXT1_X, THIRD);
			printBoxMessage(TEXT2_X, THIRD, TEXT2);
			printBoxNumber(snesdiv(currentPot, 2), WGS, THIRD);
			// players at index1 and index2 split pot
		}
	}
	else
	{
		if(sf1 < 2)
		{
			f1 = sf1;
			draw1 = 1;
		}
		else
		{
			f2 = sf1;
			draw2 = 1;
		}
	}
	
}
#pragma code-name(pop)
/************************************************************************************************************
					SIDE POTS
*************************************************************************************************************/

unsigned char inThePots[4];
unsigned char tcpInd;
unsigned char inpInd;
unsigned char curInp;
unsigned long playingFor;
unsigned long previousPot;
unsigned long smallestIn;
unsigned char pom;
unsigned char skipOne;
unsigned long deduct;

#pragma code-name(push, "CODEA")
void fillInps()
{
	for(i = 0; i < 4; i++)
	{
		inThePots[i] = i;
	}
	i = 0;
}

void sortInps() 
{  
	
   for (i = 0; i < 4-1; i++)       
  
       {
       		for (j = 0; j < 4-i-1; j++)
	   		{  
           		if (players[inThePots[j]].inThePot > players[inThePots[j+1]].inThePot) 
            	{  
              		pom = inThePots[j];
              		inThePots[j] = inThePots[j+1];
              		inThePots[j+1] = pom;
          		}	
        	}
    	}
    	i = 0;
    	j = 0;
} 			

void showInps()
{
	for(i = 0; i < 4; i++)
	{
		ppu_wait_nmi();
		printNumber(inThePots[i]+1, 12+i, 18);
	}
	i = 0;
}


void showTcps()
{
	for(i = 0; i < 4; i++)
	{
		ppu_wait_nmi();
		printNumber(toCompare[i], 12+i, 17);
	}
	i = 0;
}
void solveSidePots()
{
	fillInps();
	sortInps();
//	showInps();
	
	inpInd = 0;
	tcpInd = 0;
	curInp = 0;
	previousPot = 0;
	deduct = 0;
	
	
	
	while(curInp < 4)
	{
		playingFor = 0;
		for(tcpInd = 0; tcpInd < 4; tcpInd++)
		{
			toCompare[tcpInd] = 0;	
		}
		tcpInd = 0;
		inpInd = curInp;
		skipOne = 0;
		
		// skip players who didn't enter the hand with the money
		while(players[inThePots[inpInd]].inThePot == 0)
		{
			++inpInd;	
		}

		// skip folded players and players which are not at the table but take their inThePots
		while(players[inThePots[inpInd]].flags.flags.folded || !players[inThePots[inpInd]].flags.flags.atTheSeet)
		{
			playingFor += players[inThePots[inpInd]].inThePot; 
			++inpInd;
		}
		smallestIn = players[inThePots[inpInd]].inThePot - deduct;
		// current inTheSidePot amount for each player playing has been found
		// take inTheSidePot from the first player 
		playingFor += smallestIn;
		toCompare[tcpInd] = inThePots[inpInd] + 1;
		
		if(inpInd < 3)
		while(players[inThePots[inpInd]].inThePot == players[inThePots[inpInd+1]].inThePot)
		{
			skipOne = 1;
			if(players[inThePots[inpInd]].flags.flags.atTheSeet && !players[inThePots[inpInd]].flags.flags.folded)
			{
				toCompare[tcpInd] = inThePots[inpInd] + 1; // toCompare elements go from 1-4 not 0-3
				toCompare[tcpInd+1] = inThePots[inpInd+1] + 1;
				++tcpInd;
				if(tcpInd == 3)
				{
					break;
				}
				
			}
			playingFor += smallestIn;
			++inpInd;
			if(inpInd == 3)
			{
			//	playingFor += smallestIn;
				break;
			}
		}
		// current index may be moved for the amount of players with same inThePots
		curInp = inpInd;
		// move to the next index current one already added inThePot
		++inpInd;
		++tcpInd;
		
		// now place the rest of the players who didn't fold from inThePots into side pot
		while(inpInd < 4)
		{
			//if player is at the table and not folded qualify him for showdown
			if(players[inThePots[inpInd]].flags.flags.atTheSeet && !players[inThePots[inpInd]].flags.flags.folded)
			{
				toCompare[tcpInd] = inThePots[inpInd] + 1; // toCompare elements go from 1-4 not 0-3
				
				++tcpInd;
				
			}
			//take money from the rest of the players
			playingFor += smallestIn;
			++inpInd;
		}
		
	//	showTcps();
		deduct += smallestIn;

		currentPot = playingFor;		
		// now showdown
		
		// smallest in can become 0, in that case don't compare hands, this is for box display/erase
		if(smallestIn)
		{
			f1 = 0;
			f2 = 0;
		
			draw1 = 0;
			draw2 = 0;
			sf1 = 0;
			sf2 = 1;
			compareHands();
			sf1 = 2;
			sf2 = 3;
			compareHands();
			sf1 = f1;
			sf2 = f2;
			compareHands();
			
			while(!(pad_trigger(0) & PAD_START))
			{
				boxStart();
			}
			ppu_wait_nmi();
			printBoxMessage(TEXT0_X, FIRST,  "               ");
			printBoxMessage(TEXT0_X, SECOND, "               ");
			ppu_wait_nmi();
			printBoxMessage(TEXT0_X, THIRD,  "               ");
			printBoxMessage(TEXT0_X, FOURTH, "               ");	
			ppu_wait_nmi();
			printBoxMessage(TEXT0_X, FIFTH,  "               ");
			printBoxMessage(TEXT0_X, FIFTH + 1, "               ");
			
		}
		// then set previousPot
		previousPot = playingFor;
	//	curInp = inpInd;
		++curInp;

		
		if(curInp > 3)
		{
			ppu_wait_nmi();
			clearBox();
		}
	}
	
}

void payWinnings()
{
	currentPot = 0;
	for(i = 0; i < 4; i++)
	{
		currentPot += players[i].inThePot;
		if(!players[i].flags.flags.folded && players[i].flags.flags.atTheSeet)
		{
			uu = i;
		}
	}
	i = 0;
	text0[1] = '\0';
	if(players[uu].joyPadNum < 4)
	{
		text0[0] = 'P';
		indexToPrint = players[uu].joyPadNum + 1;
	}
	else
	{
		text0[0] = 'C';
		indexToPrint = players[uu].joyPadNum - 3;
	}
	showBox();
	printBoxMessage(TEXT0_X, FIRST, text0);
	printBoxNumber(indexToPrint, TEXT1_X, FIRST);
	printBoxMessage(TEXT2_X, FIRST, TEXT2);
	printBoxNumber(currentPot, WGS, FIRST);
	players[uu].cash += currentPot;
	uu = 0;
//	printBoxMessage(11, 3, "Main Pot");
	/*
	printNumber(j + 1, 1, 1);
	printMessage(5, 1, "Wins");
	printNumber(currentPot, 9, 1);
	players[j].cash += currentPot;
	j = 0;
	*/
	while(!(pad_trigger(0) & PAD_START))
	{
		boxStart();
	}
	j = 0;
	i = 0;
}

void prepareNextHand()
{
	++button;
	if(button == 4)
	button = 0;
	universalPokerStates = 0;
	dealingRound = 0;
	playState = 0;
	dealStates = 0;
	whoPlaysGuard = 0;
	deckSize = 52;
	shuffle();
	for(i = 0; i < 4; i++)
	{
		players[i].inThePot = 0;
		if((players[i].cash == 0) && players[i].flags.flags.atTheSeet)
		{
			players[i].flags.flags.atTheSeet = 0;
			--atTheTable;
			if(atTheTable == 1)
			{
				universalPokerStates = 11;
			}
		}
		else
		{
			// used only when final winner is declared
			winings = buyIn * 4;
			if(players[i].joyPadNum < 4)
			{
				winner = players[i].joyPadNum + 1;
				winLoseState = 1;
			}
			else
			{
				winLoseState = 0;
				winner = players[i].joyPadNum - 3;
				
			}
		}
	}
	i = 0;
//	printNumber(atTheTable,10, 14);
//	printNumber(button + 1, 20, 21);
	
	ppu_wait_nmi();
	vram_adr(0x2400);
	vram_fill(4, 1024);
	
	ppu_wait_nmi();
	vram_adr(0x2400 | (13<<5) | 0);
	vram_fill(4, 1024);
	clearBox();
	clearBG3();
}


void showBox()
{
//vram_adr(0x2C00 | ((bg3Y << 5) | bg3X + k));
	// first corners
	ppu_wait_nmi();
	vram_adr(0x2C00 | ((3 << 5) | 6));
	vram_put(187 | 0xA400);
	
	vram_adr(0x2C00 | ((3 << 5) | 23));
	vram_put(187 | 0xE400);
	vram_adr(0x2C00 | ((11 << 5) | 6));
	vram_put(187 | 0x2400);
	vram_adr(0x2C00 | ((11 << 5) | 23));
	vram_put(187 | 0x6400);
	for(i = 0; i < 7; i++)
	{
		vram_adr(0x2C00 | ((4+i)<<5)  | 6);
		vram_put(189 | 0x2400);
		vram_adr(0x2C00 | ((4+i)<<5)  | 23);
		vram_put(189 | 0x6400);
	}
	
	for(i = 0; i < 16; i+=2)
	{
		vram_adr(0x2C00 | ((3)<<5)  | 7+i);
		vram_put(188 | 0xA400);
		vram_put(188 | 0xA400);
		vram_adr(0x2C00 | ((11)<<5)  | 7+i);
		vram_put(188 | 0x2400);
		vram_put(188 | 0x2400);
	}
	
	ppu_wait_nmi();
		for(j = 0; j < 16; j++)
		{
			if(j == 8)
			{
				ppu_wait_nmi();
			}
			vram_adr(0x2C00 | ((4)<<5)  | 7+j);
			vram_put(96 | 0x2400);
			vram_adr(0x2C00 | ((5)<<5)  | 7+j);
			vram_put(96 | 0x2400);
			vram_adr(0x2C00 | ((6)<<5)  | 7+j);
			vram_put(96 | 0x2400);
			vram_adr(0x2C00 | ((7)<<5)  | 7+j);
			vram_put(96 | 0x2400);
			vram_adr(0x2C00 | ((8)<<5)  | 7+j);
			vram_put(96 | 0x2400);
			vram_adr(0x2C00 | ((9)<<5)  | 7+j);
			vram_put(96 | 0x2400);
			vram_adr(0x2C00 | ((10)<<5)  | 7+j);
			vram_put(96 | 0x2400);
		}
	i = 0;
	j = 0;
}

void clearBox()
{
	vram_adr(0x2C00);
	vram_fill(10, 1024);
}

void boxNumber()
{
	num2 = num1;
	
	k = 0;
	if(!num1)
	{
		vram_adr(0x2C00 | ((bg3Y << 5) | bg3X + k));
		vram_put((111) | 0x2400);
		return;
	}
	
	while(num1)
	{
		++k;
		num1 = num1/10;
	}
	--k;
	num1 = num2;
	ppu_wait_nmi();
	
	while(num1)
	{
	//	ppu_wait_nmi();
	//	num2 = num1 % 10;
		num2 = snesmod(num1, 10);
		vram_adr(0x2C00 | ((bg3Y << 5) | bg3X + k));
		vram_put((((unsigned int)num2) + 111) | 0x2400);
		num1 = snesdiv(num1, 10);
		--bg3X;
	}
	
	k = 0;
}

void boxMessage(char *message)
{
	unsigned char k = 0;
	while(message[k] != '\0')
	{
		if(message[k] == 32)
		{
			vram_put(96 | 0x2400);
			++k;
			continue;
		}
		
		vram_put((message[k] + 63) | 0x2400);
		++k;
	}
	k = 0;
}

void boxStart()
{
	++tileUpdate;
	if(tileUpdate == 50)
	tileUpdate = 0;
	ppu_wait_nmi();
	if(tileUpdate < 25)
	{
		
		printBoxMessage(TEXT0_X, FIFTH + 2, "Press Start");
	}
	else
	{
		printBoxMessage(TEXT0_X, FIFTH + 2, "           ");
		
	}
}

void showButton()
{
	switch(button)
	{
		case(0):
			printMessage(2, 3, "d");
			break;
		case(1):
			printMessage(25, 3, "d");
			break;
		case(2):
			printMessage(25, 25, "d");
			break;
		case(3):
			printMessage(2, 25, "d");
			break;			
	}
}
#pragma code-name(pop)

// RESET EVERYTHING
#pragma code-name(push, "CODE9")
void resetPoker()
{
	
delete1 = 0;
pokerCash = 0;

alreadyIn = 0;
aiBet = 0;

alowSprite = 0; spX = 0; spY = 0; cardArived = 0; tileUpdate = 0; currentInput = 0; playersJoypad = 0;

// deckSize;   // Check this one
playState = 0;
selState = 0;
selPos = 0;
possibilities = 0;
currentBet = 0;
bg3X = 0;
bg3Y = 0;
minBetRaise = 0;
stepWidth = 0;
maxSteps = 0;
stepCount = 0;
num1 = 0; num2 = 0;
potentialBet = 0;
toCall = 0;
dealStates = 0;
sB = 0;
forbidRaise = 0;

communityCards[0] = 0; communityCards[1] = 0; communityCards[2] = 0; communityCards[3] = 0; communityCards[4] = 0;               // Check this one
cantRaise = 0;
whoPlaysGuard = 0;
flopFinished = 0;
riverFinished = 0;
inPlay = 0;

uu = 0;

dealingRound = 0;
xend = 0; yend = 0;
saveNP = 0; saveLP = 0;

combinationSize = 0;
communitySize = 0;

button = 0;
lastPlayed = 0;
lastToPlay = 0;
firstToPlay = 0;
nextToPlay = 0;

inTheHand = 0;
atTheTable = 0;
stageEnded = 0;
raise = 0;
//bigBlind = 0;
// buyIn = 0;

cSelX = 0, cSelY = 0;

selTile = 0;
selTile1 = 0;
selTile2 = 0;


pokState = 0;
subState = 0;
initPoker = 0;
universalPokerStates = 0;


vramX = 0; vramY = 0; signNum = 0; i = 0; j = 0; k = 0; availableSprid = 0; spriteX = 0; spriteY = 0; 

// Check these two
//extern unsigned char cardsDeck[52];
//extern unsigned char fix[15];


withMoney[0] = 0; withMoney[1] = 0; withMoney[2] = 0; withMoney[3] = 0;
withMoneyNum = 0;

withoutMoney[0] = 0; withoutMoney[1] = 0; withoutMoney[2] = 0; withoutMoney[3] = 0;
withoutMoneyNum = 0;

currentPot = 0;

sf1 = 0; sf2 = 0; f1 = 0; f2 = 0;

draw1 = 0; draw2 = 0; draw3 = 0;

toCompare[0] = 0; toCompare[1] = 0; toCompare[2] = 0; toCompare[3] = 0;
toCompareSize = 0;

index1 = 0; index2 = 0;

// Check these 3
combination1[0] = 0; combination1[1] = 0; combination1[2] = 0; combination1[3] = 0; combination1[4] = 0;
combination[0] = 0; combination[1] = 0; combination[2] = 0; combination[3] = 0; combination[4] = 0; combination[5] = 0; combination[6] = 0;
combinationStrength = 0;

stage = 0;

// Ai player variables
decision = 0;
sng = 0;
aiTime = 0;

aiCommunity[0] = 0; aiCommunity[1] = 0; aiCommunity[2] = 0; aiCommunity[3] = 0; aiCommunity[4] = 0;
aiCommSize = 0;

aiPocket[0] = 0; aiPocket[1] = 0;

//seats[0] = 0; seats[1] = 0; seats[2] = 0; seats[3] = 0;

//winner = 0;
}
void pokerPause()
{
	unsigned char ii, x, y;
	unsigned char rQ = 0;
	music_pause(1);
	ppu_wait_nmi();
	// upper line
	vram_adr((0x2C00 | ((8 << 5) | 10)));
	for(ii = 0; ii < 10 ; ii++)
	{
		vram_put(191 | 0x2400);
	}
	// bottom line
	vram_adr((0x2C00 | ((14 << 5) | 10)));
	for(ii = 0; ii < 10 ; ii++)
	{
		vram_put(191 | 0xA400);
	}
	
	y = 9;
	vram_adr((0x2C00 | ((8 << 5) | 9)));
	for(ii = 0; ii < 5 ; ii++)
	{
		vram_adr((0x2C00 | ((y << 5) | 9)));
		
		vram_put(192 | 0x2400);
		vram_put(193 | 0x2400);
		
		vram_adr((0x2C00 | ((y << 5) | 18)));
		vram_put(193 | 0x2400);
		vram_put(193 | 0x2400);
		vram_put(192 | 0x6400);
		
		++y;
		
	}
	
	vram_adr((0x2C00 | ((10 << 5) | 11)));
	vram_put(194 | 0x2400); // R
	vram_put(195 | 0x2400); // e
	vram_put(196 | 0x2400); // s
	vram_put(197 | 0x2400); // u
	vram_put(198 | 0x2400); // m
	vram_put(195 | 0x2400); // e
	vram_put(193 | 0x2400);
	vram_put(199 | 0x2400);
	vram_put(200 | 0x2400);
	
	vram_adr((0x2C00 | ((12 << 5) | 11)));
	vram_put(201 | 0x2400); // Q
	vram_put(197 | 0x2400); // u
	vram_put(202 | 0x2400); // i
	vram_put(203 | 0x2400); // t
	vram_put(193 | 0x2400); // blank
	vram_put(193 | 0x2400); // blank
	vram_put(193 | 0x2400); // blank
	
	y = 9;
	x = 11;
	
	ppu_wait_nmi();
	for(ii = 0; ii < 7; ii++)
	{
		vram_adr((0x2C00 | ((y << 5) | (x + ii))));	
		vram_put(193 | 0x2400); // blank
		vram_adr((0x2C00 | (((y+2) << 5) | (x + ii))));	
		vram_put(193 | 0x2400); // blank
		vram_adr((0x2C00 | (((y+4) << 5) | (x + ii))));	
		vram_put(193 | 0x2400); // blank
	}
	
	vram_adr((0x2C00 | ((8 << 5) | 9)));
	vram_put(190 | 0x2400);
	vram_adr((0x2C00 | ((8 << 5) | 20)));
	vram_put(190 | 0x6400);
	vram_adr((0x2C00 | ((14 << 5) | 9)));
	vram_put(190 | 0xA400);
	vram_adr((0x2C00 | ((14 << 5) | 20)));
	vram_put(190 | 0xE400);
	
	

	while(!(pad_trigger(0) & PAD_START))
	{
		if(pad_trigger(0) & PAD_UP)
		{
			vram_adr((0x2C00 | ((12 << 5) | 18)));
			vram_put(193 | 0x2400);
			vram_put(193 | 0x2400);
			
			vram_adr((0x2C00 | ((10 << 5) | 18)));
			vram_put( 199 | 0x2400);
			vram_put( 200 | 0x2400);
			rQ = 0;
		}
		else if(pad_trigger(0) & PAD_DOWN)
		{
			vram_adr((0x2C00 | ((10 << 5) | 18)));
			vram_put(193 | 0x2400);
			vram_put(193 | 0x2400);
			
			vram_adr((0x2C00 | ((12 << 5) | 18)));
			vram_put( 199 | 0x2400);
			vram_put( 200 | 0x2400);
			rQ = 1;
		}
	}
	if(rQ)
	{
		resetPoker();
		winLoseState = 0;
		screenState = 5;
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

void myDelay(unsigned char d)
{
	for(i = 0; i < d; i++)
	{
		if(pad_trigger(0) & PAD_START)
		{
			pokerPause();
		}
		ppu_wait_nmi();
	}
	i = 0;
}

// set conditions if present, go all in first bet, raise all bets, 7 and 2 hole cards, have 5000 on hand have 10000 on hand
void checkConditions()
{
	
}

// Set first bet at initialize poker and in prepare next hand, reset it when player 1 plays
// set raise all bets at initialize poker and prepare next hand, reset it every time player plays check call fold

//Call this function only if player 1 wins the hand, set achievments here, reset them in the end screen all but life time achievments
void checkAchievments()
{
/*	if(fbAllIn)
	{
		
	}
*/	
}

void playChips()
{
	if(potentialBet > (snesdiv(buyIn,3) << 1))
	{
		sfx_play(LARGE_BET);
	}
	else if(potentialBet > snesdiv(buyIn,3))
	{
		sfx_play(MEDIUM_BET);
	}
	else
	{
		sfx_play(SMALL_BET);
	}
}

void playAiChips()
{
	if(aiBet > (snesdiv(buyIn,3) << 1))
	{
		sfx_play(LARGE_BET);
	}
	else if(aiBet > snesdiv(buyIn,3))
	{
		sfx_play(MEDIUM_BET);
	}
	else
	{
		sfx_play(SMALL_BET);
	}
}

void playCall()
{
	if(toCall > (snesdiv(buyIn,3) << 1))
	{
		sfx_play(LARGE_BET);
	}
	else if(toCall > snesdiv(buyIn,3))
	{
		sfx_play(MEDIUM_BET);
	}
	else
	{
		sfx_play(SMALL_BET);
	}
}

void playAllIn()
{
	if(all > (snesdiv(buyIn,3) << 1))
	{
		sfx_play(LARGE_BET);
	}
	else if(all > snesdiv(buyIn,3))
	{
		sfx_play(MEDIUM_BET);
	}
	else
	{
		sfx_play(SMALL_BET);
	}
}
#pragma code-name(pop)
