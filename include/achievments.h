#ifndef ACHIEVMENTS_H
#define ACHIVEMENTS_H

void trampoline();
#pragma wrapped-call(push, trampoline, 0x91)
void loadAchievmentScreen();
void achievmentScreen();
#pragma wrapped-call(pop)

#endif
