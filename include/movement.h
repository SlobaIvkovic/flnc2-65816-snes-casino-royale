#ifndef MOVEMENT_H
#define MOVEMENT_H

extern unsigned char y1, x1;
extern unsigned char x0, y0;
extern unsigned char framesBetween;
extern unsigned char hit;

extern unsigned char arrived;

void trampoline();

#pragma wrapped-call(push, trampoline, 0x85)
void moveCardSequence();
void moveDL();
#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x90)
void drMove();
void urMove();
#pragma wrapped-call(pop)

#define changeCardPoss(a, b, c, d) {\
										x0 = a;\
										x1 = b;\
										y0 = c;\
										y1 = d;\
										if(x0 > x1)\
										{\
											if((x0 - x1) > (y1 - y0))\
											{\
												framesBetween = (x0 - x1) >> 3;\
											}\
											else\
											{\
												framesBetween = (y1 - y0) >> 3;\
											}\
										}\
										else{\
												if((x1 - x0) > (y1 - y0))\
												{\
													framesBetween = (x1 - x0) >> 3;\
												}\
												else\
												{\
													framesBetween = (y1 - y0) >> 3;\
												}\
											}\
										moveCardSequence();\
									}\
									

#endif
