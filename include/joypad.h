#ifndef JOYPAD_H
#define JOYPAD_H

void trampoline();

#pragma wrapped-call(push, trampoline, 0x86)

void scanJoypad();
void readJoypad();

#pragma wrapped-call(pop)

#endif
