#include"neslib.h"
//#include"include/blackjack.h"
#include"include/movement.h"
#include"include/defines.h"
#include "include/drawing.h"
#include "include/shuffle.h"
#include "include/save.h"
#include "snesgss.h"
#include "include/inplayachi.h"

#define BLACKJACK    0
#define PUSH         1
#define DEALER_BUSTS 2
#define PLAYER_BUSTS 3
#define PLAYER_WINS  4
#define HOUSE_WINS   5

extern unsigned char spX, spY;
unsigned char xOld, yOld;

extern unsigned char music3[];

extern unsigned char winLoseState;
extern unsigned char screenState;
extern unsigned long betAmount;
extern unsigned long cash;
extern unsigned long winings;


extern unsigned char moveState;

extern unsigned char vramX;
extern unsigned char vramY;

extern unsigned char i;    // to be used only in for loops
extern unsigned char j;    // only in for loops
extern unsigned char k;    // only in for loops



extern unsigned char signNum;



extern unsigned char pickORdisplay;
extern unsigned char deal4state;

extern unsigned char x0, y0;
extern unsigned char x1, y1;
extern unsigned char framesBetween;
extern unsigned char cardArived;
extern unsigned char alowSprite;

extern unsigned char hit;
extern unsigned char playerCardMoved;

extern unsigned char states;
extern unsigned char dealerScore;
extern unsigned char playerScore;
extern unsigned char score;
extern unsigned char toDisplay;
extern unsigned char digit1;
extern unsigned char digit2;
extern unsigned char deckSize;


extern unsigned char hiddenCardX;
extern unsigned char hiddenCardY;
extern unsigned char hiddenCard;

extern unsigned char auxVX;
extern unsigned char auxVY;

extern unsigned char selectorState;
extern unsigned char shuffleNum;

extern unsigned char cardsDeck[52];

#define checkPause(){\
						if(pad_trigger(0) & PAD_START)\
						{\
							pauseChoice();\
						}\
					}\

void trampoline();
#pragma wrapped-call(push, trampoline, 0x91)
void calculateScore();
void displayPlayerCash();
void openDealersCard();
#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x91)
void colBjSel();
void createMessage(char* message);
void drawMessage();
static void pauseChoice();
#pragma wrapped-call(pop)

unsigned char messageSprid, messageX, messageY, acc, whichMessage;

extern unsigned char selTile, selTile1, cSelX, cSelY, tileUpdate;
#define SEL_ANIMATION_TIME 20

//201, 232

#define blackjackSel(a, b){\
								cSelX = a;\
								cSelY = b;\
								colBjSel();\
							}\

#pragma code-name(push, "CODE9")
void displayDscore()
{
	if((dealerScore & 0x7f) < 10)
	{
		vram_adr(BG3_NT_ADR(D_SCORE_X + 1, D_SCORE_Y));
		vram_put(dealerScore | 0b0011110000000000);
		vram_adr(BG3_NT_ADR(D_SCORE_X + 1, D_SCORE_Y + 1));
		vram_put((unsigned short)dealerScore + 10 | 0b0011110000000000);
		vram_adr(BG3_NT_ADR(D_SCORE_X + 1, D_SCORE_Y + 2));
		vram_put((unsigned short)dealerScore + 20 | 0b0011110000000000);
	}
	else if((dealerScore & 0x7f) < 20)
	{
			vram_adr(BG3_NT_ADR(D_SCORE_X, D_SCORE_Y));
			vram_put(1 | 0b0011110000000000);
			vram_adr(BG3_NT_ADR(D_SCORE_X, D_SCORE_Y + 1));
			vram_put((unsigned short)11 | 0b0011110000000000);
			vram_adr(BG3_NT_ADR(D_SCORE_X, D_SCORE_Y + 2));
			vram_put((unsigned short)21 | 0b0011110000000000);
//	toDisplay = (dealerScore & 0x7f) % 10;
			vram_adr(BG3_NT_ADR(D_SCORE_X + 1, D_SCORE_Y));
			vram_put(digit2 | 0b0011110000000000);
			vram_adr(BG3_NT_ADR(D_SCORE_X + 1, D_SCORE_Y + 1));
			vram_put((unsigned short)digit2 + 10 | 0b0011110000000000);
			vram_adr(BG3_NT_ADR(D_SCORE_X + 1, D_SCORE_Y + 2));
			vram_put((unsigned short)digit2 + 20 | 0b0011110000000000);
	
	}
	else
	{
		vram_adr(BG3_NT_ADR(D_SCORE_X, D_SCORE_Y + 0));
		vram_put(2 | 0b0011110000000000);
		vram_adr(BG3_NT_ADR(D_SCORE_X, D_SCORE_Y + 1));
		vram_put((unsigned short)12 | 0b0011110000000000);
		vram_adr(BG3_NT_ADR(D_SCORE_X, D_SCORE_Y + 2));
		vram_put((unsigned short)22 | 0b0011110000000000);
//	toDisplay = (dealerScore & 0x7f) % 10;
		vram_adr(BG3_NT_ADR(D_SCORE_X + 1, D_SCORE_Y));
		vram_put(digit2 | 0b0011110000000000);
		vram_adr(BG3_NT_ADR(D_SCORE_X + 1, D_SCORE_Y + 1));
		vram_put((unsigned short)digit2 + 10 | 0b0011110000000000);
		vram_adr(BG3_NT_ADR(D_SCORE_X + 1, D_SCORE_Y + 2));
		vram_put((unsigned short)digit2 + 20 | 0b0011110000000000);
	}
}


void displayPscore()
{
	if((playerScore & 0x7f) < 10)
	{
		vram_adr(BG3_NT_ADR(P_SCORE_X + 1, P_SCORE_Y));
		vram_put(playerScore | 0b0011110000000000);
		vram_adr(BG3_NT_ADR(P_SCORE_X + 1, P_SCORE_Y + 1));
		vram_put((unsigned short)playerScore + 10 | 0b0011110000000000);
		vram_adr(BG3_NT_ADR(P_SCORE_X + 1, P_SCORE_Y + 2));
		vram_put((unsigned short)playerScore + 20 | 0b0011110000000000);
	}
	else if((playerScore & 0x7f) < 20)
	{
			vram_adr(BG3_NT_ADR(P_SCORE_X, P_SCORE_Y));
			vram_put(1 | 0b0011110000000000);
			vram_adr(BG3_NT_ADR(P_SCORE_X, P_SCORE_Y + 1));
			vram_put((unsigned short)11 | 0b0011110000000000);
			vram_adr(BG3_NT_ADR(P_SCORE_X, P_SCORE_Y + 2));
			vram_put((unsigned short)21 | 0b0011110000000000);
//	toDisplay = (dealerScore & 0x7f) % 10;
			vram_adr(BG3_NT_ADR(P_SCORE_X + 1, P_SCORE_Y));
			vram_put(digit1 | 0b0011110000000000);
			vram_adr(BG3_NT_ADR(P_SCORE_X + 1, P_SCORE_Y + 1));
			vram_put((unsigned short)digit1+10 | 0b0011110000000000);
			vram_adr(BG3_NT_ADR(P_SCORE_X + 1, P_SCORE_Y + 2));
			vram_put((unsigned short)digit1+20 | 0b0011110000000000);
	
	}
	else
	{
		vram_adr(BG3_NT_ADR(P_SCORE_X, P_SCORE_Y));
		vram_put(2 | 0b0011110000000000);
		vram_adr(BG3_NT_ADR(P_SCORE_X, P_SCORE_Y + 1));
		vram_put((unsigned short)12 | 0b0011110000000000);
		vram_adr(BG3_NT_ADR(P_SCORE_X, P_SCORE_Y + 2));
		vram_put((unsigned short)22 | 0b0011110000000000);
//	toDisplay = (dealerScore & 0x7f) % 10;
		vram_adr(BG3_NT_ADR(P_SCORE_X + 1, P_SCORE_Y));
		vram_put(digit1 | 0b0011110000000000);
		vram_adr(BG3_NT_ADR(P_SCORE_X + 1, P_SCORE_Y + 1));
		vram_put((unsigned short)digit1+10 | 0b0011110000000000);
		vram_adr(BG3_NT_ADR(P_SCORE_X + 1, P_SCORE_Y + 2));
		vram_put((unsigned short)digit1+20 | 0b0011110000000000);
	}
}


void blackJack()
{
//	messageX = 10;
//	messageY = 10;
//	ppu_wait_nmi();
//	createMessage("WIN LOSES");
//	while(1);
	if(pad_trigger(0) & PAD_START)
	{
		pauseChoice();
		if(screenState == 5)
		{
			return;
		}
	}
	
	if(states == 4)
	{
		if(pad_trigger(0) & PAD_START)
		{
			pauseChoice();
		}
		selectorState = 0;
	
		playerScore = 0;
		dealerScore = 0;
		deal4state = 0;
		pickORdisplay = 0;
		hit = 0;
		playerCardMoved = 0;
		deckSize = 52;
		
		set_rand(nesclock());
		shuffle();
		
		states = 0;
		
		if(winLoseState != 2)
		{
			if(winLoseState)
			{
				++bjWins;
				if(bjWins > 100)
				{
					bjWins = 100;
				}
				++consecutiveBj;
				if(consecutiveBj > curConBj)
				{
					curConBj = consecutiveBj;
					if(curConBj > 21)
					{
						curConBj = 21;
					}
				}
			}
			else
			{
				consecutiveBj = 0;
			}
			savesram();
			screenState = 5;
		//	++handsPlayed;
		//	savesram();
		}
		else
		{
			risktaker = 0;
			blackjack = 0;
			ppu_off();
			vram_adr(NTADR_A(0, 0));
			for(i = 0; i < 32; i++)
			{
				for(j = 0; j < 32; j++)
				{
					vram_put(353);
				}
			}
			vram_adr(0x5400);
			for(i = 0; i < 32; i++)
			{
				for(j = 0; j < 32; j++)
				{
					vram_put(10);
				}
			}
			displayPlayerCash();
			oam_clear();
			pal_col(209, 0x388a);
			pal_col(212, 0x0e3c);
			pal_col(214, 0x2F1E);
			ppu_on_all();
		}
	}
/*	
	if(pad_state(0) & PAD_RIGHT)
	spX++;
	if(pad_state(0) & PAD_DOWN)
	spY++;
	if(pad_state(0) & PAD_UP)
	spY--;
	if(pad_state(0) & PAD_LEFT)
	spX--;
	
	drawSpriteCard4();
	ppu_wait_nmi();*/
	if(states == 0)
	{
	
	if(deal4state < 5)
	{
		if(!pickORdisplay)
		{
			++deal4state;
			signNum = cardsDeck[deckSize];
			++deckSize;
			pickORdisplay = 1;
		//	if(deal4state < 5)
		//	sfx_play(6);
		}
		else
		{
			switch(deal4state)
			{
				case(1):
					changeCardPoss(XSTART, XEND, YSTART, YEND);
					break;
				case(2):
					changeCardPoss(XSTART, XEND, YSTART, YEND - 64);		
					break;
				case(3):
					changeCardPoss(XSTART, XEND + 48, YSTART, YEND);
					break;
				case(4):
					changeCardPoss(XSTART, XEND + 48, YSTART, YEND - 64);
					break;
				default:
				break;			

			}
			
		}
		if(cardArived)
		{
			if(deal4state == 2 || deal4state == 4)
			{
				dealerSum();
				if(deal4state == 2)
				{
					hiddenCard = signNum;
				//	dealerSum();
				//	dealerScore = dealerScore
					digit2 = (dealerScore & 0x7f) % 10;
				}
				digit2 = (dealerScore & 0x7f) % 10;
			}
			else
			{
				playerSum();
				digit1 = snesmod((playerScore & 0x7f) , 10);
			}
			
		}
		
		ppu_wait_nmi();

		if(cardArived)
		{
			if(deal4state != 2)
			{
				ppu_wait_nmi();
				displayCard();
			}
			else
			{
				drawBackCardStatic();
			}
			if(deal4state == 4)
			{
				
			//	displayDscore();
			}
			else
			{
				displayPscore();
			}
			cardArived = 0;
		
			
		}
		else if(alowSprite)
		{
			if(deal4state == 2)
			{
				drawCardBackSprite(); 
			}
			else
			{		
				drawSpriteCard(signNum);
			}
		}
	}
	// 4 cards are dealt, now ask user if he would like to hit or stand
	else 
	{
	//	playerScore = 21;
					if((playerScore & 0x7f) == 21)
					{
						states = 0x83;
					}
					else
		states = 1;
	//	pickORdisplay = 0;
		cSelX = 200;
		cSelY = 88;
		selTile = 201;
		selTile1 = 232;
	}
}
	if (states == 1)
	{
	//	if(deal4state == 5)
	//	{
	//		++deal4state;
	//		signNum = cardsDeck[rand16() % 52];
	//	}
	if((playerScore & 0x7f) < 21)
	{
		if (pad_trigger(0) & PAD_DOWN)
		{
			sfx_play(2);
		//	hit = 1;
			selectorState = 0;
		//	clearSelector(25, 8);
		//	selectorDisplay(25, 11);
			cSelX = 200;
			cSelY = 88;
		}
		else if(pad_trigger(0) & PAD_UP)  // If player has chosen to stand, game states proceeds to dealer play
		{
			sfx_play(2);
			selectorState = 1;
		//	clearSelector(25, 11);
		//	selectorDisplay(25, 8);
			cSelX = 200;
			cSelY = 64;
		//	states = 2;
		//	playerCardMoved = 0;
		/*	auxVX = signNum;
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
			signNum = auxVX;*/
		}
		if(pad_trigger(0) & PAD_B)
		{
		//	sfx_play(2);
			if(selectorState == 0)
			{
				playerCardMoved = 0;
				openDealersCard();
				states = 2;
			//	pickORdisplay = 0;
				
			}
			else
			{
			//	sfx_play(6);
				hit = 1;
				if(playerScore > 15)
				risktaker = 1;
			}
		}
		if(hit)
		{
			changeCardPoss(XSTART, XEND + 56 + playerCardMoved, YSTART, YEND);
		}
		blackjackSel(cSelX, cSelY);
	}
	else if((playerScore & 0x7f) == 21)
	{
		states = 2;
		playerCardMoved = 0;
		openDealersCard();
		
	}
	else
	{
		states = 0x43;
		openDealersCard();
	}
		if(cardArived)
		{
			playerSum();
			digit1 = snesmod((playerScore & 0x7f) , 10);
		}
		ppu_wait_nmi();
		if(cardArived)
		{
			displayCard();
			displayPscore();
			cardArived = 0;
			playerCardMoved += 8;
		//	playerSum();
			signNum = cardsDeck[deckSize];
			++deckSize;
		}
	 	else if(alowSprite)
	 	{
				drawSpriteCard(signNum); 		
		}
	}
	if(states == 2)
	{
		if((dealerScore & 0x7f) <= 16)
		{
			hit = 1;
	//		
		/*	if(!pickORdisplay)
			{
				sfx_play(6);
				pickORdisplay = 1;	
			}*/
			changeCardPoss(XSTART, XEND + 56 + playerCardMoved, YSTART, YEND - 64);
		}
		else if((dealerScore & 0x7f) > 21)
		{
			hit = 0;
			states = 0x23;
		}
		else
		{
			hit = 0;
			states = 3;
		}
		if(cardArived)
		{
			dealerSum();
			digit2 = (dealerScore & 0x7f)%10;
		}	
		
		
		ppu_wait_nmi();
		if(cardArived)
		{
			displayCard();
			
			cardArived = 0;
			playerCardMoved += 8;
			displayDscore();
			signNum = cardsDeck[deckSize];
			++deckSize;
			
		}
	 	else if(alowSprite)
	 	{
				drawSpriteCard(signNum); 		
		}
	}
	if((states & 0x0F) == 3)
	{
		ppu_wait_nmi();
	//	states = 0x80;
		if((states & 0xF0) == 0x80 )
		{
			whichMessage = BLACKJACK;
	//		playerScore = 21;
		//	td(3, 25,"blackjack");
		
			openDealersCard();
			if(playerScore == dealerScore)
			{
				winLoseState = 2;
				whichMessage = PUSH;
			}
			else
			{
				winLoseState = 1;
				winings = (betAmount << 1) + (betAmount >> 1);
				cash += winings;
				blackjack = 1;
			}
		}
		else if((states & 0xF0) == 0x40)
		{
		//	td(3, 25,"player busts");
			whichMessage = PLAYER_BUSTS;
			winLoseState = 0;
			winings = 0;
		}
		else if((states & 0xF0) == 0x20)
		{
		//	td(3, 25,"dealer busts");
			winings = betAmount << 1;
			cash += winings;
			winLoseState = 1;
			whichMessage = DEALER_BUSTS;
		}
		else if((playerScore & 0x7f) > (dealerScore & 0x7f))
		{
		//	td(3, 25,"you won");
			whichMessage = PLAYER_WINS;
			winLoseState = 1;
			winings = betAmount << 1;
			cash += winings;
		}
		else if((playerScore & 0x7f) == (dealerScore & 0x7f))
		{
		//	td(3, 25, "its a draw");
			whichMessage = PUSH;
			winLoseState = 2;
			whichMessage = 1;
		}
		else
		{
		//	td(3, 25, "you lost");
			whichMessage = HOUSE_WINS;
			winLoseState = 0;
			winings = 0;
		}
		
	//	while(!(pad_trigger(0) & PAD_START));
		set_rand(nesclock());
		shuffleNum = (rand16() % 30) + 5;
		oam_clear();
		states = 5;
		pal_col(209, 0x146a);
		pal_col(212, 0x7f3f);
		pal_col(214, 0x59df);
	//	delay(50);
		messageY = 255;
		 
	}
	if(states == 5)
	{
		ppu_wait_nmi();
		ppu_wait_nmi();
		acc += 2;
		messageY += acc;
		drawMessage();
		
		if(messageY > 160)
		{
			delay(4);
			checkPause();
			messageY = 172;
			drawMessage();
			delay(3);
			messageY = 181;
			drawMessage();
			//
			delay(4);
			messageY = 176;
			drawMessage();
			delay(3);
			messageY = 181;
			drawMessage();
			//
			delay(3);
			messageY = 179;
			drawMessage();
			delay(3);
			messageY = 181;
			drawMessage();
			//
			
			//
			delay(90);
			checkPause();
			states = 4;
			acc = 0;
		}
	}
	
	
}

void calculateScore()
{
	if(!(signNum & 0x0f))
	{
		if((score & 0x7f) <= 10)
		{
			score += 11;
			score |= 0x80;
		}
		else
		{
			++score;
		}
	}
	else if((signNum & 0x0f) > 8)
	{
		score += 10;
	}
	else
	{
		score += (signNum & 0x0f) + 1;
	}
	if(((score & 0x7F) > 21) && (score & 0x80))
	{
			score -= 10;
			score ^= 0x80;
	}

}

void openDealersCard()
{
	xOld = spX;
	yOld = spY;
	auxVX = signNum;
			auxVY = vramY;
			vramX = XEND >> 3;
			vramY = (YEND - 64) >> 3;
			signNum = hiddenCard;
		//	oam_clear();
			
			spX = 88;
			spY = 45;
			drawSpriteCard(signNum);
			ppu_wait_nmi();
			displayCard();
			displayDscore();
			vramX = auxVX;
			vramY = auxVY;
			signNum = auxVX;
			spY = yOld;
			spX = xOld;
}				



#pragma code-name(pop)

#pragma code-name(push, "CODE9")
void colBjSel()
{
	availableSprid = 140;
	// corner tiles
//	ppu_wait_nmi();
	availableSprid = oam_spr(cSelX, cSelY, selTile, SPRITE_ATTR_FULL(5, 3, 0, 0, 1), availableSprid);
	availableSprid = oam_spr(cSelX+48, cSelY, selTile, SPRITE_ATTR_FULL(5, 3, 0, 1, 1), availableSprid);
	availableSprid = oam_spr(cSelX, cSelY+8, selTile, SPRITE_ATTR_FULL(5, 3, 1, 0, 1), availableSprid);
	availableSprid = oam_spr(cSelX+48, cSelY+8, selTile, SPRITE_ATTR_FULL(5, 3, 1, 1, 1), availableSprid);
	availableSprid = oam_spr(cSelX + 24, cSelY, selTile1, SPRITE_ATTR_FULL(5, 3, 0, 0, 1), availableSprid);
	availableSprid = oam_spr(cSelX + 24, cSelY + 8, selTile1, SPRITE_ATTR_FULL(5, 3, 1, 0, 1), availableSprid);
	
	// Corner tiles end
	
	++tileUpdate;
	if(tileUpdate > SEL_ANIMATION_TIME)
	{
		tileUpdate = 0;
		++selTile;
		if(selTile == 205)
		selTile = 201;
		++selTile1;
		if(selTile1 == 236)
		selTile1 = 232;
	}
}


void createMessage(char *message)
{
	i = 0;
	messageSprid = 164;
	while(message[i] != '\0')
	{
		if(message[i] == ' ')
		{
			++ i;
			continue;
		}
		// 'A' at 205
		
		messageSprid = oam_spr(messageX + i*8, messageY, message[i] + 140 , SPRITE_ATTR_FULL(5, 3, 0, 0, 1), messageSprid);	
		++i;
	}
	i = 0;
}

void drawMessage()
{
	switch(whichMessage)
	{
		case BLACKJACK:
		{
			messageX = 96;
			createMessage("BLACKJACK");
			break;
		}
		case PUSH:
		{
			messageX = 116;
			createMessage("PUSH");
			break;
		}
		case DEALER_BUSTS:
		{
			messageX = 84;
			createMessage("DEALER BUSTS");
			break;
		}
		case PLAYER_BUSTS:
		{
			messageX = 84;
			createMessage("PLAYER BUSTS");
			break;
		}
		case PLAYER_WINS:
		{
			messageX = 88;
			createMessage("PLAYER WINS");
			break;
		}
		case HOUSE_WINS:
		{
			messageX = 92;
			createMessage("HOUSE WINS");
			break;
		}
	}
}

static void pauseChoice()
{
	
	unsigned char ii, x, y;
	unsigned char rQ = 0;
	music_pause(1);
	ppu_wait_nmi();
	// upper line
	vram_adr((0x5400 | ((8 << 5) | 10)));
	for(ii = 0; ii < 10 ; ii++)
	{
		vram_put(72 | 0x3C00);
	}
	// bottom line
	vram_adr((0x5400 | ((14 << 5) | 10)));
	for(ii = 0; ii < 10 ; ii++)
	{
		vram_put(72 | 0xBC00);
	}
	
	y = 9;
	vram_adr((0x5400 | ((8 << 5) | 9)));
	
	for(ii = 0; ii < 5 ; ii++)
	{
		vram_adr((0x5400 | ((y << 5) | 9)));
		
		vram_put(73 | 0x3C00);
		vram_put(74 | 0x3C00);
		
		vram_adr((0x5400 | ((y << 5) | 18)));
		vram_put(74 | 0x3C00);
		vram_put(74 | 0x3C00);
		vram_put(73 | 0x7C00);
		
		++y;
		
	}
	
	vram_adr((0x5400 | ((10 << 5) | 11)));
	vram_put(75 | 0x3C00); // R
	vram_put(76 | 0x3C00); // e
	vram_put(77 | 0x3C00); // s
	vram_put(78 | 0x3C00); // u
	vram_put(79 | 0x3C00); // m
	vram_put(76 | 0x3C00); // e
	vram_put(74 | 0x3C00);
	vram_put(80 | 0x3C00);
	vram_put(81 | 0x3C00);
	
	vram_adr((0x5400 | ((12 << 5) | 11)));
	vram_put(82 | 0x3C00); // Q
	vram_put(78 | 0x3C00); // u
	vram_put(83 | 0x3C00); // i
	vram_put(84 | 0x3C00); // t
	vram_put(74 | 0x3C00); // blank
	vram_put(74 | 0x3C00); // blank
	vram_put(74 | 0x3C00); // blank
	
	y = 9;
	x = 11;
	
	ppu_wait_nmi();
	for(ii = 0; ii < 7; ii++)
	{
		vram_adr((0x5400 | ((y << 5) | (x + ii))));	
		vram_put(74 | 0x3C00); // blank
		vram_adr((0x5400 | (((y+2) << 5) | (x + ii))));	
		vram_put(74 | 0x3C00); // blank
		vram_adr((0x5400 | (((y+4) << 5) | (x + ii))));	
		vram_put(74 | 0x3C00); // blank
	}
	
	// Corners
	vram_adr((0x5400 | ((8 << 5) | 9)));
	vram_put(71 | 0x3C00);
	vram_adr((0x5400 | ((8 << 5) | 20)));
	vram_put(71 | 0x7C00);
	vram_adr((0x5400 | ((14 << 5) | 9)));
	vram_put(71 | 0xBC00);
	vram_adr((0x5400 | ((14 << 5) | 20)));
	vram_put(71 | 0xFC00);
	
	

	while(!(pad_trigger(0) & PAD_START))
	{
		if(pad_trigger(0) & PAD_UP)
		{
			vram_adr((0x5400 | ((12 << 5) | 18)));
			vram_put(74 | 0x3C00);
			vram_put(74 | 0x3C00);
			
			vram_adr((0x5400 | ((10 << 5) | 18)));
			vram_put( 80 | 0x3C00);
			vram_put( 81 | 0x3C00);
			rQ = 0;
		}
		else if(pad_trigger(0) & PAD_DOWN)
		{
			vram_adr((0x5400 | ((10 << 5) | 18)));
			vram_put(74 | 0x3C00);
			vram_put(74 | 0x3C00);
			
			vram_adr((0x5400 | ((12 << 5) | 18)));
			vram_put( 80 | 0x3C00);
			vram_put( 81 | 0x3C00);
			rQ = 1;
		}
	}
	if(rQ)
	{
		selectorState = 0;
	
		playerScore = 0;
		dealerScore = 0;
		deal4state = 0;
		pickORdisplay = 0;
		hit = 0;
		playerCardMoved = 0;
		deckSize = 52;
		
		set_rand(nesclock());
		ppu_off();
		shuffle();
		states = 0;
		winLoseState = 0;
		screenState = 5;
	}
	else
	{
		vram_adr((0x5400 | ((8 << 5) | 9)));
		vram_fill(85, 430);
		music_pause(0);
	//	vram_adr((0x2C00 | ((12 << 5) | 9)));
	//	vram_fill(114, 24);
	}
}
#pragma code-name(pop)
