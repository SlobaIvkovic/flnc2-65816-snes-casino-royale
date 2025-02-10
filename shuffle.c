#include "neslib.h"

extern unsigned char i, j, shuffleNum, dealerScore, playerScore, deckSize;
extern unsigned char cardsDeck[52];

// shuffle is done between blackjack dealings, so playerScore and dealerScore are recycled here
// as they anyway have to be 0 at the begining of the new round
#pragma code-name(push, "CODE4")
void shuffle(){
	
	deckSize = 52;
	for(i = 0; i < shuffleNum; i++)
	{
		for(j = 0; j < 52; j++)
		{
			dealerScore = rand16() % deckSize;
			playerScore = cardsDeck[deckSize-1];
			cardsDeck[deckSize-1] = cardsDeck[dealerScore];
			cardsDeck[dealerScore] = playerScore;
			--deckSize;
		}
		deckSize = 52;
	}
	dealerScore = 0;
	playerScore = 0;
	deckSize = 0;
}
#pragma code-name(pop)
