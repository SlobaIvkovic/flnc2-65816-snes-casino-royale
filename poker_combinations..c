#include "neslib.h"

unsigned char ttz;

unsigned char toaks, pairs;
unsigned char pair1[2];
unsigned char pair2[2];
unsigned char pair3[2];

unsigned char toak1[3], toak2[3];
unsigned char rest[7];
unsigned char restSize;

unsigned char combination[7];
unsigned char combination1[5];
extern unsigned char communityCards[5];
extern unsigned char communitySize;
unsigned char combinationStrength;

void trampoline();

#pragma wrapped-call(push, trampoline, 0x86)
void sortBySuit();
void sortArray();
void checkOfAkind();
void assembleOfAkind();
void acesHigh(unsigned char* auxArray);
void acesLow(unsigned char* auxArray);
void checkStraightFlush();
void checkStraight();
void checkFlush(unsigned char* flushCheck);
void checkCombination();


void preflopStrength();
void pairedStrength();

void displayNumber();
#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x85)
void displayMessage();
#pragma wrapped-call(pop)

unsigned char auxArray[7];
//unsigned char auxArray1[7];
extern unsigned char i, j, k;
unsigned char n;
unsigned char aux;

	unsigned char sizeToCheck;
	unsigned char ofAkind;
	unsigned char ii;
	unsigned char aux;
	unsigned char m;

extern unsigned char bg3X;
extern unsigned char bg3Y;
extern unsigned long num1;

unsigned char combinationSize;
unsigned char flushSize;
unsigned char suit;
unsigned char suited;
unsigned char straightCount;

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


#pragma code-name(push, "CODE5")

void checkCombination()
{
	combinationStrength = 0;
	checkStraightFlush();
	if(!combinationStrength)
	{
		checkOfAkind();
	}
	
}

void checkOfAkind()
{
	n = 0;
	i = 0;
	j = 1;
	ii = 1;
	sizeToCheck = combinationSize;
	aux = 0;
	m = 0;
	
	toaks = 0;
	pairs = 0;	
	combinationStrength = 0;
	ofAkind = 0;
	restSize = 0;
	
	for(m = 0; m < sizeToCheck - 1; m++)
	{
		while(j < sizeToCheck)
		{
			
		//	combination[j] = combination[i];
			if((combination[i] & 0x0F) == (combination[j] & 0x0F))
			{
			
				++ofAkind;
				
				aux = combination[i+ii];
				combination[i+ii] = combination[j];
				combination[j] = aux;

				++ii;
				
			}

			++j;
			
		}
	//	return;
		if(!ofAkind)
		{
			rest[restSize] = combination[i];
			
		/*	if(((rest[restSize] & 0x0f) > 9) || (rest[restSize] & 0x0f) == 0)
			{
				printMessage(2+restSize, 25, "S");
			}
			else
			printNumber(rest[restSize] & 0x0f, 1+restSize, 25);
*/
			++restSize;
		//	auxArray[i] = rest[i];
		//	while(!(pad_trigger(0) & PAD_START));
			
		}
		
		
		
		if(ofAkind)
		{
			num1 = ofAkind + 1;
		/*	printNumber(ofAkind + 1, 6, 17);
			printMessage(9, 17, " ofAkind");
				while(!(pad_trigger(0) & PAD_START));
		*/
			if(ofAkind == 3)
			{
//				printf("QUADS");
				combinationStrength = 9;
				combination1[0] = combination[i];
				combination1[1] = combination[i+1];
				combination1[2] = combination[i+2];
				combination1[3] = combination[i+3];
				aux = combination[i];
				acesHigh(combination);
				n = combinationSize;
				sortArray(combination);
				
				for(j =0 ; j < combinationSize; j++)
				{
					if((combination[j] & 0x0F) != (aux & 0x0f))
					{
						combination1[4] = combination[j];
						break;		
					}
				}
				
				return;
			}
			else if(ofAkind == 2)
			{
				
			//	displayMessage(" TRILING");
				
				++toaks;
				if(toaks == 1)
				{
					
					toak1[0] = combination[i];
					toak1[1] = combination[i+1];
					toak1[2] = combination[i+2];
				}
				else
				{
					toak2[0] = combination[i];
					toak2[1] = combination[i+1];
					toak2[2] = combination[i+2];
				}

			}
			else if(ofAkind == 1)
			{
				++pairs;
				if(pairs == 1)
				{
					pair1[0] = combination[i];
					pair1[1] = combination[i+1];
				}
				else if(pairs == 2)
				{
					pair2[0] = combination[i];
					pair2[1] = combination[i+1];
				}
				else
				{
					pair3[0] = combination[i];
					pair3[1] = combination[i];
				}
			}
		
		}
//		toaks = 0;
//		pairs = 0;
		i += ii;
		m += ii - 1;
		j = i+1;
		ii = 1;
		ofAkind = 0;
		
		
	}
	if((combination[5] & 0x0f) != (combination[6] & 0x0f))
		{
			rest[restSize] = combination[j-1];
			++restSize;
		//	printMessage(1, 26, "Nothing");
		//	printNumber(restSize, 1, 27);
		}
//	while(1);
//	printf("TOAKS %d\n", toaks);
//	printf("PAIRS %d\n", pairs);
//	k = 0;
//	printMessage(1, 25, "BEFORE HURT");
//	printNumber(restSize, 12, 25);
//	while(1);
/*	for(i = 0; i < restSize; i++)
	{
		auxArray[i] = rest[i];
	
		
	}
*/	
	n = restSize;
	acesHigh(rest);
	sortArray(rest);
/*	
	for(i = 0; i < restSize; i++)
	{
		rest[i] = auxArray[i];
			if((rest[i] & 0x0f) > 9)
		{
		//	printMessage(5+i, 25, "S");
		}
		else
		{
		//	printNumber(rest[i] & 0x0f, 4+i, 26);	
		}
		
	//	while(!(pad_trigger(0) & PAD_START));
	}
*/	
	assembleOfAkind(); 

}

unsigned char u;

void assembleOfAkind()
{
	if(toaks > 1)
	{
		combinationStrength = 8;
		if((toak1[0] & 0x0f) > (toak2[0] & 0x0f))
		{
			
			combination1[0] = toak1[0];
			combination1[1] = toak1[1];
			combination1[2] = toak1[2];
			combination1[3] = toak2[0];
			combination1[4] = toak2[1];
		}
		else
		{
			combination1[0] = toak2[0];
			combination1[1] = toak2[1];
			combination1[2] = toak2[2];
			combination1[3] = toak1[0];
			combination1[4] = toak1[1];
		}
	}
	else if(toaks && pairs)
	{
		combinationStrength = 8;
		if((pair1[0] & 0x0f) > (pair2[0] & 0x0f))
		{
			combination1[0] = toak1[0];
			combination1[1] = toak1[1];
			combination1[2] = toak1[2];
			combination1[3] = pair1[0];
			combination1[4] = pair1[1];
		}
		else
		{
			combination1[0] = toak1[0];
			combination1[1] = toak1[1];
			combination1[2] = toak1[2];
			combination1[3] = pair2[0];
			combination1[4] = pair2[1];
		}
	}
	else if(toaks)
	{
		combinationStrength = 5;
		combination1[0] = toak1[0];
		combination1[1] = toak1[1];
		combination1[2] = toak1[2];
		combination1[3] = rest[0];
		combination1[4] = rest[1];
	}
	else if(pairs)
	{
		if(pairs == 1)
		{	
			combinationStrength = 1;
			combination1[0] = pair1[0];
			combination1[1] = pair1[1];
			combination1[2] = rest[0];
			combination1[3] = rest[1];
			combination1[4] = rest[2];
		
			n = combinationSize;               // save combinationSize
			combinationSize = communitySize;   // use combinationSize to make aces high in communityCards array
			acesHigh(communityCards);
			
			combinationSize = n;               // restore combinationSize
			n = communitySize;                 // sort communityCards array
			sortArray(communityCards);
			
			if((pair1[0] & 0x0F) >= (communityCards[0] & 0x0F))
			{
				combinationStrength = 3;
			}
			else if((pair1[0] & 0x0F) >= (communityCards[1] & 0x0F))
			{
				combinationStrength = 2;
			}
			n = combinationSize;                 // save 
			combinationSize = communitySize;     // make aces low for the next player            
			acesLow(communityCards);             
			combinationSize = n;                 // restore
			
		}
		else if(pairs == 2)
		{
			if((pair1[0] & 0x0f) > (pair2[0] & 0x0f))
			{
				combination1[0] = pair1[0];
				combination1[1] = pair1[1];
				combination1[2] = pair2[0];
				combination1[3] = pair2[1];
				combination1[4] = rest[0];
			}
			else
			{
				combination1[0] = pair2[0];
				combination1[1] = pair2[1];
				combination1[2] = pair1[0];
				combination1[3] = pair1[1];
				combination1[4] = rest[0];
			}
			combinationStrength = 4;
		}
		else if(pairs == 3)
		{
			if((pair1[0] & 0x0f) > (pair2[0] & 0x0f))
			{
				if((pair1[0] & 0x0f) > (pair3[0] & 0x0f))
				{
					combination1[0] = pair1[0];
					combination1[1] = pair1[1];
					if((pair3[0] & 0x0f) > (pair2[0] & 0x0f))
					{
						combination1[2] = pair3[0];
						combination1[3] = pair3[1];
						if((pair2[0] & 0x0f) > (rest[0] & 0x0f))
						{
							rest[0] = pair2[0];
						}
					}
					else
					{
						combination1[2] = pair2[0];
						combination1[3] = pair2[1];
						if((pair3[0] & 0x0f) > (rest[0] & 0x0f))
						{
							rest[0] = pair3[0];
						}
					}
					
				}
				else
				{
					combination1[0] = pair3[0];
					combination1[1] = pair3[1];
					combination1[2] = pair1[0];
					combination1[3] = pair1[1];
					if((pair2[0] & 0x0f) > (rest[0] & 0x0f))
					{
						rest[0] = pair2[0];
					}
				}
				
			}
			else
			{
				if((pair2[0] & 0x0f) > (pair3[0] & 0x0f))
				{
					combination1[0] = pair2[0];
					combination1[1] = pair2[1];
					if((pair3[0] & 0x0f) > (pair1[0] & 0x0f))
					{
						combination1[2] = pair3[0];
						combination1[3] = pair3[1];
						if((pair1[0] & 0x0f) > (rest[0] & 0x0f))
						{
							rest[0] = pair3[0];
						}
					}
					else
					{
						combination1[2] = pair1[0];
						combination1[3] = pair1[1];
						if((pair3[0] & 0x0f) > (rest[0] & 0x0f))
						{
							rest[0] = pair3[0];
						}
					}
				}
				else
				{
					combination1[0] = pair3[0];
					combination1[1] = pair3[1];
					combination1[2] = pair2[0];
					combination1[3] = pair2[1];
					if((pair1[0] & 0x0f) > (rest[0] & 0x0f))
					{
						rest[0] = pair1[0];
					}
				}
			}
			combination1[4] = rest[0];
		
		combinationStrength = 4;				
		}
	}
	else
	{
		combinationStrength = 0;
		combination1[0] = rest[0];
		combination1[1] = rest[1];
		combination1[2] = rest[2];
		combination1[3] = rest[3];
		combination1[4] = rest[4];
	}
	toaks = 0;
	pairs = 0;
}

void checkStraightFlush()
{
	i = 0;
	j = 0;
	k = 0;
//	combinationSize = 7;  // combinationSize can be 5,6 or 7;
	// copy combination to aux for sorting

	// now sort array, n is the size of the array to be sorted, for of a kind rest can be 5, 4, 3 and 2
	//                                                          for straight and flash size to be sorted can be 5, 6 or 7
	n = combinationSize;
//	acesHigh(combination);     // make combination aces to be highest cards
//	sortArray(combination);    // sort current combination which can be 5 6 or 7
	acesHigh(combination);
	sortBySuit(combination);
	
	for(i = 0; i < combinationSize - 4; i++)
	{
		ttz = i;
		j = i;
		checkStraight();
	
		if(combinationStrength == 6)
		{
			flushSize = 5; 
		
			checkFlush(combination1);
		
			if(combinationStrength == 7)
			{
				combinationStrength = 10;
				return;
			}
		//	combinationSize = combinationSizeOld;
	
		}
		i = ttz;
		
		
	}
	
	
	
	i = 0;
	j = 0;
//	acesLow(combination);
//	sortArray(combination);


	acesLow(combination);
	sortBySuit(combination);
	
	n = combinationSize;
	
	for(i = 0; i < combinationSize - 4; i++)
	{
		j = i;
		ttz = i;
		checkStraight();
		if(combinationStrength == 6)
		{
			flushSize = 5; 
			
			checkFlush(combination1);
			
			if(combinationStrength == 7)
			{
	//			printf("STRAIGTH FLUSH\n");
				for(i = 0; i < 5; i++)
				{
	//				printf("0x%2x, ", combination1[i]);
				}
				combinationStrength = 10;
				return;
			}
		//	combinationSize = combinationSizeOld;
			
		}
		i = ttz;
	//	printf(" i = %d  < %d", i, combinationSize - 4);
	}
	
	
	//printf("\nstraightFlushend");
	//system("PAUSE");
	
	j = 0;
	i = 0;
	n = combinationSize;
	acesHigh(combination);
	sortArray(combination);
	combinationStrength = 0;
	j = 0;
	i = 0;
	checkStraight();
/*	
	if(combinationStrength == 4)
	{
	
		printMessage(20, 25, "Straight");
		ppu_wait_nmi();
		printNumber(combination[0],18, 25);

	}
 */
 	
	if(combinationStrength < 6)
	{	
		j = 0;
		i = 0;
		n = combinationSize;
		acesLow(combination);
		sortArray(combination);
		j = 0;
		i = 0;
		combinationStrength = 0;
		checkStraight();
	}
	if(combinationStrength == 6)
	{
	//	printf("\nIt is straight\n");
		for(i = 0; i < 5; i++)
		{
	//		printf("|0x%02x|",combination1[i]);
		}
	}
 	
 	j = 0;
	i = 0;
	n = combinationSize;
	acesHigh(combination);
	sortArray(combination);
//	combinationStrength = 0;
	flushSize = combinationSize;
	checkFlush(combination);
	
	if(combinationStrength == 7)
	{
		j = 0;
		for(i = 0; i < combinationSize; i++)
		{
			if(combination[i] >> 4 == suit)
			{
				combination1[j] = combination[i];
				j++;
				if(j == 5)
				{
					break;
				}
			}
		}
	
	}
	// in the end if flush found, find the right suit, place all those suits into combination
	// sort combination with aces high, copy combination to combination1

}
 



void checkStraight()
{		
		straightCount = 0;
		while(j < combinationSize - 1)
		{
			if((combination[j] & 0x0f) == (combination[j+1] & 0x0f))
			{
				++j;
				continue;
			}
			if(((combination[j] & 0x0f) - (combination[j+1] & 0x0f)) != 1)
			{
				straightCount = 0;
			
			}
			else
			{
				combination1[straightCount] = combination[j];
				combination1[straightCount+1] = combination[j+1];
				++straightCount;
				if(straightCount == 4)
				{	
					combination1[4] = combination[j + 1];
					combinationStrength = 6;
			//	printMessage(20, 25, "Straight");
					return;		
				}
				
			}
			++j;
			
		}
		
	
}


void checkFlush(unsigned char* flushCheck)
{
	suited = 0;
	j = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < flushSize; j++)
		{
			if((flushCheck[j] >> 4) == i)
			{
			//	combination1[suited] = flushCheck[j];
			//	flushCheck[suited] = flushCheck[j];
				suited++;
			}
		}
		if(suited > 4)
		{
			combinationStrength = 7;
			suit = i;
			// it is flush, 
			// if straight was checked set combination to straight flush and break
			// else if not straight already set set combination to only flush and break
		}
		
		suited = 0;
	}
}

void sortBySuit(unsigned char *auxArray)
{
	for (i = 0; i < n-1; i++)       
  
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)
	   {  
           if ((auxArray[j]) < (auxArray[j+1])) 
            {  
              aux = auxArray[j];
              auxArray[j] = auxArray[j+1];
              auxArray[j+1] = aux;
          	}	
        }
}

void sortArray(unsigned char* auxArray) 
{  
	
   for (i = 0; i < n-1; i++)       
  
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)
	   {  
           if ((auxArray[j] & 0x0f) < (auxArray[j+1] & 0x0f)) 
            {  
              aux = auxArray[j];
              auxArray[j] = auxArray[j+1];
              auxArray[j+1] = aux;
          	}	
        }
} 

void acesHigh(unsigned char* auxArray)
{
	for(i = 0; i < combinationSize; i++)
	{
		if(!(auxArray[i] & 0x0f))
		{
			auxArray[i] &= 0Xf0;
			auxArray[i] |= 13;
		}
	}
}

void acesLow(unsigned char *auxArray)
{
	for(i = 0; i < combinationSize; i++)
	{
		if((auxArray[i] & 0x0f) == 13)
		{
			auxArray[i] &= 0Xf0;
		//	auxArray[i] |= 14;
		}
	}
}

unsigned char highCard;
unsigned char connectednes;

void preflopStrength()
{
	combinationStrength = 0;
	connectednes = 0;
	// only 4 strenghts can be here, strength 3 calls any
	// strength 2 calls 20% of time at highest raise, 25% at second highest amount, 33% at second lowest and 50% at lowest
	// strength 1 call probability is 1 out of 6 at highest call range
	// strength 0 call probability is 1 out of 7 at highest, 1 out of 4 at lowest range
	
	// make aces high for the combination first
	if(!(combination[0] & 0x0f))
	{
		combination[0] &= 0Xf0;
		combination[0] |= 13;
	}
	if(!(combination[1] & 0x0f))
	{
		combination[1] &= 0Xf0;
		combination[1] |= 13;
	}
	
	// if this is paired pocket
	if((combination[0] & 0x0f) == (combination[1] & 0x0f))
	{
		pairedStrength();
	}
	// if not first find high card
	else
	{
		if((combination[0] & 0x0f) > (combination[1] & 0x0f))
		{
			if((combination[0] & 0x0f) > 8)
			{
				combinationStrength = (combination[0] & 0x0f) - 8;
			}
			if(((combination[0] & 0x0f) - (combination[1] & 0x0f) <= 4))
			{
				connectednes = 5 - ((combination[0] & 0x0f) - (combination[1] & 0x0f));
			}
		}
		else
		{
			if((combination[1] & 0x0f) > 8)
			{
				combinationStrength = (combination[1] & 0x0f) - 8;
			}
			if(((combination[1] & 0x0f) - (combination[0] & 0x0f) <= 4))
			{
				connectednes = 5 - ((combination[1] & 0x0f) - (combination[0] & 0x0f));
			}
			
		}
		// points for the high card value are added now
		// add connectednes points
		combinationStrength += connectednes;
		// is this combination suited
		if((combination[0] & 0xF0) == (combination[1] & 0xF0))
		{
			++combinationStrength;
		}
	}
	
	// at the end scale down strengths to 4 strengths, monsters being 3
	// then next 3 levels being 2
	// then next 4 being 1
	if(combinationStrength >= 9)
	{
		combinationStrength = 3;
	}
	else if(combinationStrength >= 6)
	{
		combinationStrength = 2;
	}
	else if(combinationStrength > 1)
	{
		combinationStrength = 1;
	}
	else
	{
		combinationStrength = 0;
	}
}

void pairedStrength()
{	
	// JJ and up
	if((combination[0] & 0x0f) > 9)
	{
		combinationStrength = 10;
		
		// scaled will be 3
	}
	else if((combination[0] & 0x0f) > 7)
	{
		combinationStrength = 9;
		// scaled will be 2
	}
	else
	{
		combinationStrength = 8;
		// scaled will be 1
	}
	
}

#pragma code-name(pop)
