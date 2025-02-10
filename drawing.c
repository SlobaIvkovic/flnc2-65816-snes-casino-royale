#include "neslib.h"
#include "include/drawing.h"
#include "include/defines.h"
#include "include/card_maps.h"



extern unsigned char signNum;
extern unsigned char vramX;
extern unsigned char vramY;

extern unsigned char i;    // to be used only in for loops
extern unsigned char j;    // only in for loops
extern unsigned char k;    // only in for loops

extern unsigned char availableSprid;
extern unsigned char nextSprid;
extern unsigned char spX;              // card metasprite current poss
extern unsigned char spY;

extern unsigned long cash;
extern unsigned long cash1;
unsigned char digit;


extern unsigned char auxVX;
extern unsigned char auxVY;
extern unsigned char hiddenCard;

unsigned char diAttr;
unsigned char spAttr;

#pragma code-name(push, "CODE4")
void displayCard()
{
	if(signNum >> 4 == CLOVER)
	{
		displayCloverCard();
	}
	else if(signNum >> 4 == HEART)
	{
	 	displayHeartCard();
	}
	else if(signNum >> 4 == SPADE)
	{
		displaySpadeCard();
	}
	else if(signNum >> 4 == DIAMOND)
	{
		displayDiamondCard();
	}
}


void displayCloverCard()
{
	createCloverBase();
	insertCloverMiddle();	
}

void displayHeartCard()
{
	createHeartBase();
	insertHeartMiddle();	
}


void displayDiamondCard()
{
	createDiamondBase();
	insertDiamondMiddle();	
}


void displaySpadeCard()
{
	createSpadeBase();
	insertSpadeMiddle();	
}

void drawBackCardStatic()
{
	k = 0;
	for(i = 0; i < 7; i++)
	{
		vram_adr(NTADR_A(vramX, vramY));
		for(j = 0; j < 5; j++)
		{
			vram_put(mmBc[k]);        // here every vram write increments x
			++k;		
		}
		++vramY;
	}
	vramX += 5;
}

void createCloverBase()
{
	k = 0;
	
	
	for(i = 0; i < 7; i++)
	{
		if(i == 2)
		ppu_wait_nmi();
		vram_adr(NTADR_A(vramX, vramY));
		for(j = 0; j < 5; j++)
		{
			      // here every vram write increments x
		
			if((j == 3) && (i == 2))
			{
				if((signNum & 0x0F) == 9)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x0818);
				}
				else if((signNum & 0x0F) == 10)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x081A);
				}
				else if((signNum & 0x0F) == 11)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x081D);
				}
				else if((signNum & 0x0F) == 12)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x081F);
				}
				else
				{
					vram_put(mmCl[k]);
				}
			}
			else if((j == 1) && (i == 2))
			{
				if((signNum & 0x0F) == 9)
				{
					vram_adr(NTADR_A(vramX + 1, vramY));
					vram_put(0x0816);
				}
				else
				{
					vram_put(mmCl[k]);
				}	
			}
			else
			{
				vram_put(mmCl[k]);  
			}		
				++k;
		}
		++vramY;
	}
	vramX += 5;
}


void insertCloverMiddle()
{
	ppu_wait_nmi();
	vram_adr(NTADR_A(vramX - 3, vramY - 5));
	// write middle map index
	vram_put(clNumbers[(signNum &0x0f) *5]);

	vram_adr(NTADR_A(vramX - 4, vramY - 4));
	// write middle map index determined by number to be displayed 3 times
	vram_put(clNumbers[(signNum & 0x0f) * 5 + 1]); // at X and Y, update X after 3rd write
	vram_put(clNumbers[(signNum & 0x0f) * 5 + 2]); 
	vram_put(clNumbers[(signNum & 0x0f) * 5 + 3]);
	
	vram_adr(NTADR_A(vramX - 3, vramY -3));
	vram_put(clNumbers[(signNum & 0x0F) * 5 + 4]); // update x and y after write
}

void createHeartBase()
{
	k = 0;
	for(i = 0; i < 7; i++)
	{
		if(i == 2)
		ppu_wait_nmi();
		vram_adr(NTADR_A(vramX, vramY));
		for(j = 0; j < 5; j++)
		{
			      // here every vram write increments x
		
			if((j == 3) && (i == 2))
			{
				if((signNum & 0x0F) == 9)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x0476);
				}
				else if((signNum & 0x0F) == 10)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x0478);
				}
				else if((signNum & 0x0F) == 11)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x047B);
				}
				else if((signNum & 0x0F) == 12)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x047E);
				}
				else if((signNum & 0x0F ) != 1 && (signNum & 0X0F )!= 2)
				{
					vram_put(0x0473);
				}
				else
				{
					vram_put(mmHe[k]);
				}
			}
			else if((j == 1) && (i == 2))
			{
				if((signNum & 0x0F) == 9)
				{
					vram_adr(NTADR_A(vramX + 1, vramY));
					vram_put(0x0474);
				}
				if((signNum & 0x0F) == 11)
				{
					vram_adr(NTADR_A(vramX + 1, vramY));
					vram_put(0x0479);
				}
				if((signNum & 0x0F) == 12)
				{
					vram_adr(NTADR_A(vramX + 1, vramY));
					vram_put(0x047C);
				}
				else
				{
					vram_put(mmHe[k]);
				}	
			}
			else
			{
				vram_put(mmHe[k]);  
			}		
				++k;
		}
		++vramY;
	}
	vramX += 5;
}

void insertHeartMiddle()
{
	ppu_wait_nmi();
	vram_adr(NTADR_A(vramX - 3, vramY - 5));
	// write middle map index
	vram_put(heNumbers[(signNum &0x0f) *5]);

	vram_adr(NTADR_A(vramX - 4, vramY - 4));
	// write middle map index determined by number to be displayed 3 times
	vram_put(heNumbers[(signNum & 0x0f) * 5 + 1]); // at X and Y, update X after 3rd write
	vram_put(heNumbers[(signNum & 0x0f) * 5 + 2]); 
	vram_put(heNumbers[(signNum & 0x0f) * 5 + 3]);
	
	vram_adr(NTADR_A(vramX - 3, vramY -3));
	vram_put(heNumbers[(signNum & 0x0F) * 5 + 4]); // update x and y after write
}


void createDiamondBase()
{
	k = 0;
	for(i = 0; i < 7; i++)
	{
		if(i == 2)
		ppu_wait_nmi();
		vram_adr(NTADR_A(vramX, vramY));
		for(j = 0; j < 5; j++)
		{
			        // here every vram write increments x
			
			if((j == 3) && (i == 2))
			{
				if((signNum & 0x0F) == 9)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x00CF);
				}
				else if((signNum & 0x0F) == 10)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x00D1);
				}
				else if((signNum & 0x0F) == 11)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x00D4);
				}
				else if((signNum & 0x0F) == 12)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x00D7);
				}
				else
				{
					vram_put(mmDi[k]);
				}
				
			}
			else if((j == 1) && (i == 2))
			{
				if((signNum & 0x0F) == 9)
				{
					vram_adr(NTADR_A(vramX + 1, vramY));
					vram_put(0x00CD);
				}
				else if((signNum & 0x0F) == 11)
				{
					vram_adr(NTADR_A(vramX + 1, vramY));
					vram_put(0x00D2);
				}
				else if((signNum & 0x0F) == 12)
				{
					vram_adr(NTADR_A(vramX + 1, vramY));
					vram_put(0x00D5);
				}
				else
				{
					vram_put(mmDi[k]);
				}
			
			}
			else
			{
				vram_put(mmDi[k]);
			}
			
			++k;
		}
		++vramY;
	}
	vramX += 5;
}

void insertDiamondMiddle()
{
	ppu_wait_nmi();
	vram_adr(NTADR_A(vramX - 3, vramY - 5));
	// write middle map index
	vram_put(diNumbers[snesmul((signNum &0x0f), 5)]);

	vram_adr(NTADR_A(vramX - 4, vramY - 4));
	// write middle map index determined by number to be displayed 3 times
	vram_put(diNumbers[snesmul((signNum & 0x0f) , 5) + 1]); // at X and Y, update X after 3rd write
	vram_put(diNumbers[snesmul((signNum & 0x0f) , 5) + 2]); 
	vram_put(diNumbers[snesmul((signNum & 0x0f) , 5) + 3]);
	
	vram_adr(NTADR_A(vramX - 3, vramY -3));
	vram_put(diNumbers[snesmul((signNum & 0x0F) , 5) + 4]); // update x and y after write
}

void createSpadeBase()
{
	k = 0;
	for(i = 0; i < 7; i++)
	{
		if(i == 2)
		ppu_wait_nmi();
		vram_adr(NTADR_A(vramX, vramY));
		for(j = 0; j < 5; j++)
		{
			        // here every vram write increments x
			
			if((j == 3) && (i == 2))
			{
				if((signNum & 0x0F) == 9)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x092B);
				}
				else if((signNum & 0x0F) == 10)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x052D);
				}
				else if((signNum & 0x0F) == 11)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x0530);
				}
				else if((signNum & 0x0F) == 12)
				{
					vram_adr(NTADR_A(vramX + 3, vramY));
					vram_put(0x0532);
				}
				else
				{
					vram_put(mmSp[k]);
				}
				
			}
			else if((j == 1) && (i == 2))
			{
				if((signNum & 0x0F) == 9)
				{
					vram_adr(NTADR_A(vramX + 1, vramY));
					vram_put(0x0929);
				}
				else if((signNum & 0x0F) == 11)
				{
					vram_adr(NTADR_A(vramX + 1, vramY));
					vram_put(0x052E);
				}
				else if((signNum & 0x0F) == 12)
				{
					vram_adr(NTADR_A(vramX + 1, vramY));
					vram_put(0x052E);
				}
				else
				{
					vram_put(mmSp[k]);
				}
			
			}
			else
			{
				vram_put(mmSp[k]);
			}
			
			++k;
		}
		++vramY;
	}
	vramX += 5;
}

void insertSpadeMiddle()
{
	ppu_wait_nmi();
	vram_adr(NTADR_A(vramX - 3, vramY - 5));
	// write middle map index
	vram_put(spNumbers[(signNum &0x0f) *5]);

	vram_adr(NTADR_A(vramX - 4, vramY - 4));
	// write middle map index determined by number to be displayed 3 times
	vram_put(spNumbers[(signNum & 0x0f) * 5 + 1]); // at X and Y, update X after 3rd write
	vram_put(spNumbers[(signNum & 0x0f) * 5 + 2]); 
	vram_put(spNumbers[(signNum & 0x0f) * 5 + 3]);
	
	vram_adr(NTADR_A(vramX - 3, vramY -3));
	vram_put(spNumbers[(signNum & 0x0F) * 5 + 4]); // update x and y after write
}
// 
void drawBigSelectorSprite()
{
	// SPRITE_ATTR_FULL(pal, prio, flipv, fliph, bank)
	availableSprid = 0;
	availableSprid = oam_spr(spX, spY, 0, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), availableSprid);
	availableSprid = oam_spr(spX, spY + 72, 0, SPRITE_ATTR_FULL(0, 3, 1, 0, 1), availableSprid);
	for(i = 8; i < 48; i+=8)
	{
		availableSprid = oam_spr(spX + i, spY, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), availableSprid);
		availableSprid = oam_spr(spX + i, spY + 72, 1, SPRITE_ATTR_FULL(0, 3, 1, 0, 1), availableSprid);
	}
	availableSprid = oam_spr(spX + i, spY, 0, SPRITE_ATTR_FULL(0, 3, 0, 1, 1), availableSprid);
	availableSprid = oam_spr(spX + i, spY + 72, 0, SPRITE_ATTR_FULL(0, 3, 1, 1, 1), availableSprid);
	
	for(i = 8; i < 72; i += 8)
	{
		availableSprid = oam_spr(spX, spY + i, 2, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), availableSprid);
		availableSprid = oam_spr(spX + 48, spY + i, 2, SPRITE_ATTR_FULL(0, 3, 0, 1, 1), availableSprid);
	}
	
}


// used by solitaire
void drawStaticTips()
{
	
}

void drawCash()
{
	cash1 = cash;
	availableSprid = oam_spr(spX, spY, 4, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
	availableSprid = oam_spr(spX, spY+8, 15, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
	do
	{
		cash1 = cash1/10;
		spX += 8; 
	}while(cash1 != 0);
	cash1 = cash;
	do
	{
		digit = cash1 % 10;
		availableSprid = oam_spr(spX, spY, digit + 5, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
		availableSprid = oam_spr(spX, spY+8, digit + 16, SPRITE_ATTR_FULL(1, 3, 0, 0, 1), availableSprid);
		spX -= 8;
		cash1 = cash1/10;
		
		
	}while(cash1 != 0);
}


//------------------------------------------------------------------
// BLACKJACK DRAWING FUNCTIONS
void displayLetter(char letter)
{
	vram_adr(0x5400);
	vram_put((letter - 97 + 30) | 0x2000);
}

void displayText(const char *text)
{
	i = 0;
	vram_adr(0x5400);
	for(i = 0; text[i]; i++)	
	{
			vram_put((text[i] - 97 + 30) | 0x2000);
	}
	i = 0;
}

void displaySelector()
{
	vram_adr(0x5400);
	vram_put(56 | 0b0011110000000000);
	vram_adr(0x5400 + 32);
	vram_put(58 | 0b0011110000000000);
}

/*
void openDealersCard()
{
	auxVX = signNum;
			auxVY = vramY;
			vramX = XEND >> 3;
			vramY = (YEND - 64) >> 3;
			signNum = hiddenCard;
			oam_clear();
			ppu_wait_nmi();
			displayCard();
			displayDscore();
			vramX = auxVX;
			vramY = auxVY;
			signNum = auxVX;
}
*/				
//---------------------------------------------------------


void displayPlayerCash()
{
	unsigned char x = 9;
//	td(2, 25, "player");
	vram_adr(BG3_NT_ADR(x, 24));
	vram_put(60| 0x3C00);
	vram_adr(BG3_NT_ADR(x, 25));
	vram_put((61)| 0x3C00);
	vram_adr(BG3_NT_ADR(x, 26));
	vram_put((62)| 0x3C00);
	cash1 = cash;
	x = 8;
	do
	{
		digit = cash1 % 10;
		vram_adr(BG3_NT_ADR(x, 24));
		vram_put(digit | 0x3C00);
		vram_adr(BG3_NT_ADR(x, 25));
		vram_put((digit + 10) | 0x3C00);
		vram_adr(BG3_NT_ADR(x, 26));
		vram_put((digit + 20) | 0x3C00);
		--x;
		
		cash1 = cash1 / 10;
	}while(cash1 > 0);
}

#pragma code-name(pop)

#pragma code-name(push, "CODEA")
void drawCardBackSprite()
{
	k = 0;
	
	availableSprid = nextSprid;
	for(i = 0; i < 56; i+=8)
	{
		for(j = 0; j < 40; j+=8)
		{
			availableSprid = oam_spr(spX + j, spY + i, (unsigned char)mmBcSp[k], (unsigned char)(mmBcSp[k] >>8), availableSprid);
			++k;
		}
	}
}

// Draw Clover Card metasprite, depends on the signNum and global x, y possitions
void drawSpriteCard1()
{
	k = 0;
	// u koroacima od po 8
	availableSprid = nextSprid;
	for(i = 0; i < 56; i+=8)
	{
		for(j = 0; j < 40; j += 8)
		{
			/*oam_spr(x, y + j*8, miniMap[j], SPRITE_ATTR_FULL(9, 3, 0, 0, 0), k);
			oam_spr(x+8, y+ j*8, miniMap[j], SPRITE_ATTR_FULL(9, 3, 0, 0, 0), k);
			oam_spr(x+16, y + j*8, miniMap[j], SPRITE_ATTR_FULL(9, 3, 0, 0, 0), k);
			oam_spr(x+24, y + j*8, miniMap[j], SPRITE_ATTR_FULL(9, 3, 0, 0, 0), k);*/
			if(i == 16 && j == 16)
			{
				availableSprid = oam_spr(spX + j, spY + i, clNumbers[(signNum &0x0f) *5], 0x34, availableSprid);
			}
			else if(i == 24 && j == 8)
			{
				availableSprid = oam_spr(spX + j, spY + i, clNumbers[(signNum &0x0f) *5 + 1], 0x34, availableSprid);  // add attr only palette
				j += 8;
				availableSprid = oam_spr(spX + j, spY + i, clNumbers[(signNum &0x0f) *5 + 2], 0x34, availableSprid);
				j += 8;
				if(signNum == 9 || signNum == 11)
				{
					availableSprid = oam_spr(spX + j, spY + i, clNumbers[(signNum &0x0f) *5 + 3], 0x34, availableSprid);
				}
				else
				{
					availableSprid = oam_spr(spX + j, spY + i, clNumbers[(signNum &0x0f) *5 + 3], 0x30, availableSprid);
				}
				k += 2;
			}
			else if(i == 32 && j == 16)
			{
				availableSprid = oam_spr(spX + j, spY + i, clNumbers[(signNum &0x0f) *5 + 4], 0x34, availableSprid);
			}
			else
			{
			
					if(j == 8 && i == 16)
				{
					if((signNum & 0X0F) == 9)
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x0816 , 0x34, availableSprid);
					}
					else
					{
						availableSprid = oam_spr(spX + j, spY + i, (unsigned char)mmClSp[k], (unsigned char)(mmClSp[k] >>8), availableSprid);
					}
					
				}
				else if( j == 24 && i == 16) 
				{
					if((signNum & 0x0F) == 9)     // right corner 10
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x0818 , 0x34, availableSprid);
					}
					else if((signNum & 0x0F) == 10)  // right corner J
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x081A , 0x34, availableSprid);
					}
					else if((signNum & 0x0F) == 11)  // right corner Q
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x081D , 0x34, availableSprid);
					}
					else if((signNum & 0x0F) == 12)  // right corner K
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x081F , 0x34, availableSprid);
					}
					else
					{
						availableSprid = oam_spr(spX + j, spY + i, (unsigned char)mmClSp[k], (unsigned char)(mmClSp[k] >>8), availableSprid);
					}
				}
				else
				{
					availableSprid = oam_spr(spX + j, spY + i, (unsigned char)mmClSp[k], (unsigned char)(mmClSp[k] >>8), availableSprid);
				}	
			//	availableSprid = oam_spr(spX + j, spY + i, (unsigned char)mmClSp[k], (unsigned char)(mmClSp[k] >>8), availableSprid);
			}
			k++;
		}
	}
}

// Display Heart card metasprite, spX, spY and signNum globals determine where and which card is displayed
void drawSpriteCard2()
{
	k = 0;
	// u koroacima od po 8
	availableSprid = nextSprid;
	for(i = 0; i < 56; i+=8)
	{
		for(j = 0; j < 40; j += 8)
		{
			/*oam_spr(x, y + j*8, miniMap[j], SPRITE_ATTR_FULL(9, 3, 0, 0, 0), k);
			oam_spr(x+8, y+ j*8, miniMap[j], SPRITE_ATTR_FULL(9, 3, 0, 0, 0), k);
			oam_spr(x+16, y + j*8, miniMap[j], SPRITE_ATTR_FULL(9, 3, 0, 0, 0), k);
			oam_spr(x+24, y + j*8, miniMap[j], SPRITE_ATTR_FULL(9, 3, 0, 0, 0), k);*/
			if(i == 16 && j == 16)
			{
				availableSprid = oam_spr(spX + j, spY + i, heNumbers[(signNum &0x0f) *5], 0x32, availableSprid);
				
			}
			else if(i == 24 && j == 8)
			{
				availableSprid = oam_spr(spX + j, spY + i, heNumbers[(signNum &0x0f) *5 + 1], 0x32, availableSprid);  // add attr only palette
				j += 8;
				availableSprid = oam_spr(spX + j, spY + i, heNumbers[(signNum &0x0f) *5 + 2], 0x32, availableSprid);
				j += 8;
				availableSprid = oam_spr(spX + j, spY + i, heNumbers[(signNum &0x0f) *5 + 3], 0x32, availableSprid);
				k += 2;
			}
			else if(i == 32 && j == 16)
			{
				availableSprid = oam_spr(spX + j, spY + i, heNumbers[(signNum &0x0f) *5 + 4], 0x32, availableSprid);
			}
			else
			{
				if(j == 8 && i == 16)
				{
					if((signNum & 0X0F) == 9)
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x0474 , 0x32, availableSprid);
					}
					else if((signNum & 0x0F) == 11)
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x0479 , 0x32, availableSprid);
					}
					else if((signNum & 0x0F) == 12)
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x047C , 0x32, availableSprid);
					}
				
					else
					{
						availableSprid = oam_spr(spX + j, spY + i, (unsigned char)mmHeSp[k], (unsigned char)(mmHeSp[k] >>8), availableSprid);
					}
					
				}
				else if( j == 24 && i == 16) 
				{
					if((signNum & 0x0F) == 9)     // right corner 10
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x0476 , 0x32, availableSprid);
					}
					else if((signNum & 0x0F) == 10)  // right corner J
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x0478 , 0x32, availableSprid);
					}
					else if((signNum & 0x0F) == 11)  // right corner Q
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x047B , 0x32, availableSprid);
					}
					else if((signNum & 0x0F) == 12)  // right corner K
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x047E , 0x32, availableSprid);
					}
					else if((signNum & 0x0F ) != 1 && (signNum & 0X0F )!= 2) //right corner for everything but 2 and 3
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x0473 , 0x32, availableSprid);
					}
					else
					{
						availableSprid = oam_spr(spX + j, spY + i, (unsigned char)mmHeSp[k], (unsigned char)(mmHeSp[k] >>8), availableSprid);
					}
				}
				else
				{
					availableSprid = oam_spr(spX + j, spY + i, (unsigned char)mmHeSp[k], (unsigned char)(mmHeSp[k] >>8), availableSprid);
				}
			}
			k++;
		}
	}
}

void drawSpriteCard3()
{
		k = 0;
	// u koroacima od po 8
	availableSprid = nextSprid;
	for(i = 0; i < 56; i+=8)
	{
		diAttr = (unsigned char)(spDiNumbers[k] >> 8);
		diAttr &= 0b11110001;
		for(j = 0; j < 40; j += 8)
		{
				if(i == 16 && j == 16)
			{
				availableSprid = oam_spr(spX + j, spY + i, spDiNumbers[(signNum &0x0f) *5], diAttr, availableSprid);
			}
			else if(i == 24 && j == 8)
			{
				availableSprid = oam_spr(spX + j, spY + i, spDiNumbers[(signNum &0x0f) *5 + 1], diAttr, availableSprid);  // add attr only palette
				j += 8;
				availableSprid = oam_spr(spX + j, spY + i, spDiNumbers[(signNum &0x0f) *5 + 2], diAttr, availableSprid);
				j += 8;
				availableSprid = oam_spr(spX + j, spY + i, spDiNumbers[(signNum &0x0f) *5 + 3], diAttr, availableSprid);
				k += 2;
			}
			else if(i == 32 && j == 16)
			{
				availableSprid = oam_spr(spX + j, spY + i, spDiNumbers[(signNum &0x0f) *5 + 4], diAttr, availableSprid);
			}
			else
			{
				if( j == 8 && i == 16)
				{
					if((signNum & 0x0F) == 9)
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x3b16 , diAttr, availableSprid);
					}
					else if((signNum & 0x0F) == 11)
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x3b1B , diAttr, availableSprid);
					}
					else if((signNum & 0x0F) == 12)
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x3b1E , diAttr, availableSprid);
					}
					else
					{
						availableSprid = oam_spr(spX + j, spY + i, (unsigned char)mmDiSp[k], diAttr, availableSprid);
					}
				}
				else if(j == 24 && i == 16)   // if it is 10, make the corner
				{
					if((signNum & 0x0F) == 9)
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x3b18 , diAttr, availableSprid);
					}
					else if((signNum & 0x0F) == 10)
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x3b1A , diAttr, availableSprid);
					}
					else if((signNum & 0x0F) == 11)
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x3b1D , diAttr, availableSprid);
					}
					else if((signNum & 0x0F) == 12)
					{
						availableSprid = oam_spr(spX + j, spY + i, 0x3b20 , diAttr, availableSprid);
					}
					else
					{
						availableSprid = oam_spr(spX + j, spY + i, (unsigned char)mmDiSp[k], diAttr, availableSprid);
					}
				}
				else
				{
					availableSprid = oam_spr(spX + j, spY + i, (unsigned char)mmDiSp[k], diAttr, availableSprid);
				}
			}
			k++;
		}
	}
}

void drawSpriteCard4()
{
			k = 0;
	// u koroacima od po 8
	availableSprid = nextSprid;
	for(i = 0; i < 56; i+=8)
	{
	
		
		for(j = 0; j < 40; j += 8)
		{
				spAttr = (unsigned char)(spSpNumbers[k] >> 8);
		
				if( ((spAttr >> 1) & 0x07) == 5)
				{
					spAttr &= 0b11110001;
				//	spAttr |= 0b00001100;	
				}

				if(i == 16 && j == 16)
				{
					availableSprid = oam_spr(spX + j, spY + i, spSpNumbers[(signNum &0x0f) *5], spAttr, availableSprid);
				}
			
				else if(i == 32 && j == 16)
				{
					availableSprid = oam_spr(spX + j, spY + i, spSpNumbers[(signNum &0x0f) *5 + 4], spAttr, availableSprid);
				}
				else if(i == 24 && j == 8)
				{
					availableSprid = oam_spr(spX + j, spY + i, spSpNumbers[(signNum &0x0f) *5 + 1], spAttr, availableSprid);  // add attr only palette
					j += 8;
					++k;
					spAttr = (unsigned char)(spSpNumbers[k] >> 8);
		
					if( ((spAttr >> 1) & 0x07) == 5)
					{
						spAttr &= 0b11110001;
					//	spAttr |= 0b00001100;	
					}
					availableSprid = oam_spr(spX + j, spY + i, spSpNumbers[(signNum &0x0f) *5 + 2], spAttr, availableSprid);
					j += 8;
					++k;
					spAttr = (unsigned char)(spSpNumbers[k] >> 8);
		
					if( ((spAttr >> 1) & 0x07) == 5)
					{
						spAttr &= 0b11110001;
					//	spAttr |= 0b00001100;	
					}
					availableSprid = oam_spr(spX + j, spY + i, spSpNumbers[(signNum &0x0f) *5 + 3], spAttr, availableSprid);
				}
				else
				{
					spAttr = (unsigned char)(mmSpSp[k] >>8);
					if( ((spAttr >> 1) & 0x07) == 5)
					{
						spAttr &= 0b11110001;
					}
					
					if(j == 8 && i == 16)
					{
						if((signNum & 0x0F) == 9)
						{
							spAttr |= 0b00001100;
							availableSprid = oam_spr(spX + j, spY + i, 0x3d73 , spAttr, availableSprid);
						}
						
						else if((signNum & 0x0F) == 11)
						{
							spAttr |= 0b00001100;
							availableSprid = oam_spr(spX + j, spY + i, 0x3d78 , spAttr, availableSprid);
						}
						else if((signNum & 0x0F) == 12)
						{
							spAttr |= 0b00001100;
							availableSprid = oam_spr(spX + j, spY + i, 0x3d78 , spAttr, availableSprid);
						}
						else
						{
							availableSprid = oam_spr(spX + j, spY + i, (unsigned char)mmSpSp[k], spAttr, availableSprid);
						}
					}
					else if( j == 24 && i == 16)   // if it is 10, make the corner
					{
						if((signNum & 0x0F) == 9)
						{
							availableSprid = oam_spr(spX + j, spY + i, 0x3d75 , spAttr, availableSprid);
						}
						else if((signNum & 0x0F) == 10)
						{
							availableSprid = oam_spr(spX + j, spY + i, 0x3d77 , spAttr, availableSprid);
						}
						else if((signNum & 0x0F) == 11)
						{
							availableSprid = oam_spr(spX + j, spY + i, 0x3d7A , spAttr, availableSprid);
						}
						else if((signNum & 0x0F) == 12)
						{
							availableSprid = oam_spr(spX + j, spY + i, 0x3d7C, spAttr, availableSprid);
						}
						else
						{
							availableSprid = oam_spr(spX + j, spY + i, (unsigned char)mmSpSp[k], spAttr, availableSprid);
						}
					}
					else
					{
						availableSprid = oam_spr(spX + j, spY + i, (unsigned char)mmSpSp[k], spAttr, availableSprid);
					}
				}
				k++;
		}
	}
}
#pragma code-name(pop)
