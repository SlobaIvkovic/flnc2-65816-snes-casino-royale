#ifndef DRAWING_SMALL_CARDS_H
#define DRAWING_SMALL_CARDS_H

#define BG1_NT(x, y) (0x2400 | ((y<<5) | (x)))

void trampoline();

#pragma wrapped-call(push, trampoline, 0x85)

void solBa();
void solClBase();
void solClMiddle();
void solHeBase();
void solHeMiddle();
void solDiBase();
void solDiMiddle();
void solSpBase();
void solSpMiddle();
void solCardDisplay();


#pragma wrapped-call(pop)

#pragma wrapped-call(push, trampoline, 0x86)
void pokSpriteCl();
void pokSpriteCard();
void alternativePokSprite();
void smallSpadeSprite();
void smallBackSprite();
#pragma wrapped-call(pop)


#define pokBa() solBa()
#define pokCardDisplay() solCardDisplay()


#endif
