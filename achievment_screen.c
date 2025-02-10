#include"neslib.h"
#include"include/binsizes.h"
#include"include/achievments.h"
#include"include/joypad.h"
#include"include/save.h"
#include "snesgss.h"
#include "include/mask.h"

#define NTADR1_A(x,y)	 	(0x0000|(((y)<<5)|(x)))

#pragma rodata-name(push, "RODATA")
#pragma data-name(push, "RODATA")

static unsigned short achi_palette [] = {
0x7fff, 0x2f1e, 0x146a, 0x388a, 0x412b, 0x1d36, 0x4628, 0x5e85, 0x0e3c,
};

static unsigned short achi_bg_palette [] = {
0x7fff, 0x146a, 0x388a, 0x412b, 0x1d36, 0x4628, 0x5e85, 0x0e3c, 0x2f1e,
};

static unsigned short fill_slot_palette[] = {
0x7fff, 0x146a, 0x388a, 0x412b, 0x1d36, 0x4628, 0x5e85, 0x0e3c
};

#pragma data-name(pop)
#pragma rodata-name(pop)

unsigned char howMuch;
extern unsigned char i;
unsigned short toAchieve, achieved;
unsigned char propX, propY;
unsigned char scrollit;
extern unsigned char currentInput;
extern unsigned char playersJoypad;
extern unsigned char pressPoint;
unsigned char previousInput;
extern unsigned char screenState;
extern unsigned char vramX, vramY;
extern unsigned char curConBj;

extern unsigned long num1, num2;
extern unsigned char bg3X, bg3Y;

void trampoline();

#pragma wrapped-call(push, trampoline, 0x91)
void fillSlot();
void calculateHowMuch();
void achievmentScreen();
static void displayNumber();
#pragma wrapped-call(pop)

#define printNumber(a, x, y) {\
								num1 = a; bg3X = x; bg3Y = y;\
								ppu_wait_nmi();\
								displayNumber();\
							}\

#pragma code-name(push, "CODE9")
void loadAchievmentScreen()
{
	music_stop();
	scrollit = 4;
	ppu_off();
	oam_clear();
	asm("lda #%b", 0b00001001);
	asm("sta $2105");
	
	asm("lda #%b", 0b00110010); // BG1 tiles at 2000, BG2 tiles at 3000
	asm("sta $210B");
	
	asm("lda #%b", 0b01000101); // BG3 tiles at 5000,
	asm("sta $210C");
	
	asm("lda #%b", 0b00000010); // base address 0 screen height = 2 BG1
	asm("sta $2107");
	
	asm("lda #%b", 0b01110000); // base address 7000 screen height = 1 BG2
	asm("sta $2108");
	
	asm("lda #%b", 0b00010010); // base address 1000 screen height = 2 BG3
	asm("sta $2109");
	
	vram_adr(0x2000);
	// load BG1 charset
	vram_write_bank(CHR7_BANK, (char *) 0x8000, 3200);
	vram_write_bank(CHR7_BANK, (char *) 0x8000 + 3200 + 4096 + 7712 + 4096 + 7680 + 1794, 192);
	vram_adr(0x0000);
	vram_write_bank(CHR7_BANK, (char *) 0x8000 + 3200, 4096);
	
	
	vram_adr(0x5000);
	// load BG3 charset
	vram_write_bank(CHR7_BANK, (char *) 0x8000 + 3200 + 4096, 7712);
	vram_write_bank(CHR7_BANK, (char *) 0x8000 + 3200 + 4096 + 7712 + 4096 + 7680 + 1794 + 192, 352);
	vram_adr(0x1000);
	vram_write_bank(CHR7_BANK, (char *) 0x8000 + 3200 + 4096 + 7712, 4096);
	
	vram_adr(0x3000);
	// load BG2 charset
	vram_write_bank(CHR7_BANK, (char *) 0x8000 + 3200 + 4096 + 7712 + 4096, 7680);
	vram_adr(0x7000);
	vram_write_bank(CHR7_BANK, (char *) 0x8000 + 3200 + 4096 + 7712 + 4096 + 7680, 1794);
	
	pal_all(0, achi_palette);
	pal_all(1, achi_bg_palette);
	pal_all(2, fill_slot_palette);
	
	scroll(0, scrollit);
	scroll_bg3(0, scrollit);
	
	// set achieved to achieve vramX and vramY for every achievment separately, vramX same fo all achievments
//	curConBj = 1;
	achieved = curConPok;
	toAchieve = 10;
	vramX = 9;
	vramY = 10;
	fillSlot();
	printNumber(achieved, 28, 10);


	achieved = curConBj;
	toAchieve = 21;
	vramX = 9;
	vramY = 12;
	fillSlot();
	printNumber(achieved, 28, 12);
	
	achieved = curConBelWins;
	toAchieve = 10;
	vramX = 9;
	vramY = 14;
	fillSlot();
	printNumber(achieved, 28, 14);
	
	achieved = suitWins;
//	achieved = 10;
	toAchieve = 40;
	vramX = 9;
	vramY = 20;
	fillSlot();
	printNumber(achieved, 28, 20);
	
	achieved = bjWins;
	toAchieve = 100;
	vramX = 9;
	vramY = 16;
	fillSlot();
	printNumber(achieved, 27, 16);
	
	achieved = pokWins;
	toAchieve = 100;
	vramX = 9;
	vramY = 18;
	fillSlot();
	printNumber(achieved, 27, 18);
	
	achieved = playSingle;
	toAchieve = 50;
	vramX = 9;
	vramY = 32;
	fillSlot();
	printNumber(achieved, 28, 32);
	
	achieved = loseSingle;
	toAchieve = 50;
	vramX = 9;
	vramY = 34;
	fillSlot();
	printNumber(achieved, 28, 34);
	
	achieved = winSingle;
	toAchieve = 50;
	vramX = 9;
	vramY = 36;
	fillSlot();
	printNumber(achieved, 28, 36);
	
	achieved = playSingle100;
	toAchieve = 100;
	vramX = 9;
	vramY = 38;
	fillSlot();
	printNumber(achieved, 27, 38);
	
	achieved = winSingle1000;
	toAchieve = 1000;
	vramX = 9;
	vramY = 40;
	fillSlot();
	printNumber(achieved , 27, 40); 
	
	achieved = solUnder10;
	toAchieve = 100;
	vramX = 9;
	vramY = 22;
	fillSlot();
	printNumber(achieved, 27, 22);
	driveAchiMask();
	ppu_on_all();
}

void fillSlot()
{
	calculateHowMuch();
	if(!howMuch)
	return;
	vram_adr(NTADR1_A(vramX,vramY));
	vram_put(100 | 0b0010010000000000);
	vram_adr(NTADR1_A(vramX,vramY + 1));
	vram_put(103 | 0b0010010000000000);
	//fill first tile
	for(i = 1; i < howMuch - 1; i++)
	{
		vram_put(104 | 0b0010010000000000);
	}
	vram_adr(NTADR1_A(vramX + 1,vramY));
	for(i = 1; i < howMuch - 1; i++)
	{
		vram_put(101 | 0b0010010000000000);
	}
	
	if(howMuch == 23)
	{
		vram_put(102 | 0b0010010000000000);
		vram_adr(NTADR1_A(31,vramY + 1));
		vram_put(105 | 0b0010010000000000);
		// fill in last tile
	}
	i = 0;
}

void calculateHowMuch()
{
	if(toAchieve == 1000)
	{
		howMuch = (achieved * 23)/1000;
		return;
	}
	howMuch = snesdiv(snesmul(achieved, 23), toAchieve);
}

void achievmentScreen()
{
	playersJoypad = 0;
	readJoypad();
	if(currentInput == 4)
	{
		if((currentInput != previousInput) || ((unsigned char)(nesclock() - pressPoint)> 2))
		{
			pressPoint = nesclock();
			if(scrollit)
			--scrollit;
		}
		previousInput = currentInput;
	}
	else if(currentInput == 3)
	{
		if((currentInput != previousInput) || ((unsigned char)(nesclock() - pressPoint)> 2))
		{
			pressPoint = nesclock();
			if(scrollit < 140)
			++scrollit;
		}
		previousInput = currentInput;
	}
	else if(pad_trigger(0) & PAD_START)
	{
		ppu_off();
		screenState = 17;
		disableBG13Window();
		disableHDMA();
	}
	scroll(0, scrollit);
	scroll_bg3(0, scrollit);
}

static void displayNumber()
{
	num2 = num1;
	do
	{
		num2 = snesmod(num1, 10);

		vram_adr(0x1000 | ((bg3Y << 5) | bg3X));
		vram_put((((unsigned int)num2) + 482) | 0x2000);
		
		vram_adr(0x1000 | (((bg3Y + 1) << 5) | bg3X));
		vram_put((((unsigned int)num2) + 493) | 0x2000);
		
		num1 = snesdiv(num1, 10);
		--bg3X;
	}while(num1);
}

#pragma code-name(pop)
