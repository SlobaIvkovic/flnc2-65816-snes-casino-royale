#include "neslib.h"
#include "include/binsizes.h"
#include "snesgss.h"
//#include "include/palettes.h"

extern unsigned int bigBlind, buyIn;
unsigned int digitNumber1, digitNumber2;
unsigned char x, y;
extern unsigned char i, j;
unsigned char multiState;
unsigned char selCor, sX, sY;

unsigned char blinkTurn;
unsigned char printed;

extern unsigned char tileUpdate;
extern unsigned char gameSel;

extern unsigned long cash;

#pragma rodata-name(push, "RODATA")
#pragma data-name(push, "RODATA")
static unsigned short multi_screen_palette [] = {
0x146a, 0x388a, 0x0ccd, 0x412b, 0x1d36, 0x4628, 0x5e85, 0x0e3c, 0x2f1e, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static unsigned short multi_field_palette [] = {
0x0000, 0x0000, 0x0000, 0x146a, 0x388a, 0x0ccd, 0x412b, 0x1d36, 0x4628, 0x5e85, 0x0e3c, 0x2f1e, 0x0000, 0x0000, 0x0000, 0x0000,
};

#pragma data-name(pop)
#pragma rodata-name(pop)

void trampoline();
#pragma wrapped-call(push, trampoline, 0x90)
void displayBigNumbers();
void multiField();
void multiScreen();
void entering();
void pickAplace();
void individualPick();
void cornerPrint();
void middlePrint();
void multiSelector();
void blindBuyIn();
void incrementBlind();
void dimTokens();
void pressSta();
void blinking();
void loadBelote();
void setBackgrounds();
#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x85)
void displayMessage(char *message);
void loadGame();
#pragma wrapped-call(pop)

#define printSelector(a, b){\
								sX = a;\
								sY = b;\
								multiSelector();\
							}\

#define printMessage(x, y, mess){\
									ppu_wait_nmi();\
									vram_adr((0x2C00 | ((y << 5) | x)));\
									displayMessage(mess);\
								}\

#define printBigNumbers(a, b, c){\
									x = a;\
									y = b;\
									digitNumber1 = c;\
									displayBigNumbers();\
								}\

#define printField(a, b){\
							x = a;\
							y = b;\
							multiField();\
						}\

#define clearField(a){\
						vram_adr(BG3_NT_ADR(14, a));\
						vram_fill(0, 10);\
						vram_adr(BG3_NT_ADR(14, a+1));\
						vram_fill(0, 10);\
						vram_adr(BG3_NT_ADR(14, a+2));\
						vram_fill(0, 10);\
					}\

#define BG1_NT_ADR(x, y) (0x2400 | ((y<<5) | (x)))
#define BG3_NT_ADR(x, y) (0x2C00 | ((y<<5) | (x)))



unsigned char allEntered, entered, picked;
unsigned char seats[4];
unsigned char pl[4];
unsigned char individual;

#pragma code-name(push, "CODE8")

void multiField()
{
	vram_adr(BG1_NT_ADR(x, y));
	for(i = 0; i < 6; i++)
	{
		vram_put(i | 0x2800);
		if(i == 2)
		{
			vram_put(i | 0x2800);
			vram_put(i | 0x2800);
		}
	}
	vram_adr(BG1_NT_ADR(x, y+1));
	for(i = 0; i < 6; i++)
	{
		vram_put((i + 6) | 0x2800);
		if(i == 2)
		{
			vram_put((i + 6) | 0x2800);
			vram_put((i + 6) | 0x2800);
	
		}
	}
	i = 0;
}

void displayBigNumbers()
{
//	unsigned char x = 9;
//	td(2, 25, "player");
ppu_wait_nmi();
	
//	digitNumber1 = buyIn;
	
	--x;
	do
	{
		digitNumber2 = snesmod(digitNumber1, 10);
		vram_adr(BG3_NT_ADR(x, y));
		vram_put((digitNumber2 + 96) | 0x2800);
		vram_adr(BG3_NT_ADR(x, y+1));
		vram_put((digitNumber2 + 96 + 10) | 0x2800);
		vram_adr(BG3_NT_ADR(x, y+2));
		vram_put((digitNumber2 + 96 + 20) | 0x2800);
		--x;
		
		digitNumber1 = snesdiv(digitNumber1 , 10);
	}while(digitNumber1 > 0);

	
}


void loadMultiPscreen()
{
	ppu_off();
	music_stop();
	pal_clear();
	
		// SPRITE SET 1 goes to 0x0000 SPRITE SET 2 GOES to 0x1000
	asm("lda #0");
	asm("sta $2101");
	
	asm("lda #%b", 0b00001001);
	asm("sta $2105");
	
		// SPRITE SET 1 goes to 0x0000 SPRITE SET 2 GOES to 0x1000
	asm("lda #0");
	asm("sta $2101");
	
	// BG1 CHARSET starts at 0x3000 BG2 CHARSET starts at 0x5000
	asm("lda #%b", 0b01010011);
	asm("sta $210B");
	
	// BG3 CHARSET starts at 0x7000
	asm("lda #%b", 0b00000111);
	asm("sta $210C");
	
	// BG1 MAP start at 0x2400
	asm("lda #%b", 0b00100100);
	asm("sta $2107");
	
	// BG2 MAP start at 0x2800
	asm("lda #%b", 0b00101000);
	asm("sta $2108");
	
	// BG3 MAP start at 0x2C00
	asm("lda #%b", 0b00101100);
	asm("sta $2109");
	
	vram_adr(0x0000);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + SOL_BACK_S, 128);
	
	
	vram_adr(0x5000);
	// load BG2 charset
		vram_write_bank(CHR4_BANK, (char *) 0x8000 + 1120 + 1504, 4000);
		// load BG2 map
		vram_adr(0x2800);
		vram_write_bank(CHR4_BANK, (char *) 0x8000 + 1120 + 1504 + 4000, 1794);
	
	
	
	vram_adr(0x3000);
	
	
		vram_write_bank(CHR4_BANK, (char *) 0x8000 + 1120 + 1504 + 4000 + 1794, 384);
	//	vram_adr(0x386);
		vram_fill(0, 64);

	
		vram_adr(0x2400);
		vram_fill(12, 2048);
		vram_adr(0x2C00);
		vram_fill(0, 2048);
	
	
	
	// load BG3
	vram_adr(0x7000);
	// THISI IS NOT FINISHED LOAD ENDSCREEN NUMS DOLAR AND LETTERS here
	//vram_write_bank(0x87, (char *) 0x8000 + 11328 + 12192 + 1794 + 5312, 480 + 416 + 64);
	vram_write_bank(CHR2_BANK, (char *) 0x8000+BETTING_ROOM_TOTAL_S+END_SCREEN_BASE_S+END_SCREEN_BASE_MAP_S+JACKPOT_S
	                                          +JACKPOT_MAP_S+LOSE_MESSAGE_S+LOSE_MESSAGE_MAP_S + END_SELECTOR_S, END_SCREEN_FONT_S);
	
	vram_adr(0x7300);
	vram_write_bank(CHR3_BANK, (char *) 0x8000 + CARDS_S + BLACKJACK_TABLE_S + BLACKJACK_TABLE_MAP_S + CARD_SET_2_S, NUMS_S);
	vram_write_bank(CHR3_BANK, (char *) 0x8000 + CARDS_S + BLACKJACK_TABLE_S + BLACKJACK_TABLE_MAP_S + CARD_SET_2_S + NUMS_S, LETTERS_S);
	vram_write_bank(CHR3_BANK, (char *) 0x8000 + CARDS_S + BLACKJACK_TABLE_S + BLACKJACK_TABLE_MAP_S + CARD_SET_2_S + NUMS_S + LETTERS_S, SEL_S);
	vram_write_bank(CHR3_BANK, (char *) 0x8000 + CARDS_S + BLACKJACK_TABLE_S + BLACKJACK_TABLE_MAP_S + CARD_SET_2_S + NUMS_S + LETTERS_S + SEL_S, DOLAR_S);
//	pal_all(0, sol_font_palette);
// 0x388a, 0x0c74, 0x023a, 0x2f1e,
	pal_col(0, 0x388a);
	pal_col(1, 0x0c74);
	pal_col(2, 0x023a);
	pal_col(3, 0x2f1e);
	
// 0x24a7, 0x14ab, 0x0d4d, 0x21af,	
	pal_col(4, 0x24a7);
	pal_col(5, 0x14ab);
	pal_col(6, 0x0d4d);
	pal_col(7, 0x21af);
	
	pal_col(8, 0x388a);
	pal_col(11, 0x0c74);
	pal_col(10, 0x023a);
	pal_col(9, 0x2f1e);
	
	pal_col(128, 0xFFFF);
	pal_col(129, 0xFFFF);
	pal_col(130, 0xFFFF);
	pal_col(131, 0xFFFF);
	pal_col(132, 0xFFFF);
	pal_col(133, 0xFFFF);
	// Make BG2 to use second palette                                          
	pal_all(1, multi_screen_palette);
	pal_all(2, multi_field_palette);
	// Make BG1 (field) to use third palette
	oam_clear();
//	if(multitap_connected())
	buyIn = 1000;
	bigBlind = 50;
	clearField(17);
	printBigNumbers(19, 17, buyIn);
	printField(12, 18);
	clearField(21);
	printBigNumbers(19, 21, bigBlind);
	printField(12, 22);
//	printMessage(10, 10, "Press Start");
	dimTokens();
	scroll(0, 4);
	scroll_bg3(0, 0);
/*	asm("lda #0");
	__asm__("sta $2112");
	asm("lda #0");
	asm("sta $2112");
*/	
	
	// Reset variables before entering
	multiState = 0;
	entered = 0; picked = 0; multiState = 0;
	seats[0] = 0; seats[1] = 0; seats[2] = 0; seats[3] = 0;
	pl[0] = 0; pl[1] = 0; pl[2] = 0; pl[3] = 0;
	allEntered = 0; entered = 0;
	printed = 0;
	
	ppu_on_all();
	                                          
}

void multiScreen()
{
//	if(allEntered != 2)
//	{
//		entering();
//	}
//	--entered;
//	if(entered != picked)
//	{
//		pickAplace();
//	}
/*	else if(picked != 10)
	{
		// chose big blind and buy in
	}
	else
	{
		// load poker screen and change screen state to poker
	}*/
	if(!multiState)
	{
		entering();
		pickAplace();
		if((entered == picked) && (picked != 0))
		{
		//	printMessage(12, 9, "Press Start");
			if(pad_trigger(0) & PAD_START)
			{
				printSelector(96, 139);
				++multiState;
			}
		}
	}
	else if(multiState == 1)
	{
		printMessage(10, 10, "           ");
		blindBuyIn();
		
	}
	else
	{
		j = 0;
		for(i = 0; i < 4; i++)
		{
			if(!seats[i])
			{
				seats[i] = 6 + j;
				++j;
			}
		}
		i = 0;
		j = 0;
		loadGame();
	}
}


void entering()
{
	if((pad_trigger(0) & PAD_START) && !pl[0])
	{
		++allEntered;
		printMessage(12, 11, "1p");
		pl[0] = 1;
		++entered;
	}
	if((pad_trigger(1) & PAD_START) && !pl[1])
	{
		printMessage(18, 11, "2p");
		pl[1] = 1;
		++entered;
	}
	if((pad_trigger(2) & PAD_START) && !pl[2])
	{
		printMessage(12, 14, "3p");
		pl[2] = 1;
		++entered;
	}
	if((pad_trigger(3) & PAD_START) && !pl[3])
	{
		printMessage(18, 14, "4p");
      pl[3] = 1;
      ++entered;
	}
	if(entered && !printed)
	{
		if(gameSel == 2)
		printMessage(10, 9, "Pick a place");
	//	else
	//	printMessage(10, 9, "Pick a team");
		printed = 1;
	}
	
}

void pickAplace()
{
	individual = 0;
	individualPick();
/*	if(seats[0] == 1)
	{
		x = 6;
		y = 4;
		pressSta();	
	}*/
	individual = 1;
	individualPick();
/*	if(seats[1] == 1)
	{
		x = 24;
		y = 4;
		pressSta();	
	}*/
	individual = 2;
	individualPick();
/*	if(seats[2] == 1)
	{
		x = 24;
		y = 20;
		pressSta();	
	}*/
	individual = 3;
	individualPick();
/*	if(seats[3] == 1)
	{
		x = 6;
		y = 20;
		pressSta();	
	}*/
	if(seats[blinkTurn] == 1)
	{
		blinking();
	}
	++blinkTurn;
	if(blinkTurn == 4)
	blinkTurn = 0;
/*	individual = 0;
	individualPick();
	individual = 1;
	individualPick();
	individual = 2;
	individualPick();
	individual = 3;
	individualPick();*/
/*	if(seats[0] == 1)
	{
		x = 6;
		y = 4;
		pressSta();	
	}
	if(seats[1] == 1)
	{
		x = 24;
		y = 4;
		pressSta();	
	}
	if(seats[2] == 1)
	{
		x = 24;
		y = 20;
		pressSta();	
	}
	if(seats[3] == 1)
	{
		x = 6;
		y = 20;
		pressSta();	
	}
*/						
}

void individualPick()
{
	if(!seats[0] && pl[individual] == 1)
		{
		//	if(!(pad_trigger(individual) ^ 0x00A00))
			if((pad_trigger(individual) & PAD_UP) && pad_state(individual) & PAD_LEFT)
			{
				x = 6;
				y = 4;
				cornerPrint();
				pl[individual] = 2;
				seats[0] = 1;
			}
		}
		if(!seats[1] && pl[individual] == 1)
		{	
			if((pad_trigger(individual) & PAD_UP) && pad_state(individual) & PAD_RIGHT)
			{
				x = 24;
				y = 4;
				cornerPrint();
				pl[individual] = 3;
				seats[1] = 1;
			}
		}
		if(!seats[2] && pl[individual] == 1)
		{
			if((pad_trigger(individual) & PAD_DOWN) && pad_state(individual) & PAD_RIGHT)
			{
				x = 24;
				y = 20;
				cornerPrint();
				pl[individual] = 4;
				seats[2] = 1;
			}
		}
		if(!seats[3] && pl[individual] == 1)
		{
			if((pad_trigger(individual) & PAD_DOWN) && pad_state(individual) & PAD_LEFT)
			{
				x = 6;
				y = 20;
				cornerPrint();
				pl[individual] = 5;
				seats[3] = 1;
			}
		}
	
		else if(pl[individual] == 2)
		{
			x = 6;
			y = 4;
			if((pad_trigger(individual) & PAD_DOWN) && pad_state(individual) & PAD_RIGHT)
			{
			//	x = 6;
			//	y = 4;
				middlePrint();
				pl[individual] = 1;
				seats[0] = 0;
			}
			else if((pad_trigger(individual) & PAD_START))
			{
				pl[individual] = 7;
				seats[0] = individual + 2;
				++picked;
				printMessage(x - 4, y - 3, "Ready      ");
			}
		}
		else if(pl[individual] == 3)
		{
			x = 24;
			y = 4;
			if((pad_trigger(individual) & PAD_DOWN) && pad_state(individual) & PAD_LEFT)
			{
			//	x = 24;
			//	y = 4;
				middlePrint();
				pl[individual] = 1;
				seats[1] = 0;
			}
			else if((pad_trigger(individual) & PAD_START))
			{
				pl[individual] = 7;
				seats[1] = individual + 2;
				++picked;
				printMessage(x - 4 , y - 3, "Ready      ");
			}
		}
		else if(pl[individual] == 4)
		{
			x = 24;
			y = 20;
			if((pad_trigger(individual) & PAD_UP) && pad_state(individual) & PAD_LEFT)
			{
			//	x = 24;
			//	y = 20;
				middlePrint();
				pl[individual] = 1;
				seats[2] = 0;
			}
			else if((pad_trigger(individual) & PAD_START))
			{
				pl[individual] = 7;
				seats[2] = individual + 2;
				++picked;
				printMessage(x - 4, y - 3, "Ready      ");
			}
		}
		else if(pl[individual] == 5)
		{
			x = 6;
			y = 20;
			if((pad_trigger(individual) & PAD_UP) && pad_state(individual) & PAD_RIGHT)
			{
			//	x = 6;
			//	y = 20;
				middlePrint();
				pl[individual] = 1;
				seats[3] = 0;
			}
			else if((pad_trigger(individual) & PAD_START))
			{
				pl[individual] = 7;
				seats[3] = individual + 2;
				++picked;
				printMessage(x - 4, y - 3, "Ready      ");
			}
		}
}

void cornerPrint()
{
	if(!individual)
	{
		// set x y above, then print 1p 2p etc using x and y
		printMessage(x, y, "1p");
		printMessage(12, 11, "  ")
	}
	else if(individual == 1)
	{
		printMessage(x, y, "2p");
		printMessage(18, 11, "  ");
	}
	else if(individual == 2)
	{
		printMessage(x, y, "3p");
		printMessage(12, 14, "  ")
	}
	else if(individual == 3)
	{
		printMessage(x, y, "4p");
		printMessage(18, 14, "  ");
	}
	
}

void middlePrint()
{
	if(!individual)
	{
		// set x y above, then print 1p 2p etc using x and y
		printMessage(12, 11, "1p");
		printMessage(x, y, "  ")
	}
	else if(individual == 1)
	{
		printMessage(18, 11, "2p");
		printMessage(x, y, "  ");
	}
	else if(individual == 2)
	{
		printMessage(12, 14, "3p");
		printMessage(x, y, "  ")
	}
	else if(individual == 3)
	{
		printMessage(18, 14, "4p");
		printMessage(x, y, "  ")
	}
	printMessage(x-4, y - 3, "           ");
}

void multiSelector()
{
	oam_spr(sX, sY, 0, SPRITE_ATTR_FULL(0, 3, 0, 0, 0), 0);
	oam_spr(sX+56, sY, 0, SPRITE_ATTR_FULL(0, 3, 0, 1, 0), 4);
	oam_spr(sX+56, sY+8, 0, SPRITE_ATTR_FULL(0, 3, 1, 1, 0), 8);
	oam_spr(sX, sY+8, 0, SPRITE_ATTR_FULL(0, 3, 1, 0, 0), 12);
}


void blindBuyIn()
{
	if(pad_trigger(0) & PAD_UP)
	{
		selCor = 0;
		printSelector(96, 139);
	}
	else if(pad_trigger(0) & PAD_DOWN)
	{
		selCor = 1;
		printSelector(96, 171);
	}
	else if(pad_trigger(0) & PAD_B)
	{
		if(selCor)
		{
			incrementBlind();
			clearField(21);
			printBigNumbers(19, 21, bigBlind);
		}
		else
		{
			if(buyIn == 100)
			{
				buyIn = 1000;
				bigBlind = 50;
			//	printBigNumbers(19, 17, buyIn);
			//	printMessage(1, 1, "1");
			//	return;	
			}
			else if(buyIn == 1000)
			{
				buyIn = 10000;
				bigBlind = 500;
			//	printBigNumbers(19, 17, buyIn);
			//	printMessage(1, 1, "2");
			//	return;	
			}
			else if(buyIn == 10000)
			{
				buyIn = 100;
				bigBlind = 10;
			}
				clearField(17);
				printBigNumbers(19, 17, buyIn);
				clearField(21);
				printBigNumbers(19, 21, bigBlind);
			//	printMessage(1, 1, "3");
			//	return;	
		//	}
			
		}
	}
	else if((pad_trigger(0) & PAD_START) && cash >= buyIn)
	{
		cash -= buyIn;
		++multiState;
	}
}

void incrementBlind()
{
	if(buyIn == 100)
	{
		if(bigBlind == 10)
		bigBlind = 2;
		else
		bigBlind += 2;
	}
	if(buyIn == 1000)
	{
		if(bigBlind == 50)
		bigBlind = 10;
		else
		bigBlind += 10;
	}
	if(buyIn == 10000)
	{
		if(bigBlind == 500)
		bigBlind = 100;
		else
		bigBlind += 100;
	}
}

void dimTokens()
{
	ppu_wait_nmi();
	vram_adr(BG3_NT_ADR(12, 11));
	vram_put(17 | 0x2400);
	vram_put(80 | 0x2400);
	vram_adr(BG3_NT_ADR(18, 11));
	vram_put(18 | 0x2400);
	vram_put(80 | 0x2400);
	vram_adr(BG3_NT_ADR(12, 14));
	vram_put(19 | 0x2400);
	vram_put(80 | 0x2400);
	vram_adr(BG3_NT_ADR(18, 14));
	vram_put(20 | 0x2400);
	vram_put(80 | 0x2400);
}

void pressSta()
{
	++tileUpdate;
	if(tileUpdate == 50)
	tileUpdate = 0;
	if(tileUpdate < 25)
	{
		printMessage(x-4, y - 3, "Press Start");
	}
	else
	{
		printMessage(x-4, y - 3, "            ");
	}
}

void blinking()
{
	if(blinkTurn == 0)
	{
		x = 6;
		y = 4;
		pressSta();
		return;	
	}
	if(blinkTurn == 1)
	{
		x = 24;
		y = 4;
		pressSta();
		return;	
	}
	if(blinkTurn == 2)
	{
		x = 24;
		y = 20;
		pressSta();
		return;	
	}
	if(blinkTurn == 3)
	{
		x = 6;
		y = 20;
		pressSta();
		return;	
	}
}




void setBackgrounds()
{
			// SPRITE SET 1 goes to 0x0000 SPRITE SET 2 GOES to 0x1000
	asm("lda #0");
	asm("sta $2101");
	
	asm("lda #%b", 0b00001001);
	asm("sta $2105");
	
		// SPRITE SET 1 goes to 0x0000 SPRITE SET 2 GOES to 0x1000
	asm("lda #0");
	asm("sta $2101");
	
	// BG1 CHARSET starts at 0x3000 BG2 CHARSET starts at 0x5000
	asm("lda #%b", 0b01010011);
	asm("sta $210B");
	
	// BG3 CHARSET starts at 0x7000
	asm("lda #%b", 0b00000111);
	asm("sta $210C");
	
	// BG1 MAP start at 0x2400
	asm("lda #%b", 0b00100100);
	asm("sta $2107");
	
	// BG2 MAP start at 0x2800
	asm("lda #%b", 0b00101000);
	asm("sta $2108");
	
	// BG3 MAP start at 0x2C00
	asm("lda #%b", 0b00101100);
	asm("sta $2109");
}
#pragma code-name(pop)
