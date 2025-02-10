#ifndef BETTING_SCREEN_H
#define BETTING_SCREEN_H

void trampoline();

#pragma wrapped-call(push, trampoline, 0x85)
void bettingScreen();
#pragma wrapped-call(pop)

#endif
