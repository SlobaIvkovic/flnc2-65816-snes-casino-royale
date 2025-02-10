#include "neslib.h"
#include "snesgss.h"
#include "include/load_screens.h"
#include "include/drawing.h"
#include "include/title_screen.h"
#include "include/blackjack.h"
#include "include/solitaire.h"
#include "include/poker.h"
#include "include/betting_screen.h"
#include "include/end_screen.h"
#include "include/multi_screen.h"
#include "include/shuffle.h"
//#include "include/small_maps.h"
#include "include/save.h"
#include "include/achievments.h"
#include "include/options.h"
#include "include/mask.h"


void trampoline(void);
#pragma wrapped-call(push, trampoline, 0x85)
void sayhi(void); // This function is in bank 0x82
#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x90)
void belote();
#pragma wrapped-call(pop)

void str(const char *in) {
	static unsigned char i;
	for (i = 0; in[i]; i++)
		vram_put(in[i] - ' ');
}

extern char music1[];
extern const unsigned char font_bank;
extern const unsigned char font_chr[];

#pragma code-name(push, "CODE4")
void sayhi() {
	vram_adr(NTADR_A(5, 14));
	str("HI FROM 0X85");
}
#pragma code-name(pop)


unsigned char cardsDeck[52];

#pragma wrapped-call(push, trampoline, 0x86)
void loadDeck();
#pragma wrapped-call(pop)

unsigned char moveState;

unsigned char vramX;
unsigned char vramY;

unsigned char i;    // to be used only in for loops
unsigned char j;    // only in for loops
unsigned char k;    // only in for loops

unsigned char availableSprid;

unsigned char signNum;

unsigned char nextSprid;
unsigned char spX;              // card metasprite current poss
unsigned char spY;

unsigned char pickORdisplay;
unsigned char deal4state;

unsigned char x0, y0;
unsigned char x1, y1;
unsigned char framesBetween;
unsigned char cardArived;
unsigned char alowSprite;

unsigned char hit;
unsigned char playerCardMoved;

unsigned char states;
unsigned char dealerScore;
unsigned char playerScore;
unsigned char score;
unsigned char toDisplay;
unsigned char digit1;
unsigned char digit2;
unsigned char deckSize;


unsigned char hiddenCardX;
unsigned char hiddenCardY;
unsigned char hiddenCard;
unsigned char auxVX;
unsigned char auxVY;

unsigned char selectorState;
unsigned char shuffleNum;


unsigned char screenState;
unsigned long cash;
unsigned long cash1;
unsigned long betAmount;
unsigned long winings;

//---------------------------
//move these to their respective modules when added
unsigned char gameSel;
unsigned char winLoseState;
//unsigned char bigSelX;

extern unsigned char volume, volume1, slx1, slx2;

#pragma rodata-name(push, "RODATA")
#pragma data-name(push, "RODATA")
static unsigned char cD[52] = {
0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c,
0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c,
0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c,
}; 


const unsigned char fix[13] = {0x0A, 0x3A, 0x30, 0x3C, 0x03, 0x29, 0x10, 0x18, 0x18, 0x21, 0x2a, 0x29, 0x1C};
 
#pragma rodata-name(pop)
#pragma data-name(pop)

void resetSave();
// Main is in CODE, aka RAM here. Could put it elsewhere too
void main() {
	
	volume1 = 63;
	volume = 63;
	slx1 = 171;
	slx2 = 171;
	
	spc_sfxpan = 128;
	spc_sfxvol = 63;
	spc_sfxchan = 7;
	
	spc_global_volume(volume);
	loadsram();
	if (checksum[0] != 0x123 || checksum[1] != 0x4444)
	 {
    //memzero(achievements, NUM_ACHIEVEMENTS);
    // etc
    // for simpler games, use a static checksum; complex games, dynamic (adler32)
    		resetSave();
    		checksum[0] = 0x123;
    		checksum[1] = 0x4444;
    		savesram();
	}
//	resetSave();
/*	if(checksum[0] == 78)
	{
		cash = 5;
		checksum[0] = 77;
		handsPlayed = 0;	
	}
	else
	{
		cash = handsPlayed;
	}
*/	
	cash = 9999;

	deckSize = 52;
	pickORdisplay = 0;
	deal4state = 0;
	i = 0;
	j = 0;
	k = 0;
	vramX = 1;
	vramY = 0;
	signNum = 0x00;
	availableSprid = 0;
	spX = XSTART;
	spY = YSTART;
	
	
	ppu_wait_nmi();

	loadDeck();

	loadTitleScreen();

	while(screenState == 0)
	{
		titleScreen();
	
	}
//	while(!(pad_trigger(0) & PAD_START));
	set_rand(nesclock());
	shuffleNum = (rand16() % 30) + 5;
	shuffle();
	

	while (1) {
		if(screenState == 0)
		{
			titleScreen();
		/*	if(pad_trigger(0) & PAD_SELECT)
			{
				resetSave();
			}
	*/
		}
		else if(screenState == BETTING_SCREEN)
		{
			bettingScreen();
		}		
		else if(screenState == 2)
		{
			
			blackJack();
		}
		else if(screenState == 1)
		{
			solitaire();
		}
		else if(screenState == 5)
		{
			loadEndScreen();
		//	delay(255);
			screenState = 6;
		}
		else if(screenState == 6)
		{
			endScreen();
		}
		else if(screenState == 7)
		{
			poker();
		}
		else if(screenState == 8)
		{
			multiScreen();
		}
		else if(screenState == 9)
		{
			belote();
		}
		else if(screenState == 15)
		{
			achievmentScreen();
		}
		else if(screenState == 16)
		{
			optionsScreen();
		}
		else if(screenState == 17)
		{
			loadTitleScreen();
			screenState = 0;
		}
	
	}
}

void resetSave()
{
	consecutiveBj = 0;
	curConBj = 0;
	bjWins = 0;
	
	consecutivePok = 0;
	curConPok = 0;
	pokWins = 0;
	
	winSingle = 0;
	playSingle = 0;
	loseSingle = 0;
	
	solUnder10 = 0;
	
	winSingle1000 = 0;
	playSingle100 = 0;
	
	cloverWins = 0;
	heartWins = 0;
	diamondWins = 0;
	spadeWins = 0;
	suitWins = 0;
	conBelWins = 0;
	curConBelWins = 0;
	
	savesram();
}

#pragma code-name(push, "CODE5")
void loadDeck()
{


for (i = 0; i < 52; i++)
{
	
	cardsDeck[i] = cD[i];
	ppu_wait_nmi();

}

i = 0;
}
#pragma code-name(pop)
