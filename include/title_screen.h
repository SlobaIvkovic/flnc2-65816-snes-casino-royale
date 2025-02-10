#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "defines.h"

void trampoline();

#pragma wrapped-call(push, trampoline, 0x85)
void titleScreen();
#pragma wrapped-call(pop)

#endif
