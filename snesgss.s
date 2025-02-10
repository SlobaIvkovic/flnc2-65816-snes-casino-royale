.p816
.smart

.export _spc_stream_enable, _spc_stream_stop_flag, _spc_stream_block_repeat
.export _spc_stream_block_size, _spc_stream_block_ptr_prev, _spc_stream_block_ptr
.export _spc_stream_block, _spc_stream_block_src, _spc_stream_block_list
.export _spc_fadespeed, _spc_channelmask, _spc_sfxpan, _spc_sfxvol
.export _spc_sfxchan, _spc_dstaddr, _spc_size, _spc_music_load_adr

.exportzp _sneslib_ptr

.export _spc_command, _spc_load_data, _spc_stereo, _spc_global_volume
.export _spc_channel_volume, _sfx_play, _music_stop, _music_pause, _sound_stop_all
.export spc_stream_update, _music_play

.include "zeropage.inc"
.import popa

;.define DISABLE_SOUND

APU0 = $2140
APU1 = $2141
;APU01 = $2140
APU2 = $2142
APU3 = $2143
APU23 = $2142

PPUNMI = $4200

SCMD_NONE = $00
SCMD_INITIALIZE = $01
SCMD_LOAD = $02
SCMD_STEREO = $03
SCMD_GLOBAL_VOLUME = $04
SCMD_CHANNEL_VOLUME = $05
SCMD_MUSIC_PLAY = $06
SCMD_MUSIC_STOP = $07
SCMD_MUSIC_PAUSE = $08
SCMD_SFX_PLAY = $09
SCMD_STOP_ALL_SOUNDS = $0a
SCMD_STREAM_START = $0b
SCMD_STREAM_STOP = $0c
SCMD_STREAM_SEND = $0d

.macro A8
	sep #$20
.endmacro

.macro A16
	rep #$20
.endmacro

.macro AXY8
	sep #$30
.endmacro

.macro AXY16
	rep #$30
.endmacro

.macro XY8
	sep #$10
.endmacro

.macro XY16
	rep #$10
.endmacro



.segment "ZEROPAGE"

_sneslib_ptr:	.res 4

gss_param:	.res 2
gss_command:	.res 2

brr_stream_src:	.res 4
brr_stream_list: .res 4

.segment "BSS"

_spc_stream_enable: .res 2
_spc_stream_stop_flag: .res 2
_spc_stream_block_repeat: .res 2
_spc_stream_block_size: .res 2
_spc_stream_block_ptr_prev: .res 2
_spc_stream_block_ptr: .res 2
_spc_stream_block: .res 2
_spc_music_load_adr: .res 2

_spc_stream_block_src: .res 4
_spc_stream_block_list: .res 4

_spc_fadespeed: .res 1
_spc_channelmask: .res 1
_spc_sfxpan: .res 1
_spc_sfxvol: .res 1
_spc_sfxchan: .res 1
_spc_dstaddr: .res 2
_spc_size: .res 2

.segment "CODE"

;void spc_load_data(void)

_spc_load_data:

	php
	AXY16

	.ifndef DISABLE_SOUND

	sei

	A8
	lda #$aa
@1:
	cmp f:APU0
	bne @1

	A16
	ldx _spc_size					;size
	lda _spc_dstaddr
	sta f:APU23

	A8
	lda #$01
	sta f:APU1
	lda #$cc
	sta f:APU0

@2:
	cmp f:APU0
	bne @2

	ldy #0

_load_loop:

	xba
	lda [_sneslib_ptr],y
	sta f:APU1
	tya
	sta f:APU0

@3:
	cmp f:APU0
	bne @3

	iny
	dex
	bne _load_loop

	xba
	lda #$00
	xba
	clc
	adc #$02
	A16
	tax

	lda #$0200			;loaded code starting address
	sta f:APU23

	txa
	A8

	xba
	sta f:APU1
	xba
	sta f:APU0

@4:
	cmp f:APU0
	bne @4

	A16
@5:
	lda f:APU0			;wait until SPC700 clears all communication ports, confirming that code has started
	ora f:APU2
	bne @5

	cli					;enable IRQ

	.endif

	plp
	rts



spc_command_asm:

	.ifndef DISABLE_SOUND

	A8

@1:
	lda f:APU0
	bne @1

	A16

	lda gss_param
	sta f:APU23
	lda gss_command
	A8
	xba
	sta f:APU1
	xba
	sta f:APU0

	cmp #SCMD_LOAD	;don't wait acknowledge
	beq @out

@2:
	lda f:APU0
	beq @2

@out:
	.endif

	rts



;void spc_command(unsigned char command);

_spc_command:

	php
	AXY16

	and #$ff ; so it's 8-bit
	sta gss_command
	stz gss_param ; zero, nothing in C needs this

	jsr spc_command_asm

	plp
	rts



;void spc_stereo(unsigned char stereo);

_spc_stereo:

	php
	AXY16

	and #$ff ; so it's 8-bit
	sta gss_param

	lda #SCMD_STEREO
	sta gss_command

	jsr spc_command_asm

	plp
	rts



;void spc_global_volume(unsigned char volume);

_spc_global_volume:

	php
	AXY16

	and #$00ff
	sta gss_param

	lda _spc_fadespeed	;speed
	xba
	and #$ff00
	ora gss_param
	sta gss_param

	lda #SCMD_GLOBAL_VOLUME
	sta gss_command

	jsr spc_command_asm

	plp
	rts



;void spc_channel_volume(unsigned char volume);

_spc_channel_volume:

	php
	AXY16

	and #$00ff
	sta gss_param

	lda _spc_channelmask		;channels
	xba
	and #$ff00
	ora gss_param
	sta gss_param

	lda #SCMD_CHANNEL_VOLUME
	sta gss_command

	jsr spc_command_asm

	plp
	rts



;void music_stop(void);

_music_stop:

	php
	AXY16

	lda #SCMD_MUSIC_STOP
	sta gss_command
	stz gss_param

	jsr spc_command_asm

	plp
	rts



;void music_pause(unsigned char pause);

_music_pause:

	php
	AXY16

	and #$ff ; so it's 8-bit
	sta gss_param

	lda #SCMD_MUSIC_PAUSE
	sta gss_command

	jsr spc_command_asm

	plp
	rts



;void sound_stop_all(void);

_sound_stop_all:

	php
	AXY16

	lda #SCMD_STOP_ALL_SOUNDS
	sta gss_command
	stz gss_param

	jsr spc_command_asm

	plp
	rts



;void sfx_play(unsigned char sfx);

_sfx_play:

	php
	AXY16

	and #$00ff
	sta gss_param

	lda _spc_sfxpan
	xba
	and #$ff00
	ora gss_param
	sta gss_param

	lda _spc_sfxvol
	xba
	and #$ff00
	sta gss_command

	lda _spc_sfxchan
	asl a
	asl a
	asl a
	asl a
	and #$00f0
	ora #SCMD_SFX_PLAY
	ora gss_command
	sta gss_command

	jsr spc_command_asm

	plp
	rts

;void spc_stream_update(void);

spc_stream_update:

	php
	AXY16

_stream_update_loop:

	lda _spc_stream_enable
	bne _stream_update_loop1

_stream_update_done:

	plp
	rts

_stream_update_loop1:

	lda _spc_stream_stop_flag
	beq _stream_update_play

_test2:

	stz _spc_stream_enable

	A8

_stream_wait7:

	lda f:APU0
	bne _stream_wait7

	lda #SCMD_STREAM_STOP
	sta f:APU0

_stream_wait8:

	lda f:APU0
	beq _stream_wait8

	bra _stream_update_done

_stream_update_play:

	A8

_stream_wait1:

	lda f:APU0
	bne _stream_wait1

	lda #SCMD_STREAM_SEND
	sta f:APU0

_stream_wait2:

	lda f:APU0
	beq _stream_wait2

_stream_wait3:

	lda f:APU0
	bne _stream_wait3

	lda f:APU2
	beq _stream_update_done

	A16

	lda _spc_stream_block_src+0
	sta brr_stream_src+0
	lda _spc_stream_block_src+2
	sta brr_stream_src+2

	lda _spc_stream_block_repeat
	beq _norepeat

	dec _spc_stream_block_repeat
	ldy _spc_stream_block_ptr_prev

	bra _stream_update_send

_norepeat:

	ldy _spc_stream_block_ptr

	A8
	lda [brr_stream_src],y
	iny
	A16
	and #$00ff
	lsr a
	bcc _stream_update_repeat

	sta _spc_stream_block_repeat

	sty _spc_stream_block_ptr
	cpy _spc_stream_block_size
	bcc _stream_update_nojump1

	inc _spc_stream_block
	jsr _stream_set_block

_stream_update_nojump1:

	ldy _spc_stream_block_ptr_prev
	bra _stream_update_send

_stream_update_repeat:

	lda _spc_stream_block_ptr
	sta _spc_stream_block_ptr_prev
	tay
	clc
	adc #9
	sta _spc_stream_block_ptr
	cmp _spc_stream_block_size
	bcc _stream_update_send

	inc _spc_stream_block

	phy
	jsr _stream_set_block
	ply

_stream_update_send:

	A8

	lda [brr_stream_src],y
	iny
	sta f:APU1
	lda [brr_stream_src],y
	iny
	sta f:APU2
	lda [brr_stream_src],y
	iny
	sta f:APU3

	lda #1
	sta f:APU0
	inc a

_stream_wait5:

	cmp f:APU0
	bne _stream_wait5

	lda [brr_stream_src],y
	iny
	sta f:APU1
	lda [brr_stream_src],y
	iny
	sta f:APU2
	lda [brr_stream_src],y
	iny
	sta f:APU3

	lda #2
	sta f:APU0
	inc a

_stream_wait6:

	cmp f:APU0
	bne _stream_wait6

	lda [brr_stream_src],y
	iny
	sta f:APU1
	lda [brr_stream_src],y
	iny
	sta f:APU2
	lda [brr_stream_src],y
	iny
	sta f:APU3

	lda #3
	sta f:APU0

	jmp _stream_update_loop


_stream_set_block:

	lda _spc_stream_block_list+0
	sta brr_stream_list+0
	lda _spc_stream_block_list+2
	sta brr_stream_list+2

	lda _spc_stream_block
	asl a
	asl a
	tay

	lda [brr_stream_list],y
	iny
	iny
	sta _spc_stream_block_src+0

	lda [brr_stream_list],y
	iny
	iny
	sta _spc_stream_block_src+2

	ora _spc_stream_block_src+0
	bne _noeof

	lda #1
	sta _spc_stream_stop_flag

_noeof:

	lda _spc_stream_block_src+0
	sta brr_stream_list+0
	lda _spc_stream_block_src+2
	sta brr_stream_list+2

	ldy #0
	lda [brr_stream_list],y

	sta _spc_stream_block_size

	stz _spc_stream_block_repeat

	lda #2
	sta _spc_stream_block_ptr
	sta _spc_stream_block_ptr_prev

	rts

; void music_play(unsigned char bank, const unsigned char *data);
_music_play:
	sta _sneslib_ptr
	stx _sneslib_ptr+1
	jsr popa
	sta _sneslib_ptr+2

	stz PPUNMI

	AXY16

	lda [_sneslib_ptr]
	sta _spc_size

	lda #SCMD_MUSIC_STOP
	jsr _spc_command
	lda #SCMD_LOAD
	jsr _spc_command

	inc _sneslib_ptr
	inc _sneslib_ptr

	lda _spc_music_load_adr
	sta _spc_dstaddr
	jsr _spc_load_data

	lda #SCMD_MUSIC_PLAY
	jsr _spc_command

	AXY8

	lda #$81
	sta PPUNMI

	rts
