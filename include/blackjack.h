#ifndef BLACKJACK_H
#define BLACKJACK_H

void trampoline();

#pragma wrapped-call(push, trampoline, 0x91)
void blackJack();
#pragma wrapped-call(pop)

#endif
