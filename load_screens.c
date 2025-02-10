#include "neslib.h"
#include "include/drawing.h"
#include "include/defines.h"
#include "include/palettes.h"
#include "snesgss.h"
#include "include/binsizes.h"
#include "include/save.h"
#include "include/mask.h"
#include "include/inplayachi.h"
#include "include/move_vars.h"


extern unsigned char pickORdisplay, deal4state;
extern unsigned char allEntered;
extern unsigned int buyIn;

// Belote and Poker share some variables for the multiscreen reset them when belote screen is loaded
// in case of poker theese variables are reset when multiscreen is loaded, multiscreen load is in separate file
extern unsigned char pl[4], allEntered, entered, printed;

//-----------------------------------
// Title screen variables
extern unsigned char gameSel;
extern unsigned char spY;
extern unsigned char spX;
extern unsigned char availableSprid;
extern unsigned char bigSelX;
//------------------------------------

//------------------------------------
// Betting screen variables
extern unsigned long cash;
extern unsigned long cash1;
unsigned long cash2;
extern unsigned char upperLower;
extern unsigned long betAmount;
extern unsigned char allInFlag;
extern unsigned char chipY[6];
//------------------------------------

//------------------------------------
extern const unsigned char title_screen_1_chr[];
extern const unsigned char title_screen_map[];
extern const unsigned char title_screen_map[];
extern const unsigned char title_big_selector[];
extern const unsigned char title_numbers[];


//----------------------------------------------
// END SCREEN variables and functions
extern unsigned char winLoseState;
extern unsigned char endSel;
extern unsigned long winings;
extern unsigned char digit;
unsigned int position;
unsigned char position1;


extern unsigned char seconds;
extern unsigned char minutes;
extern unsigned char hours;
//-----------------------------------------------


extern unsigned char i, j, screenState;
extern unsigned char clockCount;
extern unsigned char vramX, vramY;

//Print number variables
extern unsigned long num1, num2;
extern unsigned char bg3X, bg3Y;
//

extern unsigned char k, winner;
unsigned long pokerCash;

extern unsigned char music3[];
extern unsigned char music5[];
extern unsigned char music6[];
extern unsigned char music7[];

#pragma wrapped-call(push, trampoline, 0x85)
void displayWinings();
void displayTime();
//void displayMessage(char* message);
#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x90)
void loadBelote();
void beloteEnd();
#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x90)
void setBackgrounds();
void dimTokens();
#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x91)
static void displayNumber();
void pokerEnd();
void singlePlayerAchievments();
static void displayMessage(char *message);
void printAchievments();
void resetAchievments();
void resetMoveVars();
#pragma wrapped-call(pop)


#define printMessage(x, y, mess){\
									ppu_wait_nmi();\
									vram_adr((0x5400 | ((y << 5) | x)));\
									displayMessage(mess);\
								}\

#define printNumber(a, x, y) {\
								num1 = a; bg3X = x; bg3Y = y;\
								ppu_wait_nmi();\
								displayNumber();\
							}\

#pragma code-name(push, "CODE4")
void loadTitleScreen()
{
	gameSel = 0;
	olchipper = 0;
	ppu_off();
	oam_clear();
	asm("lda #3");
	asm("sta $2105");
	
	asm("lda #%b", 0b00110001); // BG1 tiles at 0x1000, BG2 tiles at 0x2000
	asm("sta $210B");
	 
	asm("lda #%b", 0b00000000); // base address 0 screen height = 1 BG1
	asm("sta $2107"); 
	
	asm("lda #%b", 0b00000100); // base address 0x0400 screen height = 1 BG2
	asm("sta $2108"); 
	 
	asm("lda #%b", 0b01010100); // base address 0x5400 screen height = 1 BG3
	asm("sta $2109");
	 
	vram_adr(0x0400);
	vram_fill(1, 0x700);
	// upload charset
	vram_adr(0x1000);
	vram_write_bank(CHR0_BANK, (char *) 0x8000, TITLE_SCREEN_1_2_S);
	vram_adr(0x1000 + 10736);
	vram_write_bank(CHR1_BANK, (char *) 0x8000, TITLE_SCREEN_2_2_S);
	// upload charmap
	vram_adr(0x0000);
	vram_write_bank(CHR0_BANK, (char *) 0x8000 + TITLE_SCREEN_1_2_S, TITLE_SCREEN_MAP_S);
	
	
	
	vram_adr(0x7000);
	vram_write_bank(CHR1_BANK, (char *) 0x8000 + TITLE_SCREEN_2_2_S, 2240);
	vram_adr(0x7000 + 1120);
	vram_write_bank(CHR1_BANK, (char *) 0x8000 + TITLE_SCREEN_2_2_S + 2240, 704);
	
	
	scroll(0, 255);
		asm("lda #255");
	__asm__("sta $2110");
	asm("lda #0");
	asm("sta $2110");	
	
	
	pal_clear();
	pal_all(0, title_palette);
	pal_all(1, &title_palette[16]);
//	pal_all(2, &complete_palette[32]);
	pal_col(129, 0xffff);
	pal_col(145,  0x0e3c);
	pal_col(146, 0x2f1e);
	
	
	bigSelX = 11;
	
	
	gameSel = 0;
				
	spX = bigSelX;
	spY = 94;
	drawBigSelectorSprite();
	spY = 16;
	spX = 32;
	availableSprid = 128;
	drawCash();
	
	SetupColorWindow();
	driveMask(0);
	
	ppu_on_all();
	music_play(0x88, music6);
}

void loadBettingScreen()
{
	music_stop();
	cash2 = cash;
	olchipper = 0;
	ppu_off();
	
	vram_adr(0x11F0);
		vram_fill(0, 64);

		vram_adr(NTADR_A(0, 0));
		vram_fill(31, 2048);
	
	asm("lda #%b", 0b00001001);
	asm("sta $2105");
	
		asm("lda #3");
	asm("sta $2101");
	
	asm("lda #%b", 0b00001001);
	asm("sta $2105");
	
	asm("lda #%b", 0b00110001); // BG1 tiles at 0x1000, BG2 tiles at 0x2000
	asm("sta $210B");
	 
	asm("lda #%b", 0b00000000); // base address 0 screen height = 1 BG1
	asm("sta $2107"); 
	
	asm("lda #%b", 0b00000100); // base address 0x0400 screen height = 1 BG2
	asm("sta $2108"); 
	 
	asm("lda #%b", 0b01010100); // base address 0x5400 screen height = 1 BG3
	asm("sta $2109");
	
	// BG3 CHARSET starts at 0x5000
	asm("lda #%b", 0b00000101);
	asm("sta $210C");
	
	// load BG1 charset
	vram_adr(0x1000);
	vram_write_bank(CHR2_BANK, (char *) 0x8000 + BETTING_ROOM_S + BETTING_ROOM_MAP_S, COLORED_CHIPS_S);
	// load BG2 charset
	
	// load betiing room cahrset
	vram_adr(0x3000);
	vram_write_bank(CHR2_BANK, (char *) 0x8000, BETTING_ROOM_S);
	// load betting room map
	vram_adr(0x400);
	vram_write_bank(CHR2_BANK, (char *) 0x8000 + BETTING_ROOM_S, BETTING_ROOM_MAP_S);
	

	
	vram_adr(0x6000);
	vram_write_bank(0x8C, (char *) 0x8000 + 8128 + 1794 + 3776 + 1794 + 112, 96);
	
	vram_adr(0x7000);
	vram_write_bank(0x8A, (char *) 0x8000 + 21472, 2240);
	vram_adr(0x7000 + 1120);
	vram_write_bank(0x8A, (char *) 0x8000 + 21472 + 2240, 704);

	vram_adr(0x5000);
	vram_write_bank(CHR2_BANK, (char *) 0x8000 + BETTING_ROOM_S + BETTING_ROOM_MAP_S + COLORED_CHIPS_S, BETTING_SELECTOR_S);
	
	

	vram_adr(0x5400);
	vram_fill(3, 2048);
	
	scroll(0, 255);
	ppu_wait_nmi();
	asm("lda #255");
	__asm__("sta $2110");
	asm("lda #0");
	asm("sta $2110");
	
	asm("lda #255");
	__asm__("sta $2111");
//	asm("lda #0");
//	asm("sta $2112");
	scroll_bg3(0, 255);
	// end of scolling portion
	
//	pal_all(0, betting_selector_palette);
	pal_clear();
	pal_col(0, 0x0000);
	pal_col(1, 0x7fff);
	pal_all(1, betting_room_palette);
	pal_all(3, chips_palette);
	pal_all(2, &chips_palette[16]);
	
	pal_all(8, chips_palette);
	pal_col(145,  0x0e3c);
	pal_col(146, 0x2f1e);
	

	chipY[0] = 0;
	chipY[1] = 0;
	chipY[2] = 0;
	chipY[3] = 0;
	chipY[4] = 0;
	chipY[5] = 0;
	upperLower = 0;
	betAmount = 0;
	allInFlag = 0;
	ppu_wait_nmi();
	oam_clear();
	spX = 50;
	spY = 16;
	availableSprid =  132;
//	availableSprid = 240;
	drawCash();
	ppu_on_all();
	ppu_wait_nmi();
}

void loadBlackJack()
{
	ppu_off();
	asm("lda #%b", 0b00001001);
	asm("sta $2105");
	
	vram_adr(0x1000);
	// load BG1 charset
	vram_write_bank(CHR3_BANK, (char *) 0x8000, CARDS_S);
	// load BG2 charset
	vram_adr(0x3000);
	vram_write_bank(CHR3_BANK, (char *) 0x8000 + CARDS_S, BLACKJACK_TABLE_S);
	
	vram_adr(0x400);
	vram_write_bank(CHR3_BANK, (char *) 0x8000 + CARDS_S + BLACKJACK_TABLE_S, BLACKJACK_TABLE_MAP_S);
	vram_adr(NTADR_A(0, 0));
	for(i = 0; i < 32; i++)
	{
		for(j = 0; j < 32; j++)
		{
			vram_put(353);
		}
	}

	// Load it to the sprite tiles as well
	vram_adr(0x6000);
	vram_write_bank(CHR3_BANK, (char *) 0x8000, CARDS_S);
	// load to second sprite tiles set
	vram_adr(0x7000);
	vram_write_bank(CHR3_BANK, (char *) 0x8000 + CARDS_S + BLACKJACK_TABLE_S + BLACKJACK_TABLE_MAP_S, CARD_SET_2_S);
	
	// load BG3 used for nums letters and selector
	vram_adr(0x5000);
	vram_write_bank(CHR3_BANK, (char *) 0x8000 + CARDS_S + BLACKJACK_TABLE_S + BLACKJACK_TABLE_MAP_S + CARD_SET_2_S, NUMS_S);
	vram_write_bank(CHR3_BANK, (char *) 0x8000 + CARDS_S + BLACKJACK_TABLE_S + BLACKJACK_TABLE_MAP_S + CARD_SET_2_S + NUMS_S, LETTERS_S);
	vram_write_bank(CHR3_BANK, (char *) 0x8000 + CARDS_S + BLACKJACK_TABLE_S + BLACKJACK_TABLE_MAP_S + CARD_SET_2_S + NUMS_S + LETTERS_S, SEL_S);
	vram_write_bank(CHR3_BANK, (char *) 0x8000 + CARDS_S + BLACKJACK_TABLE_S + BLACKJACK_TABLE_MAP_S + CARD_SET_2_S + NUMS_S + LETTERS_S + SEL_S, DOLAR_S);
	
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + SOL_BACK_S + 128, 128);
//	vram_write_bank(CHR6_BANK, (char *) 0x8000 + SOL_BACK_S + 128 + 128, 224);
	vram_write_bank(CHR8_BANK, (char *) 0x8000 + 10592 + 1792 + 1440 + 320 + 1120, 224);
	vram_fill(0, 64);
	
	// load card back to second sprite set, CardBack.chr
	vram_adr(0x7A60); // + 2656);
	vram_write_bank(CHR4_BANK, (char *) 0x8000 , CARD_BACK_S);
	
	vram_write_bank(CHR8_BANK, (char *) 0x8000 + 10592 + 1792 + 1440 + 320, 1120);
	
	// load card back into bg1 tiles CardBack.chr
	vram_adr(0x1000 + 5664);
	vram_write_bank(CHR4_BANK, (char *) 0x8000 , CARD_BACK_S);
	
	vram_adr(0x5400);
	for(i = 0; i < 32; i++)
	{
		for(j = 0; j < 32; j++)
		{
			vram_put(85);
		}
	}
	
	oam_clear();

	scroll(0, 250);
	ppu_wait_nmi();
	asm("lda #255");
	__asm__("sta $2110");
	asm("lda #0");
	asm("sta $2110");
	
	scroll_bg3(0, 255);
	
	pal_all(0, cards_palette);
	pal_all(1, &cards_palette[16]);
	pal_all(2, &cards_palette[32]);
	pal_all(3, &cards_palette[48]);
	pal_all(4, &cards_palette[64]);
	pal_all(5, blackjack_table_palette);
	pal_all(6, back_card_palette);
	pal_all(7, letter_number_palette);
	
	pal_all(8, cards_palette);
	pal_all(9,  &cards_palette[16]);
	pal_all(10, &cards_palette[32]);
	pal_all(11, &cards_palette[48]);
	pal_all(12, &cards_palette[64]);
	pal_all(13, letsel_palette);
//	pal_col()
	pal_all(14, &card_set_2_palette[16]);
	pal_all(15, back_card_palette);
	// 253c, 5e85, 0e3c, 3b0f, 7fff
	pal_col(247, 0x253c);
	pal_col(248, 0x5e85);
	pal_col(249, 0x0e3c);
	pal_col(250, 0x3b0f);
	pal_col(251, 0x7fff);	
	
	displayPlayerCash();
	
	pickORdisplay = 0;
	deal4state = 0;
	ppu_on_all();
	ppu_wait_nmi();
	music_play(0x88, music3);
}

extern unsigned char tile;
void displaySeconds();
void loadSolitaire()
{
	tile = 8;
	ppu_off();
	asm("lda #%b", 0b00001001);
	asm("sta $2105");
	
	vram_adr(0x0000);
	vram_write_bank(CHR5_BANK, (char *) 0x8000 + SOLITAIRE_TABLE_S + SOLITAIRE_TABLE_MAP_S + SOLITAIRE_CARDS_S, SOLITAIRE_CARDS_16_S);
	// use remaining space in first sprite memory to place selector there, selector size is 128, place one half of the selector sprites
	vram_adr(0x80);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + SOL_BACK_S, 64);
	vram_adr(0x180);
	// then the other half of the selector
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + SOL_BACK_S, 64);
	vram_adr(0x1000);
	vram_write_bank(CHR5_BANK, (char *) 0x8000 + SOLITAIRE_TABLE_S + SOLITAIRE_TABLE_MAP_S + SOLITAIRE_CARDS_S + 8192, 16384 - 8192);

	
	vram_adr(0x3000);
	// load BG1 charset
	vram_write_bank(CHR5_BANK, (char *) 0x8000 + SOLITAIRE_TABLE_S + SOLITAIRE_TABLE_MAP_S, SOLITAIRE_CARDS_S);
	// Load BG2
	vram_adr(0x5000);
	vram_write_bank(CHR5_BANK, (char *) 0x8000, SOLITAIRE_TABLE_S);
	// load BACK CARD TO BG1
	vram_adr(0x3000 + 9312/2);
	vram_write_bank(CHR6_BANK, (char *) 0x8000, SOL_BACK_S);
	// load BG3
	vram_adr(0x7000);
	// THISI IS NOT FINISHED LOAD ENDSCREEN NUMS DOLAR AND LETTERS here
//	vram_write_bank(0x87, (char *) 0x8000 + 11328 + 12192 + 1794 + 5312, 480 + 416 + 64);
	vram_write_bank(CHR2_BANK, (char *) 0x8000+BETTING_ROOM_TOTAL_S+END_SCREEN_BASE_S+END_SCREEN_BASE_MAP_S+JACKPOT_S
	                                          +JACKPOT_MAP_S+LOSE_MESSAGE_S+LOSE_MESSAGE_MAP_S + END_SELECTOR_S, END_SCREEN_FONT_S);
	// Pause dialog
	vram_adr(0x72E0);
	// then the other half of the selector
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + SOL_BACK_S + 128, 128);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + SOL_BACK_S + 128 + 128, 224);
	vram_adr(0x1000);
	
	
	
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
	
	// LOAD BG2 map
	vram_adr(0x2800);
	vram_write_bank(CHR5_BANK, (char *) 0x8000 + SOLITAIRE_TABLE_S, SOLITAIRE_TABLE_MAP_S);
//	vram_adr(NTADR_A(0, 0));
	vram_adr(0x2400);
	for(i = 0; i < 32; i++)
	{
		for(j = 0; j < 32; j++)
		{
			vram_put(4);
		}
	}
	
	vram_adr(0x2C00);
	for(i = 0; i < 32; i++)
	{
		for(j = 0; j < 32; j++)
		{
			vram_put(10);
		}
	}
	
	scroll(0, 254);
	ppu_wait_nmi();
	asm("lda #255");
	__asm__("sta $2110");
	asm("lda #0");
	asm("sta $2110");
	
	asm("lda #255");
	__asm__("sta $2111");
	asm("lda #0");
	asm("sta $2111");
	
	asm("lda #255");
	__asm__("sta $2112");
	asm("lda #0");
	asm("sta $2112");
	scroll_bg3(0, 255);
	
	pal_all(0, sol_font_palette);
	pal_all(1, solitaire_palette);
	pal_all(2, &solitaire_palette[16]);
	pal_all(3, &solitaire_palette[32]);
//	pal_all(3, small_bc_palette);
	pal_all(6, small_bc_palette);
	
	pal_all(5, bg2_palette);
/*	pal_all(6, Bc_palette);
	pal_all(7, letNum_palette);
*/

// This was done by pure chance, it happened that some of the cards_palette palettes, namely 0 and 4 match solitaire cards colors	
	pal_all(8, cards_palette);
	pal_all(9, &solitaire_palette[16]);
	pal_all(10, &solitaire_palette[32]);
	pal_all(11, &cards_palette[48]);
	pal_all(12, color_sol_sel_palette);
	
	
	minutes = 0;
	seconds = 0;
	hours = 0;

	displaySeconds();
	clockCount = nesclock();
	ppu_on_all();
	ppu_wait_nmi();

}

extern unsigned char universalPokerStates;

void loadPoker()
{
	pokerCash = 1000;
	ppu_off();
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
	
	// LOAD BG2 map
	vram_adr(0x2800);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + BEFORE_POKER_S + POKER_TABLE_S, POKER_TABLE_MAP_S);
//	vram_adr(NTADR_A(0, 0));
	//BG1 fill with transparent tile
	vram_adr(0x2400);
	vram_fill(4, 2048);
	
	vram_adr(0x3000);
	// load BG1 charset
	vram_write_bank(CHR5_BANK, (char *) 0x8000 + SOLITAIRE_TABLE_S + SOLITAIRE_TABLE_MAP_S, SOLITAIRE_CARDS_S);
	// load BACK CARD TO BG1
	vram_adr(0x3000 + 9312/2);
	vram_write_bank(CHR6_BANK, (char *) 0x8000, SOL_BACK_S);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + BEFORE_POKER_S + POKER_TABLE_S + POKER_TABLE_MAP_S + SMALL_SPADES_S, POKER_FIELDS_S);
	
	// load BG2 charset
	vram_adr(0x5000);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + BEFORE_POKER_S, POKER_TABLE_S);
	
	// load BG3
	vram_adr(0x7000);
	
	
	
	
	// THISI IS NOT FINISHED LOAD ENDSCREEN NUMS DOLAR AND LETTERS here
	//vram_write_bank(0x87, (char *) 0x8000 + 11328 + 12192 + 1794 + 5312, 480 + 416 + 64);
	vram_write_bank(CHR2_BANK, (char *) 0x8000+BETTING_ROOM_TOTAL_S+END_SCREEN_BASE_S+END_SCREEN_BASE_MAP_S+JACKPOT_S
	                                          +JACKPOT_MAP_S+LOSE_MESSAGE_S+LOSE_MESSAGE_MAP_S + END_SELECTOR_S, END_SCREEN_FONT_S);
	
	// load poker dialog after font
	vram_adr(0x7300);
	vram_write_bank(CHR4_BANK, (char *) 0x8000+1120, 1504);
	// pause dialog
vram_write_bank(CHR8_BANK, (char *) 0x8000 + 10592 + 1792 + 1440 + 320 + 1120 + 224, 224);
//	vram_write_bank(CHR0_BANK, (char *) 0x8000+21472+1972, 1504);
	
	vram_adr(0x2C00);
	vram_fill(10, 2048);
	
	// Load sprite cards
	vram_adr(0x0000);
	vram_write_bank(CHR5_BANK, (char *) 0x8000 + SOLITAIRE_TABLE_S + SOLITAIRE_TABLE_MAP_S, SOLITAIRE_CARDS_S);
	
	vram_adr(0x1000);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + BEFORE_POKER_S + POKER_TABLE_S + POKER_TABLE_MAP_S, SMALL_SPADES_S);
	vram_write_bank(CHR6_BANK, (char *) 0x8000, SOL_BACK_S);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + BEFORE_POKER_S + POKER_TABLE_S + POKER_TABLE_MAP_S + SMALL_SPADES_S + POKER_FIELDS_S, SLIDER_BAR_S);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + SOL_BACK_S, 128);
	
	pal_all(0, sol_font_palette);
	// load colors for the dialog, the dialog will use second set of colors
	// 0x388a, 0x0c74, 0x023a, 0x2f1e,
	pal_col(4, 0x0000);
	pal_col(5, 0x388a);
	pal_col(6, 0x023a);
	pal_col(7, 0x2f1e);
	pal_all(1, solitaire_palette);
	pal_all(2, &solitaire_palette[16]);
	pal_all(3, &solitaire_palette[32]);
	pal_all(4, poker_fields_palette);
	pal_all(6, small_bc_palette);
	
	pal_all(5, bg2_palette);
	
	pal_all(9, solitaire_palette);
	pal_all(10, &solitaire_palette[16]);
	pal_all(11, &solitaire_palette[32]);
	pal_all(8, small_spade_palette);
	pal_all(15, small_bc_palette);
	pal_all(12, color_sol_sel_palette);
	
	scroll_bg3(251, 3);
	
	asm("lda #251");
	__asm__("sta $2111");
	asm("lda #0");
	asm("sta $2111");
	
	scroll(0, 255);
	
	universalPokerStates = 10;
	
	ppu_on_all();
	ppu_wait_nmi();
}

void loadGame()
{
	if(gameSel == 1)
		{
			oam_clear();
			ppu_off();
			loadBlackJack();
			screenState = 2;
			return;
		}
		if(gameSel == 0)
		{
			oam_clear();
			ppu_off();
			loadSolitaire();
			screenState = 1;
			return;
		}
		if(gameSel == 2)
		{
			oam_clear();
			ppu_off();
			loadPoker();
			screenState = 7;
			return;
		}
		if(gameSel == 3)
		{
			oam_clear();
			ppu_off();
			loadBelote();
			screenState = 9;
			return;
		}
}

void loadEndScreen()
{
	music_stop();
	ppu_off();
	
	asm("lda #3");
	asm("sta $2101");
	
	asm("lda #%b", 0b00001001);
	asm("sta $2105");
	
	asm("lda #%b", 0b00110001); // BG1 tiles at 0x1000, BG2 tiles at 0x2000
	asm("sta $210B");
	 
	asm("lda #%b", 0b00000000); // base address 0 screen height = 1 BG1
	asm("sta $2107"); 
	
	asm("lda #%b", 0b00000100); // base address 0x0400 screen height = 1 BG2
	asm("sta $2108"); 
	 
	asm("lda #%b", 0b01010100); // base address 0x5400 screen height = 1 BG3
	asm("sta $2109");
	
	
	vram_adr(0x0000);
	vram_fill(0, 2048);
	
	vram_adr(0x1000);
	// load BG1 charset
	if(winLoseState)
	{
		vram_write_bank(CHR2_BANK, (char *) 0x8000 + BETTING_ROOM_TOTAL_S + END_SCREEN_BASE_S + END_SCREEN_BASE_MAP_S, JACKPOT_S);
		vram_adr(NTADR_A(0, 0));
		vram_write_bank(CHR2_BANK, (char *) 0x8000 + BETTING_ROOM_TOTAL_S + END_SCREEN_BASE_S + END_SCREEN_BASE_MAP_S + JACKPOT_S, JACKPOT_MAP_S);
		pal_all(1, win_palette);
		vram_adr(NTADR_A(0, 9));
		vram_fill(0, 1216);
		
		music_play(0x95, music7);
	//	sfx_play(0);
	}
	else
	{
		vram_write_bank(CHR2_BANK, (char *) 0x8000+BETTING_ROOM_TOTAL_S+END_SCREEN_BASE_S+END_SCREEN_BASE_MAP_S+JACKPOT_S+JACKPOT_MAP_S, LOSE_MESSAGE_S);
		vram_adr(NTADR_A(1, 1));
		vram_write_bank(CHR2_BANK, (char *) 0x8000+BETTING_ROOM_TOTAL_S+END_SCREEN_BASE_S+END_SCREEN_BASE_MAP_S+JACKPOT_S+JACKPOT_MAP_S+LOSE_MESSAGE_S, LOSE_MESSAGE_MAP_S);
		pal_all(1, lose_palette);
		vram_adr(NTADR_A(0, 9));
		vram_fill(0, 1216);
		scroll(0, 2);
		sfx_play(1);
	}


	// load BG2 charset
	vram_adr(0x3000);
	vram_write_bank(CHR2_BANK, (char *) 0x8000 + BETTING_ROOM_TOTAL_S, END_SCREEN_BASE_S);
	
	vram_adr(NTADR_A(0, 0));
	
	vram_adr(0x400);
	vram_write_bank(CHR2_BANK, (char *) 0x8000 + BETTING_ROOM_TOTAL_S + END_SCREEN_BASE_S, END_SCREEN_BASE_MAP_S);
	
	
	vram_adr(0x5000);
	vram_write_bank(CHR2_BANK, (char *) 0x8000+BETTING_ROOM_TOTAL_S+END_SCREEN_BASE_S+END_SCREEN_BASE_MAP_S+JACKPOT_S
	                                          +JACKPOT_MAP_S+LOSE_MESSAGE_S+LOSE_MESSAGE_MAP_S + END_SELECTOR_S, END_SCREEN_FONT_S);
	vram_adr(0x5400);
	vram_adr(0x5400);
	vram_fill(0, 2048);
	
	vram_adr(0x6000);
	vram_write_bank(CHR2_BANK, (char *) 0x8000+BETTING_ROOM_TOTAL_S+END_SCREEN_BASE_S+END_SCREEN_BASE_MAP_S+JACKPOT_S
										+JACKPOT_MAP_S+LOSE_MESSAGE_S+LOSE_MESSAGE_MAP_S, END_SELECTOR_S);
	
	scroll(0, 255);
	ppu_wait_nmi();
	asm("lda #255");
	__asm__("sta $2110");
	asm("lda #0");
	asm("sta $2110");



/*	
	asm("lda #255");
	__asm__("sta $2110");
	asm("lda #0");
	asm("sta $2110");
*/	
	asm("lda #255");
	__asm__("sta $2111");
	asm("lda #0");
	asm("sta $2111");
	// end of scolling portion
/*	asm("lda #253");
	__asm__("sta $2112");
	asm("lda #0");
	asm("sta $2112");
*/
	scroll_bg3(0, 253);
		
//	pal_all(0, betting_selector_palette);
	pal_all(0, win_lose_base_palette);
	pal_col(129, 0xFFFF);
	
	pal_col(12, 0x388A);
	pal_col(13, 0x0C74);
	pal_col(14, 0x023A);
	pal_col(15, 0x2F1E);
//	pal_all(2, chips_palette);
	
	endSel = 0;
	oam_clear();
	displayWinings();
	vramY = 10;
	vramX = 30;
	displayTime();
	pal_bright(15);
	
	beloteEnd();
	if(gameSel == 2)
	pokerEnd();
	
	if(winLoseState)
	{
		printAchievments();
	}
	else
	{
		printMessage(2, 15, "House Edged");
		
		cash += 100;
		printMessage(25, 15, "%");
		printNumber(100, 26, 15);
	}
	
	singlePlayerAchievments();
	resetAchievments();
	resetMoveVars();
	ppu_on_all();
	
	ppu_wait_nmi();
}

void displayWinings()
{
	position = (0x5400 | ((12 << 5) | 30));
	vram_adr(position);
	do
	{
		digit = winings % 10;
		vram_put(((unsigned int)digit + 16) | 0x2C00);
		winings = winings / 10;
		--position;
		vram_adr(position);	
	}while(winings != 0);
	
	vram_put(5 | 0x2C00);
}


void displayTime()
{
	position = (0x5400 | ((vramY << 5) | vramX));   // 10 30 for the endScreen
	vram_adr(position);
	
	// position is recycled variable
	position = seconds;
	vram_put((((unsigned int)position % 10) + 16) | 0x2C00);
	vram_adr((0x5400 | ((vramY << 5) | vramX - 1)));
	position = seconds / 10;
	vram_put((((unsigned int)position % 10) + 16) | 0x2C00);
	//
//	vram_adr((0x2C00 | ((0 << 5) | 28)));
//	vram_put(26 | 0x3000);
	//
	vram_adr((0x5400 | ((vramY << 5) | vramX - 2)));
	vram_put(26 | 0x2C00);
	
	vram_adr((0x400 | ((10 << 5) | 28)));
	vram_put(0x001A);
	
	vram_adr((0x5400 | ((vramY << 5) | vramX - 3)));
	position = minutes;
	vram_put((((unsigned int)position % 10) + 16) | 0x2C00);
	vram_adr((0x5400 | ((vramY << 5) | 26)));
	position = minutes / 10;
	vram_put((((unsigned int)position % 10) + 16) | 0x2C00);
	//
	
}

#pragma code-name(pop)

#pragma code-name(push, "CODE8")
void loadBelote()
{
	setBackgrounds();
	
	vram_adr(0x5000);
	vram_write_bank(CHR4_BANK, (char *) 0x8000 + 1120 + 1504 + 4000 + 1794 + 384, 4480);
	vram_adr(0x2800);
	vram_write_bank(CHR4_BANK, (char *) 0x8000 + 1120 + 1504 + 4000 + 1794 + 384 + 4480, 1792);
	
	vram_adr(0x3000);
	vram_write_bank(CHR5_BANK, (char *) 0x8000 + SOLITAIRE_TABLE_S + SOLITAIRE_TABLE_MAP_S, SOLITAIRE_CARDS_S);
	vram_write_bank(CHR6_BANK, (char *) 0x8000, SOL_BACK_S);

	vram_adr(0x2400);
	vram_fill(4, 2048);
	
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
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + BEFORE_POKER_S + POKER_TABLE_S + POKER_TABLE_MAP_S + SMALL_SPADES_S + POKER_FIELDS_S + SLIDER_BAR_S + 5728 + 1794, 352);
	vram_write_bank(CHR8_BANK, (char *) 0x8000 + 10592 + 1792 + 1440 + 320 + 1120 + 224 + 224, 224);
	
	// Load sprite cards
	vram_adr(0x0000);
	vram_write_bank(CHR5_BANK, (char *) 0x8000 + SOLITAIRE_TABLE_S + SOLITAIRE_TABLE_MAP_S, SOLITAIRE_CARDS_S);
	
	vram_adr(0x1000);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + BEFORE_POKER_S + POKER_TABLE_S + POKER_TABLE_MAP_S, SMALL_SPADES_S);
	vram_write_bank(CHR6_BANK, (char *) 0x8000, SOL_BACK_S);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + BEFORE_POKER_S + POKER_TABLE_S + POKER_TABLE_MAP_S + SMALL_SPADES_S + POKER_FIELDS_S, SLIDER_BAR_S);
	vram_write_bank(CHR6_BANK, (char *) 0x8000 + SOL_BACK_S, 128);
		
	pal_all(5, multi_screen_palette);
	vram_adr(0x2C00);
	vram_fill(0, 2048);	
	
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
	
	pal_col(13, 0x388a);
	pal_col(12, 0x0c74);
	pal_col(14, 0x023a);
	pal_col(15, 0x2f1e);
	
	pal_all(1, solitaire_palette);
	pal_all(2, &solitaire_palette[16]);
	pal_all(3, &solitaire_palette[32]);
	pal_all(6, small_bc_palette);
	
	pal_all(9, solitaire_palette);
	pal_all(10, &solitaire_palette[16]);
	pal_all(11, &solitaire_palette[32]);
	pal_all(8, small_spade_palette);
	pal_all(15, small_bc_palette);
	pal_all(12, color_sol_sel_palette);
	
	pal_col(208, 0xFFFF);
	pal_col(209, 0xFFFF);
	pal_col(210, 0xFFFF);
	pal_col(211, 0xFFFF);
	pal_col(212, 0xFFFF);
	pal_col(213, 0xFFFF);
	

	
	scroll(0, 255);

	asm("lda #255");
	__asm__("sta $2110");
	asm("lda #0");
	asm("sta $2110");
	
	asm("lda #255");
	__asm__("sta $2111");
	asm("lda #0");
	asm("sta $2112");
	
	dimTokens();	
	pl[0] = 0; pl[1] = 0; pl[2] = 0; pl[3] = 0;
	allEntered = 0; entered = 0;
	printed = 0;
	
	ppu_on_all();	
}

void beloteEnd()
{
	if(gameSel == 3)
	{
		vram_adr((0x400 | ((12 << 5) | 2)));
		vram_fill(0x001A, 18);
		vram_adr((0x400 | ((10 << 5) | 2)));
		vram_fill(0x001A, 10);
		printMessage(2, 10, "Time:")
		if(winLoseState == 1)
		{
		//	vram_adr((0x400 | ((10 << 5) | 2)));
		//	vram_fill(0x001A, 10)
			printMessage(2, 12, "Team 1 wins:");
		}
		else
		{
			printMessage(2, 12, "Team 2 wins:");
		}
	}
}

#pragma code-name(pop)

#pragma code-name(push, "CODE9")

static void displayNumber()
{
	num2 = num1;
	
	k = 0;

	while(num1)
	{
		++k;
		num1 = snesdiv(num1, 10);
	}

	ppu_wait_nmi();
	vram_adr(0x5400 | ((bg3Y << 5) | bg3X + k));
	displayMessage("      ");

	if(k)
	--k;
	
	num1 = num2;
	do
	{
		num2 = snesmod(num1, 10);
	//	ppu_wait_nmi();
		vram_adr(0x5400 | ((bg3Y << 5) | bg3X + k));
		vram_put((((unsigned int)num2) + 16) | 0x2C00);
		num1 = snesdiv(num1, 10);
		--bg3X;
	}while(num1);
	
	k = 0;
	
	bg3X = 0; bg3Y = 0; num1 = 0; num2 = 0;
}



void pokerEnd()
{
	
	vram_adr((0x400 | ((12 << 5) | 2)));
		vram_fill(0x001A, 18);
		vram_adr((0x400 | ((10 << 5) | 2)));
		vram_fill(0x001A, 10);
		printMessage(2, 10, "Time:")
		if(gameSel == 2)
		{
			if(winLoseState == 1)
			{
				printMessage(2, 12, "Player");
				printNumber(winner, 9, 12);
				printMessage(11, 12, "wins:");
				if(winner == 1)
				{
					++pokWins;
					if(pokWins > 100)
					{
						pokWins = 100;
					}
					++consecutivePok;
					if(consecutivePok > curConPok)
					{
						curConPok = consecutivePok;
						if(curConPok > 10)
						{
							curConPok = 10;
						}
					}
				}
					
			}
			else
			{
				printMessage(2, 12, "CPU");
				printNumber(winner, 6, 12);
				printMessage(8, 12, "wins:");
			}
		}
		if(winner != 1)
		{
			consecutivePok = 0;
		}
		savesram();
}

void singlePlayerAchievments()
{
	if(gameSel <= 1)
	{
		++playSingle;
		++playSingle100;
		if(playSingle100 > 100)
		{
			playSingle100 = 100;
		}
		if(playSingle > 50)
		{
			playSingle = 50;
		}
		if(winLoseState)
		{
			++winSingle;
			if(winSingle > 50)
			{
				winSingle = 50;
			}
			++winSingle1000;
			if(winSingle1000 > 1000)
			{
				winSingle1000 = 1000;
			}
		}
		else
		{
			++loseSingle;
			if(loseSingle > 50)
			{
				loseSingle = 50;
			}
		}
	}
	if(!gameSel)
	{
		if(winLoseState)
		{
			if(minutes < 10)
			{
				++solUnder10; 
				if(solUnder10 > 100)
				{
					solUnder10 = 100;	
				}	
			}
		}
		
	}
	savesram();
//	solUnder10 = 0;
}

static void displayMessage(char *message)
{
	unsigned char k = 0;
	while(message[k] != '\0')
	{
		vram_put((message[k] - 32) | 0x2C00);
		++k;
	}
	k = 0;
}

void printAchievments()
{
	unsigned char x, y;
	x = 2;
	y = 15;
	if(olchipper)
	{
		printMessage(x, y, "Ol Chipper");
		printMessage(25, y, "%");
		printNumber(100, 26, y);
		++y;
	}
	if(blackjack)
	{
		printMessage(x, y, "Blackjack!");
		printMessage(25, y, "%");
		printNumber(betAmount*2, 26, y);
		++y;
	}
	if(risktaker)
	{
		printMessage(x, y, "Risk Taker");
		printMessage(25, y, "%");
		printNumber(betAmount*2, 26, y);
		++y;
	}
	if(trickster)
	{
		printMessage(x, y, "Trickster");
		printMessage(25, y, "%");
		printNumber(betAmount, 26, y);
		++y;
		cash += 300; 
	}
	if(prestige)
	{
		printMessage(x, y, "The Prestige");
		printMessage(25, y, "%");
		printNumber(betAmount, 26, y);
		++y;
		cash += 800; 
	}
	if(gameSel == 0 && minutes < 10)
	{
		printMessage(x, y, "Solid-taire");
		printMessage(25, y, "%");
		printNumber(betAmount, 26, y);
		++y;
		cash += betAmount; 
	}
	
}

void resetAchievments()
{
	olchipper = 0;
	blackjack = 0;
	risktaker = 0;
	trickster = 0;
	prestige = 0;
}

void resetMoveVars()
{
 xProgress= 0;
  yProgress = 0;
nSteps = 0;
arrived = 0;
skipIt = 0;
mStates = 0;

dx = 0, dy = 0;
D = 0;

DLstate = 0;
moveState = 0;

spY = 0;
spX = 0;
vramX = 0;
vramY = 0;

cardArived = 0;

skipNum = 0;
pickORdisplay = 0;

alowSprite = 0;
}

#pragma code-name(pop)

