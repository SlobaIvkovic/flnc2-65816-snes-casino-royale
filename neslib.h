/*
 (C) 2015 Alex Semenov (Shiru)
 (C) 2016 Lauri Kasanen

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

// set this palette, data is 32 bytes array (16 colors)
void __fastcall__ pal_all(const unsigned char idx, const unsigned short *data);

// set a palette entry, index is 0..31
void __fastcall__ pal_col(unsigned char index, unsigned short color);

// reset palette to $0f
void __fastcall__ pal_clear(void);

// set virtual bright both for sprites and background, 0 to 15, 15 = normal
void __fastcall__ pal_bright(unsigned char bright);


// wait actual TV frame, 50hz for PAL, 60hz for NTSC
void __fastcall__ ppu_wait_nmi(void);

// turn off rendering, nmi still enabled when rendering is disabled
void __fastcall__ ppu_off(void);

// turn on bg, spr
void __fastcall__ ppu_on_all(void);

// get current video system, 0 for PAL, not 0 for NTSC
unsigned char __fastcall__ ppu_system(void);

// Return an 8-bit counter incremented at each vblank
unsigned char __fastcall__ nesclock(void);


// clear OAM buffer, all the sprites are hidden
void __fastcall__ oam_clear(void);

// set this sprite as big
// only accesses the first 64 sprites
void __fastcall__ oam_big(unsigned char sprid);

// set sprite in OAM buffer, chrnum is tile, attr is attribute, sprid is offset in OAM in bytes
// returns sprid+4, which is offset for a next sprite
// only accesses the first 64 sprites
unsigned char __fastcall__ oam_spr(unsigned char x, unsigned char y,
					unsigned char chrnum, unsigned char attr,
					unsigned char sprid);

// set sprite in OAM buffer, chrnum is tile, attr is attribute, sprid is offset in OAM in bytes
// returns sprid+4, which is offset for a next sprite
// only accesses the first 64 sprites
// allows negative X
unsigned char __fastcall__ oam_spr_x(int x, unsigned char y,
					unsigned char chrnum, unsigned char attr,
					unsigned char sprid);

// set metasprite in OAM buffer
// meta sprite is a const unsigned char array, it contains four bytes per sprite
// in order x offset, y offset, tile, attribute
// x=128 is end of a meta sprite
// returns sprid+4, which is offset for a next sprite
// only accesses the first 64 sprites
unsigned char __fastcall__ oam_meta_spr(unsigned char x, unsigned char y,
					unsigned char sprid, const unsigned char *data);

// same but allows negative X
unsigned char __fastcall__ oam_meta_spr_x(int x, unsigned char y,
					unsigned char sprid, const unsigned char *data);

// hide all remaining sprites from given offset
void __fastcall__ oam_hide_rest(unsigned char sprid);


// get controller in trigger mode, a flag is set only on button down, not hold
unsigned int __fastcall__ pad_trigger(unsigned char pad);

// get previous pad state without polling ports
unsigned int __fastcall__ pad_state(unsigned char pad);

// is a multitap connected in port 2?
unsigned char __fastcall__ multitap_connected(void);

// set scroll
// it is always applied at beginning of a TV frame, not at the function call
void __fastcall__ scroll(unsigned char x, unsigned char y);

// added by me
void __fastcall__ scroll_bg3(unsigned char x, unsigned char y);

// get random number 0..255 or 0..65535
unsigned char __fastcall__ rand8(void);
unsigned int  __fastcall__ rand16(void);

// set random seed
void __fastcall__ set_rand(unsigned int seed);



// when display is enabled, vram access could only be done with this vram update system
// the function sets a pointer to the update buffer that contains data and addresses
// in a special format. It allows to write non-sequental bytes, as well as horizontal or
// vertical nametable sequences.
// buffer pointer could be changed during rendering, but it only takes effect on a new frame
// number of transferred bytes is limited by vblank time
// to disable updates, call this function with NULL pointer

// NOTE: can only update the lower 32kb, and address is in 16bit units!

// the update data format:
//  MSB, LSB, u16 for a non-sequental write
//  MSB|NT_UPD_HORZ, LSB, LEN, [u16s] for a horizontal sequence
//  MSB|NT_UPD_VERT, LSB, LEN, [u16s] for a vertical sequence
//  NT_UPD_EOF to mark end of the buffer

// length of this data should be under 256 bytes

void __fastcall__ set_vram_update(unsigned char *buf);

// all following vram functions only work when display is disabled

// do a series of VRAM writes, the same format as for set_vram_update, but writes done right away
void __fastcall__ flush_vram_update(unsigned char *buf);

// set vram pointer to write operations if you need to write some data to vram
// Note: in 16bit units!
void __fastcall__ vram_adr(unsigned int adr);

// put a 16-bit value at current vram address, works only when rendering is turned off
void __fastcall__ vram_put(unsigned int n);

// fill a block with an u16 at current vram address, works only when rendering is turned off
// length must be divisible by 2
void __fastcall__ vram_fill(unsigned int n, unsigned int len);

// set vram autoincrement, 0 for +1 and not 0 for +32
void __fastcall__ vram_inc(unsigned char n);

// read a block from current address of vram, works only when rendering is turned off
// assumes (vram) start at an even address and size divisible by 2
void __fastcall__ vram_read(unsigned char *dst, unsigned int size);

// write a block to current address of vram, works only when rendering is turned off
// assumes (vram) start at an even address and size divisible by 2
void __fastcall__ vram_write(const unsigned char *src, unsigned int size);

// write a block to current address of vram, works only when rendering is turned off
// assumes (vram) start at an even address and size divisible by 2
void __fastcall__ vram_write_bank(unsigned char bank, const unsigned char *src,
					unsigned int size);

// add a VRAM transfer to the DMA queue, to be sent on the next vblank
// size in bytes, dest addr in 16-bit units
void __fastcall__ dma_queue(unsigned char bank, const unsigned char *src,
					unsigned int dst, unsigned int size);

// unpack RLE data to current address of vram, mostly used for nametables
// assumes start at an even address
void __fastcall__ vram_unrle(const unsigned char *data);

// like memset, but does not return anything
void __fastcall__ memfill(void *dst, unsigned char value, unsigned int len);

// faster bzero
void __fastcall__ memzero(void *dst, unsigned int len);

void __fastcall__ memcpy_fast(void *dest, const void *src, unsigned int n);

// delay for N frames
void __fastcall__ delay(unsigned char frames);

// profiling funcs
void __fastcall__ bstart(void);
void __fastcall__ bend(void);

// get current V line
unsigned short __fastcall__ vline(void);

// maths: u8 * u8 = u16, u16 / u8 = u16
unsigned short __fastcall__ snesmul(const unsigned char a, const unsigned char b);
unsigned short __fastcall__ snesdiv(const unsigned short a, const unsigned char b);
unsigned short __fastcall__ snesmod(const unsigned short a, const unsigned char b);

// banked lz4 decompressor
void __fastcall__ decompress_lz4_bank(const unsigned char srcbank,
					const unsigned char *in,
					const unsigned char dstbank,
					unsigned char * const out,
					const unsigned short outlen);

#define PAD_R			0x10
#define PAD_L			0x20
#define PAD_X			0x40
#define PAD_A			0x80
#define PAD_RIGHT		0x100
#define PAD_LEFT		0x200
#define PAD_DOWN		0x400
#define PAD_UP			0x800
#define PAD_START		0x1000
#define PAD_SELECT		0x2000
#define PAD_Y			0x4000
#define PAD_B			0x8000

#define OAM_FLIP_V		0x80
#define OAM_FLIP_H		0x40

#define MAX(x1,x2)		((x1)<(x2)?(x2):(x1))
#define MIN(x1,x2)		((x1)<(x2)?(x1):(x2))

#define NAMETABLE_A		0x0

#define NULL			0
#define TRUE			1
#define FALSE			0

#define NT_UPD_HORZ		0x40
#define NT_UPD_VERT		0x80
#define NT_UPD_EOF		0xff

// macro to calculate nametable address from X,Y in compile time
#define NTADR_A(x,y)	 	(NAMETABLE_A|(((y)<<5)|(x)))

// macro to get MSB and LSB
#define MSB(x)			(((x)>>8))
#define LSB(x)			(((x)&0xff))

// nametable u16 macro: pal 0-7, prio and flips 0-1, tile 0-1023
#define TILE_ATTR_FULL(pal, prio, flipv, fliph, tile) (tile | ((pal) << 10) | ((prio) << 13) | ((fliph) << 14) | ((flipv) << 15))

// sprite attr macro: pal 0-7, prio 0-3, flips and bank 0-1
#define SPRITE_ATTR_FULL(pal, prio, flipv, fliph, bank) (bank | ((pal) << 1) | ((prio) << 4) | ((fliph) << 6) | ((flipv) << 7))

// color
#define RGB24_TO_VDPCOLOR(color) (((color & 0xff0000) >> 19) | ((color & 0xf800) >> 6) | ((color & 0xf8) << 7))
