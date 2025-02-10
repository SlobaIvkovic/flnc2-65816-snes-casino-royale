#ifndef OPTIONS_H
#define OPTIONS_H

void trampoline();
#pragma wrapped-call(push, trampoline, 0x91)
void loadOptionsScreen();
void optionsScreen();
#pragma wrapped-call(pop)

#endif
