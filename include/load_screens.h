#ifndef LOAD_SCREENS_H
#define LOAD_SCREENS_H

#include "defines.h"

void trampoline(void);

#pragma wrapped-call(push, trampoline, 0x85)
void loadTitleScreen();
void loadBettingScreen();
void loadEndScreen();
void loadGame();
void loadBlackJack();
#pragma wrapped-call(pop)

#endif
