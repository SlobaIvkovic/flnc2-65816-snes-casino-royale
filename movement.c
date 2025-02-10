#include "include/defines.h"
#include "include/movement.h"
#include "snesgss.h"

void trampoline();

#pragma wrapped-call(push, trampoline, 0x85)
void moveLD();
void moveDL();
void moveDR();
void moveRD();
#pragma wrapped-call(pop)

unsigned char xProgress, yProgress;
unsigned char nSteps;
unsigned char arrived;
unsigned char skipIt;
unsigned char mStates;

unsigned char dx, dy;
int D;

unsigned char DLstate;
extern unsigned char moveState;
//unsigned char i;
//unsigned char j;

//static unsigned char i;
//static unsigned char j;   // maybe can be recycled because these are used only in for loops

extern unsigned char spY;
extern unsigned char spX;
extern unsigned char vramX;
extern unsigned char vramY;

extern unsigned char cardArived;

unsigned char skipNum;
extern unsigned char pickORdisplay;

extern unsigned char alowSprite;
extern unsigned char i;


#pragma code-name(push, "CODE4")
void moveCardSequence()
{

	// Let card be displayed at origin possiton 3 frames
	if(moveState < MOVE_STATE_0)
	{
		if(!moveState)
		sfx_play(6);
		alowSprite = 1;
		spX = x0;
		spY = y0;
		// Set all prerequisites here, starting position end position, available sprid
		++moveState;
	
		return;
		skipNum = 5;
		
	}
	
//		while(1);
			
	// Now place card at 4 possitions one possition each frame
	// distance between card top left top position and card top left at bottom position 
	// will be fixed, y1 - y0 will be constant
	if(moveState < MOVE_STATE)
	{
		// card at bottom y coordinate will be 120
		// so algorithm calculating traveling line points should be ran 4 times 30
		// so we get card frames at y = 30, y = 60 and y = 90
//		if(holdFrame < 10)
//		{
//			++holdFrame;
			if(skipNum < 0)
			{
				++skipNum;
				return;	
			}
			
			skipNum = 0;
			for(i = 0; i < framesBetween; i++)
			{
				if(x1 > x0)
				{
					if((x1 - x0) >= (y1 - y0))
					{
						moveRD();
					}
					else
					{
						moveDR();
					}
				}
				else if((x0 - x1) > (y1 - y0))
										{
											moveLD();
										}
										else
										{
											moveDL();
										}
			}
		
		++moveState;	
		i = 0;
		return;  
	}

	if(moveState < MOVE_STATE + 1)
	{
		spY = y1;
		spX = x1;
		++moveState;
		return;
	}
	
	if(moveState < MOVE_STATE + 2)
	{
		vramX = x1 >> 3;
		vramY = y1 >> 3;
		++moveState;
		return;
	}
	if(moveState < MOVE_STATE + 3)
	{
		++moveState;
		return;
	}
	if(moveState < MOVE_STATE + PAUSE_BETWEEN_CARDS)
	{
		++moveState;
		return;
	}
	pickORdisplay = 0;
	moveState = 0;	
	DLstate = 0;
	cardArived = 1;
	alowSprite = 0;
	hit = 0;

//	availableSprid = 140;
	
}



void moveDL()
{
	if(!DLstate)
	{	
		spX = x0;
		spY = y0;
		dx = x0 - x1;
		dy = y1 - y0; // change with absoulte value y
		D = ((dx << 1) - dy);
		DLstate = 1;
	}

		if(D > 0)
		{
			D += (2 * (dx - dy));
			--spX;
		}
		else
		{
			D += 2*dx;
		}
		spY++;
//		DLstate = 0;
		return;

	
}

void moveLD()
{
	if(!DLstate)
	{	
		spX = x0;
		spY = y0;
		dx = x0 - x1;
		dy = y1 - y0; // change with absoulte value y
		D = (dy << 1) - dx;
		DLstate = 1;
	}

		if(D > 0)
		{
			D -= dx << 1;
			++spY;
		}
			D += dy << 1;
		--spX;
//		DLstate = 0;
		return;
}

void moveDR()
{
	if(!DLstate)
	{	
		spX = x0;
		spY = y0;
		dx = x1 - x0;
		dy = y1 - y0; // change with absoulte value y
		D = ((dx << 1) - dy);
		DLstate = 1;
	}

		if(D > 0)
		{
			D += (2 * (dx - dy));
			++spX;
		}
		else
		{
			D += 2*dx;
		}
		spY++;
//		DLstate = 0;
		return;

	
}

void moveRD()
{
	if(!DLstate)
	{	
		spX = x0;
		spY = y0;
		dx = x1 - x0;
		dy = y1 - y0; // change with absoulte value y
		D = (dy << 1) - dx;
		DLstate = 1;
	}

		if(D > 0)
		{
			D -= dx << 1;
			++spY;
		}
			D += dy << 1;
		++spX;
//		DLstate = 0;
		return;
}
#pragma code-name(pop)

#pragma code-name(push, "CODE8")
void drMove()
{
	if(!mStates)
	{
		xProgress = (x1 - x0)/4;
		yProgress = (y1 - y0)/4;
		spX = x0;
		spY = y0;
		
		++skipIt;
		if(skipIt < 10)
		{
			if(skipIt == 1)
			sfx_play(6);
			return;
		}
		else
		{
			skipIt = 0;
			++mStates;
		}
		return;
	}
	else
	{
		++skipIt;
		if(skipIt < 2)
		{
			return;
		}
		else
		{
			skipIt = 0;
		}
	
		spX += xProgress;
		spY += yProgress;
	
		++nSteps;
		if(nSteps == 4)
		{
			spX = x1;
			spY = y1;
			arrived = 1;
			nSteps = 0;
			mStates = 0;
		}
	}
}

void urMove()
{
	if(!mStates)
	{
		xProgress = (x1 - x0)/4;
		yProgress = (y0 - y1)/4;
		spX = x0;
		spY = y0;
		
		++skipIt;
		if(skipIt < 10)
		{
			if(skipIt == 1)
			sfx_play(6);
			return;
		}
		else
		{
			skipIt = 0;
			++mStates;
		}
		return;
	}
	else
	{
		++skipIt;
		if(skipIt < 2)
		{
			return;
		}
		else
		{
			skipIt = 0;
		}
	
		spX += xProgress;
		spY -= yProgress;
	
		++nSteps;
		if(nSteps == 4)
		{
			spX = x1;
			spY = y1;
			arrived = 1;
			nSteps = 0;
			mStates = 0;
		}
	}
}
#pragma code-name(pop)
