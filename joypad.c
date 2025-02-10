#include "neslib.h"
#include "include/joypad.h"

unsigned char pressPoint;
unsigned char lastInput;
unsigned char currentInput;
unsigned char playersJoypad;       // make this extern in poker and give it value from the struct before calling scanKeypad readKeypad

#define TIMEOUT 5





#pragma code-name(push, "CODE5")
void measureMilliseconds()
{/*
	if((unsigned char)(nesclock() - clockCount) > 59)
	{
	//	ppu_wait_nmi();
		
		++seconds;
		if(seconds == 60)
		{
			seconds = 0;
			++minutes;
			if(minutes == 60)
			{
				minutes = 0;
				++hours;
			}
			ppu_wait_nmi();
		}
		clockCount = nesclock();
		displaySeconds();
	
	}*/
}


void scanJoypad()
{	
	readJoypad();
	if((lastInput != currentInput) || (TIMEOUT <= (unsigned char) (nesclock() - pressPoint)))  // if not new button
    {
        // remember point in time only if button is acctualy pressed
    //    readJoypad();
        if(currentInput)
        {
        	pressPoint = nesclock();           // remember point in time when button was pressed
        }
        
        lastInput = currentInput;   // update previous key press
            
    }
    else
    {
        currentInput = 0;
    }
    
}
    
void readJoypad()
{
	if(pad_state(playersJoypad) & PAD_LEFT)
	currentInput = 1;
	else if(pad_state(playersJoypad) & PAD_RIGHT)
	currentInput = 2;
	else if(pad_state(playersJoypad) & PAD_DOWN)
	currentInput = 3;
	else if(pad_state(playersJoypad) & PAD_UP)
	currentInput = 4;
	else if(pad_state(playersJoypad) & PAD_B)
	currentInput = 5;
	else 
	currentInput = 0;
	
}
#pragma code-name(pop)
