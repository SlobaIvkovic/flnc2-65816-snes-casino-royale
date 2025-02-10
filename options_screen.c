#include "neslib.h"
#include "include/options.h"
#include "include/joypad.h"
#include"include/binsizes.h"
#include"snesgss.h"

#pragma rodata-name(push, "RODATA")
#pragma data-name(push, "RODATA")
static unsigned short option_palette [] = {
//0x7fff, 0x146a, 0x388a, 0x412b, 0x1d36, 0x4628, 0x5e85, 0x0e3c, 0x2f1e,
0x7fff, 0x146a, 0x1d36, 0x0e3c, 0x2f1e, 0x388a, 0x412b, 0x4628, 0x5e85,
};
#pragma data-name(pop)
#pragma rodata-name(pop)

extern unsigned char vramX, vramY;
extern unsigned char screenState;

unsigned char sel_X, sel_Y, sel_W, sel_H;

unsigned char slx1, sly1, slx2, sly2;

unsigned char slide_X, slide_Y;
unsigned char slider_as;
unsigned char selPosUD, selPosLR;

extern unsigned char currentInput;
extern unsigned char playersJoypad;

extern unsigned char music5[];

unsigned char volume, volume1;

#pragma wrapped-call(push, trampoline, 0x91)
void drawNote();
void crossNote();
void drawSpeaker();
void crossSpeaker();
static void drawSlider();
void makeSelector();
void driveSelector();
void adjustVolume();
#pragma wrapped-call(pop)


#pragma code-name(push, "CODE9")

void loadOptionsScreen()
{
	sel_H = 15;
		sel_W = 15;
		sel_Y = 99;
		selPosUD = 0;
		selPosLR = 0;
		
	ppu_off();
	oam_clear();
	
	asm("lda #3");              
	asm("sta $2101");
	
	asm("lda #%b", 0b00001001);
	asm("sta $2105");
	
	asm("lda #%b", 0b00110010); // BG1 tiles at 2000, BG2 tiles at 3000
	asm("sta $210B");
	
	asm("lda #%b", 0b01000101); // BG3 tiles at 5000,
	asm("sta $210C");
	
	asm("lda #%b", 0b00000000); // base address 0 screen height = 2 BG1
	asm("sta $2107");
	
	asm("lda #%b", 0b01110000); // base address 7000 screen height = 1 BG2
	asm("sta $2108");
	
	asm("lda #%b", 0b00010000); // base address 1000 screen height = 2 BG3
	asm("sta $2109");
	
	vram_adr(0x3000);
	// load BG2 charset
	vram_write_bank(CHR8_BANK, (char *) 0x8000, 10592);
	vram_adr(0x7000);
	vram_write_bank(CHR8_BANK, (char *) 0x8000 + 10592, 1792);
	
	vram_adr(0x2000);
	// load BG1 charset
	vram_write_bank(CHR8_BANK, (char *) 0x8000 + 10592 + 1792, 1440);
	
	vram_adr(0x6000);
	// load BG1 charset
	vram_write_bank(CHR8_BANK, (char *) 0x8000 + 10592 + 1792 + 1440, 320);
	
	vram_adr(0x0000);
	vram_fill(0, 4096);
	
	vram_adr(0x7000 | ((20 << 5) | 2));
	vram_fill(155, 16);
	vram_adr(0x7000 | ((21 << 5) | 2));
	vram_fill(155, 24);
	vram_adr(0x7000 | ((22 << 5) | 2));
	vram_fill(155, 24);
	
	
	vram_adr(0x7000 | ((23 << 5) | 2));
	vram_fill(264, 16);
	vram_fill(277, 10);
	
	
	vram_adr(0x5000);
	vram_fill(0, 128);
	vram_adr(0x1000);
	vram_fill(0, 2048);
	
	pal_all(0, option_palette);
	pal_all(8, option_palette);
	
	pal_col(144, 0xFFFF);
	pal_col(145, 0xFFFF);
	pal_col(146, 0xFFFF);
	pal_col(147, 0xFFFF);
	
	
	vramX = 10;
	vramY = 12;
	drawNote();
	
	vramX = 10;
	vramY = 16;
	drawSpeaker();
	
//	slx1 = 171;
	slide_X = slx1;
	sly1 = 96;
	slide_Y = sly1;
	slider_as = 0;
	drawSlider();
	slider_as = 48;
//	slx2 = 171;
	slide_X = slx2;
	sly2 = 128;
	slide_Y = sly2;
	drawSlider();
	driveSelector();
	if(volume)
	{
		vramX = 10;
						vramY = 12;
						drawNote();
	}
	else
	{
		vramX = 10;
						vramY = 12;
						crossNote();
	}
	if(volume1)
	{
		vramX = 10;
						vramY = 16;
						drawSpeaker();
	}
	else
	{
		vramX = 10;
						vramY = 16;
						crossSpeaker();
	}
//	adjustVolume(63);
	spc_fadespeed = 255;
//	spc_global_volume(63);
	ppu_on_all();
	
	
}

void optionsScreen()
{
	driveSelector();
}

void drawNote()
{
	ppu_wait_nmi();
	vram_adr(NTADR_A(vramX,vramY));
	vram_put(0 | 0b0010000000000000);
	vram_put(1 | 0b0010000000000000);
	vram_put(4 | 0b0010000000000000);
	vram_put(5 | 0b0010000000000000);
	vram_adr(NTADR_A(vramX,vramY + 1));
	vram_put(0 | 0b0010000000000000);
	vram_put(9 | 0b0010000000000000);
	vram_put(10 | 0b0010000000000000);
	vram_put(11 | 0b0010000000000000);
	vram_adr(NTADR_A(vramX,vramY + 2));
	vram_put(16 | 0b0010000000000000);
	vram_put(17 | 0b0010000000000000);
	vram_put(18 | 0b0010000000000000);
	vram_put(15 | 0b0010000000000000);
	vram_adr(NTADR_A(vramX,vramY + 3));
	vram_put(22 | 0b0010000000000000);
	vram_put(23 | 0b0010000000000000);
	vram_put(21 | 0b0010000000000000);
	vram_put(0 | 0b0010000000000000);
	
}

void crossNote()
{
	ppu_wait_nmi();
	vram_adr(NTADR_A(vramX,vramY));
	vram_put(0 | 0b0010000000000000);
	vram_put(1 | 0b0010000000000000);
	vram_put(2 | 0b0010000000000000);
	vram_put(3 | 0b0010000000000000);
	vram_adr(NTADR_A(vramX,vramY + 1));
	vram_put(0 | 0b0010000000000000);
	vram_put(6 | 0b0010000000000000);
	vram_put(7 | 0b0010000000000000);
	vram_put(8 | 0b0010000000000000);
	vram_adr(NTADR_A(vramX,vramY + 2));
	vram_put(12 | 0b0010000000000000);
	vram_put(13 | 0b0010000000000000);
	vram_put(14 | 0b0010000000000000);
	vram_put(15 | 0b0010000000000000);
	vram_adr(NTADR_A(vramX,vramY + 3));
	vram_put(19 | 0b0010000000000000);
	vram_put(20 | 0b0010000000000000);
	vram_put(21 | 0b0010000000000000);
	vram_put(0 | 0b0010000000000000);
}

void drawSpeaker()
{
	ppu_wait_nmi();
	vram_adr(NTADR_A(vramX,vramY));
	vram_put(0 | 0b0010000000000000);
	vram_put(24 | 0b0010000000000000);
	vram_put(27 | 0b0010000000000000);
	vram_put(28 | 0b0010000000000000);
	vram_adr(NTADR_A(vramX,vramY + 1));
	vram_put(29 | 0b0010000000000000);
	vram_put(33 | 0b0010000000000000);
	vram_put(34 | 0b0010000000000000);
	vram_put(35 | 0b0010000000000000);
	vram_adr(NTADR_A(vramX,vramY + 2));
	vram_put(40 | 0b0010000000000000);
	vram_put(41 | 0b0010000000000000);
	vram_put(42 | 0b0010000000000000);
	vram_put(39 | 0b0010000000000000);
	vram_adr(NTADR_A(vramX,vramY + 3));
	vram_put(0 | 0b0010000000000000);
	vram_put(0 | 0b0010000000000000);
	vram_put(0 | 0b0010000000000000);
	vram_put(0 | 0b0010000000000000);
}

void crossSpeaker()
{
	ppu_wait_nmi();
	vram_adr(NTADR_A(vramX,vramY));
	vram_put(0 | 0b0010000000000000);
	vram_put(24 | 0b0010000000000000);
	vram_put(25 | 0b0010000000000000);
	vram_put(26 | 0b0010000000000000);
	vram_adr(NTADR_A(vramX,vramY + 1));
	vram_put(29 | 0b0010000000000000);
	vram_put(30 | 0b0010000000000000);
	vram_put(31 | 0b0010000000000000);
	vram_put(32 | 0b0010000000000000);
	vram_adr(NTADR_A(vramX,vramY + 2));
	vram_put(36 | 0b0010000000000000);
	vram_put(37 | 0b0010000000000000);
	vram_put(38 | 0b0010000000000000);
	vram_put(39 | 0b0010000000000000);
	vram_adr(NTADR_A(vramX,vramY + 3));
	vram_put(43 | 0b0010000000000000);
	vram_put(44 | 0b0010000000000000);
	vram_put(0 | 0b0010000000000000);
	vram_put(0 | 0b0010000000000000);
}

void drawSlider()
{
	oam_spr(slide_X, slide_Y, 0, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), slider_as);
	oam_spr(slide_X+8, slide_Y, 1, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), slider_as + 4);
	
	oam_spr(slide_X, slide_Y+8, 3, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), slider_as + 8);
	oam_spr(slide_X+8, slide_Y+8, 4, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), slider_as + 12);
	
	oam_spr(slide_X, slide_Y+16, 3, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), slider_as + 16);
	oam_spr(slide_X+8, slide_Y+16, 4, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), slider_as + 20);
	
	oam_spr(slide_X, slide_Y+24, 0, SPRITE_ATTR_FULL(0, 3, 1, 0, 0), slider_as + 24);
	oam_spr(slide_X+8, slide_Y+24, 1, SPRITE_ATTR_FULL(0, 3, 1, 0, 0), slider_as + 28);
	
}

void makeSelector()
{
	oam_spr(sel_X, sel_Y, 2, SPRITE_ATTR_FULL(1, 3, 0, 0, 0), 32);
	oam_spr(sel_X + sel_W, sel_Y, 2, SPRITE_ATTR_FULL(1, 3, 0, 1, 0), 36);
	oam_spr(sel_X, sel_Y + sel_H, 2, SPRITE_ATTR_FULL(1, 3, 1, 0, 0), 40);
	oam_spr(sel_X + sel_W, sel_Y + sel_H, 2, SPRITE_ATTR_FULL(1, 3, 1, 1, 0), 44);
}

void driveSelector()
{
	if(pad_trigger(0) & PAD_UP)
	{
		if(selPosUD)
		{
			--selPosUD; 
		}
		if(!selPosUD)
		{
			music_play(0x88, music5);
		}
	}
	else if(pad_trigger(0) & PAD_DOWN)
	{
		++selPosUD;
		if(selPosUD > 1)
		selPosUD = 1;
	}
	else if(pad_trigger(0) & PAD_RIGHT)
	{
		selPosLR = 1;
	}
	else if(pad_trigger(0) & PAD_LEFT)
	{
		selPosLR = 0;
	}
	
	if(selPosUD < 2)
	{
		sel_H = 15;
		sel_W = 15;
		
		if(!selPosUD)
		{
			
			sel_Y = 99;
		}
		else
		{
			music_stop();
			sel_Y = 128;
		}
		
		if(selPosLR)
		sel_X = 48;
		else
		sel_X = 19;
	}
	else
	{
		sel_H = 7;
		sel_W = 79;
		sel_X = 18;
		sel_Y = 173;
	}
	ppu_wait_nmi();
	makeSelector();
	
	playersJoypad = 0;
	readJoypad();
	if(currentInput == 5)
	{
		if(!selPosUD)
		{
			slide_Y = sly1;
			if(selPosLR)
			{
				if(volume < 115)
				{
					++slx1;
					
					++volume;
					if(volume == 1)
					{
						vramX = 10;
						vramY = 12;
						drawNote();
					}
				}
			}
			else
			{
				if(volume)
				{
					--slx1;
					slide_X = slx1;
					--volume;
				}
				else
				{
					vramX = 10;
					vramY = 12;
					crossNote();
				}
			}
			slider_as = 0;
			slide_X = slx1;
			drawSlider();
		//	spc_global_volume(volume);
			adjustVolume();
		}
		else if(selPosUD == 1)
		{
			slide_Y = sly2;
			if(selPosLR)
			{
				if(volume1 < 115)
				{
					++slx2;
					
					++volume1;
					if(volume1 == 1)
					{
						vramX = 10;
						vramY = 16;
						drawSpeaker();
					}
				}
			}
			else
			{
				if(volume1)
				{
					--slx2;
					slide_X = slx2;
					--volume1;
				}
				else
				{	
					vramX = 10;
					vramY = 16;
					crossSpeaker();
				}
			}
			slider_as = 48;
			slide_X = slx2;
			drawSlider();
		//	spc_channelmask = 0b10000000;
			spc_sfxvol = volume1;
			sfx_play(0);
		}
		
	}
	if(pad_trigger(0) & PAD_START)
	{
		screenState = 17;
	}
}

void adjustVolume()
{
	spc_channelmask = 0b001111111;
	spc_channel_volume(volume);
/*	spc_channelmask= 1;
	spc_channel_volume(volume);
	spc_channelmask= 2;
	spc_channel_volume(volume);
	spc_channelmask= 3;
	spc_channel_volume(volume);
	spc_channelmask= 4;
	spc_channel_volume(volume);
	spc_channelmask= 5;
	spc_channel_volume(volume);
	spc_channelmask= 6;
	spc_channel_volume(volume);*/
//	spc_channelmask= 7;
//	spc_channel_volume(volume);
}
#pragma code-name(pop)
