#ifndef END_SCREEN_H
#define END_SCREEN_H

//void trampoline();

#pragma wrapped-call(push, trampoline, 0x86)
void endScreen();
#pragma wrapped-call(pop)

#endif
