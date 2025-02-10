#include "neslib.h"
#include "include/drawing_small_cards.h"
#include "include/solitaire_maps.h"
#include "include/defines.h"
#include "snesgss.h"
#include "include/shuffle.h"

#define MOVE 2
#define CONFIRM 3
#define CANCEL 4
#define SNAP 7
#define DEAL 5

#define SELECTOR_TABLEAU       0
#define SELECTOR_WASTE         1
#define SELECTOR_FOUNDATION    2
#define SELECTOR_STOCK         4
#define SELECTOR_REDEAL        5

#define FON_SIZE 13
#define TAB_SIZE 19

#define TABLEAU_ROW_SIZE 13


#define DIR_LEFT  1
#define DIR_RIGHT 2

#define SNAP_DISTANCE    12
#define STEPS_AFTER_SNAP SNAP_DISTANCE * 2 + 1
#define SPEED             1
#define MIDDLE_SPEED      2
#define SPEED_UP          3
#define SNAPING_SPEED     2

#define COLUMNS_START_X 2
#define COLUMNS_INCREMENT 4
#define COLUMNS_MAX_X (7 * COLUMNS_INCREMENT)

#define COLUMNS_START_Y 9

#define FOUNDATION_Y_SNAP 9
#define DELAY_DURATION 35
#define WATCHDOG_TIME 10500                    // 7500 coresponds to 25 non blocking dealy

#undef BG1_NT(x, y)
#define BG1_NT(x, y) (0x2400 | ((y<<5) | (x)))
#define BG2_NT(x, y) (0x2800 | ((y<<5) | (x)))
#define BG3_NT(x, y) (0x2C00 | ((y<<5) | (x)))


#undef SPADE
#undef DIAMOND
#define SPADE 2
#define DIAMOND 3

// Depending on where the card currently driven (or multiple cards) has been taken
// snap sequence behaves differently
#define placeSnapSprite(){\
							if(TABLEAU_SOURCE == source)\
							{\
								solSprite(g,h);\
							}\
							else if(WASTE_SOURCE == source || FOUNDATION_SOURCE == source)\
							{\
								spriteX = g;\
								spriteY = h;\
								solSpriteCard();\
							}\
						}\

// FOUNDATIONS SIZES START FROM 0, AT FIRST FOUNDATIONS ARE EMPTY
// SMALLEST TABLEAU AT FIRST HAS SIZE 1
// THIS POTENTIALY COULD MAKE CONFUSION WHEN RETREIVING CARDS FROM FOUNDATION AND TABLEAU ARRAYS

unsigned char frames;

extern char music1[];

unsigned char signNum2;  // used to compare driven card with the card at snaping place

unsigned int primitiveWatchdog;

unsigned char auxiliary;

unsigned char numberOfCardsSelected;
unsigned char foundationColumn;

unsigned char spriteX;
unsigned char spriteY;

static unsigned char solState;
static unsigned char m, n, l, g, h;
static unsigned char originG, originH;
extern unsigned char i, j, k, vramX, vramY, signNum;
extern unsigned char nextSprid;
static unsigned char size, firstOpened, selectorState, selectorX, selectorY, selectorTableau, selectorFoundation, selectorIndex;


static unsigned char tableau[133];
static unsigned char firstCardInColumn[7];
static unsigned char columnSizes[7];

static unsigned char foundations[52];
unsigned char foundationSizes[4];
unsigned char alowFoundationSnap;
unsigned char foundationNum;

static unsigned char stock[24];
unsigned char stockSize;
static unsigned char waste[24];
unsigned char wasteSize;
unsigned char wasteX;

#define SEL_ANIMATION_TIME 15
unsigned char tileUpdate;
unsigned char tile;
unsigned char cSelX, cSelY;


#pragma wrapped-call(push, trampoline, 0x85)
void coloredSelector();
void solClBase();
void solClMiddle();
void solHeBase();
void solHeMiddle();
void solDiBase();
void solDiMiddle();
void solSpBase();
void solSpMiddle();
void solCardDisplay();

void spriteClCard();
void spriteHeCard();
void spriteDiCard();
void spriteSpCard();
void solSpriteCard();
void clearColumnMess();
void tableauSelector();
void moveSelector();
void foundationSelector();
void wasteSelector();
void stockSelector();
void redealSelector();
void createSolSprite();
void moveSolSprite();
void clearMove();
void checkSnap();
void determineColumn();
void determineFoundation();
void drawCardTip(); 

// This function returns number of cards that are currently driven by joypad, 
// it depends on the source of the slected card/cards, source can be tableau, waste, or foundation
unsigned char numberOfDrivenCards();
// This functions returns value of the card that is currently driven and that possibly could be snaped into tableau/foundation
// below this card could be other cards, function depends on the card source tableau/foundation/waste 
unsigned char cardValue();

// function transfers card from tableau to foundation, transfer will only be alowed if snap to the one of the foundations is confirmed
// source tableau and destination foundation sizes will be updated and card value will be added to foundation row
// foundations array is considered as matrix where each row represents one foundations array with card values
void transferTabFoun();
// FOR CARD TRANSFERS BETWEEN TWO TABLEAUS
void drawStaticTip();
void columnUpdate();
void tableauUpdate();
void snapSequence();

// transfer card from stock to waste
void transferStockWaste();
void stockToWasteCardMove();
// when stock is empty and there are cards in the waste, flip cards and use them from the stock again
void turnStock();
// update waste and corresponding tableau if the card has been transfered from waste to one of the tableaus
void transferWasteTableau();
// transfer card from wast to foundation
void transferWasteFoundation();
// same from foundation to tableau
void transferFoundationTableau();
// duration display
void displaySeconds();
// reset solitaire game variables and switch back settings as they were set in crt0.s for betting screen and possibly another game played after solitaire
void resetSolitaire();
// Did you win
void checkWin();
// pause, used in case of deadlock
void pauseDialog();
// new pause dialog
void pauseChoice();
// text message display on the screen
void displayMessage(char *message);
#pragma wrapped-call(pop)



#define TABLEAU_SOURCE    0
#define FOUNDATION_SOURCE 1
#define WASTE_SOURCE      2

unsigned char source;
unsigned char destination;
unsigned char swTime;

unsigned char seconds;
unsigned char minutes;
unsigned char hours;
unsigned char clockCount;
unsigned char timeBuff;

// used to confirm snapping in foundation
unsigned char foundationSnap;
unsigned char tableauSnap;
unsigned char pause;
unsigned char measureTime; // mark if time after snap has to be measured hence does sprite movement should be alowed

unsigned char as;  //available sprid

extern unsigned char spX;
extern unsigned char spY;

unsigned char messHeight;
unsigned char dir;
unsigned char column;
unsigned char snapX;
unsigned char snapY;
unsigned char numSteps;
unsigned char distanceX;
unsigned char distanceY;

extern unsigned char cardsDeck[52];
extern unsigned char screenState;
extern unsigned char winLoseState;

extern unsigned long betAmount;
extern unsigned long winings;
extern unsigned long cash;

#define COMPENSATE_SHIFT_L_PX 8
#define COMPENSATE_SHIFT_D_PX 8
#define COMPENSATE_SHIFT_L_TILES 1
#define COMPENSATE_SHIFT_D_TILES 1

#define solSprite(x,y) {\
						n = y;\
						g = x;\
						createSolSprite();\
}\


#pragma code-name(push, "CODE4")
void solitaire()
{
//	cSelX = 1;
//	cSelY = 1;
//	coloredSelector();
//	clockCount = nesclock();
	if((unsigned char)(nesclock() - clockCount) > 59)
	{
	//	ppu_wait_nmi();
		clockCount = nesclock();
		
		++seconds;
		if(seconds == 60)
		{
			seconds = 0;
			++minutes;
			if(minutes == 60)
			{
				minutes = 0;
				++hours;
			}
			ppu_wait_nmi();
		}
		displaySeconds();
	
	}
	
	//	ppu_wait_nmi();
	//	vram_adr((0x2C00 | ((24 << 5) | 30)));
	//vram_put((((unsigned int)auxiliary) + 16) | 0x3000);	
	//l = 0;
	if(solState == 3)
	{
		moveSolSprite();
		if((pad_trigger(0) & PAD_B))
		{
			if(tableauSnap || foundationSnap)
			{
				sfx_play(SNAP);
				solState = 4;
			}
		}
		
	}
	else if(solState == 0)
	{
	//	sfx_play(DEAL);
		music_play(0x88, music1);
		size = 1;
		k = 0;
		for(m = 0; m < 115; m += TAB_SIZE)
		{
			columnSizes[firstOpened] = size;
			firstCardInColumn[firstOpened] = firstOpened;
			
			for(n = 0; n < size; n++)
			{
				tableau[m + n] = cardsDeck[k];
				++k;	
			}
			++size;
			++firstOpened;
		}
		//stock loaded with cards
		for(k = 28; k < 52; k++)
		{
			stock[k - 28] = cardsDeck[k];
		}
		// initialy stock holds 24 cards
		stockSize = 24;
		// initialy waste x is at 13
		wasteX = COLUMNS_START_X + 6;
		wasteSize = 0;
		vramX = COLUMNS_START_X;
		vramY = 2;
		ppu_wait_nmi();
		solBa();
		
		k = 0;
		// first draw all back cards
		for(m = COLUMNS_START_X; m < COLUMNS_MAX_X; m += COLUMNS_INCREMENT)
		{	
			
			for(n = 0; n < l; n++)
			{
				ppu_wait_nmi();
				vramX = m;
				vramY = n + COLUMNS_START_Y;
				solBa();
			}
			++l;
		}
		n = 0;
		l = 0;
		firstOpened = 0;
		for(m = COLUMNS_START_X; m < COLUMNS_MAX_X; m += COLUMNS_INCREMENT)
		{
			vramX = m;
			vramY = n + COLUMNS_START_Y;
			signNum = tableau[n * TAB_SIZE + firstCardInColumn[n]];
			ppu_wait_nmi();
			
			vramX = m;
			vramY = n + COLUMNS_START_Y;
		//	signNum = 0;
		//	solSpBase();
			solCardDisplay();
			++n;
		}
		
		m -= 8;
		--n;

	selectorX = COLUMNS_START_X;
	selectorY = COLUMNS_START_Y;
	solState = 1;
	}
	else if(solState == 1)
	{
		moveSelector();
	}
	else if(solState == 2)
	{
		solSprite(g, h);
	//	ppu_wait_nmi();
	
		solState = 3;
	}
	
	else if(solState == 4)
	{
		measureTime = 0;
		solState = 3;
		if(tableauSnap)
		{		
		
			if(WASTE_SOURCE == source)
			{
				// clear moving card here
				oam_clear();
				// draw it staticaly using this function
				transferWasteTableau();
				solState = 1;
				tableauSnap = 0;
				return;
			}
			if(FOUNDATION_SOURCE == source)
			{
				// clear moving card here
				oam_clear();
				// draw it staticaly using this function
				transferFoundationTableau();
				solState = 1;
				tableauSnap = 0;
				return;
			}
				// EVERYTHING BELOW IS TABLEAU TABLEAU TRANSFER
				// first draw it if it is the same column
				vramX = snapX >> 3;
				vramY = snapY >> 3;
				columnUpdate();
				oam_clear();
				if(column != selectorTableau)
				{
					tableauUpdate();
					// draw cards from position snapX snapY
					// rearange tablau[column * 13] and tableau[selectorTableau * 13]
				}
				//rearange and draw
				// or just draw
				
				// reset state to moving selector
			//	selectorIndex = 0;
				tableauSnap = 0; 
				solState = 1;
			
		}
		else if(foundationSnap)
		{
			if(WASTE_SOURCE == source)
			{
				oam_clear();
				// draw it staticaly using this function
				transferWasteFoundation();
				solState = 1;
				foundationSnap = 0;
			//	return;
			}
			else if(TABLEAU_SOURCE == source)
			{
				transferTabFoun();
				foundationSnap = 0;
				solState = 1;
			}
			checkWin();
		}
	}
	
}

void moveSelector()
{
/*	ppu_wait_nmi();
	cSelX = 0;
	cSelY = 0;
	coloredSelector();*/
	l = 0;
	if(pad_trigger(0) & PAD_START)
	{
	//	pauseDialog();
		pauseChoice();
	}
	if(selectorState == SELECTOR_TABLEAU)
	{
		tableauSelector();
	}
	else if(selectorState == SELECTOR_FOUNDATION)
	{
		foundationSelector();
	}
	else if(selectorState == SELECTOR_WASTE)
	{
		wasteSelector();
	}
	else if(selectorState == SELECTOR_STOCK)
	{
		stockSelector();
	}
	else
	{
	//	redealSelector();
	}
}

void tableauSelector()
{
	if(pad_trigger(0) & PAD_RIGHT)
	{
		sfx_play(MOVE);
		selectorIndex = 0;
		
		if(selectorTableau == 6)
		{
			selectorTableau = 0;
			selectorX = COLUMNS_START_X;
			selectorY = firstCardInColumn[selectorTableau] + COLUMNS_START_Y;
		}
		else
		{
			++selectorTableau;
			selectorX += COLUMNS_INCREMENT;
			selectorY = firstCardInColumn[selectorTableau] + COLUMNS_START_Y; // Move it above to save space
		}
	}
	else if(pad_trigger(0) & PAD_LEFT)
	{
		sfx_play(MOVE);
		selectorIndex = 0;

		if(selectorTableau == 0)
		{
			selectorTableau = 6;
			selectorX = 26;
			selectorY = firstCardInColumn[selectorTableau] + COLUMNS_START_Y;
		}
		else
		{
			--selectorTableau;
			selectorX -= COLUMNS_INCREMENT;
			selectorY = firstCardInColumn[selectorTableau] + COLUMNS_START_Y;
		}
	}
	else if(pad_trigger(0) & PAD_UP)
	{
		
		if(selectorIndex)
		{
			sfx_play(MOVE);
			--selectorY;
			--selectorIndex; 
		}
		else
		{
			sfx_play(MOVE);
			if(selectorTableau > 2)
			{
				selectorState = SELECTOR_FOUNDATION;
				selectorFoundation = selectorTableau - 3;
				return;
			}
			else if(selectorTableau == 0)
			{
				selectorState = SELECTOR_STOCK;
				selectorX = COLUMNS_START_X;
				selectorY = 2;
			}
			else
			{
				selectorState = SELECTOR_WASTE;
				selectorX = COLUMNS_START_X + 6;
				selectorY = 2;
			}
		}
		
	}
	else if(pad_trigger(0) & PAD_DOWN)
	{
		if((firstCardInColumn[selectorTableau] + selectorIndex) < (columnSizes[selectorTableau] - 1))
		{
			sfx_play(MOVE);
			++selectorY;
			++selectorIndex;
		}
	}
	else if(pad_trigger(0) & PAD_B)
	{
		//if there is a card in the column
		if(columnSizes[selectorTableau])
		{
			sfx_play(CONFIRM);
			// Card selected, delete selector
		//	vram_adr(BG3_NT(selectorX, selectorY));
		//	vram_put(0x00);
			oam_clear();
		// now in a state where card is moved, no more selector movement
			g = (selectorX << 3);
			h = (selectorY << 3);
			// These two are used if player decides to place card/cards back to where he took it
			originG = g;
			originH = h + 1; 
			// First draw a sprite on top of the background card, then delete card beneath it
			// the other way around will cause sprite to flicker because for a brief moment card will desapear then apear again as sprite
			signNum = tableau[(TAB_SIZE * selectorTableau) + firstCardInColumn[selectorTableau] + selectorIndex];
			solSprite(g, h);
			solState = 3;
			source = TABLEAU_SOURCE;

		}
		// selectorIndex marks which of opened cards has been chosen, 0 if first opened card loked from above, 1 is second etc
		// if selector index is positive that means that some opened cards has to be drawn on the background
		// backcard will not be shown because open card will be on top of it
		if(selectorIndex)
		{
			signNum = tableau[(TAB_SIZE * selectorTableau) + firstCardInColumn[selectorTableau] + selectorIndex - 1];
			vramY = firstCardInColumn[selectorTableau] + selectorIndex - 1;
			vramX = selectorTableau * COLUMNS_INCREMENT + COLUMNS_START_X;
			vramY += COLUMNS_START_Y;
			ppu_wait_nmi();
			// opened card above selected one is now shown, mess starts at the bottom of this card, which is vramY + 6
			solCardDisplay();
				
			// Now sprite is displayed on top of the background card, and background card which was one place above sprite card
			// is fully shown, when sprite card moves that card will be visible, but bellow that card is a mess from previous cards
			// mess starts at vramY + 6, mess size is equal to column size - (first opened + index) - 1, thats all
			messHeight = columnSizes[selectorTableau] - (firstCardInColumn[selectorTableau] + selectorIndex);
			vramY += 6;  
		
			ppu_wait_nmi();
			
		//	messHeight = 10;
			clearColumnMess();
			
			g = (selectorX << 3);
			h = (selectorY << 3);
			originG = g;
			originH = h + 1;
		}
		// if first opened card is chosen means that back card must be drawn if the size of the column is greater than 1
		// but only if there 
		else if(columnSizes[selectorTableau] > 1)
		{
			if(firstCardInColumn[selectorTableau] != 0)
			{
				vramY = firstCardInColumn[selectorTableau] - 1;
				vramY += COLUMNS_START_Y;
				vramX = selectorTableau * COLUMNS_INCREMENT + COLUMNS_START_X;
				ppu_wait_nmi();
				solBa();
				messHeight = columnSizes[selectorTableau] - (firstCardInColumn[selectorTableau] + selectorIndex);	
			}
			else
			{
				vramY = firstCardInColumn[selectorTableau];
				vramY += COLUMNS_START_Y;
				vramX = selectorTableau * COLUMNS_INCREMENT + COLUMNS_START_X;
				messHeight = columnSizes[selectorTableau] - (firstCardInColumn[selectorTableau] + selectorIndex) + 5;
			}
			
		
		//	messHeight = columnSizes[selectorTableau] - (firstCardInColumn[selectorTableau] + selectorIndex);
		//	vramY += 6;
			ppu_wait_nmi();	
			clearColumnMess();
			
			g = (selectorX << 3);
			h = (selectorY << 3);
			originG = g;
			originH = h + 1;
		}
		// if column size is equal to 1 when card is selected means that nothing was beneath the card and that area has to be cleared
		else
		{
			vramY = firstCardInColumn[selectorTableau];
			vramY += COLUMNS_START_Y;
			vramX = selectorTableau * COLUMNS_INCREMENT + COLUMNS_START_X;
			messHeight = columnSizes[selectorTableau] + 5;
			ppu_wait_nmi();
//			messClear(vramX, vramY, 10);
			clearColumnMess();
			g = (selectorX << 3);
			h = (selectorY << 3);
			originG = g;
			originH = h + 1;
			
		}
		
		return;
	}
	//ppu_wait_nmi();
	//vram_adr(BG3_NT(selectorX, selectorY));
	//vram_put(56 | 0b0011110000000000);
	ppu_wait_nmi();
	cSelX = selectorX << 3;
	cSelY = selectorY << 3;
	coloredSelector();
}

void foundationSelector()
{
	selectorY = 2;
	if(pad_trigger(0) & PAD_RIGHT)
	{
		sfx_play(MOVE);
		if(selectorFoundation == 3)
		{
			selectorFoundation = 0;
			selectorX = COLUMNS_START_X + 12;
		}
		else
		{
			++selectorFoundation;
			selectorX += COLUMNS_INCREMENT;
		}
	}
	else if(pad_trigger(0) & PAD_LEFT)
	{
		sfx_play(MOVE);
		
		if(selectorFoundation == 0)
		{
			selectorFoundation = 3;
			selectorX = wasteX;             // samae as tableau 7th column
			selectorState = SELECTOR_WASTE;
		//	return;
			
		}
		else
		{
			--selectorFoundation;
			selectorX -= COLUMNS_INCREMENT;
		}
	}
	else if(pad_trigger(0) & PAD_DOWN)
	{
		sfx_play(MOVE);
		selectorState = SELECTOR_TABLEAU;
		selectorTableau = selectorFoundation + 3;
		selectorY = firstCardInColumn[selectorTableau] + COLUMNS_START_Y;
		ppu_wait_nmi();
		return;
	}
		else if(pad_trigger(0) & PAD_B)
	{
		// select card if one available
		if(foundationSizes[selectorFoundation])
		{
			// If the selected card is the last one it the waste clear everything beneath it
			sfx_play(CONFIRM);
			oam_clear();
			signNum = foundations[FON_SIZE * selectorFoundation + foundationSizes[selectorFoundation] - 1];
			// now in a state where card is moved, no more selector movement
			g = (COLUMNS_START_X << 3) + 96 + ((selectorFoundation << 2) << 3) ;
			h = 16;
			originG = g;
			originH = h + 1;
			// First draw a sprite on top of the background card, then delete card beneath it
			// the other way around will cause sprite to flicker because for a brief moment card will desapear then apear again as sprite
			spriteX = g;
			spriteY = h;
			solSpriteCard();
			if(!(foundationSizes[selectorFoundation] - 1))
			{
				vramX = COLUMNS_START_X + 12 + (selectorFoundation << 2);
				vramY = 2;
				messHeight = 6;
				ppu_wait_nmi();
				clearColumnMess();
			}
			if(foundationSizes[selectorFoundation] - 1)
			{
				vramX = COLUMNS_START_X + 12 + (selectorFoundation << 2);
				vramY = 2;
				signNum = foundations[FON_SIZE * selectorFoundation + foundationSizes[selectorFoundation] - 2];
				ppu_wait_nmi();
				solCardDisplay();
			}
			signNum = foundations[FON_SIZE * selectorFoundation + foundationSizes[selectorFoundation] - 1];
			solState = 3;
			source = FOUNDATION_SOURCE;
			return;
		}
	}
	ppu_wait_nmi();
	cSelX = selectorX << 3;
	cSelY = selectorY << 3;
	coloredSelector();
}


void wasteSelector()
{
	if(pad_trigger(0) & PAD_RIGHT)
	{
		sfx_play(MOVE);
		selectorX = COLUMNS_START_X + 12;
		selectorState = SELECTOR_FOUNDATION;
		selectorFoundation = 0;
	//	return;
	}
	else if(pad_trigger(0) & PAD_LEFT)
	{
		sfx_play(MOVE);
		selectorX = COLUMNS_START_X;
		selectorState = SELECTOR_STOCK;
	}
	else if(pad_trigger(0) & PAD_DOWN)
	{
		sfx_play(MOVE);
		selectorState = SELECTOR_TABLEAU;
		selectorTableau = 1;
		selectorX = COLUMNS_START_X + COLUMNS_INCREMENT;
		selectorY = firstCardInColumn[selectorTableau] + COLUMNS_START_Y;
	}
	else if(pad_trigger(0) & PAD_B)
	{
		// select card if one available
		if(wasteSize)
		{
			sfx_play(CONFIRM);
			oam_clear();
			signNum = waste[wasteSize-1];
			// now in a state where card is moved, no more selector movement
			g = (COLUMNS_START_X << 3) + 48;
			h = 16;
			originG = g;
			originH = h + 1;
			// First draw a sprite on top of the background card, then delete card beneath it
			// the other way around will cause sprite to flicker because for a brief moment card will desapear then apear again as sprite
			spriteX = g;
			spriteY = h;
			solSpriteCard();
			
			// If the selected card is the last one it the waste clear everything beneath it
			if(!(wasteSize - 1))
			{
				vramX = COLUMNS_START_X + 6;
				vramY = 2;
				messHeight = 6;
				ppu_wait_nmi();
				clearColumnMess();
			}
			if(wasteSize-1)
			{
				vramX = COLUMNS_START_X + 6;
				vramY = 2;
				signNum = waste[wasteSize - 2];
				ppu_wait_nmi();
				solCardDisplay();
			}
			signNum = waste[wasteSize-1];
			solState = 3;
			source = WASTE_SOURCE;
			return;
		}
	}
	ppu_wait_nmi();
	cSelX = selectorX << 3;
	cSelY = selectorY << 3;
	coloredSelector();
}

void stockSelector()
{
	if(swTime)
	{
		
		transferStockWaste();
		return;
	}
	if(pad_trigger(0) & PAD_RIGHT)
	{
		sfx_play(MOVE);
		selectorX = wasteX;             // samae as tableau 7th column
		selectorState = SELECTOR_WASTE;
	}
	else if(pad_trigger(0) & PAD_DOWN)
	{
		sfx_play(MOVE);
		selectorX = COLUMNS_START_X;
		selectorTableau = 0;
		selectorY = firstCardInColumn[selectorTableau] + COLUMNS_START_Y;
		selectorState = SELECTOR_TABLEAU;
	}
	else if(pad_trigger(0) & PAD_B)
	{
		if(stockSize)
		{
			if(!swTime)
			{
			//	oam_clear();
				swTime = nesclock();
				if(!swTime)
				{
					++swTime;
				}	
			}
		//	transferStockWaste();
			
		}
		else
		{
			sfx_play(DEAL);
			turnStock();
			if(!swTime)
			{
			//	oam_clear();
				swTime = nesclock();
				if(!swTime)
				{
					++swTime;
				}	
			}
		}
		oam_clear();
		return;
	}
	else if(pad_trigger(0) & PAD_UP)
	{
	//	selectorX = COLUMNS_START_X;
	//	selectorY = 0;
	//	selectorState = SELECTOR_REDEAL;
		
	}
	ppu_wait_nmi();
	cSelX = selectorX << 3;
	cSelY = selectorY << 3;
	coloredSelector();
}

void pauseDialog()
{
	unsigned char rQ = 0;
	// darken the screen
	pal_bright(10);
	// draw dialog
	
	ppu_wait_nmi();
//	vram_adr((0x2C00 | ((0 << 5) | 00)));
//	displayMessage("PAUSE");
	vram_adr((0x2C00 | ((10 << 5) | 11)));
	vram_put(104 | 0x2400);
//	displayMessage("RESUME");
	vram_put( 98 | 0x2000);
	vram_put( 99 | 0x2000);
	vram_adr((0x2C00 | ((12 << 5) | 11)));
	displayMessage("QUIT ");
	while(!(pad_trigger(0) & PAD_START))
	{
		if(pad_trigger(0) & PAD_UP)
		{
			vram_adr((0x2C00 | ((12 << 5) | 17)));
			vram_put(89);
			vram_put(89);
			
			vram_adr((0x2C00 | ((10 << 5) | 17)));
			vram_put( 98 | 0x2000);
			vram_put( 99 | 0x2000);
			rQ = 0;
		}
		else if(pad_trigger(0) & PAD_DOWN)
		{
			vram_adr((0x2C00 | ((10 << 5) | 17)));
			vram_put(89);
			vram_put(89);
			
			vram_adr((0x2C00 | ((12 << 5) | 17)));
			vram_put( 98 | 0x2000);
			vram_put( 99 | 0x2000);
			rQ = 1;
		}
	}
	if(rQ)
	{
		resetSolitaire();
		screenState = 5;
	}
	else
	{
		vram_adr((0x2C00 | ((10 << 5) | 11)));
		vram_fill(16, 89);
	
		vram_adr((0x2C00 | ((12 << 5) | 11)));
		vram_fill(16, 89);
	}
	pal_bright(15);
	
}

void pauseChoice()
{
	unsigned char ii, x, y;
	unsigned char rQ = 0;
	music_pause(1);
	ppu_wait_nmi();
	// upper line
	vram_adr((0x2C00 | ((8 << 5) | 10)));
	for(ii = 0; ii < 10 ; ii++)
	{
		vram_put(101 | 0x2400);
	}
	// bottom line
	vram_adr((0x2C00 | ((14 << 5) | 10)));
	for(ii = 0; ii < 10 ; ii++)
	{
		vram_put(101 | 0xA400);
	}
	
	y = 9;
	vram_adr((0x2C00 | ((8 << 5) | 9)));
	for(ii = 0; ii < 5 ; ii++)
	{
		vram_adr((0x2C00 | ((y << 5) | 9)));
		
		vram_put(102 | 0x2400);
		vram_put(103 | 0x2400);
		
		vram_adr((0x2C00 | ((y << 5) | 18)));
		vram_put(103 | 0x2400);
		vram_put(103 | 0x2400);
		vram_put(102 | 0x6400);
		
		++y;
		
	}
	
	vram_adr((0x2C00 | ((10 << 5) | 11)));
	vram_put(104 | 0x2400); // R
	vram_put(105 | 0x2400); // e
	vram_put(106 | 0x2400); // s
	vram_put(107 | 0x2400); // u
	vram_put(108 | 0x2400); // m
	vram_put(105 | 0x2400); // e
	vram_put(103 | 0x2400);
	vram_put(109 | 0x2400);
	vram_put(110 | 0x2400);
	
	vram_adr((0x2C00 | ((12 << 5) | 11)));
	vram_put(111 | 0x2400); // Q
	vram_put(107 | 0x2400); // u
	vram_put(112 | 0x2400); // i
	vram_put(113 | 0x2400); // t
	vram_put(103 | 0x2400); // blank
	vram_put(103 | 0x2400); // blank
	vram_put(103 | 0x2400); // blank
	
	y = 9;
	x = 11;
	
	ppu_wait_nmi();
	for(ii = 0; ii < 7; ii++)
	{
		vram_adr((0x2C00 | ((y << 5) | (x + ii))));	
		vram_put(103 | 0x2400); // blank
		vram_adr((0x2C00 | (((y+2) << 5) | (x + ii))));	
		vram_put(103 | 0x2400); // blank
		vram_adr((0x2C00 | (((y+4) << 5) | (x + ii))));	
		vram_put(103 | 0x2400); // blank
	}
	
	vram_adr((0x2C00 | ((8 << 5) | 9)));
	vram_put(100 | 0x2400);
	vram_adr((0x2C00 | ((8 << 5) | 20)));
	vram_put(100 | 0x6400);
	vram_adr((0x2C00 | ((14 << 5) | 9)));
	vram_put(100 | 0xA400);
	vram_adr((0x2C00 | ((14 << 5) | 20)));
	vram_put(100 | 0xE400);
	
	

	while(!(pad_trigger(0) & PAD_START))
	{
		if(pad_trigger(0) & PAD_UP)
		{
			vram_adr((0x2C00 | ((12 << 5) | 18)));
			vram_put(103 | 0x2400);
			vram_put(103 | 0x2400);
			
			vram_adr((0x2C00 | ((10 << 5) | 18)));
			vram_put( 109 | 0x2400);
			vram_put( 110 | 0x2400);
			rQ = 0;
		}
		else if(pad_trigger(0) & PAD_DOWN)
		{
			vram_adr((0x2C00 | ((10 << 5) | 18)));
			vram_put(103 | 0x2400);
			vram_put(103 | 0x2400);
			
			vram_adr((0x2C00 | ((12 << 5) | 18)));
			vram_put( 109 | 0x2400);
			vram_put( 110 | 0x2400);
			rQ = 1;
		}
	}
	if(rQ)
	{
		resetSolitaire();
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

void redealSelector()
{
	if(pad_trigger(0) & PAD_DOWN)
	{
		selectorX = COLUMNS_START_X;
		selectorY = 1;
		selectorState = SELECTOR_STOCK;
	}
	else if(pad_trigger(0) & PAD_B)
	{
		// reset all solitaire related variables
		// and reset mode 1 related settings, solitaire has different mode 1 settings mainly because card sprites has to 16*16 for the solitaire
		resetSolitaire();
		screenState = 5;
	}
	ppu_wait_nmi();
	cSelX = selectorX << 3;
	cSelY = selectorY << 3;
	coloredSelector();
}

void createSolSprite()
{
	displaySeconds();
	nextSprid = 0;
	// selector tableau reci ce koja vrsta iz tableau niza treba da se uzme, vrste predstavljaju po jedan tableau
	// index govori koja karata po redu posle prve otvorene je uzeta
	
	// size govori koliko vrhova karata treba da se iscrta
	// treba iscrtati razliku izmedju velicine vrste i prve otvorene karte
	// broj vrhova jednak je size-1 - firstOpened, sto je columnSizes[selectorTableau] - firstCardInColumn[selectorTableau]
	// selektor indeks krece od nule i krece se izmedju prve i poslednje otvorene karte
//	n = 0;
	l = 0;
	// g i h su koordinate celog sprajta koji se pomera, ovaj sprajt moze biti jenda karta ili jedna karta zajedno sa vrhovima drugih karata
	// g i h koordinate se menjaju u funkciji moveSolSprite, funkcija menja ove koordinate samo pritiscima na strelice, funkcija je dozvoljena 
	// samo u odredjenom stanju soliter igre
	for(m = 0; m < columnSizes[selectorTableau] - firstCardInColumn[selectorTableau] - selectorIndex - 1; m++)
	{
		// sigNum odredjen je brojem table, izabranom kartom koja je odredjena indeksom i prvom otvorenom
		// selectorIndex je redni broj otvorene karte, nulta otvorena, prva otvorena, druga otvorena, treca otvorena....
		// firstCardInColumn je indeks mesta u tableau vrsti gde se nalazi prva otvorena karta
		// tako da se prvo treba dobiti odgovarajuca vrsta iz niza tableau
		// svaka vrsta pomerena je za 13 mesta od prethodne, tako da 13 treba pomnoziti sa selectorTableau
		// selectorTableau govori u kojoj se vrsti trenutno nalazimo i zavisi od pritisaka levo desno
		// kada smo na pocetku vrste treba se pomeriti prvo do prve otvorene karte, firstCardColumn niz govori na kom je indeksu u vrsti ta karta
		// kada se pomerimo do prve otvorene, treba se pomeriti do one koje smo izabrali a nju odredjuje selectorIndex 
		// selectorIndex se menja pritiscima gore dole
		// kada smo dosli do karte svaki sledeci vrh je pomeren za jedno mesto od prethodnog
		
		// uzmi odgovarajucu kartu iz tableau
		signNum = tableau[TAB_SIZE*selectorTableau + selectorIndex + firstCardInColumn[selectorTableau] + m];
		// zadaj x, g dalje moze da se menja u moveSolSprite funkciji, prvi put je zadat u tableauSelector funkciji kada je karta izabrana
		spriteX = g;
		// l svaki put inkrementira za 8, na kraju kada se funkcija zavrsi l + h bice Y za celu kartu
		// svaki sledeci vrh je pomeren od prethodng za 8 piksela, 0*8 = 0, 1*8 = 8, + h koji je prvi put zadat u selectorTableau kasnije se menja u moveSol
		// u moveSolSpriteFunkciji 
		spriteY = h + l;
		// zadaje se visoki broj sprida da bi kasniji vrhovi bilo preko prethodnih, svaki vrh trosi 2 sprida odnosno 8 jer je svaki sprid velicine 4 
		as = 140 - l;
	//	ppu_wait_nmi();
		// sada iscrtati vrh
		drawCardTip();
		l += 8;	
	}
	as = 0;
	
	signNum = tableau[TAB_SIZE*selectorTableau + columnSizes[selectorTableau] - 1];
	spriteX = g;
	spriteY = h+l;
	// iscrtati celu kartu
	solSpriteCard();
	
	
	// selectorY i selectorX reci ce gde sprajt pocinje
	// zadnja karta crta se po osnovu indeksa size - 1, 
}

void moveSolSprite()
{
	if(pad_trigger(0) & PAD_START)
	{
		pauseChoice();
	}
 if(measureTime)
 {
 	clearMove();
 	++primitiveWatchdog;
 	if(primitiveWatchdog > WATCHDOG_TIME)
 	{
 		measureTime = 0;
 		primitiveWatchdog = 0;
	 }
 	if((unsigned char)(nesclock() - pause) <= DELAY_DURATION)
 	{
 		return;
	}
	measureTime = 0;
	primitiveWatchdog = 0;
 }	
	if(pad_state(0) & PAD_LEFT)
	{
		if(g)
		g-= SPEED;
		dir = DIR_LEFT;
		if(pad_state(0) & PAD_R)
		{
			if(g>2)
			g -= SPEED_UP;
			if(numSteps < STEPS_AFTER_SNAP)
			numSteps += SPEED_UP; // 3
		}
		if(numSteps < STEPS_AFTER_SNAP)
		numSteps += SPEED;
	}
	else if(pad_state(0) & PAD_RIGHT)
	{
		if(g<224)
		g+=SPEED;
		dir = DIR_RIGHT;
		if(pad_state(0) & PAD_R)
		{
			if(g<222)
			g += SPEED_UP;
			if(numSteps < STEPS_AFTER_SNAP)
			numSteps += SPEED_UP;
		}
		if(numSteps < STEPS_AFTER_SNAP)
		numSteps += SPEED;
	}
	if(pad_state(0) & PAD_UP)
	{
		if(h)
		h-=SPEED;
		if(pad_state(0) & PAD_R)
		{
			if(h>2)
			h -= SPEED_UP;
			if(numSteps < STEPS_AFTER_SNAP)
			numSteps += SPEED_UP;
		}
		if(numSteps < STEPS_AFTER_SNAP)
		numSteps += SPEED;
	}
	else if(pad_state(0) & PAD_DOWN)
	{
		if((h+l)<209)
		h+=SPEED;
		if(pad_state(0) & PAD_R)
		{
			if((h+l)<205)
			h += SPEED_UP;
			if(numSteps < STEPS_AFTER_SNAP)
			numSteps += SPEED_UP;
		}
		if(numSteps < STEPS_AFTER_SNAP)
		numSteps += SPEED;
	}
	clearMove();
	
}

void clearMove()
{
	if(pad_trigger(0) & PAD_X)
	{
		sfx_play(CANCEL);
		//move whole sprite to the origin
		if(WASTE_SOURCE == source)
		{
			/*
			spriteX = originG;
			spriteY = originH;
			solSpriteCard();
			*/
			oam_clear();
			vramX = COLUMNS_START_X + 6;
			vramY = 2;
			ppu_wait_nmi();
			solCardDisplay();
		}
		else if(TABLEAU_SOURCE == source)
		{
			ppu_wait_nmi();
			g = originG;
			h = originH;
			solSprite(g, h);
			
			
			vramX = selectorX;
			vramY = selectorY;
			columnUpdate();
			oam_clear();
		}
		if(FOUNDATION_SOURCE == source)
		{
			/*
			spriteX = originG;
			spriteY = originH;
			solSpriteCard();
			*/
			oam_clear();
			vramX = COLUMNS_START_X + 12 + (selectorFoundation << 2);
			vramY = 2;
			ppu_wait_nmi();
			solCardDisplay();
		}
		solState = 1;
		return;	
	}
	ppu_wait_nmi();
	if(WASTE_SOURCE == source || FOUNDATION_SOURCE == source)
	{
		spriteX = g;
		spriteY = h;
		solSpriteCard();
	}
	else if(TABLEAU_SOURCE == source)
	{
		solSprite(g, h);
	}
//	ppu_wait_nmi();
	checkSnap();
}

/*
(1, 3, 0, 0, 0)
(0, 3, 0, 0, 1)
(1, 3, 0, 0, 1)
*/
void drawCardTip() 
{
//	as = 0;
	if(signNum >> 4 == CLOVER)
	{
		oam_big(as);
		as = oam_spr(spriteX, spriteY, spriteClMM[0], SPRITE_ATTR_FULL(2, 3, 0, 0, 0), as);
		oam_big(as);
		as = oam_spr(spriteX+16, spriteY, spriteClMM[1], SPRITE_ATTR_FULL(2, 3, 0, 0, 0), as);						
	}
	else if(signNum >> 4 == HEART)
	{
		oam_big(as);
		as = oam_spr(spriteX, spriteY, spriteHeMM[0], SPRITE_ATTR_FULL(1, 3, 0, 0, 0), as);
		oam_big(as);
		as = oam_spr(spriteX+16, spriteY, spriteHeMM[1], SPRITE_ATTR_FULL(1, 3, 0, 0, 0), as);	
	}
	else if(signNum >> 4 == DIAMOND)
	{
		oam_big(as);
		as = oam_spr(spriteX, spriteY, spriteDiMM[0], SPRITE_ATTR_FULL(0, 3, 0, 0, 1), as);
		oam_big(as);
		as = oam_spr(spriteX+16, spriteY, spriteDiMM[1], SPRITE_ATTR_FULL(0, 3, 0, 0, 1), as);		
	}
	else if(signNum >> 4 == SPADE)
	{
		oam_big(as);
		as = oam_spr(spriteX, spriteY, spriteSpMM[0], SPRITE_ATTR_FULL(1, 3, 0, 0, 1), as);
		oam_big(as);
		as = oam_spr(spriteX+16, spriteY, spriteSpMM[1], SPRITE_ATTR_FULL(1, 3, 0, 0, 1), as);	
	}
}

void checkSnap()
{
	
	determineColumn();
	snapY = (columnSizes[column] << 3) + (COLUMNS_START_Y << 3) + 1; // +1 because bg1 is scrolled naknadno
	if(TABLEAU_SOURCE == source)
	{
		if(column == selectorTableau)
		{
			snapY -= ((columnSizes[column] - 1) - (firstCardInColumn[column] + selectorIndex) + 1) * 8;
		}
	}
	if(numSteps >= STEPS_AFTER_SNAP)
	{
		if(g>=snapX)
		{
			distanceX = g - snapX;
		}
		else
		{
			distanceX = snapX - g;
		}
		
		if(h >= (snapY - 8))
		{
			distanceY = h - (snapY - 8);
		}
		else
		{
			distanceY = (snapY - 8) - h;
		}
		
		if((distanceX <= SNAP_DISTANCE) && (distanceY <= SNAP_DISTANCE))
		{
		//	solSprite(snapX, snapY);
			if(TABLEAU_SOURCE == source)
			{
				signNum = tableau[selectorTableau*TAB_SIZE+ firstCardInColumn[selectorTableau] + selectorIndex];
			}
			signNum2 = tableau[column * TAB_SIZE + columnSizes[column] - 1];
			// Check if difference between card colors and card values is exactly equal to 1
			if(((MAX((signNum2 >> 4), (signNum >> 4)) - MIN((signNum2 >> 4), (signNum >> 4)) == 1)
			|| (MAX((signNum2 >> 4), (signNum >> 4)) - MIN((signNum2 >> 4), (signNum >> 4)) == 3))
			&& (((signNum2 & 0x0F) > (signNum & 0x0F)) && (((signNum2 & 0x0F) - (signNum & 0x0F)) == 1))   )
			//(MAX((signNum2 & 0x0F), (signNum & 0x0F)) - MIN((signNum2 & 0x0F), (signNum & 0x0F)) == 1)   )
			{
				snapSequence();
				tableauSnap = 1;
				//	g = snapX;
				//	h = snapY;
				//	delay(15);
				pause = nesclock();
				measureTime = 1;
				numSteps = 0;
			}
			else if(!columnSizes[column] && ((signNum & 0x0F) == 0x0C))
			{
				snapSequence();
				tableauSnap = 1;
				//	g = snapX;
				//	h = snapY;
				//	delay(15);
				pause = nesclock();
				measureTime = 1;
				numSteps = 0;
			}
		}
		
	}
	else if(numSteps > 8)
	{
			tableauSnap = 0;
			foundationSnap = 0;
	}
	
	// set it here if all conditions come true this value will be set to 1
	alowFoundationSnap = 0;
	// first condition to be in upper half of the table
	if(h < 58)
	{
		
		snapY = FOUNDATION_Y_SNAP + 8;
		determineFoundation();
		//if more than one card drivend dont check for foundation snap
		// number of driven cards depends on the card source, either foundation, tableau or waste
		if(numberOfDrivenCards() == 1 || WASTE_SOURCE == source)
		{
			// determine if the sign in the foundation and the sign of driven card are the same and card value is exactly 1 greater
			// upper nibble of card value hods sign(clover, heart etc.) if the driven card is valid for snaping upper nibble wont change with -1
			
			// if foundation is empty          and     card is ACE
			if(TABLEAU_SOURCE == source)
			{
				if(!foundationSizes[foundationColumn] && !(cardValue() & 0x0F))
				{
					alowFoundationSnap = 1;	
				}
				// foundations size starts from 0 not from 1, every time card from foundation has to be retrieved it will be at size index in the row
				if(foundationSizes[foundationColumn])
				{
					if(foundations[13 * foundationColumn + foundationSizes[foundationColumn] - 1 ] == (cardValue() - 1))
					{
						alowFoundationSnap = 1;	
					} 
				}
			}
			else if(WASTE_SOURCE == source || FOUNDATION_SOURCE == source)
			{
				if(!foundationSizes[foundationColumn] && !(signNum & 0x0F))
				{
					alowFoundationSnap = 1;	
				}
				if(foundationSizes[foundationColumn])
				{
					if(foundations[13 * foundationColumn + foundationSizes[foundationColumn] - 1 ] == (signNum - 1))
					{
						alowFoundationSnap = 1;	
					} 
				}
			}
		}
	}
	
	
	
		// other conditions, foundation empty, foundation same sign but number exactly one smaller
		// if foundation empty, clear foundation sign 
	//	if(foundationEmpty) allow snapCheck
	//
	if(alowFoundationSnap)
	{  
		if(h < 58)
		{
		//	snapY = FOUNDATION_Y_SNAP;
		//	determineFoundation();
			if(numSteps >= STEPS_AFTER_SNAP)
			{
				if(g>=snapX)
				{
					distanceX = g - snapX;
				}
				else
				{
					distanceX = snapX - g;
				}
				if(h >= (snapY - 8))
				{
					distanceY = h - (snapY - 8);
				}
				else
				{
					distanceY = (snapY - 8) - h;
				}
				if((distanceX <= SNAP_DISTANCE) && (distanceY <= SNAP_DISTANCE))
				{
					//	solSprite(snapX, snapY);
					//		if(numSteps > 16)
					snapSequence();
					foundationSnap = 1;
					//	g = snapX;
					//	h = snapY;
					pause = nesclock();
					measureTime = 1;
					numSteps = 0;
				}
			}
			else
			{
			//	foundationSnap = 0;
			}
		}
	}
}

void determineFoundation()
{
	 if(g <= 136)
	 {
	 	foundationColumn = 0;
	 	snapX = 120;
	 }
	 else if(g > 136 && g <= 168)
	 {
	 	foundationColumn = 1;
	 	snapX = 152;
	 }
	 else if(g > 168 && g <= 200)
	 {
	 	foundationColumn = 2;
	 	snapX = 184;
	 }
	 else if(g > 200)
	 {
	 	foundationColumn = 3;
	 	snapX = 216;
	 }
	 snapX -= 8;
}

void determineColumn()
{
		if(g <= 40)
		{
			column = 0;
			snapX = 24;
		//	return;
		}
		else if(g > 40 && g <= 72)
		{
			column = 1;
			snapX = 56;
		//	return;
		}
		else if(g > 72 && g <= 104)
		{
			column = 2;
			snapX = 88;
		//	return;
		}
		else if(g > 104 && g <= 136)
		{
			column = 3;
			snapX = 120;
		//	return;
		}
		else if(g > 136 && g <= 168)
		{
			column = 4;
			snapX = 152;
		//	return;
		}
		else if(g > 168 && g <= 200)
		{
			column = 5;
			snapX = 184;
		//	return;
		}
		else if(g > 200)
		{
			column = 6;
			snapX = 216;
		//	return;
		}
	snapX -= 8;
}

void snapSequence()
{
	ppu_wait_nmi();
	if(g > snapX)
	{
		while(g > snapX)
		{
			if(h > snapY)
			{
				h -= SNAPING_SPEED;
			}
			else if(h < snapY)
			{
				h += SNAPING_SPEED;
			}
			g -= SNAPING_SPEED;
	
		//	solSprite(g, h);
			placeSnapSprite();
			ppu_wait_nmi();
		}
	}
	else if(g < snapX)
	{
		while(g < snapX)
		{
			if(h > snapY)
			{
				h -= SNAPING_SPEED;
			}
			else if(h < snapY)
			{
				h += SNAPING_SPEED;
			}
			g += SNAPING_SPEED;
		//	solSprite(g, h);
			placeSnapSprite();
			ppu_wait_nmi();
		}
	}
//	ppu_wait_nmi();
    if(h > snapY)
	{
		while(h > snapY)
		{
			if(g > snapX)
			{
				g -= SNAPING_SPEED;
			}
			else if(g < snapX)
			{
				g += SNAPING_SPEED; 
			}
			h -= SNAPING_SPEED;
		//	solSprite(g, h);
			placeSnapSprite();
			ppu_wait_nmi();
		}
	}
	if(h < snapY)
	{
		while(h < snapY)
		{
			if(g > snapX)
			{
				g -= SNAPING_SPEED;
			}
			else if(g < snapX)
			{
				g += SNAPING_SPEED;
			}
			h += SNAPING_SPEED;
		//	solSprite(g, h);
			placeSnapSprite();
			ppu_wait_nmi();
		}
	}
		g = snapX;
		h = snapY;
		placeSnapSprite();
	//	ppu_wait_nmi();

}



void spriteClCard()
{
	as = 0;
	oam_big(0);
	as = oam_spr(spriteX, spriteY, spriteClMM[0], SPRITE_ATTR_FULL(2, 3, 0, 0, 0), as);
	oam_big(as);
	as = oam_spr(spriteX+16, spriteY, spriteClMM[1], SPRITE_ATTR_FULL(2, 3, 0, 0, 0), as);
	oam_big(as);
	as = oam_spr(spriteX, spriteY + 16, spriteClMidd[(signNum & 0x0F)*2], SPRITE_ATTR_FULL(2, 3, 0, 0, 0), as);
	oam_big(as);
	as = oam_spr(spriteX+16, spriteY+ 16, spriteClMidd[(signNum & 0x0F)*2 + 1], SPRITE_ATTR_FULL(2, 3, 0, 0, 0), as);
	oam_big(as);
	as = oam_spr(spriteX, spriteY + 32, spriteClMM[4], SPRITE_ATTR_FULL(2, 3, 0, 0, 0), as);
	oam_big(as);
	as = oam_spr(spriteX+16, spriteY + 32, spriteClMM[5], SPRITE_ATTR_FULL(2, 3, 0, 0, 0), as);
}



void spriteHeCard()
{
	as = 0;
	oam_big(0);
	as = oam_spr(spriteX, spriteY, spriteHeMM[0], SPRITE_ATTR_FULL(1, 3, 0, 0, 0), as);
	oam_big(as);
	as = oam_spr(spriteX+16, spriteY, spriteHeMM[1], SPRITE_ATTR_FULL(1, 3, 0, 0, 0), as);
	oam_big(as);
	as = oam_spr(spriteX, spriteY + 16, spriteHeMidd[(signNum & 0x0F)*2], SPRITE_ATTR_FULL(1, 3, 0, 0, 0), as);
	oam_big(as);
	as = oam_spr(spriteX+16, spriteY+ 16, spriteHeMidd[(signNum & 0x0F)*2 + 1], SPRITE_ATTR_FULL(1, 3, 0, 0, 0), as);
	oam_big(as);
	as = oam_spr(spriteX, spriteY + 32, spriteHeMM[4], SPRITE_ATTR_FULL(1, 3, 0, 0, 0), as);
	oam_big(as);
	as = oam_spr(spriteX+16, spriteY + 32, spriteHeMM[5], SPRITE_ATTR_FULL(1, 3, 0, 0, 0), as);
}

void spriteDiCard()
{
	as = 0;
	oam_big(0);
	as = oam_spr(spriteX, spriteY, spriteDiMM[0], SPRITE_ATTR_FULL(0, 3, 0, 0, 1), as);
	oam_big(as);
	as = oam_spr(spriteX+16, spriteY, spriteDiMM[1], SPRITE_ATTR_FULL(0, 3, 0, 0, 1), as);
	oam_big(as);
	as = oam_spr(spriteX, spriteY + 16, spriteDiMidd[(signNum & 0x0F)*2], SPRITE_ATTR_FULL(0, 3, 0, 0, 1), as);
	oam_big(as);
	as = oam_spr(spriteX+16, spriteY+ 16, spriteDiMidd[(signNum & 0x0F)*2 + 1], SPRITE_ATTR_FULL(0, 3, 0, 0, 1), as);
	oam_big(as);
	as = oam_spr(spriteX, spriteY + 32, spriteDiMM[4], SPRITE_ATTR_FULL(0, 3, 0, 0, 1), as);
	oam_big(as);
	as = oam_spr(spriteX+16, spriteY + 32, spriteDiMM[5], SPRITE_ATTR_FULL(0, 3, 0, 0, 1), as);
}

void spriteSpCard()
{
	as = 0;
	oam_big(0);
	as = oam_spr(spriteX, spriteY, spriteSpMM[0], SPRITE_ATTR_FULL(1, 3, 0, 0, 1), as);
	oam_big(as);
	as = oam_spr(spriteX+16, spriteY, spriteSpMM[1], SPRITE_ATTR_FULL(1, 3, 0, 0, 1), as);
	oam_big(as);
	as = oam_spr(spriteX, spriteY + 16, spriteSpMidd[(signNum & 0x0F)*2], SPRITE_ATTR_FULL(1, 3, 0, 0, 1), as);
	oam_big(as);
	as = oam_spr(spriteX+16, spriteY+ 16, spriteSpMidd[(signNum & 0x0F)*2 + 1], SPRITE_ATTR_FULL(1, 3, 0, 0, 1), as);
	oam_big(as);
	as = oam_spr(spriteX, spriteY + 32, spriteSpMM[4], SPRITE_ATTR_FULL(1, 3, 0, 0, 1), as);
	oam_big(as);
	as = oam_spr(spriteX+16, spriteY + 32, spriteSpMM[5], SPRITE_ATTR_FULL(1, 3, 0, 0, 1), as);
}

void solSpriteCard()
{
	switch(signNum >> 4)
	{
		case(CLOVER):
			spriteClCard();
			break;
		case(HEART):
			spriteHeCard();
			break;
		case(DIAMOND):
			spriteDiCard();
			break;
		case(SPADE):
			spriteSpCard();
			break;			
	}
}


// parameters start xy, and mess height
void clearColumnMess()
{
	for(i = 0; i < messHeight; i++)
	{
		vram_adr(BG1_NT(vramX, vramY));
		vram_put(4);
		vram_put(4);
		vram_put(4);
		vram_put(4);
		++vramY;
	//	vramX -= 5;
		
	}
	i = 0;
}

void drawStaticTip()
{
		vram_adr(BG1_NT(vramX, vramY));
		for(i = 0; i < 4; i++)
		{	
			if(signNum >> 4 == CLOVER)
			{
				vram_put(solClMM[i]);
			}
			else if(signNum >> 4 == HEART)
			{
				vram_put(solHeMM[i]);
			}
			else if(signNum >> 4 == DIAMOND)
			{
				vram_put(solDiMM[i]);
			}
			else if(signNum >> 4== SPADE)
			{
				vram_put(solSpMM[i]);
			}
		}
		i = 0;
}

unsigned char numberOfDrivenCards()
{
	if(source == TABLEAU_SOURCE)
	{
		return columnSizes[selectorTableau] - (firstCardInColumn[selectorTableau] + selectorIndex);
	}
}

unsigned char cardValue()
{
	if(source == TABLEAU_SOURCE)
	{
		return tableau[TAB_SIZE * selectorTableau + firstCardInColumn[selectorTableau] + selectorIndex];
	}
}

void transferTabFoun()
{
	// draw card on foundation
	vramY = 2;
	vramX = snapX >> 3;
	signNum = cardValue();
	ppu_wait_nmi();
	solCardDisplay();
	oam_clear();
	// store card value at current foundation size
	foundations[foundationColumn * 13 + foundationSizes[foundationColumn]] = signNum;
//	signNum = foundations[1];
	vramX = 0;
	vramY = 0;
	
//	solCardDisplay();
//	ppu_wait_nmi();
	// increment foundation size
	++foundationSizes[foundationColumn];
	// decrement tableau size
	--columnSizes[selectorTableau];
	if(!selectorIndex && columnSizes[selectorTableau] )
	{
		vramX = COLUMNS_START_X + COLUMNS_INCREMENT * selectorTableau;
		vramY = COLUMNS_START_Y + firstCardInColumn[selectorTableau] - 1;
		signNum = tableau[TAB_SIZE * selectorTableau + firstCardInColumn[selectorTableau] - 1];
		ppu_wait_nmi();
		solCardDisplay();
		--firstCardInColumn[selectorTableau];
		--selectorY;
	}
		else if(selectorIndex && columnSizes[selectorTableau])
	{
		--selectorY;
		--selectorIndex;
	}
}


// Two functions are neccessary when transfering card from one tableau to another
// One to update tableau from which the card has been taken
// And another one to update tableau to which the card has been transfered, tableauUpdate and columnUpdate, column == tableau in this case
//----------------------------------------------------------------------------------------------------------------------------------------
void tableauUpdate()
{
	k = 0;
	while((firstCardInColumn[selectorTableau] + selectorIndex + k) < (columnSizes[selectorTableau]))
	{
		tableau[TAB_SIZE * column + columnSizes[column] + k] = tableau[TAB_SIZE * selectorTableau + firstCardInColumn[selectorTableau] + selectorIndex + k];
	//	++columnSizes[column];
	//	--columnSizes[selectorTableau];
		++k;
	}
	columnSizes[column] += k;
	columnSizes[selectorTableau] -= k;
	
	
	// if the first opened card from the source tableau has been taken and there is closed card beneath taken card
	if(!selectorIndex && columnSizes[selectorTableau] )
	{
		vramX = COLUMNS_START_X + COLUMNS_INCREMENT * selectorTableau;
		vramY = COLUMNS_START_Y + firstCardInColumn[selectorTableau] - 1;
		signNum = tableau[TAB_SIZE * selectorTableau + firstCardInColumn[selectorTableau] - 1];
		ppu_wait_nmi();
		solCardDisplay();
		--firstCardInColumn[selectorTableau];
		--selectorY;
	}
	else if(selectorIndex && columnSizes[selectorTableau])
	{
		--selectorY;
		--selectorIndex;
	}
	
	k = 0;
		
//	}
	
}

void columnUpdate()
{
	k = 0;
	ppu_wait_nmi();
	while((firstCardInColumn[selectorTableau] + selectorIndex + k) < (columnSizes[selectorTableau] - 1))
	{
		ppu_wait_nmi();
		signNum = tableau[TAB_SIZE * selectorTableau + firstCardInColumn[selectorTableau] + selectorIndex + k];
		drawStaticTip();
		++vramY;
		++k;
	}
	ppu_wait_nmi();
	signNum = tableau[TAB_SIZE * selectorTableau + firstCardInColumn[selectorTableau] + selectorIndex + k];
	solCardDisplay();
	k = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------


void transferFounTab()
{
	// draw card at current tableau
	// increment tableau size
	// decrement foundation size	
}

void transferStockWaste()
{

/*
	stock[k - 27] = cardsDeck[k];
		}
		// initialy stock holds 24 cards
		stockSize = 24;
		// initialy waste x is at 13
		wasteX = COLUMNS_START_X + 6;
		wasteSize = 0;
*/
//	oam_clear();	
//	auxiliary = 0;
	
	if(stockSize && (auxiliary < 3))
	{
		// auxiliary will increment and stockSize will decrement only when card finishes movement from stock to waste
		// this function will call stockCardMovement function as long as there is cards on the stock and less than 3 transfers ocured
		signNum = waste[wasteSize] = stock[stockSize - 1];
		
		stockToWasteCardMove();
	
	}
	else
	{
		swTime = 0;
		auxiliary = 0;
	}

}

void stockToWasteCardMove()
{
	
//	ppu_wait_nmi();
	
	++frames;
	if(frames < 5)
	{
		if(frames == 1)
		sfx_play(6);
	    spriteX = COLUMNS_START_X << 3;
	    spriteY = 16;
	 //   ppu_wait_nmi();
	    solSpriteCard();
	    if(stockSize == 1)
		{
			//vramX, vramY mess start, messHeight = 6
			vramX = COLUMNS_START_X;
			vramY = 2;
			messHeight = 6;
			ppu_wait_nmi();
			clearColumnMess();
		}
	}
	ppu_wait_nmi();
	if(frames > 5 && frames < 10)
	{
	//delay(15);
	spriteX = 48;
	ppu_wait_nmi();
	solSpriteCard();
   }
   else if(frames > 10 && frames < 15)
   {
	//delay(2);
	vramX = COLUMNS_START_X + 6;
	vramY = 2;
	
	oam_clear();	
	
	ppu_wait_nmi();
	solCardDisplay();
   }
   else if(frames > 15)
   {
	//delay(10);
	// Card finished movement from the stock to the waste, decrement stock size increment waste size,
	// so transfer function could update stock and waste
	++wasteSize;
	++auxiliary;
	--stockSize;
	
	// set movement starting time for the next card, swTime variable is also used as condition for the transfer function to be called
	// make sure this variable is true as long as card need to be transfered from stock to waste
	swTime = nesclock();
	if(!swTime)
	++swTime;
	frames = 0;
   }
   
	
	
}

void turnStock()
{
	if(wasteSize)
	{
	ppu_wait_nmi();
	vramX = COLUMNS_START_X + 6;
	vramY = 2;
	messHeight = 6;
	clearColumnMess();
	ppu_wait_nmi();
	vramX = COLUMNS_START_X;
	vramY = 2;
	solBa();
	delay(5);
	stockSize = wasteSize;
	for(k = 0; k < wasteSize; k++)
	{
		stock[k] = waste[wasteSize - k - 1];
	}
	k = 0;
	wasteSize = 0;
//	transferStockWaste();
	}
}

void coloredSelector()
{
	as = 244;
	as = oam_spr(cSelX, cSelY, tile, SPRITE_ATTR_FULL(4, 3, 0, 0, 0), as);
	oam_spr(cSelX+24, cSelY, tile, SPRITE_ATTR_FULL(4, 3, 0, 1, 0), as);
	
	++tileUpdate;
	if(tileUpdate > SEL_ANIMATION_TIME)
	{
		tileUpdate = 0;
		++tile;
		if(tile == 10)
		tile = 24;
		if(tile == 26)
		tile = 8;
	}
} 

void transferWasteTableau()
{
	--wasteSize;
	tableau[TAB_SIZE * column + columnSizes[column] ] = waste[wasteSize];
	// static card should be drawn on the snapX * 8 snapY * 8 position
	vramX = snapX >> 3;
	vramY = snapY >> 3;
	signNum = waste[wasteSize];
	ppu_wait_nmi();
	solCardDisplay();
	++columnSizes[column];
	//
	

}

void transferWasteFoundation()
{
	--wasteSize;
	foundations[foundationColumn * 13 + foundationSizes[foundationColumn]] = signNum;
	// static card should be drawn on the snapX * 8 snapY * 8 position
	vramX = snapX >> 3;
	vramY = snapY >> 3;
	signNum = waste[wasteSize];
	ppu_wait_nmi();
	solCardDisplay();
	++foundationSizes[foundationColumn];
	//
	

}

void transferFoundationTableau()
{
	//--wasteSize;
	--foundationSizes[selectorFoundation];
	//tableau[13 * column + columnSizes[column] ] = waste[wasteSize];
	tableau[TAB_SIZE * column + columnSizes[column] ] = foundations[FON_SIZE * selectorFoundation + foundationSizes[selectorFoundation]];
	// static card should be drawn on the snapX * 8 snapY * 8 position
	vramX = snapX >> 3;
	vramY = snapY >> 3;
	//signNum = waste[wasteSize];
	signNum = foundations[FON_SIZE * selectorFoundation + foundationSizes[selectorFoundation]];
	ppu_wait_nmi();
	solCardDisplay();
	++columnSizes[column];
	//
}

void displayMessage(char *message)
{
	unsigned char k = 0;
	while(message[k] != '\0')
	{
		vram_put((message[k] - 32) | 0x2000);
		++k;
	}
	k = 0;
}

void displaySeconds()
{
//	position = (0x2C00 | ((12 << 5) | 30));
//	ppu_wait_nmi();
	vram_adr((0x2C00 | ((1 << 5) | 29)));
	timeBuff = seconds;
	vram_put((((unsigned int)timeBuff % 10) + 16) | 0x2000);
	vram_adr((0x2C00 | ((1 << 5) | 28)));
	timeBuff = seconds / 10;
	vram_put((((unsigned int)timeBuff % 10) + 16) | 0x2000);
	//
	vram_adr((0x2C00 | ((1 << 5) | 27)));
	vram_put(26 | 0x2000);
	//
	vram_adr((0x2C00 | ((1 << 5) | 26)));
	timeBuff = minutes;
	vram_put((((unsigned int)timeBuff % 10) + 16) | 0x2000);
	vram_adr((0x2C00 | ((1 << 5) | 25)));
	timeBuff = minutes / 10;
	vram_put((((unsigned int)timeBuff % 10) + 16) | 0x2000);
	//
	vram_adr((0x2C00 | ((1 << 5) | 24)));
	vram_put(26 | 0x2000);
	//
	vram_adr((0x2C00 | ((1 << 5) | 23)));
	timeBuff = hours;
	vram_put((((unsigned int)timeBuff % 10) + 16) | 0x2000);
	

}

void resetSolitaire()
{
	// registers 2101, 210B, 210C, 2107, 2108, 2109 has to switch back to its original settings, as they were set in crt0.s
	// reset BG3 charset base address
	// lda #%00110001
	//sta BGCHRADDR 210b
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
	
	// NOW clear garbage from BG1 map
	vram_adr(0);
	vram_fill(0, 66);
	vram_adr(0x400);
	vram_fill(0, 2048);
	
	//-------------------------------- RESET ALL THE VARIABLES RELEVANT FOR THE SOLITAIRE GAME ----------------------------------------
signNum2 = 0;

frames = 0;

primitiveWatchdog = 0;

auxiliary = 0;

numberOfCardsSelected = 0;
foundationColumn = 0;

spriteX = 0;
spriteY = 0;

solState = 0;
m = 0; n=0; l = 0; g = 0; h = 0;
for(m = 0; m < 7; m++)
{
	columnSizes[m] = 0;
}

for(m = 0; m < 4; m++)
{
	foundationSizes[m] = 0;
	
}

for(m = 0; m < 52; m++)
{
	foundations[m] = 0;
}

m = 0;

originG = 0; originH = 0;
size = 0;
firstOpened = 0;
selectorState = 0; selectorX = 0; selectorY = 0; selectorTableau = 0; selectorFoundation = 0; selectorIndex = 0;



alowFoundationSnap = 0;
foundationNum = 0;
stockSize = 0;
wasteSize = 0;
wasteX = 0;

tileUpdate = 0;
// tile; // set in load function
cSelX = 0;
cSelY = 0;


source = 0;
destination = 0;

//seconds = 0;
//minutes = 0;
//hours = 0;
clockCount = 0;
timeBuff = 0;

// used to confirm snapping in foundation
foundationSnap = 0;
tableauSnap = 0;
pause = 0;
measureTime = 0; // mark if time after snap has to be measured hence does sprite movement should be alowed

as = 0;  //available sprid

messHeight = 0;
dir = 0;
column = 0;
snapX = 0;
snapY = 0;
numSteps = 0;
distanceX = 0;
distanceY = 0;
shuffle();
}

void checkWin()
{
	// foundationSizes[4];
	if(foundationSizes[0] == 13 && foundationSizes[1] == 13 && foundationSizes[2] == 13 && foundationSizes[3] == 13)
	{
		winLoseState = 1;
		winings = betAmount * 3;
		cash += winings;
		screenState = 5;
		resetSolitaire();
	}
}

#pragma code-name(pop)
