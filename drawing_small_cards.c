#include "neslib.h"
#include "include/small_maps.h"
#include "include/drawing_small_cards.h"

#define CLOVER 0
#define HEART 1
#define SPADE 2
#define DIAMOND 3





extern unsigned char vramX, vramY, signNum, i, j, k, availableSprid, spriteX, spriteY;

#pragma code-name(push, "CODE4")

void solBa()
{
	vram_adr(BG1_NT(vramX, vramY));
	k = 0;
	for(i = 0; i < 6; i++)
	{
		vram_adr(BG1_NT(vramX, vramY));
		for(j = 0; j < 4; j++)
		{
			vram_put(solBaMM[k]);        // here every vram write increments x
			++k;		
		}
		++vramY;
	}
	i = 0;
	j = 0;
	k = 0;
}


void solClBase()
{
	k = 0;
	for(i = 0; i < 6; i++)
	{
		vram_adr(BG1_NT(vramX, vramY));
		for(j = 0; j < 4; j++)
		{
			vram_put(solClMM[k]);        // here every vram write increments x
			++k;		
		}
		++vramY;
	}
//	vramX += 4;
	vramY -= 6;
	i = 0;
	j = 0;
	k = 0;
}

void solClMiddle()
{

	vram_adr(BG1_NT(vramX + 1, vramY + 2));
	// write middle map index
	vram_put(solClMidd[(signNum &0x0f) * 4]);
	vram_put(solClMidd[(signNum &0x0f) * 4 + 1]);
	
	vram_adr(BG1_NT(vramX + 1, vramY + 3));
	vram_put(solClMidd[(signNum &0x0f) * 4 + 2]);
	vram_put(solClMidd[(signNum &0x0f) * 4 + 3]);
	
	

}

void solHeBase()
{
	k = 0;
	for(i = 0; i < 6; i++)
	{
		vram_adr(BG1_NT(vramX, vramY));
		for(j = 0; j < 4; j++)
		{
			vram_put(solHeMM[k]);        // here every vram write increments x
			++k;		
		}
		++vramY;
	}
//	vramX += 4;
	vramY -= 6;
	i = 0;
	j = 0;
	k = 0;
}

void solHeMiddle()
{

	vram_adr(BG1_NT(vramX + 1, vramY + 2));
	// write middle map index
	vram_put(solHeMidd[(signNum &0x0f) * 4]);
	vram_put(solHeMidd[(signNum &0x0f) * 4 + 1]);
	
	vram_adr(BG1_NT(vramX + 1, vramY + 3));
	vram_put(solHeMidd[(signNum &0x0f) * 4 + 2]);
	vram_put(solHeMidd[(signNum &0x0f) * 4 + 3]);
	
	

}

void solDiBase()
{
	k = 0;
	for(i = 0; i < 6; i++)
	{
		vram_adr(BG1_NT(vramX, vramY));
		for(j = 0; j < 4; j++)
		{
			vram_put(solDiMM[k]);        // here every vram write increments x
			++k;		
		}
		++vramY;
	}
//	vramX += 4;
	vramY -= 6;
	i = 0;
	j = 0;
	k = 0;
}

void solDiMiddle()
{

	vram_adr(BG1_NT(vramX + 1, vramY + 2));
	// write middle map index
	vram_put(solDiMidd[(signNum &0x0f) * 4]);
	vram_put(solDiMidd[(signNum &0x0f) * 4 + 1]);
	
	vram_adr(BG1_NT(vramX + 1, vramY + 3));
	vram_put(solDiMidd[(signNum &0x0f) * 4 + 2]);
	vram_put(solDiMidd[(signNum &0x0f) * 4 + 3]);
	
}

void solSpBase()
{
	k = 0;
	for(i = 0; i < 6; i++)
	{
		vram_adr(BG1_NT(vramX, vramY));
		for(j = 0; j < 4; j++)
		{
			vram_put(solSpMM[k]);        // here every vram write increments x
			++k;		
		}
		++vramY;
	}
//	vramX += 4;
	vramY -= 6;
	i = 0;
	j = 0;
	k = 0;
}

void solSpMiddle()
{

	vram_adr(BG1_NT(vramX + 1, vramY + 2));
	// write middle map index
	vram_put(solSpMidd[(signNum &0x0f) * 4]);
	vram_put(solSpMidd[(signNum &0x0f) * 4 + 1]);
	
	vram_adr(BG1_NT(vramX + 1, vramY + 3));
	vram_put(solSpMidd[(signNum &0x0f) * 4 + 2]);
	vram_put(solSpMidd[(signNum &0x0f) * 4 + 3]);
	
	

}



void solCardDisplay()
{
	switch(signNum >> 4)
	{
		case(CLOVER):
			solClBase();
			solClMiddle();
			break;
		case(HEART):
			solHeBase();
			solHeMiddle();
			break;
		case(DIAMOND):
			solDiBase();
			solDiMiddle();
			break;
		case(SPADE):
			solSpBase();
			solSpMiddle();
			break;		
	}
}


#pragma code-name(pop)


#pragma code-name(push, "CODE5")

unsigned char attr;
unsigned short auxTile;

void makeAttribute()
{
	attr = auxTile >> 8;
			attr &= 0xC0;  // leave vertical and horizontal flip
			attr |= 0x30;  // set highest priority
			attr |= ((auxTile >> 9) & 0b00001110); // set palette
}

void pokSpriteCl()
{
	
	k = 0;
	
	for(i = 0; i < 6; i++)
	{	
		for(j = 0; j < 4; j++)
		{
			
			if(i == 2 && j == 1)
			{
				auxTile = solClMidd[(signNum &0x0f) * 4];
				makeAttribute();
				availableSprid = oam_spr(spriteX, spriteY, (solClMidd[(signNum &0x0f) * 4] & 0x00FF), attr, availableSprid);
				++j;
				++k;
				spriteX += 8;
				auxTile = solClMidd[(signNum &0x0f) * 4 + 1];
				makeAttribute();
				availableSprid = oam_spr(spriteX, spriteY, (solClMidd[(signNum &0x0f) * 4 + 1] & 0x00FF), attr, availableSprid);
				++k;
				spriteX += 8;	
			}
			else if(i == 3 && j == 1)
			{
				auxTile = solClMidd[(signNum &0x0f) * 4 + 2];
				makeAttribute();
				availableSprid = oam_spr(spriteX, spriteY, (solClMidd[(signNum &0x0f) * 4 + 2] & 0x00FF), attr, availableSprid);
				++j;
				++k;
				spriteX += 8;
				auxTile = solClMidd[(signNum &0x0f) * 4 + 3];
				makeAttribute();
				availableSprid = oam_spr(spriteX, spriteY, (solClMidd[(signNum &0x0f) * 4 + 3] & 0x00FF), attr, availableSprid);
				++k;
				spriteX += 8;	
			}
			else
			{
				auxTile = solClMM[k];
				makeAttribute();
			
				availableSprid = oam_spr(spriteX, spriteY, (solClMM[k] & 0x00FF), attr, availableSprid);
			//	vram_put(solSpMM[k]);        // here every vram write increments x
				++k;
				spriteX += 8;
			}
		}
		spriteY += 8;
		spriteX -= 32;
	}

}

void smallSpadeSprite()
{
	
	k = 0;
	availableSprid = 0;
	for(i = 0; i < 6; i++)
	{	
		for(j = 0; j < 4; j++)
		{
			
			if(i == 2 && j == 1)
			{
				auxTile = smallSpadeMidd[(signNum &0x0f) * 4];
				makeAttribute();
				attr |= 1;
				availableSprid = oam_spr(spriteX, spriteY, (smallSpadeMidd[(signNum &0x0f) * 4] & 0x00FF), attr, availableSprid);
				++j;
				++k;
				spriteX += 8;
				auxTile = smallSpadeMidd[(signNum &0x0f) * 4 + 1];
				makeAttribute();
				attr |= 1;
				availableSprid = oam_spr(spriteX, spriteY, (smallSpadeMidd[(signNum &0x0f) * 4 + 1] & 0x00FF), attr, availableSprid);
				++k;
				spriteX += 8;	
			}
			else if(i == 3 && j == 1)
			{
				auxTile = smallSpadeMidd[(signNum &0x0f) * 4 + 2];
				makeAttribute();
				attr |= 1;
				availableSprid = oam_spr(spriteX, spriteY, (smallSpadeMidd[(signNum &0x0f) * 4 + 2] & 0x00FF), attr, availableSprid);
				++j;
				++k;
				spriteX += 8;
				auxTile = smallSpadeMidd[(signNum &0x0f) * 4 + 3];
				makeAttribute();
				attr |= 1;
				availableSprid = oam_spr(spriteX, spriteY, (smallSpadeMidd[(signNum &0x0f) * 4 + 3] & 0x00FF), attr, availableSprid);
				++k;
				spriteX += 8;	
			}
			else
			{
				auxTile = smallSpadeMM[k];
				makeAttribute();
				attr |= 1;
				availableSprid = oam_spr(spriteX, spriteY, (smallSpadeMM[k] & 0x00FF), attr, availableSprid);
			//	vram_put(solSpMM[k]);        // here every vram write increments x
				++k;
				spriteX += 8;
			}
		}
		spriteY += 8;
		spriteX -= 32;
	}

}

const unsigned short* cardMap;
const unsigned short* cardMapMidd;

void alternativePokSprite()
{
//	oam_clear();
	if((signNum >> 4) == CLOVER)
	{
		cardMap = solClMM;
		cardMapMidd = solClMidd;
		
	}
	else if((signNum >> 4) == SPADE)
	{
		smallSpadeSprite();
		return;
		cardMap = solSpMM;
		cardMapMidd = solSpMidd;
	}
	else if((signNum >> 4) == HEART)
	{
		cardMap = solHeMM;
		cardMapMidd = solHeMidd;
	}
	else if((signNum >> 4) == DIAMOND)
	{
		cardMap = solDiMM;
		cardMapMidd = solDiMidd;
	}
	k = 0;
	availableSprid = 0;
	for(i = 0; i < 6; i++)
	{	
		for(j = 0; j < 4; j++)
		{
			
			if(i == 2 && j == 1)
			{
				auxTile = cardMapMidd[(signNum &0x0f) * 4];
				makeAttribute();
				availableSprid = oam_spr(spriteX, spriteY, (cardMapMidd[(signNum &0x0f) * 4] & 0x00FF), attr, availableSprid);
				++j;
				++k;
				spriteX += 8;
				auxTile = cardMapMidd[(signNum &0x0f) * 4 + 1];
				makeAttribute();
				availableSprid = oam_spr(spriteX, spriteY, (cardMapMidd[(signNum &0x0f) * 4 + 1] & 0x00FF), attr, availableSprid);
				++k;
				spriteX += 8;	
			}
			else if(i == 3 && j == 1)
			{
				auxTile = cardMapMidd[(signNum &0x0f) * 4 + 2];
				makeAttribute();
				availableSprid = oam_spr(spriteX, spriteY, (cardMapMidd[(signNum &0x0f) * 4 + 2] & 0x00FF), attr, availableSprid);
				++j;
				++k;
				spriteX += 8;
				auxTile = cardMapMidd[(signNum &0x0f) * 4 + 3];
				makeAttribute();
				availableSprid = oam_spr(spriteX, spriteY, (cardMapMidd[(signNum &0x0f) * 4 + 3] & 0x00FF), attr, availableSprid);
				++k;
				spriteX += 8;	
			}
			else
			{
				auxTile = cardMap[k];
				makeAttribute();
			
				availableSprid = oam_spr(spriteX, spriteY, (cardMap[k] & 0x00FF), attr, availableSprid);
			//	vram_put(solSpMM[k]);        // here every vram write increments x
				++k;
				spriteX += 8;
			}
		}
		spriteY += 8;
		spriteX -= 32;
	}

}

void pokSpriteClMiddle()
{
	unsigned char attr;
	spriteY -= 32; 
//	vram_adr(BG1_NT(vramX + 1, vramY + 2));
	// write middle map index
//	vram_put(solClMidd[]);
	attr = solClMM[(signNum &0x0f) * 4] >> 8;
	attr &= 0xC0;  // leave vertical and horizontal flip
	attr |= 0x30;  // set highest priority
	attr |= ((solClMM[(signNum &0x0f) * 4] >> 9) & 0b00001110); // set palette
			
	availableSprid = oam_spr(spriteX, spriteY, (solClMM[(signNum &0x0f) * 4] & 0x00FF), attr, availableSprid);
			
//	vram_put(solClMidd[(signNum &0x0f) * 4 + 1]);
	spriteX += 8;	
	attr = solClMM[(signNum &0x0f) * 4 + 1] >> 8;
	attr &= 0xC0;  // leave vertical and horizontal flip
	attr |= 0x30;  // set highest priority
	attr |= ((solClMM[(signNum &0x0f) * 4 + 1] >> 9) & 0b00001110); // set palette
	
	availableSprid = oam_spr(spriteX, spriteY, (solClMM[(signNum &0x0f) * 4 + 1] & 0x00FF), attr, availableSprid);
			
	
	vram_adr(BG1_NT(vramX + 1, vramY + 3));
	vram_put(solClMidd[(signNum &0x0f) * 4 + 2]);
	vram_put(solClMidd[(signNum &0x0f) * 4 + 3]);
	
	

}

void pokSpriteCard()
{
	pokSpriteCl();
	pokSpriteClMiddle();
	
}

void smallBackSprite()
{
	availableSprid = 0;
	k = 75;
	for(i = 0; i < 6; i++)
	{
		for(j = 0; j < 4; j++)
		{
			availableSprid = oam_spr(spriteX, spriteY, k, SPRITE_ATTR_FULL(7,3,0,0,1), availableSprid);
			spriteX += 8;
			++k;		
		}
		spriteX -= 32;
		spriteY +=8;
	}
	k = 0;
}

#pragma code-name(pop)

extern unsigned char auxSignNum;


#pragma code-name(push, "CODE8")
void findMap()
{
	if((auxSignNum >> 4) == CLOVER)
	{
		cardMap = solClMM;
	}
	else if((auxSignNum >> 4) == SPADE)
	{
		cardMap = solSpMM;
	}
	else if((auxSignNum >> 4) == HEART)
	{
		cardMap = solHeMM;
	}
	else if((auxSignNum >> 4) == DIAMOND)
	{
		cardMap = solDiMM;
	}
}

void leftEdge()
{
	// set pointer and draw edge, aux and vramX Y will be set in belote
	
	findMap();
	for(i = 0; i < 24; i+= 4)
	{
		vram_adr(BG1_NT(vramX, vramY));
		vram_put(cardMap[i]);        // here every vram write increments x
		++vramY;
	}
	i = 0;
	
	
}

void rightEdge()
{
	findMap();
	for(i = 3; i < 24; i+= 4)
	{
		vram_adr(BG1_NT(vramX, vramY));
		vram_put(cardMap[i]);        // here every vram write increments x
		++vramY;
	}
	i = 0;
}

void baRightEdge()
{
	for(i = 3; i < 24; i+= 4)
	{
		vram_adr(BG1_NT(vramX, vramY));
		vram_put(solBaMM[i]);        // here every vram write increments x
		++vramY;
	}
	i = 0;
}

void baLeftEdge()
{
	for(i = 0; i < 24; i+= 4)
	{
		vram_adr(BG1_NT(vramX, vramY));
		vram_put(solBaMM[i]);        // here every vram write increments x
		++vramY;
	}
	i = 0;
}
#pragma code-name(pop)
