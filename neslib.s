.p816
.smart

	.import push0,popa,popax,_main,OAM_BUF,incsp5
	.importzp RAND_SEED, HASMULTITAP
	.exportzp PPU_MASK_VAR

	.include "zeropage.inc"
	.include "snes.sinc"

DMA_MAX = 10

.segment "BSS"

PAL_BUF:		.res 512
DMA_BUF:		.res 7 * DMA_MAX
DMA_OFF:		.res 1

.segment "ZEROPAGE"

FRAME_CNT1: 		.res 1
VRAM_UPDATE: 		.res 1
NAME_UPD_ADR: 		.res 2
NAME_UPD_ENABLE: 	.res 1
PAL_UPDATE: 		.res 1
SCROLL_X: 		.res 1
SCROLL_Y: 		.res 1
;added by me
SCROLL3_Y: 		.res 1
SCROLL3_X: 		.res 1
;
PAD_STATE: 		.res 10
PAD_STATEP: 		.res 10
PAD_STATET: 		.res 10
PPU_MASK_VAR: 		.res 1

TEMP: 			.res 11

PAD_BUF			=TEMP+1

PTR			=TEMP	;word
LEN			=TEMP+2	;word
EVEN			=TEMP+4
SCRX			=TEMP+5
SCRY			=TEMP+6
SRC			=TEMP+7	;word
DST			=TEMP+9	;word

RLE_LOW			=TEMP
RLE_HIGH		=TEMP+1
RLE_TAG			=TEMP+2
RLE_BYTE		=TEMP+3

.segment "CODE"
;NES hardware-dependent functions by Shiru (shiru@mail.ru)
;SNES port by Lauri Kasanen

	.export _pal_all,_pal_col,_pal_clear
	.export _pal_bright
	.export _ppu_off,_ppu_on_all,_ppu_system
	.export _oam_clear,_oam_big,_oam_spr,_oam_spr_x,_oam_meta_spr,_oam_hide_rest
	.export _oam_meta_spr_x
	.export _ppu_wait_nmi
	.export _scroll, _scroll_bg3 ; added by me
	.export _vram_read,_vram_write,_vram_write_bank
	.export _dma_queue
	.export _pad_trigger,_pad_state,_multitap_connected
	.export _rand8,_rand16,_set_rand
	.export _vram_adr,_vram_put,_vram_fill,_vram_inc,_vram_unrle
	.export _set_vram_update,_flush_vram_update
	.export _memfill,_memzero,_memcpy_fast,_delay
	.export _nesclock
	.export nmi_handler
	.export _bstart, _bend
	.export _vline
	.export _snesmul, _snesdiv, _snesmod



;NMI handler

nmi_handler:
	phb

	pha
	phx
	phy

	php

	setaxy8

	; set our data bank to 80 for access to both registers and low ram
	lda #$80
	pha
	plb


	lda <PPU_MASK_VAR	;if rendering is disabled, do not access the VRAM at all
	and #$80
	beq @doUpdate
	jmp @skipAll

@doUpdate:

	; OAM
	lda #(DMA_00) ; write from cpu to io, increasing, in 8bit units
	sta DMAMODE
	lda #$04 ; to oam
	sta DMAPPUREG
	lda #<OAM_BUF
	sta DMAADDR
	lda #>OAM_BUF
	sta DMAADDRHI
	lda #$80 ; from the default lorom bank (with access to low ram)
	sta DMAADDRBANK
	lda #<(512 + 32)
	sta DMALEN
	lda #>(512 + 32)
	sta DMALENHI
	lda #1
	sta MDMAEN

	lda <PAL_UPDATE		;update palette if needed
	bne @updPal
	jmp @updVRAM

@updPal:

	ldx #0
	stx <PAL_UPDATE

	stz CGADD

	lda #(DMA_00) ; write from cpu to io, increasing, in 8bit units
	sta DMAMODE
	lda #$22 ; to cgram
	sta DMAPPUREG
	lda #<PAL_BUF
	sta DMAADDR
	lda #>PAL_BUF
	sta DMAADDRHI
	lda #$80 ; from the default lorom bank (with access to low ram)
	sta DMAADDRBANK
	lda #<(512)
	sta DMALEN
	lda #>(512)
	sta DMALENHI
	lda #1
	sta MDMAEN

@updVRAM:

	lda <VRAM_UPDATE
	beq @updCtrls
	lda #0
	sta <VRAM_UPDATE

	jsr dma_queue_run

	lda <NAME_UPD_ENABLE
	beq @updCtrls

	jsr _flush_vram_update_nmi

@updCtrls:
	lda PAD_STATE
	sta PAD_STATEP
	lda PAD_STATE+1
	sta PAD_STATEP+1
	lda PAD_STATE+2
	sta PAD_STATEP+2
	lda PAD_STATE+3
	sta PAD_STATEP+3

	lda JOY1CUR
	sta PAD_STATE
	eor PAD_STATEP
	and PAD_STATE
	sta PAD_STATET
	lda JOY1CUR+1
	sta PAD_STATE+1
	eor PAD_STATEP+1
	and PAD_STATE+1
	sta PAD_STATET+1

	lda JOY2CUR
	sta PAD_STATE+2
	eor PAD_STATEP+2
	and PAD_STATE+2
	sta PAD_STATET+2
	lda JOY2CUR+1
	sta PAD_STATE+3
	eor PAD_STATEP+3
	and PAD_STATE+3
	sta PAD_STATET+3

	lda HASMULTITAP
	beq @skipUpd

	; Read the other three controllers
	seta16

	lda PAD_STATE+4
	sta PAD_STATEP+4
	lda PAD_STATE+6
	sta PAD_STATEP+6
	lda PAD_STATE+8
	sta PAD_STATEP+8

	lda JOY2B1CUR
	sta PAD_STATE+4
	eor PAD_STATEP+4
	and PAD_STATE+4
	sta PAD_STATET+4

	; controllers 4 and 5 need manual reading, autoread only does the first 3
	ldx #0
	stx WRIO

	stz PAD_STATE+6
	stz PAD_STATE+8

	ldy #16
@multiread:
	lda JOY1

	lsr
	rol PAD_STATE+6
	lsr
	rol PAD_STATE+8

	dey
	bne @multiread

	ldx #$80
	stx WRIO

	lda PAD_STATE+6
	eor PAD_STATEP+6
	and PAD_STATE+6
	sta PAD_STATET+6

	lda PAD_STATE+8
	eor PAD_STATEP+8
	and PAD_STATE+8
	sta PAD_STATET+8

	seta8


@skipUpd:

	lda <SCROLL_X
	sta BG1HOFS
	lda #0
	sta BG1HOFS
	lda <SCROLL_Y
	sta BG1VOFS
	lda #0
	sta BG1VOFS

; added by me
	lda <SCROLL3_X
	sta BG3HOFS
	lda #0
	sta BG3HOFS
	lda <SCROLL3_Y
	sta BG3VOFS
	lda #0
	sta BG3VOFS
;

@skipAll:

	lda <PPU_MASK_VAR
	sta PPUBRIGHT

	inc <FRAME_CNT1

;	jsr FamiToneUpdate

	plp

	ply
	plx
	pla

	plb

	rti



;void __fastcall__ pal_all(const unsigned char idx, const unsigned short *data);

_pal_all:

	sta <SRC
	stx <SRC+1

	seta16
	lda #0
	seta8

	jsr popa

	seta16

	asl a
	asl a
	asl a
	asl a
	asl a

	clc
	adc #PAL_BUF

	sta DST

	lda #32
	sta LEN

	seta8

	inc <PAL_UPDATE

	jmp memcpy


;void __fastcall__ pal_col(unsigned char index,unsigned short color);

_pal_col:

	sta PTR
	stx PTR+1

	seta16
	lda #0
	seta8

	jsr popa
	setaxy16
	asl a
	tax

	lda PTR
	sta PAL_BUF,x

	setaxy8
	inc <PAL_UPDATE
	rts



;void __fastcall__ pal_clear(void);

_pal_clear:

	ldx #0
@1:
	stz PAL_BUF,x
	stz PAL_BUF+256,x
	inx
	bne @1
	inc <PAL_UPDATE
	rts


;void __fastcall__ pal_bright(unsigned char bright);

_pal_bright:
	and #15
	sta TEMP
	lda PPU_MASK_VAR
	and #$80
	ora TEMP
	sta PPU_MASK_VAR
	rts


;void __fastcall__ ppu_off(void);

_ppu_off:

	lda <PPU_MASK_VAR
	ora #$80
	sta <PPU_MASK_VAR
	jmp _ppu_wait_nmi



;void __fastcall__ ppu_on_all(void);

_ppu_on_all:

	lda <PPU_MASK_VAR
	and #15
	sta <PPU_MASK_VAR
	jmp _ppu_wait_nmi

;unsigned char __fastcall__ ppu_system(void);

_ppu_system:

	lda f:PPUSTATUS2
	eor #$ff
	and #$10
	ldx #$00
	rts

_bstart:
	lda #8
	sta f:PPUBRIGHT
	rts

_bend:
	lda #15
	sta f:PPUBRIGHT
	rts

;void __fastcall__ oam_clear(void);

_oam_clear:

	setxy16

	ldx #1
	lda #224
@1:
	sta OAM_BUF,x
	inx
	inx
	inx
	inx
	cpx #512
	bcc @1

	; and the high table
	ldx #0
@2:
	stz OAM_BUF+512,x
	inx
	cpx #32
	bcc @2

	setxy8

	rts

;void __fastcall__ oam_big(unsigned char sprid);
_oam_big:
	lsr
	lsr
	tay
	lsr
	lsr
	sta TEMP ; byte in high oam

	tya
	and #3
	asl
	ina
	tay
	lda Nthbit,y
	sta TEMP+1

	ldy TEMP
	lda OAM_BUF+512,y
	ora TEMP+1
	sta OAM_BUF+512,y

	rts

;unsigned char __fastcall__ oam_spr_x(int x,unsigned char y,unsigned char chrnum,unsigned char attr,unsigned char sprid);

_oam_spr_x:

	tax

	ldy #0		;five popa calls replacement
	lda (sp),y
	iny
	sta OAM_BUF+3,x
	lda (sp),y
	iny
	sta OAM_BUF+2,x
	lda (sp),y
	iny
	sta OAM_BUF+1,x
	lda (sp),y
	iny
	sta OAM_BUF+0,x

	lda (sp),y
	beq @posX

	txa
	lsr
	lsr
	tay
	lsr
	lsr
	sta TEMP ; byte in high oam

	tya
	and #3
	asl
	tay
	lda Nthbit,y
	sta TEMP+1

	ldy TEMP
	lda OAM_BUF+512,y
	ora TEMP+1
	sta OAM_BUF+512,y

@posX:
	lda <sp
	clc
	adc #5
	sta <sp
	bcc @1
	inc <sp+1

@1:

	txa
	clc
	adc #4
	ldx #$00
	rts

;unsigned char __fastcall__ oam_spr(unsigned char x,unsigned char y,unsigned char chrnum,unsigned char attr,unsigned char sprid);

_oam_spr:

	tax

	ldy #0		;four popa calls replacement
	lda (sp),y
	iny
	sta OAM_BUF+3,x
	lda (sp),y
	iny
	sta OAM_BUF+2,x
	lda (sp),y
	iny
	sta OAM_BUF+1,x
	lda (sp),y
	sta OAM_BUF+0,x

	lda <sp
	clc
	adc #4
	sta <sp
	bcc @1
	inc <sp+1

@1:

	txa
	clc
	adc #4
	ldx #$00
	rts



;unsigned char __fastcall__ oam_meta_spr(unsigned char x,unsigned char y,unsigned char sprid,const unsigned char *data);

_oam_meta_spr:

	sta <PTR
	stx <PTR+1

	ldy #2		;three popa calls replacement, performed in reversed order
	lda (sp),y
	dey
	sta <SCRX
	lda (sp),y
	dey
	sta <SCRY
	lda (sp),y
	tax

@1:

	lda (PTR),y		;x offset
	cmp #$80
	beq @2
	iny
	clc
	adc <SCRX
	sta OAM_BUF+0,x
	lda (PTR),y		;y offset
	iny
	clc
	adc <SCRY
	sta OAM_BUF+1,x
	lda (PTR),y		;tile
	iny
	sta OAM_BUF+2,x
	lda (PTR),y		;attribute
	iny
	sta OAM_BUF+3,x
	inx
	inx
	inx
	inx
	jmp @1

@2:

	lda <sp
	adc #2			;carry is always set here, so it adds 3
	sta <sp
	bcc @3
	inc <sp+1

@3:

	txa
	ldx #$00
	rts


;unsigned char __fastcall__ oam_meta_spr_x(int x,unsigned char y,unsigned char sprid,const unsigned char *data);

_oam_meta_spr_x:

	sta <PTR
	stx <PTR+1

	ldy #3		;four popa calls replacement, performed in reversed order
	lda (sp),y
	dey
	sta <SRC+1
	lda (sp),y
	dey
	sta <SRC
	lda (sp),y
	dey
	sta <SCRY
	lda (sp),y
	tax

@1:

	lda (PTR),y		;x offset
	cmp #$80
	beq @2
	iny

	; sign-extend
	cmp #$80
	bcc @pos
@neg:
	xba
	lda #$ff
	xba
	bra @extended
@pos:
	xba
	lda #$0
	xba

@extended:
	seta16

	clc
	adc <SRC

	seta8

	sta OAM_BUF+0,x

	xba
	beq @posX

	sty DST

	txa
	lsr
	lsr
	tay
	lsr
	lsr
	sta LEN ; byte in high oam

	tya
	and #3
	asl
	tay
	lda Nthbit,y
	sta LEN+1

	ldy LEN
	lda OAM_BUF+512,y
	ora LEN+1
	sta OAM_BUF+512,y

	ldy DST

@posX:
	lda (PTR),y		;y offset
	iny
	clc
	adc <SCRY
	sta OAM_BUF+1,x
	lda (PTR),y		;tile
	iny
	sta OAM_BUF+2,x
	lda (PTR),y		;attribute
	iny
	sta OAM_BUF+3,x
	inx
	inx
	inx
	inx
	jmp @1

@2:

	lda <sp
	adc #3			;carry is always set here, so it adds 4
	sta <sp
	bcc @3
	inc <sp+1

@3:

	txa
	ldx #$00
	rts



;void __fastcall__ oam_hide_rest(unsigned char sprid);

_oam_hide_rest:

	tax
	setxy16
	inx
	lda #224

@1:

	sta OAM_BUF,x
	inx
	inx
	inx
	inx
	cpx #512
	bcc @1

	setxy8
	rts


;void __fastcall__ ppu_wait_nmi(void);

_ppu_wait_nmi:

	lda #1
	sta <VRAM_UPDATE
	lda <FRAME_CNT1
@1:

	cmp <FRAME_CNT1
	beq @1
	rts



;void __fastcall__ vram_unrle(const unsigned char *data);

_vram_unrle:

	stz EVEN

	tay
	stx <RLE_HIGH
	lda #0
	sta <RLE_LOW

	lda (RLE_LOW),y
	sta <RLE_TAG
	iny
	bne @1
	inc <RLE_HIGH

@1:

	lda (RLE_LOW),y
	iny
	bne @11
	inc <RLE_HIGH

@11:

	cmp <RLE_TAG
	beq @2
	lsr EVEN
	bcs @firstodd
	sta f:VMDATAL
	inc EVEN
	jmp @firstdone
@firstodd:
	sta f:VMDATAH
@firstdone:
	sta <RLE_BYTE
	cmp <RLE_TAG
	bne @1

@2:

	lda (RLE_LOW),y
	beq @4
	iny
	bne @21
	inc <RLE_HIGH

@21:

	tax
	lda <RLE_BYTE

@3:
	lsr EVEN
	bcs @secondodd
	sta f:VMDATAL
	inc EVEN
	jmp @seconddone
@secondodd:
	sta f:VMDATAH
@seconddone:

	dex
	bne @3
	beq @1

@4:

	rts



;void __fastcall__ scroll(unsigned char x,unsigned char y);

_scroll:

	sta SCROLL_Y
	jsr popa
	sta SCROLL_X
	rts

;void __fastcall__ scroll_bg3(unsigned char x,unsigned char y);

_scroll_bg3:

	sta SCROLL3_Y
	jsr popa
	sta SCROLL3_X
	rts

;void __fastcall__ vram_read(unsigned char *dst,unsigned int size);

_vram_read:

	sta <TEMP
	stx <TEMP+1

	jsr popax
	sta <TEMP+2
	stx <TEMP+3

	setaxy16

	lsr TEMP
	ldx TEMP

	lda f:RDVRAML

	ldy #0

@1:

	lda f:RDVRAML
	sta (TEMP+2),y
	iny
	iny

	dex
	bne @1

	setaxy8

	rts



;void __fastcall__ vram_write(unsigned char *src,unsigned int size);

_vram_write:

	sta <TEMP
	stx <TEMP+1

	jsr popax
	sta <TEMP+2
	stx <TEMP+3

	setaxy16

	lsr TEMP
	ldx TEMP

	ldy #0

@1:

	lda (TEMP+2),y
	sta f:VMDATAL
	iny
	iny

	dex
	bne @1

	setaxy8

	rts

;void __fastcall__ vram_write_bank(u8 bank, unsigned char *src,unsigned int size);

_vram_write_bank:

	sta <TEMP
	stx <TEMP+1

	jsr popax
	sta <TEMP+2
	stx <TEMP+3

	jsr popa
	tax

	lda #(DMA_01) ; write from cpu to io, increasing, in 16bit units
	sta f:DMAMODE
	lda #$18 ; to vram
	sta f:DMAPPUREG
	lda TEMP+2
	sta f:DMAADDR
	lda TEMP+3
	sta f:DMAADDRHI
	txa
	sta f:DMAADDRBANK
	lda TEMP
	sta f:DMALEN
	lda TEMP+1
	sta f:DMALENHI
	lda #1
	sta f:MDMAEN

	rts

; void __fastcall__ dma_queue(unsigned char bank, const unsigned char *src,
;                                        unsigned int dst, unsigned int size);
_dma_queue:

	ldy DMA_OFF
	cpy #DMA_MAX * 7
	bcc @ok
	jmp incsp5
@ok:
	sta DMA_BUF,y
	iny
	txa
	sta DMA_BUF,y
	iny
	sty DMA_OFF

	jsr popax

	; dst
	ldy DMA_OFF
	sta DMA_BUF,y
	iny
	txa
	sta DMA_BUF,y
	iny
	sty DMA_OFF

	jsr popax

	; src
	ldy DMA_OFF
	sta DMA_BUF,y
	iny
	txa
	sta DMA_BUF,y
	iny
	sty DMA_OFF

	jsr popa

	; bank
	ldy DMA_OFF
	sta DMA_BUF,y
	iny

	sty DMA_OFF

	rts

; internal DMA queue func, called during NMI to empty the queue
dma_queue_run:

	ldy #0
@loop:	cpy DMA_OFF
	bcs @out

	lda DMA_BUF+2,y
	sta f:VMADDL
	lda DMA_BUF+3,y
	sta f:VMADDH

	lda #(DMA_01) ; write from cpu to io, increasing, in 16bit units
	sta f:DMAMODE
	lda #$18 ; to vram
	sta f:DMAPPUREG
	lda DMA_BUF+4,y
	sta f:DMAADDR
	lda DMA_BUF+5,y
	sta f:DMAADDRHI
	lda DMA_BUF+6,y
	sta f:DMAADDRBANK
	lda DMA_BUF,y
	sta f:DMALEN
	lda DMA_BUF+1,y
	sta f:DMALENHI
	lda #1
	sta f:MDMAEN

	tya
	clc
	adc #7
	tay

	bra @loop

@out:
	stz DMA_OFF

	rts

;unsigned char __fastcall__ pad_trigger(unsigned char pad);

_pad_trigger:

	asl a
	tay
	lda <PAD_STATET,y
	iny
	ldx <PAD_STATET,y
	rts

;unsigned char __fastcall__ multitap_connected(void);

_multitap_connected:
	lda HASMULTITAP
	ldx #0
	rts


;unsigned char __fastcall__ pad_state(unsigned char pad);

_pad_state:

	asl a
	tay
	lda <PAD_STATE,y
	iny
	ldx <PAD_STATE,y
	rts



;unsigned char __fastcall__ rand8(void);
;Galois random generator, found somewhere
;out: A random number 0..255

rand1:

	lda <RAND_SEED
	asl a
	bcc @1
	eor #$cf

@1:

	sta <RAND_SEED
	rts

rand2:

	lda <RAND_SEED+1
	asl a
	bcc @1
	eor #$d7

@1:

	sta <RAND_SEED+1
	rts

_rand8:

	jsr rand1
	jsr rand2
	adc <RAND_SEED
	ldx #$00
	rts



;unsigned int __fastcall__ rand16(void);

_rand16:

	jsr rand1
	tax
	jsr rand2

	rts


;void __fastcall__ set_rand(unsigned char seed);

_set_rand:

	sta <RAND_SEED
	stx <RAND_SEED+1

	rts



;void __fastcall__ set_vram_update(unsigned char *buf);

_set_vram_update:

	sta <NAME_UPD_ADR+0
	stx <NAME_UPD_ADR+1
	ora <NAME_UPD_ADR+1
	sta <NAME_UPD_ENABLE

	rts



;void __fastcall__ flush_vram_update(unsigned char *buf);

_flush_vram_update:

	sta <NAME_UPD_ADR+0
	stx <NAME_UPD_ADR+1

_flush_vram_update_nmi:

	ldy #0

@updName:

	lda (NAME_UPD_ADR),y
	iny
	cmp #$40				;is it a non-sequental write?
	bcs @updNotSeq
	sta f:VMADDH
	lda (NAME_UPD_ADR),y
	iny
	sta f:VMADDL
	lda (NAME_UPD_ADR),y
	iny
	sta f:VMDATAL
	lda (NAME_UPD_ADR),y
	iny
	sta f:VMDATAH
	jmp @updName

@updNotSeq:

	tax
	cpx #$80				;is it a horizontal or vertical sequence?
	bcc @updHorzSeq
	cpx #$ff				;is it end of the update?
	beq @updDone

@updVertSeq:

	lda #$81
	bra @updNameSeq

@updHorzSeq:

	lda #$80

@updNameSeq:

	sta f:VMAIN

	txa
	and #$3f
	sta f:VMADDH
	lda (NAME_UPD_ADR),y
	iny
	sta f:VMADDL
	lda (NAME_UPD_ADR),y
	iny
	tax

@updNameLoop:

	lda (NAME_UPD_ADR),y
	iny
	sta f:VMDATAL
	lda (NAME_UPD_ADR),y
	iny
	sta f:VMDATAH
	dex
	bne @updNameLoop

	lda #$80
	sta f:VMAIN

	jmp @updName

@updDone:

	rts



;void __fastcall__ vram_adr(unsigned int adr);

_vram_adr:

	sta f:VMADDL
	txa
	sta f:VMADDH

	rts



;void __fastcall__ vram_put(unsigned int n);

_vram_put:

	sta f:VMDATAL
	txa
	sta f:VMDATAH

	rts



;void __fastcall__ vram_fill(unsigned int n,unsigned int len);

_vram_fill:

	sta <LEN
	stx <LEN+1
	jsr popax

	sta PTR
	stx PTR+1

	setaxy16

	lda PTR
	lsr LEN
	ldx LEN

@1:
	sta f:VMDATAL
	dex
	bne @1

	setaxy8

	rts



;void __fastcall__ vram_inc(unsigned char n);

_vram_inc:

	tax
	beq @1
	lda #$81
	bra @2
@1:
	lda #$80
@2:
	sta f:VMAIN

	rts



;void __fastcall__ memfill(void *dst,unsigned char value,unsigned int len);

_memfill:

	sta LEN
	stx LEN+1
	and #1
	sta EVEN
	jsr popa
	sta SRC
	sta SRC+1
	bra memfill_internal

;void __fastcall__ memzero(void *dst, unsigned int len);
_memzero:
	sta LEN
	stx LEN+1
	and #1
	sta EVEN
	stz SRC
	stz SRC+1
memfill_internal:
	jsr popax
	sta PTR
	stx PTR+1

	setaxy16

	lda LEN
	and #$fffe
	sta LEN
	ldy #0
	lda SRC
@1:	cpy LEN
	bcs @2

	sta (PTR),y

	iny
	iny
	bra @1
@2:
	seta8

	; Need to fill the odd byte?
	lda EVEN
	beq @3

	lda SRC
	sta (PTR),y

@3:
	setxy8

	rts

;void __fastcall__ memcpy_fast(void *dest, const void *src, unsigned int n);
_memcpy_fast:
	sta LEN
	stx LEN+1

	jsr popax
	sta SRC
	stx SRC+1

	jsr popax
	sta DST
	stx DST+1
memcpy:
	phb
	setaxy16

	lda LEN
	dea

	ldx SRC
	ldy DST
; Note the fixed bank!
	mvn #$83, #$83

	setaxy8
	plb
	rts

;unsigned char __fastcall__ nesclock(void);

_nesclock:
	lda <FRAME_CNT1
	ldx #$00
	rts

;unsigned short __fastcall__ snesmul(const unsigned char a, const unsigned char b);
_snesmul:
	sta CPUMCAND
	jsr popa
	sta CPUMUL

	; wait 8 cycles, nop is 2
	nop
	nop
	nop
	nop

	lda CPUPROD
	ldx CPUPRODHI
	rts

;unsigned short __fastcall__ snesdiv(const unsigned short a, const unsigned char b);
_snesdiv:
	sta TEMP
	jsr popax
	sta CPUNUM
	stx CPUNUMHI
	lda TEMP
	sta CPUDEN

	; wait 16 cycles
	ldx #3
@wait:
	dex
	bne @wait

	lda CPUQUOT
	ldx CPUQUOTHI
	rts

;unsigned short __fastcall__ snesmod(const unsigned short a, const unsigned char b);
_snesmod:
	jsr _snesdiv
	lda CPUREM
	ldx CPUREMHI
	rts

;unsigned short __fastcall__ vline(void);
_vline:
	lda GETXY
	ldy YCOORD
	lda YCOORD
	and #1 ; the rest of the high byte is garbage
	tax
	tya
	rts

;void __fastcall__ delay(unsigned char frames);

_delay:

	tax

@1:

	jsr _ppu_wait_nmi
	dex
	bne @1

	rts

.segment "RODATA"
Nthbit:
	.byte 1 << 0
	.byte 1 << 1
	.byte 1 << 2
	.byte 1 << 3
	.byte 1 << 4
	.byte 1 << 5
	.byte 1 << 6
	.byte 1 << 7
