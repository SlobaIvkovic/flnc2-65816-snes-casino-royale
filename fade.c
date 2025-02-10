#include "neslib.h"

#define FADE 31
#define FADEDIV (FADE / 15)

void fadein() {

	static unsigned char  iii;
	for (iii = 0; iii < FADE; ++iii) {
		pal_bright(iii / FADEDIV);
		ppu_wait_nmi();
	}
}

void fadeout() {

	static unsigned char iii;
	for (iii = 0; iii < FADE; ++iii) {
		pal_bright(15 - (iii / FADEDIV));
		ppu_wait_nmi();
	}

}
