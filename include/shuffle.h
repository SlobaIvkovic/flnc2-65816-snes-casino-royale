#ifndef SHUFFLE_H
#define SHUFFLE_H

#pragma wrapped-call(push, trampoline, 0x85)
void shuffle();
#pragma wrapped-call(pop)

#endif
