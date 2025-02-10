#include "neslib.h"

#define PAUSE while(!(pad_trigger(0) & PAD_START));

extern unsigned long toCall;
extern unsigned int bigBlind;
extern unsigned char possibilities;
unsigned long alreadyIn;                   //if the player is already in the pot twice as much he is asked to call, the player will call
unsigned long aiBet;

unsigned char raiseOfset;
unsigned char raiseMax;

unsigned char randomNumber;
unsigned char heroCall;
//
unsigned char aiCommunity[5];
unsigned char aiCommSize;

unsigned char aiPocket[2];
unsigned char fullComb[7];
unsigned char aiComb[5];

unsigned char aii, aij, aik, ain, aiAux;


unsigned long BBs;

extern unsigned long minBetRaise;

unsigned long range;
unsigned char probability;
unsigned char intimidated;
unsigned char sng;
unsigned char sng1;
unsigned char decision;
unsigned int probInc;      // how much probability increases depending on hand strength
extern unsigned char stage;

extern unsigned char bg3X;
extern unsigned char bg3Y;
extern unsigned long num1;

void trampoline();

#pragma wrapped-call(push, trampoline, 0x85)
void displayMessage();
#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x86)
void displayNumber();
#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x90)
void shouldIraise();
void raiseAmount();
void aiAcesHigh(unsigned char* aiArray);
void aiAcesLow(unsigned char* aiArray);
static void aiSortArray(unsigned char* aiArray);
void formFullComb();

unsigned char straightDraw();
unsigned char highCards();
unsigned char flushDraw();

unsigned char realTopPair();
unsigned char realMiddlePair();
unsigned char realBottomPair();
unsigned char real2pair();
#pragma wrapped-call(pop)

#define printNumber(a, x, y) {\
								num1 = a; bg3X = x; bg3Y = y;\
								ppu_wait_nmi();\
								displayNumber();\
							}\

#define printMessage(x, y, mess){\
									ppu_wait_nmi();\
									vram_adr((0x2C00 | ((y << 5) | x)));\
									displayMessage(mess);\
								}\
								
#define howMuch(a, b){\
					raiseOfset = a;\
					raiseMax   = b;\
					raiseAmount();\
					}\
					
#define randNum(){\
					randomNumber = rand8();\
					if(randomNumber == 0)\
					{\
						++randomNumber;\
					}\
				}\
					
#pragma code-name(push, "CODE8")
void findRange()
{
	range = snesdiv(toCall, bigBlind);
	if(range < 4)
	{
		probability = 4; // with weakest hand there is 25% chance that player will call at smallest amount range
	}
	else if(range < 11)
	{
		probability = 5; // with weakest hand there is 20% chance that player will call if amount to call is less than 10 bigBlinds
	}
	else if(range < 21)
	{
		probability = 6;
	}
	else
	{
		probability = 7;
	}
}

void aiDecision()
{
//	decision = 2;
	findRange();
	probInc = sng;
	// print pocket
	// print community
	// print full comb
	// print community size
//	printNumber(aiCommSize, 10, 15);
	aik = aiCommSize + 2;
	formFullComb();
	aiAcesHigh(fullComb);
	ain = aiCommSize + 2;
	aiSortArray(fullComb);
	
	aik = aiCommSize;
	aiAcesHigh(aiCommunity);
	ain = aiCommSize;
	aiSortArray(aiCommunity);
/*	
	for(aii = 0; aii < aiCommSize + 2; aii++)
	{
		if((fullComb[aii] & 0x0f) == 9)
		{
			printMessage(20+aii, 10, "T");
		}
		else if((fullComb[aii] & 0x0f) == 10)
		{
			printMessage(20+aii, 10, "J");
		}
		else if((fullComb[aii] & 0x0f) == 11)
		{
			printMessage(20+aii, 10, "Q");
		}
		else if((fullComb[aii] & 0x0f) == 12)
		{
			printMessage(20+aii, 10, "K");
		}
		else if((fullComb[aii] & 0x0f) == 13)
		{
			printMessage(20+aii, 10, "A");
		}
		else
		{
			printNumber((fullComb[aii] & 0x0f)+1, 20+aii, 10);
		}
	}
*/	
	// EVALUATE YOUR CARDS, TOP PAIR will be considered the strongest combination at FLOP and TURN
	// AVOID FALSE TOP PAIR COMPRISED ONLY OF COMMUNITY CARDS, if this is the case sng will drop to 0
	// AVOID FALSE 3 of A KIND, sng will drop to 0 in this case
	
	// THERE WILL BE rare situations with false QUADS STRAIGTHS AND FLUSHES, this is not covered currently, AIs will call in this case
	
	sng1 = sng; // used for straightDraw, straightDraw makes sense only if there isn't any pair, false pair will make sng 0
	            // so sng1 will keep track of any pair 
	
	// Check here if there is false top pair, 3 of Kind or 2 pairs
	// Check it only after preflop stage
	if(stage)
	{
		if(sng == 3)
		{
			realTopPair();
		}
		else if(sng == 4)
		{
			real2pair();	
		}
		else if(sng == 5)
		{
			///realTrio();
		}
	}
	// If any of these strengts were false, sng will be updated, sng1 will stay the same to be used for the straightDraw()
	// because straight draw function requires that there arent any pairs at all (on the table or combined with pocket cards) 
	
	
	//first check if should call to continue playing
	if(possibilities)
	{
		//now check if there is high chance that player is strongest at this point
		// check this for preflop
		if(stage == 0)
		{
			// preflop scaled strengths will go from 0 - 3
			if(sng >= 3)
			{
				//first check if this is realy top pair
			//	if(sng == 3)
			//	if((pocketCards[1] & 0x0f) == (pComb[0] & 0x0f) || (pocketCards[0] & 0x0f) == (pComb[0] & 0x0f))
				
				// I am the strongest
				
				decision = 0;   // player calls any
				// here raise should be added also, so player further decides if he is going to just call or raise
			//	shouldIraise();
				return;
			}

		}
		// 
	//	printNumber(stage, 10, 12);
	//			while(!(pad_trigger(0) & PAD_START));
		else if(stage == 1)
		{
			// at the flop stage of the game top pair(sng 3) is considered call any hand for the ai
			// check if this is realy a top PAir
			
			// if there werent false strength readings strength will stay the same
			if(sng >= 3)
			{
#ifdef TESTING				
				printMessage(5, 10, "CALL ANY");
				PAUSE;
				printMessage(5, 10, "                ");
#endif			
				decision = 0;
			//	shouldIraise();
				return;
			}
			// defend straight draw flush draw and high pocket cards at certain bets
			// defend middle pair and bottom pair at certain bets
			
			// if the strength is less than 3, or there were false strength readings folowing functionality will be executed
			// Ok I don't have top pair, maybe I have middle pair, if the bet is not to high it is worth defending it
			
			heroCall = 0;
			range = snesdiv(toCall, bigBlind);
			if(realMiddlePair())
			{
				heroCall = 2; // If the bet is to high to be called, this variable will make AI to make about 50% to call anyway with the middle pair
#ifdef TESTING				
				printMessage(5, 10, "REAL MIDDLE PAIR");
				PAUSE;
				printMessage(5, 10, "                ");
#endif			
				if(range <= 15)
				{
#ifdef TESTING	
					printMessage(5, 10, "WILL CALL");
					PAUSE;
					printMessage(5, 10, "         ");
#endif			
					decision = 0;
					return;
				}
			}
			// Same for the bottom pair, it should be defended at flop stage
			else if(realBottomPair())
			{
				heroCall = 2;
#ifdef TESTING				
				printMessage(5, 10, "REAL BOTTOM PAIR");
				PAUSE;
				printMessage(5, 10, "                 ");
#endif			
				if(range <= 10)
				{
#ifdef TESTING					
					printMessage(5, 10, "WILL CALL");
					PAUSE;
					printMessage(5, 10, "         ");
#endif					
					decision = 0;
					return;
				}
			}	
				// determine at which bet levels 0 strength combiantions are to be defended
				// HERE TO DO
				
				// THESE SITUATIONS WILL ONLY BE CHECKED IF THERE ISNT ANY PAIR PRESENT, sng1 used for that, sng will drop to 0 if whole pair is on the table 
			if(!sng1)
			{	
				if(highCards())
				{
					heroCall = 4;
#ifdef TESTING				
					printMessage(5, 10, "High CARDS");
					PAUSE;
					printMessage(5, 10, "          ");
#endif			
					if(range <= 8)
					{
#ifdef TESTING	
						printMessage(5, 10, "WILL CALL");
						PAUSE;
						printMessage(5, 10, "         ");
#endif			
						decision = 0;
						return;
					}
				}
				if(straightDraw())
				{
					heroCall = 3;
#ifdef TESTING					
					printMessage(5, 10, "STRAIGHT DRAW");
					PAUSE;
					printMessage(5, 10, "             ");
#endif			
					if(range <= 8)
					{
#ifdef TESTING					
						printMessage(5, 10, "WILL CALL");
						PAUSE;
						printMessage(5, 10, "              ");
#endif						
						decision = 0;
						return;
					}
				}
				if(flushDraw())
				{
					heroCall = 3;
#ifdef TESTING				
					printMessage(5, 10, "FLUSH DRAW");
					PAUSE;
					printMessage(5, 10, "           ");
#endif					
					if(range <= 8)
					{
#ifdef TESTING	
						printMessage(5, 10, "WILL CALL");
						PAUSE;
						printMessage(5, 10, "            ");
#endif						
						decision = 0;
						return;
					}
				}
				
				
				// 50% for hero call if There is middle pair
				// 33% for hero call if there is bottom pair
				
				// Dont alow to be blufed, call aprox every 4th time
				randomNumber = rand8();
				if(randomNumber == 0)
				{
					++randomNumber;
				}
				
				
				// Take hero call every aprox 4th without good combination
				if(heroCall)
				{
					if(!snesmod(randomNumber, heroCall))
					{
#ifdef TESTING						
						printMessage(5, 10, "HERO CALL");
						PAUSE;
						printMessage(5, 10, "         ");
#endif			
						decision = 0;
						return;
					}
				
				}
				
				
			}
			
			decision = 2;
			return;
			// Defend combinations weaker than top pair at certain bet levels
			// Defend it 1 out of 4 times if the bet is to high for the certain strength
		
		}
		// At turn don't defend bottom pair and high cards, defend middle pair nad straight flush draws
		else if(stage == 2)
		{
			heroCall = 0;
			range = snesdiv(toCall, bigBlind);
			
			if(sng >= 3)
			{
#ifdef TESTING				
				printMessage(5, 10, "CALL ANY");
				while(!(pad_trigger(0) & PAD_START));
				printMessage(5, 10, "                ");
#endif				
				decision = 0;
				shouldIraise();
				return;
			}
			
			// Defend Middle Pair at certain bet levels
			if(realMiddlePair())
			{
				heroCall = 2;
				if(range <= 15)
				{
#ifdef TESTING					
					printMessage(5, 10, "REAL MIDDLE PAIR");
					while(!(pad_trigger(0) & PAD_START));
					printMessage(5, 10, "                ");
#endif			
					decision = 0;
					return;
				}
			}
			if(realBottomPair())
			{
				heroCall = 2;
				if(range <= 10)
				{
#ifdef TESTING					
					printMessage(5, 10, "REAL BOTTOM PAIR");
					while(!(pad_trigger(0) & PAD_START));
					printMessage(5, 10, "                ");
#endif			
					decision = 0;
					return;
				}
				
			}
			// If there arent pairs present, check for straight draw and flush draw at certaind bet levels
			if(!sng1)
			{	
				if(straightDraw())
				{
					heroCall = 4;
					if(range <= 8)
					{
#ifdef TESTING						
						printMessage(5, 10, "STRAIGHT DRAW");
						while(!(pad_trigger(0) & PAD_START));
						printMessage(5, 10, "             ");
#endif			
						decision = 0;
						return;
					}
				}
				if(flushDraw())
				{
					heroCall = 4;
					if(range <= 8)
					{
#ifdef TESTING	
						printMessage(5, 10, "FLUSH DRAW");
						while(!(pad_trigger(0) & PAD_START));
						printMessage(5, 10, "           ");
#endif
						decision = 0;
						return;
					}
				}
				if(highCards())
				{
					heroCall = 0;
					if(range <= 3)
					{
#ifdef TESTING						
						printMessage(5, 10, "High CARDS");
						while(!(pad_trigger(0) & PAD_START));
						printMessage(5, 10, "           ");
#endif			
						decision = 0;
						return;
					}
				}
				// If non of these is worth defending at current bet level, defend your hand 1 out of 4 times if there is something to defend
			}
			
			randomNumber = rand8();
			if(randomNumber == 0)
			{
				++randomNumber;
			}
				
				
			// Take hero call every aprox 4th without good combination
			if(heroCall)
			{
				if(!snesmod(randomNumber, heroCall))
				{
#ifdef TESTING					
					printMessage(5, 10, "HERO CALL");
					while(!(pad_trigger(0) & PAD_START));
					printMessage(5, 10, "         ");
#endif			
					decision = 0;
					return;
				}
				
			}
			decision = 2;
			return;
			
		}
		// at river set is considered call any hand
		// maybe this strength value should be lowered for this stage
		else if(stage == 3)
		{
			if(sng >= 3)
			{
				// DROP CARDS 1 OUT OF 4 TIMES AT REALY BIG BET WITH TOP PAIR IN 
#ifdef TESTING				
				printMessage(5, 10, "CALL ANY");
					while(!(pad_trigger(0) & PAD_START));
					printMessage(5, 10, "                ");
#endif			
				decision = 0;
				shouldIraise();
				return;
			}
			range = snesdiv(toCall, bigBlind);
			heroCall = 0;
			if(realMiddlePair())
			{
				heroCall = 2;
				if(range <= 10)
				{
#ifdef TESTING					
					printMessage(5, 10, "REAL MIDDLE PAIR");
					while(!(pad_trigger(0) & PAD_START));
					printMessage(5, 10, "                ");
#endif			
					decision = 0;		
					return;
					}
			}
			if(realBottomPair())
			{
				heroCall = 4;
				if(range <= 10)
				{
#ifdef TESTING					
					printMessage(5, 10, "REAL BOTTOM PAIR");
					while(!(pad_trigger(0) & PAD_START));
					printMessage(5, 10, "                ");
#endif			
					decision = 0;
					return;
				}
				
			}
				decision = 2;
				return;
		}	
		// At River Only defend Top Pair at high stakes (HOW HIGH?)
		// and Middle pair At lower stakes
		// defend Bottom pair only at the lowest stakes (1-3 BBs)
		
		// If the bet is to high for the current hand strength, defend 1 out of 4 times if top Pair is present
		// If the middle pair is not worth defending, defend it 1 out of 4 times for stake higher than Middle pair defence default, but not to high
		
		// so increase probability depending on the players hand strength
		// there are hand strengths below call any strength
		// these strengths can be 0-2, 0-2, 0-3 and 0-4
		// so if current probability is 4 and player current strength at river is 4(two pairs) probability should increase to 2
		// if probability is 4 at turn and player strength is 3(top pair) probabiliti increases to 2 from 4
		//                                                    middle pair probability increases to 3 from 4
		// probability increases (decrements actualy) for the hand strength value, if probability becomes 1 increment it by 1 (for turn and river)
		
		if(!stage)
		{
			probability -= sng;
			if(probability == 1)
			{
				++probability;
			} 
			intimidated = rand8() % probability;
//		printNumber(probInc + 1, 25, 14);
//		printNumber(intimidated + 1, 25, 15);
//		printNumber(probability + 1, 25, 16);
			if(!intimidated)
			{
				decision = 0;
			// random number decides that the player is not intimidated
			// so the player will call this bet, or he could bluf raise this is to do
			}
			else
			{
				decision = 2; // the player is intimidated so he will fold
			}
			if(snesdiv(alreadyIn, toCall) >= 5)
			{
				decision = 0;
			}
			if((snesdiv(toCall, bigBlind) < 2) && (sng > 0))
			{
				decision = 0;
			}
		}
		
	}
	else
	{
		decision = 0;
		if(stage)
		{
			if(sng >= 3)
			{
				shouldIraise();
			}
			else if(realMiddlePair())
			{
				shouldIraise();
			}
			else if(realBottomPair())
			{
				shouldIraise();
			}
		}
		// Check for realBottom realMiddle, if present call shouldIraise,
		// this way all sng >= 3 will be skipped but the other half will be executed bacause there wont be toCall present
	
	}
}


void shouldIraise()
{
	// It is a river stage and you are asked to check, you have a big combination, so definitely you should raise, raise big
	if(stage == 3)
	{
		if(sng >= 3)
		{
			decision = 1;
			howMuch(11, 90);
			
		}
		else if(sng)
		{
			randNum();
			
			if(snesmod(randomNumber, 4) == 1)
			{
				decision = 1;
#ifdef TESTING				
				printMessage(5, 10, "Bluf attack");
				PAUSE;
				printMessage(5, 10, "            ");
#endif			
				howMuch(11, 90);
			}
		}
		
		return;
	}
	// same thing for flop and turn, if you are the strongest 50 % that it is going to be a raise
	// if it is raise you are most probably going to make small raise
	else if(stage == 2 || stage == 1)
	{
	//	return;
		// most probably make small or medium bet here
		// less probably make big raise here
		if(sng >= 3)
		{
			// 50 % chance it is going to be a raise, change decision only if raise decided
			if(snesmod(rand8(), 2))
			{
				// check toCall here, if there is toCall, counter attack, attack big with big toCalls and high middle with small ones
				// find the raise amount here and return
				
				// if there isnt toCall, then small medium at flop turn, big at river
				
				decision = 1;
				randNum();
				intimidated = snesmod(randomNumber, 6);
				if(!intimidated || intimidated == 1 || intimidated == 2)
				{
					// this is going to be small or medium raise
					// greater chance that this is going to be small raise 3 - 10 BBs
					if(!intimidated || intimidated == 1 || intimidated == 2)
					{
#ifdef TESTING						// make a small raise
						printMessage(5, 10, "Small offensive");
						PAUSE;
						printMessage(5, 10, "               ");
#endif						
						howMuch(3, 5);
					}
					else
					{
#ifdef TESTING						// make medium raise
						printMessage(5, 10, "Medium offensive");
						PAUSE;
						printMessage(5, 10, "               ");
#endif			
						howMuch(5, 10);
					}
				}
				else
				{
#ifdef TESTING						// make a bigger raise
						printMessage(5, 10, "Big offensive");
						PAUSE;
						printMessage(5, 10, "               ");
#endif			
						howMuch(10, 15);
				}
			}
		}
		
		// if !possibilities set variable for small attacking raise inside ai decision function
		// this variable is set only if player has middle or bottom pair, chose probability for this type of raise
		
		// if you are not the strongest, this is small raise, make it small 3 to 5 bbs
		//
		else
		{
			// make a small defensive raise 1 out of 4 times
			randNum();
			randomNumber = snesmod(randomNumber, 4);
			
			if(!randomNumber)
			{
				decision = 1;
#ifdef TESTING				
				printMessage(1, 14, "Defensive Raise");
				PAUSE;
				printMessage(1, 14, "               ");
#endif			
				randomNumber = rand8();
				if(!randomNumber)
				{
					++randomNumber;
				}
				howMuch(1, 3);
			}
		}
	}
	return;
	
	
	intimidated = snesmod(rand8(), 3); // raise half of the time with potentialy strongest hand
	// raise rarely otherwise
	if(intimidated == 1 || intimidated == 0)
	{
		decision = 1;// ai wants to raise, aiRaise will be called then the function will decide if this is going to be raise or just a call
		// based on the aiAmount of money
		
		//calculate how much to raise
		// first find raise range
		// 1 to 4 bbs
		// 5 to 10 bbs
		// 11 to all in
		// then pick random number from that range
		range = snesmod(rand8(), 10);
		if(range < 5)
		{
			range = snesmod(rand8(), 4);
			++range;
			BBs = snesmul(range, bigBlind);
		}
		else if(range < 8)
		{
			range = snesmod(rand8(), 6);
			range += 5;
			BBs = snesmul(range, bigBlind);
		}
		else
		{
			range = snesmod(rand16(), 90);
			range += 11;
			BBs = snesmul(range, bigBlind);
		}
	
/*		else
		{
			BBs = 1000;
		}
*/		
		
		// then calculate how much is that  is that, minBetRaise + number of bbs this is total number of bbs from above
		
		// number of bbs (acctualRaise) must be bigger than minBetRaise
		// example, I want to raise 10bbs, potentialBet = (toCall + minBetRaise) + 10bbs - minBetRaise
		
		if(BBs > minBetRaise)
		{
			BBs = snesdiv((BBs - minBetRaise), bigBlind);
			BBs = snesmul(BBs, bigBlind);
			aiBet = (toCall + minBetRaise) + BBs;
		}
		else
		{
			aiBet = minBetRaise + toCall;
		}
		// it it is not bigger
		// potentialBet = toCall + minBetRaise;
		
		// when the number is found, check if there is enough money to make souch a bet, this should be done in aiRaise
		// if(potentialBet > cash) potentialBet = (toCall + minBetRaise) + 10bbs - minBetRaise
		// else go all in
		
		// aiRaise, if cash <= toCall, go all in play call SF
		// else, enough to make complete bet, deduct cash add to the in the pot play raise SF
		// else go all in, cash = 0, inThePot += cash, inThePlay--, .inThePlay = 0 
		
		
		// lowerSelector function calls aiRaise function
		// then do raise and set all necessary variables, add raise amount to the inThePot, deduct raise amount from the cash
		
	}
}

void raiseAmount()
{
	
	range = snesmod(rand8(), raiseMax - raiseOfset);
	range += raiseOfset;
	BBs = snesmul(range, bigBlind);
	
	if(BBs > minBetRaise)
	{
		BBs = snesdiv((BBs - minBetRaise), bigBlind);
		BBs = snesmul(BBs, bigBlind);
		aiBet = (toCall + minBetRaise) + BBs;
	}
	else
	{
		aiBet = minBetRaise + toCall;
	}
}

void aiAcesHigh(unsigned char* aiArray)
{
	for(aii = 0; aii < aik; aii++)
	{
		if(!(aiArray[aii] & 0x0f))
		{
			aiArray[aii] &= 0Xf0;
			aiArray[aii] |= 13;
		}
	}
	aii = 0;
}

void aiAcesLow(unsigned char* aiArray)
{
	for(aii = 0; aii < aik; aii++)
	{
		if((aiArray[aii] & 0x0f) == 13)
		{
			aiArray[aii] &= 0Xf0;
		//	auxArray[i] |= 14;
		}
	}
	aii = 0;
}

static void aiSortArray(unsigned char* aiArray)
{
	for (aii = 0; aii < ain-1; aii++)       
  
       // Last i elements are already in place    
       for (aij = 0; aij < ain-aii-1; aij++)
	   {  
           if ((aiArray[aij] & 0x0f) < (aiArray[aij+1] & 0x0f)) 
            {  
              aiAux = aiArray[aij];
              aiArray[aij] = aiArray[aij+1];
              aiArray[aij+1] = aiAux;
          	}	
        }
}

void formFullComb()
{
	fullComb[0] = aiPocket[0];
	fullComb[1] = aiPocket[1];
	for(aii = 2; aii < aiCommSize + 2; aii++)
	{
		fullComb[aii] = aiCommunity[aii - 2];
	}
	aii = 0;
}

unsigned char straightDraw()
{
	if(stage == 1)
	{
		if(((fullComb[0] & 0x0f) - (fullComb[3] & 0x0f)) <= 4)
		return 1;
		if(((fullComb[1] & 0x0f) - (fullComb[4] & 0x0f)) <= 4)
		return 1;
	}
	else if(stage == 2)
	{
		if(((fullComb[0] & 0x0f) - (fullComb[3] & 0x0f)) <= 4)
		return 1;
		if(((fullComb[1] & 0x0f) - (fullComb[4] & 0x0f)) <= 4)
		return 1;
		if(((fullComb[2] & 0x0f) - (fullComb[5] & 0x0f)) <= 4)
		return 1;
	}
	return 0;
}

unsigned char highCards()
{
	if((aiPocket[0] & 0x0f) == 0)
	{
		aiPocket[0] |= 13;
	}
	if((aiPocket[1] & 0x0f) == 0)
	{
		aiPocket[1] |= 13;
	}
	
	if(((aiPocket[0] & 0x0f) == 13) || ((aiPocket[1] & 0x0f) == 13))
	return 1;
	if(((aiPocket[0] & 0x0f) + (aiPocket[1] & 0x0f)) > 19)
	{
		return 1;
	}
	return 0;
}

unsigned char flushDraw()
{
	aij = 0;
	for(aii = 0; aii < aiCommSize + 2; aii++)
	{
		if((fullComb[aii] >> 4) == 0)
		{
			++aij;
			if(aij == 4)
			{
				return 1;
			}
		}
	}
	
	aij = 0;
		for(aii = 0; aii < aiCommSize + 2; aii++)
	{
		if((fullComb[aii] >> 4) == 1)
		{
			++aij;
			if(aij == 4)
			{
				return 1;
			}
		}
	}
	aij = 0;
		for(aii = 0; aii < aiCommSize + 2; aii++)
	{
		if((fullComb[aii] >> 4) == 2)
		{
			++aij;
			if(aij == 4)
			{
				return 1;
			}
		}
	}
	aij = 0;
		for(aii = 0; aii < aiCommSize + 2; aii++)
	{
		if((fullComb[aii] >> 4) == 3)
		{
			++aij;
			if(aij == 4)
			{
				return 1;
			}
		}
	}
}

unsigned char realTopPair()
{
	if((aiPocket[0] & 0x0f) == 0)
	{
		aiPocket[0] |= 13;
	}
	if((aiPocket[1] & 0x0f) == 0)
	{
		aiPocket[1] |= 13;
	}	
	if(stage)
	if(sng == 3)
	{
		if((aiPocket[1] & 0x0f) == (aiCommunity[0] & 0x0f) || (aiPocket[0] & 0x0f) == (aiCommunity[0] & 0x0f))
		{
#ifdef TESTING			
			printMessage(5, 10, "Real Top Pair");
			PAUSE;
			printMessage(5, 10, "              ");
#endif			
			sng = 3;
			return 1;
		}
		else if(((aiPocket[0] & 0x0f) == (aiPocket[1] & 0x0f)) && ((aiPocket[0] & 0x0f) > (aiCommunity[0] & 0x0f)))
		{
#ifdef TESTING	
			printMessage(5, 10, "Real Top Pair");
			PAUSE;
			printMessage(5, 10, "             ");
#endif			
			sng = 3;
			return 1;
		}
		else
		{
#ifdef TESTING			
			printMessage(5, 10, "False Top Pair");
			PAUSE;
			printMessage(5, 10, "                ");
#endif			
			sng = 0;
			return 0;
		}

	}
	return 0;
	
}

unsigned char realMiddlePair()
{
//	if(stage)
	if(sng == 2)
	{
		if(((aiPocket[1] & 0x0f) == (aiCommunity[1] & 0x0f)) || ((aiPocket[0] & 0x0f) == (aiCommunity[1] & 0x0f)))
		{
#ifdef TESTING			
			printMessage(5, 10, "Real Second Pair");
			PAUSE;
			printMessage(5, 10, "                 ");
#endif			
			sng = 2;
			return 1;
		}
		else if(((aiPocket[0] & 0x0f) == (aiPocket[1] & 0x0f)) && ((aiPocket[0] & 0x0f) > (aiCommunity[1] & 0x0f)))
		{
#ifdef TESTING			
			printMessage(5, 10, "Real Second Pair");
			PAUSE;
			printMessage(5, 10, "                ");
#endif			
			sng = 2;
			return 1;
		}
		else
		{
#ifdef TESTING			
			printMessage(5, 10, "False Second Pair");
			PAUSE;
			printMessage(5, 10, "                 ");
#endif			
			sng = 0;
			return 0;
		}
		return 0;
	}
	return 0;
}

// Third strongest pair if all community cards are paired
unsigned char realBottomPair()
{
	if(stage)
	if(sng == 1)
	{
		if((aiPocket[1] & 0x0f) == (aiCommunity[2] & 0x0f) || (aiPocket[0] & 0x0f) == (aiCommunity[2] & 0x0f))
		{
#ifdef TESTING			
			printMessage(5, 10, "Real Third Pair");
			sng = 1;
			PAUSE;
			printMessage(5, 10, "               ");
#endif			
			return 1;
		}
		else if(((aiPocket[0] & 0x0f) == (aiPocket[1] & 0x0f)) && ((aiPocket[0] & 0x0f) > (aiCommunity[2] & 0x0f)))
		{
#ifdef TESTING			
			printMessage(5, 10, "Real Third Pair");
			PAUSE;
			printMessage(5, 10, "                ");
#endif			
			sng = 1;
			return 1;
		}
		else
		{
#ifdef TESTING			
			printMessage(5, 10, "False Third Pair");
			printNumber(sng, 5, 11);
			PAUSE;
			printMessage(5, 10, "                ");
#endif			
			sng = 0;
			return 0;
		}

	}
	return 0;
}

unsigned char real2pair()
{
			
	if(sng == 4)
	{
		for(aii = 0; aii < aiCommSize - 1; aii++)
		{
			for(aij = aii + 1; aij < aiCommSize; aij++)
			{
				if((aiCommunity[aii] & 0x0f) == (aiCommunity[aij] & 0x0f))
				{
#ifdef TESTING					
					printMessage(5, 10, "FALSE 2 PAIR");
					PAUSE;
					printMessage(5, 10, "            ");
#endif			
					// This isnt real 2 PAIR, so check if the player at least has top PAir or Middle Pair
					sng = 3;
					if(realTopPair())
					{
						return 0;
					}
					sng = 2;
					if(realMiddlePair())
					{
						return 0;
					}
					sng = 0;
					return 0;
				}
			} 
		}
#ifdef TESTING		
		printMessage(5, 10, "REAL 2 PAIR");
					PAUSE;
					printMessage(5, 10, "            ");			
#endif		
		return 1;
	}
	
}
#pragma code-name(pop)
