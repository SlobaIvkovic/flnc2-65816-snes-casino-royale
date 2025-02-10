#ifndef SAVING_H
#define SAVING_H

extern unsigned short checksum[2];

extern unsigned char handsPlayed;
extern unsigned char testing;
extern unsigned char guard;
extern unsigned char consecutiveBj;
extern unsigned char curConBj;
extern unsigned char bjWins;

extern unsigned char pokWins;
extern unsigned char consecutivePok;
extern unsigned char curConPok;

extern unsigned char winSingle;
extern unsigned char playSingle;
extern unsigned char loseSingle;

extern unsigned char solUnder10;

extern unsigned char playSingle100;
extern unsigned short winSingle1000;

extern unsigned char cloverWins;
extern unsigned char heartWins;
extern unsigned char diamondWins;
extern unsigned char spadeWins;
extern unsigned char suitWins;
extern unsigned char conBelWins;
extern unsigned char curConBelWins;

void __fastcall__ loadsram();
void __fastcall__ savesram();


#endif
