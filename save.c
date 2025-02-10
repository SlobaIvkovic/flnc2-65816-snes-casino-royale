#pragma bss-name(push, "SRAM")

// Magic bytes, or for complex games, an actual checksum
unsigned short checksum[2];

// Whatever data you want to save
unsigned char handsPlayed;
unsigned char testing;
unsigned char guard;
unsigned char consecutiveBj;
unsigned char curConBj;
unsigned char bjWins;

unsigned char pokWins;
unsigned char consecutivePok;
unsigned char curConPok;

unsigned char solUnder10;

unsigned char winSingle;
unsigned char playSingle;
unsigned char loseSingle;

unsigned short winSingle1000;
unsigned char playSingle100;

unsigned char cloverWins;
unsigned char heartWins;
unsigned char diamondWins;
unsigned char spadeWins;
unsigned char suitWins;
unsigned char conBelWins;
unsigned char curConBelWins;

#pragma bss-name(pop)

// Call loadsram() at the start of main, and savesram() whenever you beat a level,
// or otherwise want to save.

// Finally, edit snesheader.s with the desired save chip size
