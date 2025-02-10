#include "neslib.h"
#include "include/drawing.h"
#include "include/defines.h"
#include "include/load_screens.h"
#include "include/multi_screen.h"
#include "include/achievments.h"
#include "include/options.h"
#include "include/mask.h"

extern unsigned char screenState;
extern unsigned char spX, spY, i;
extern unsigned char gameSel;
unsigned long cashOld;
extern unsigned long cash;
unsigned char bigSelX;
extern unsigned char music6[];
unsigned char ud;                          // is it up or down
unsigned char ao;                          // achievment or option screen
//void drawCash();

unsigned char smallX;
unsigned char smallX1;

#pragma code-name(push, "CODE4")
void titleScreen()
{
	ppu_wait_nmi();
	if(pad_trigger(0) & PAD_DOWN)
	{
		ud = 1;
	//	oam_clear();
		if(gameSel < 2)
		{
			ao = 0;
			oam_spr(19, 202, 0, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), 0);
			oam_spr(67, 202, 0, SPRITE_ATTR_FULL(0, 3, 0, 1, 1), 4);
			oam_spr(20, 210, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), 8);
			oam_spr(66, 210, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), 12);	
		}
		else
		{
			ao = 1;
		//	oam_clear();
			oam_spr(173, 202, 0, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), 0);
			oam_spr(237, 202, 0, SPRITE_ATTR_FULL(0, 3, 0, 1, 1), 4);
			oam_spr(174, 210, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), 8);
			oam_spr(236, 210, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), 12);
		}
		
		for(i = 16; i < 128; i += 4)
		{
			oam_spr(66, 254, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), i);
		}
		i = 0;
		disableColorWindow();
	}
	else if(pad_trigger(0) & PAD_UP)
	{
		ud = 0;
		bigSelectorSprite(bigSelX , 94);
		SetupColorWindow();
		driveMask(gameSel);
	}
	if(!ud)
	{
		if(pad_trigger(0) & PAD_LEFT)
		{
		//	music_play(0x88, music6);
			if(gameSel == 0)
			{
				gameSel = 3;
				bigSelX = 191;
				bigSelectorSprite(bigSelX , 94);
			}
			else
			{
				--gameSel;
				bigSelX -= 60;
				bigSelectorSprite(bigSelX , 94);
			}
			driveMask(gameSel);
		}
		else if(pad_trigger(0) & PAD_RIGHT)
		{
		//	music_play(0x88, music6);
			if(gameSel == 3)
			{
				gameSel = 0;
				bigSelX = 11;
				bigSelectorSprite(bigSelX , 94);
			}
			else
			{
				++gameSel;
				bigSelX +=60;
				bigSelectorSprite(bigSelX , 94);
			}
			driveMask(gameSel);
		}
	
	}
	else
	{
		if(pad_trigger(0) & PAD_RIGHT)
		{
			// set it to option
			ao = 1;
		//	oam_clear();
			oam_spr(173, 202, 0, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), 0);
			oam_spr(237, 202, 0, SPRITE_ATTR_FULL(0, 3, 0, 1, 1), 4);
			oam_spr(174, 210, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), 8);
			oam_spr(236, 210, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), 12);
			
		}
		else if(pad_trigger(0) & PAD_LEFT)
		{
			// set to achievment screen
			ao = 0;
		//	oam_clear();
			oam_spr(19, 202, 0, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), 0);
			oam_spr(67, 202, 0, SPRITE_ATTR_FULL(0, 3, 0, 1, 1), 4);
			oam_spr(20, 210, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), 8);
			oam_spr(66, 210, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 1), 12);
		}
	}

	if(pad_trigger(0) & PAD_START)
	{
		//if achievments load achievments screen
		// else if options load options screen
		disableColorWindow();
		disableHDMA();
		// else
		if(!ud)
		{
			if(gameSel == 2)
			{
				loadMultiPscreen();
				availableSprid = 0;
				screenState = 8;
				return;	
			}
			else
			{
				loadBettingScreen();
				screenState = BETTING_SCREEN;
			}
		
			availableSprid = 0;
			return;
			screenState = BETTING_SCREEN;
		}
		else
		{
			if(!ao)
			{
				loadOptionsScreen();
				screenState = 16;
			}
			else
			{
				loadAchievmentScreen();
				screenState = 15;
			}
			ud = 0;
			
			// check is it option or achievment screen
			// load achievment screen
			// set screen state to achievment screen
		}
	}

	
//	if(cashOld != cash)
//	{
/*		cashOld = cash;
		spY = 0;
		spX = 32;
		availableSprid = 120;
				
		drawCash();*/
//	}
}

#pragma code-name(pop)
