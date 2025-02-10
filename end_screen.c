#include "neslib.h"
#include "include/defines.h"
#include "include/load_screens.h"
#include "include/multi_screen.h"

unsigned char endSel;
extern unsigned char gameSel;
static unsigned char selX;
extern unsigned char screenState;
extern unsigned long cash;
extern unsigned long betAmount;
extern unsigned char availableSprid;


#define SEL_SAME_BET 0
#define SEL_NEW_BET  1
#define SEL_TITLE    2

#define SEL_SAME_BET_X 100
#define SEL_NEW_BET_X  140
#define SEL_TITLE_X    180

#define SEL_RIGHT_SIDE_NEW
#define SEL_RIGHT_SIDE_SAME
#define SEL_RIGHT_SIDE_TITLE

#pragma code-name(push, "CODE5")
void endScreen()
{
	ppu_wait_nmi();
	if(pad_trigger(0) & PAD_RIGHT)
	{
		if(endSel == 2)
		{
			endSel = 0;
		}
		else
		{
			++endSel;
		}
	}
	else if(pad_trigger(0) & PAD_LEFT)
	{
		if(endSel == 0)
		{
			endSel = 2;
		}
		else
		{
			--endSel;
		}
	}
	else if(pad_trigger(0) & PAD_B)
	{
		if(!endSel)
		{
			
			if(cash >= betAmount)
			{
				cash -= betAmount;
				loadGame();
				endSel = 0;
				return;
			}
		}
		else if(endSel == 1)
		{
			if((cash > 0) || (!gameSel))  // You must have some cash to play or you could play Solitaire for free
			{
				if(gameSel != 2)
				{
					ppu_wait_nmi();
					loadBettingScreen();
			
					screenState = BETTING_SCREEN;
					endSel = 0;
					return;
				}
				else if((cash >= 100 ) && gameSel == 2)
				{
				//	ppu_wait_nmi();
					loadMultiPscreen();
					availableSprid = 0;
					screenState = 8;
					return;	
				}
			}

	//	loadTitleScreen();
	//	screenState = 0;
		}
		else if(endSel == 2)
		{
			endSel = 0;
			loadTitleScreen();
			screenState = 0;
			return;
		}
	}
	switch(endSel)
	{
		ppu_wait_nmi();
		case(0):
			oam_spr(75, 184, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), 0);
		//	oam_spr(83, 185, 2, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), 4);
			
		//	oam_spr(108, 185, 2, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), 8);
			oam_spr(116, 184, 1, SPRITE_ATTR_FULL(0, 3, 0, 1, 0), 4);
			
			oam_spr(75, 192, 1, SPRITE_ATTR_FULL(0, 3, 1, 0, 0), 8);
		//	oam_spr(83, 193, 2, SPRITE_ATTR_FULL(0, 3, 1, 0, 0), 20);
			
			oam_spr(116, 192, 3, SPRITE_ATTR_FULL(0, 3, 0, 1, 0), 12);
			break;
		case(1):
			oam_spr(135, 184, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), 0);
			oam_spr(176, 184, 1, SPRITE_ATTR_FULL(0, 3, 0, 1, 0), 4);
			oam_spr(135, 192, 1, SPRITE_ATTR_FULL(0, 3, 1, 0, 0), 8);
			oam_spr(176, 192, 3, SPRITE_ATTR_FULL(0, 3, 0, 1, 0), 12);
			break;
		case(2):
			oam_spr(194, 184, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), 0);
			oam_spr(237, 184, 1, SPRITE_ATTR_FULL(0, 3, 0, 1, 0), 4);
			oam_spr(194, 192, 1, SPRITE_ATTR_FULL(0, 3, 1, 0, 0), 8);
			oam_spr(237, 192, 3, SPRITE_ATTR_FULL(0, 3, 0, 1, 0), 12);
			break;		
			
	}
}

#pragma code-name(pop)
