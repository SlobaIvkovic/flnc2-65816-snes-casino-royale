#include "neslib.h"
#include "include/inplayachi.h"

#define CASH_SPRID 132
#define CASH_X 50
#define CASH_Y 8

#define BET_SPRID 52
#define BET_X 180
#define BET_Y 8

extern unsigned char vramX, vramY;

void trampoline();

#pragma wrapped-call(push, trampoline, 0x85)
void drawBetSelectorL();
void drawBetSelectorR();
void clearSelectorL();
void clearSelectorR();
void selectorDown();
void selectorUp();
void pointerClear();
void pointerDraw();
void addChip();
void loadGame();
void calculateBet();
static void drawCash();
void drawBet();
void clearChips();
void allIn();
void drawAllIn();
void fillColumn();
#pragma wrapped-call(pop)

unsigned char bet;
unsigned char upperLower;
unsigned char chipPointer;
unsigned char chipY[6];
unsigned char h;
extern unsigned char availableSprid;
extern unsigned char screenState;
extern unsigned char gameSel;
extern unsigned long cash;
extern unsigned long cash1;
extern unsigned long betAmount;
extern unsigned char digit;

extern unsigned char spX;
extern unsigned char spY;
extern unsigned long cash2;
unsigned char spXold;
unsigned char spYold;
unsigned char allInFlag;
unsigned int columnValue;



#define chipAdd(x, y) {\
						availableSprid = oam_spr(x, y, 0, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), availableSprid);\
						availableSprid = oam_spr(x+8, y, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), availableSprid);\
						availableSprid = oam_spr(x+16, y, 2, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), availableSprid);\
						availableSprid = oam_spr(x+24, y, 0, SPRITE_ATTR_FULL(0, 3, 0, 1, 0), availableSprid);\
						}\

#define clearPointer(a) {\
							vramX = a;\
							vramY = 22;\
							pointerClear();\
}\

#define drawPointer(a) {\
							vramX = a;\
							vramY = 22;\
							pointerDraw();\
}\

#define betSelector(x, y, z, u) {\
									vramX = x;\
									vramY = y;\
									drawBetSelectorL();\
									vramX = z;\
									vramY = u;\
									drawBetSelectorR();\
                                }\


#define clearSelector(x, y, z, u) {\
									vramX = x;\
									vramY = y;\
									clearSelectorL();\
									vramX = z;\
									vramY = u;\
									clearSelectorR();\
                                }


#pragma code-name(push, "CODE4")
void bettingScreen()
{
//	ppu_wait_nmi();
	
	if(pad_trigger(0) & PAD_UP)
	{
		if(upperLower == 1)
		{
			if(!bet)
			{
				clearSelector(23, 23, 29, 23);
			}
			else if(bet == 1)
			{
				clearSelector(10, 23, 20, 23);
			}
			else if(bet == 2)
			{
				clearSelector(1, 23, 7, 23);
			}
			drawPointer(chipPointer+3);
		}
		upperLower = 0;
	}
	else if(pad_trigger(0) & PAD_DOWN)
	{
		if(upperLower == 0)
		{
			bet = 0;
			betSelector(23, 23, 29, 23);
			clearPointer(chipPointer+3);
		}
		upperLower = 1;
	}

	if(!upperLower)
	{
		selectorUp();
	}
	else if(upperLower == 1)
	{
		
		selectorDown();
	}

}

void selectorDown()
{
		if(pad_trigger(0) & PAD_RIGHT)
		{
		//	vram_put(5 | 0b0010000000000000);
			if(!bet)
			{
				bet = 2;
				ppu_wait_nmi();
				// clear selector clear start
				clearSelector(23, 23, 29, 23);
				// draw selector all in
				betSelector( 1, 23, 7, 23);
			}
			else
			{
				// clear bet -1
				--bet;
				if(bet == 1)
				{
					ppu_wait_nmi();
					clearSelector(1, 23, 7, 23);
					betSelector(10, 23, 20, 23);
				}
				else
				{
					ppu_wait_nmi();
					clearSelector(10, 23, 20, 23);
					betSelector(23, 23, 29, 23);
				}
				// draw bet
			}
		}
		else if(pad_trigger(0) & PAD_LEFT)
		{
			if(bet == 2)
			{
				bet = 0;
				ppu_wait_nmi();
				clearSelector(1, 23, 7, 23);
				betSelector(23, 23, 29, 23);
			}
			else
			{
				++bet;
				if(bet == 1)
				{
					ppu_wait_nmi();
					clearSelector(23, 23, 29, 23);
					betSelector(10, 23, 20, 23);
				}
				else
				{
					ppu_wait_nmi();
					clearSelector(10, 23, 20, 23);
					betSelector(1, 23, 7, 23);
				}
			}
		}
		else if(pad_trigger(0) & PAD_START)
		{
			if(!bet)
			{
				if(cash == 0)
				{
					olchipper = 1;
				}
				if(betAmount >= 10 && gameSel == 1)
				loadGame();
				else if(gameSel == 0)
				loadGame();
				else if(gameSel == 3 && betAmount >= 10)
				loadGame();
				return;
				
			}
			else if(bet == 1)
			{
				olchipper = 0;
				oam_clear();
				chipY[0] = 0;
				chipY[1] = 0;
				chipY[2] = 0;
				chipY[3] = 0;
				chipY[4] = 0;
				chipY[5] = 0;
				cash = cash2;
				betAmount = 0;
				spXold = spX;
				spYold = spY;
				availableSprid = CASH_SPRID;
				spX = CASH_X;
				spY = CASH_Y + 8;
				cash1 = cash;
				drawCash();
				
				availableSprid = BET_SPRID;
				spX = BET_X;
				spY = BET_Y + 8;
				cash1 = betAmount;
				drawBet();
				spX = spXold;
				spY = spYold;
				
				clearChips();
				allInFlag = 0;
				return;
			}
			else
			{
				allIn();
				olchipper = 1;
				return;
				
			}
		}
		
	
}

void selectorUp()
{
	if(pad_trigger(0) & PAD_RIGHT)
	{
		if(chipPointer < 25)
		{
			chipPointer += 5;
			++h;
			ppu_wait_nmi();
			drawPointer(chipPointer+3);
			clearPointer(chipPointer+3 - 5);
			
		}
	}
	else if(pad_trigger(0) & PAD_LEFT)
	{
		if(chipPointer > 0)
		{
			--h;
			ppu_wait_nmi();
			clearPointer(chipPointer+3);
			chipPointer -= 5;
			ppu_wait_nmi();
			drawPointer(chipPointer+3);
		}
	}
	else if(pad_trigger(0) & PAD_B)
	{
		// draw chip
	
		calculateBet();
		
		spXold = spX;
		spYold = spY;
		spX = CASH_X;
		spY = CASH_Y + 8;
		cash1 = cash;
		availableSprid = CASH_SPRID;
		drawCash();
		ppu_wait_nmi();
		spX = BET_X;
		spY = 16;
		cash1 = betAmount;
		availableSprid = BET_SPRID;
		drawBet();
	
		spX = spXold;
		spY = spYold;
		
		
	}
	else
	{
//		ppu_wait_nmi();
		drawPointer(chipPointer+3);
	}
	return;
}

void drawBetSelectorL()
{
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(0 | 0b0010000000000000);
	vram_put(1 | 0b0010000000000000);
	++vramY;
//	vramX -= 2;
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(2 | 0b0010000000000000);
	++vramY;
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(2 | 0b0010000000000000);
	++vramY;
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(5 | 0b0010000000000000);
	vram_put(6 | 0b0010000000000000);
								
}

void drawBetSelectorR()
{
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(1 | 0b0010000000000000);
	vram_put(0 | 0b0110000000000000);
	++vramY;
//	vramX -= 2;
	++vramX;
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(2 | 0b0110000000000000);
	++vramY;
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(2 | 0b0110000000000000);
	++vramY;
	--vramX;
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(6 | 0b0110000000000000);
	vram_put(5 | 0b0110000000000000);
}

void clearSelectorL()
{
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(3 | 0b0010000000000000);
	vram_put(3 | 0b0010000000000000);
	++vramY;
//	vramX -= 2;
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(3 | 0b0010000000000000);
	++vramY;
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(3 | 0b0010000000000000);
	++vramY;
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(3 | 0b0010000000000000);
	vram_put(3 | 0b0010000000000000);
}

void clearSelectorR()
{
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(3 | 0b0010000000000000);
	vram_put(3 | 0b0110000000000000);
	++vramY;
//	vramX -= 2;
	++vramX;
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(3 | 0b0110000000000000);
	++vramY;
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(3 | 0b0110000000000000);
	++vramY;
	--vramX;
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(3 | 0b0110000000000000);
	vram_put(3 | 0b0110000000000000);
}

void pointerClear()
{
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(3);
}

void pointerDraw()
{
	vram_adr(0x5400 + (((vramY)<<5) | (vramX)));
	vram_put(4 | 0b0010000000000000);
}

void addChip()
{
	if(chipY[h] < 11)
	{
		ppu_wait_nmi();
		//	chipAdd(h*40 + 16, 130 - chipY[h]);
		vramX = h * 5 + 1;
		vramY = 15 - chipY[h];
		vram_adr(NTADR_A(vramX, vramY));
		vram_put(0 | 0b0000100000000000);
		vram_put(1 | 0b0000100000000000);
		vram_put(2 | 0b0000100000000000);
		vram_put(3 | 0b0000100000000000);
		chipY[h] += 1;  // current chip height
	}
	
}

void clearChips()
{
	ppu_wait_nmi();
	vram_adr(NTADR_A(0, 4));
	vram_fill(31, 836);
}


void calculateBet()
{
	switch(h)
	{
		case(0):
			if(cash >= 1)
			{
				cash -= 1;
				betAmount += 1;
				addChip();
			}
			break;
		case(1):
			if(cash >= 5)
			{
				cash -= 5;
				betAmount += 5;
				addChip();
			}
			break;
		case(2):
			if(cash >= 20)
			{
				cash -= 20;
				betAmount += 20;
				addChip();
			}
			break;
		case(3):
			if(cash >= 50)
			{
				cash -= 50;
				betAmount += 50;
				addChip();
			}
			break;
		case(4):
			if(cash >= 100)
			{
				cash -= 100;
				betAmount += 100;
				addChip();
			}
			break;
		case(5):
			if(cash >= 500)
			{
				cash -= 500;
				betAmount += 500;
				addChip();	
			}
			break;			
	}
}

static void drawCash()
{
	// delete first digit
	oam_spr(spX + 8, spY, 4, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid + 8);
	oam_spr(spX +8, spY + 8, 3, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid + 12);

	

	availableSprid = oam_spr(spX, spY, 4, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
	availableSprid = oam_spr(spX, spY+8, 15, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
	

	do
	{
		cash1 = cash1/10;
		spX += 8; 
		availableSprid = oam_spr(spX, spY, 3, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
		availableSprid = oam_spr(spX, spY+8, 3, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);	
	}while(cash1 != 0);

	oam_spr(spX + 16, spY, 3, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid + 4);
	oam_spr(spX + 16, spY + 8, 3, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid + 8);
	
	oam_spr(spX + 16, spY, 3, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid + 12);
	oam_spr(spX + 16, spY + 8, 3, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid +16);
	
	cash1 = cash;
	
	do
	{
		digit = cash1 % 10;
	//	availableSprid = oam_spr(spX, spY, digit + 5, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
		oam_spr(spX, spY, digit + 5, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
		availableSprid -= 4;
		oam_spr(spX, spY +8, digit + 16, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
		availableSprid -= 4;
	//	availableSprid = oam_spr(spX, spY+8, digit + 16, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
		spX -= 8;
		cash1 = cash1/10;
	
	}while(cash1 != 0);
	

}


void drawBet()
{
	availableSprid = oam_spr(spX, spY, 4, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
	availableSprid = oam_spr(spX, spY+8, 15, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
	

	do
	{
		cash1 = cash1/10;
		spX += 8; 
		availableSprid = oam_spr(spX, spY, 3, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
		availableSprid = oam_spr(spX, spY+8, 3, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
	}while(cash1 != 0);
	
	cash1 = betAmount;
	do
	{
		digit = cash1 % 10;
		oam_spr(spX, spY, digit + 5, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
		availableSprid -= 4;
		oam_spr(spX, spY+8, digit + 16, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
		availableSprid -= 4;
		spX -= 8;
		cash1 = cash1/10;
		
		
	}while(cash1 != 0);
	

}

void allIn()
{
	if(!allInFlag)
	{
		oam_clear();
		betAmount += cash;
		cash = 0;
		cash1 = cash;
		availableSprid = CASH_SPRID;
		spX = CASH_X;
		spY = CASH_Y + 8;
		drawCash();
	
		cash1 = betAmount;
		availableSprid = BET_SPRID;
		spX = BET_X;
		spY = BET_Y + 8;
		drawBet();
		cash1 = betAmount;
		drawAllIn();
		allInFlag = 1;
	}
	
}

#define chipYClear() {\
	chipY[0] = 0;\
	chipY[1] = 0;\
	chipY[2] = 0;\
	chipY[3] = 0;\
	chipY[4] = 0;\
	chipY[5] = 0;\
}\

void drawAllIn()
{
	clearChips();
	spXold = h;
	h = 5;
	chipYClear();
	ppu_wait_nmi();
	columnValue = 500;
	fillColumn();
	--h;
	columnValue = 100;
	fillColumn();
	--h;
	columnValue = 50;
	fillColumn();
	--h;
	columnValue = 20;
	fillColumn();
	--h;
	columnValue = 5;
	fillColumn();
	--h;
	columnValue = 1;
	fillColumn();
	h = spXold;
	
}

void fillColumn()
{
	while(cash1 >= columnValue)
	{
		cash1 -= columnValue;
		addChip();
	}
}
#pragma code-name(pop)
