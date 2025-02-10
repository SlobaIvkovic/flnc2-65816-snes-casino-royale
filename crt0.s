.p816
.export __STARTUP__ : absolute = 1 ; Mark as startup
.export resetstub, irq_handler, OAM_BUF
.import _main, nmi_handler, __ZEROPAGE_RUN__, __BSS_SIZE__, __BSS_START__
.import spc_code, _spc_music_load_adr, _spc_stream_enable, _spc_dstaddr, _spc_size
.import spc_code2, _spc_load_data, _spc_command
.import __CODE_SIZE__, __CODE_RUN__, __CODE_LOAD__
.exportzp RAND_SEED, _oam_off, HASMULTITAP
.importzp PPU_MASK_VAR, _sneslib_ptr, _curbank

.smart

.include "snes.sinc"
.include "zeropage.inc"

SCMD_INITIALIZE = $01
SCMD_LOAD = $02

; Mask off low byte to allow use of $000000-$00000F as local variables
ZEROPAGE_BASE		= __ZEROPAGE_RUN__ & $FF00

; Make sure these conform to the linker script (e.g. lorom256.cfg).
STACK_BASE			= $0100
STACK_SIZE			= $0100
LAST_STACK_ADDR = STACK_BASE + STACK_SIZE - 1

PPU_BASE				= $2100
CPUIO_BASE			= $4200

; MMIO is mirrored into $21xx, $42xx, and $43xx of all banks $00-$3F
; and $80-$BF.	To make it work no matter the current data bank, we
; can use a long address in a nonzero bank.
; Bit 0 of MEMSEL enables fast ROM access above $808000.
LONGMEMSEL					= $80420D

; Bit 4 of the byte at $FFD5 in the cartridge header specifies
; whether a game should be manufactured with slow or fast ROM.
; The init code enables fast ROM if this bit is true.
map_mode				= $00FFD5

; A tiny stub in bank $00 needs to set interrupt priority to 1,
; leave 6502 emulation mode, and long jump to the rest of init code
; in another bank. This should set 16-bit mode, turn off decimal
; mode, set the stack pointer, load a predictable state into writable
; MMIO ports of the S-PPU and S-CPU, and set the direct page base.
; For explanation of the values that this writes, see docs/init.txt
;
; For advanced users: Long stretches of STZ are a useful place to
; shuffle code when watermarking your binary.

.segment "BOOTCODE"
.proc resetstub
	sei								 ; turn off IRQs
	clc
	xce								 ; turn off 6502 emulation mode
	cld								 ; turn off decimal ADC/SBC
	jml reset_fastrom  ; Bank $00 is not fast, but its mirror $80 is
.endproc

.proc irq_handler
	rti
.endproc

.segment "ZEROPAGE"

RAND_SEED:	.res 2
_oam_off: .res 2

HASMULTITAP: .res 1

.segment "BSS"

OAM_BUF:	.res 512+32

.segment "BOOTCODE"
.proc reset_fastrom
	rep #$30				 ; 16-bit AXY
	ldx #LAST_STACK_ADDR
	txs							 ; set the stack pointer

	; Initialize the CPU I/O registers to predictable values
	lda #CPUIO_BASE
	tad							 ; temporarily move direct page to S-CPU I/O area
	lda #$FF00
	sta $00			; disable NMI and HVIRQ; don't drive controller port pin 6
	stz $02			; clear multiplier factors
	stz $04			; clear dividend
	stz $06			; clear divisor and low byte of hcount
	stz $08			; clear high bit of hcount and low byte of vcount
	stz $0A			; clear high bit of vcount and disable DMA copy
	stz $0C			; disable HDMA and fast ROM

	; Initialize the PPU registers to predictable values
	lda #PPU_BASE
	tad							 ; temporarily move direct page to PPU I/O area

	; first clear the regs that take a 16-bit write
	lda #$0080
	sta $00			; Forced blank, brightness 0, sprite size 8/16 from VRAM $0000
	stz $02			; OAM address = 0
	stz $05			; BG mode 0, no mosaic
	stz $07			; BG 1-2 map 32x32 from VRAM $0000
	stz $09			; BG 3-4 map 32x32 from VRAM $0000
	stz $0B			; BG tiles from $0000
	stz $16			; VRAM address $0000
	stz $23			; disable BG window
	stz $26			; clear window 1 x range
	stz $28			; clear window 2 x range
	stz $2A			; clear window mask logic
	stz $2C			; disable all layers on main and sub
	stz $2E			; disable all layers on main and sub in window
	ldx #$0030
	stx $30			; disable color math and mode 3/4/7 direct color
	ldy #$00E0
	sty $32			; clear RGB components of COLDATA; disable interlace+pseudo hires

	; now clear the regs that need 8-bit writes
	sep #$20
	sta $15			; still $80: add 1 to VRAM pointer after high byte write
	stz $1A			; enable mode 7 wrapping and disable flipping
	stz $21			; set CGRAM address to color 0
	stz $25			; disable obj and math window

	; The scroll registers $210D-$2114 need double 8-bit writes
	.repeat 8, I
		stz $0D+I
		stz $0D+I
	.endrepeat

	; As do the mode 7 registers, which we set to the identity matrix
	; [ $0100  $0000 ]
	; [ $0000  $0100 ]
	lda #$01
	stz $1B
	sta $1B
	stz $1C
	stz $1C
	stz $1D
	stz $1D
	stz $1E
	sta $1E
	stz $1F
	stz $1F
	stz $20
	stz $20

	; Set fast ROM if the internal header so requests
	lda f:map_mode
	and #$10
	beq not_fastrom
		lda #$01
	not_fastrom:
	sta LONGMEMSEL

	rep #$20
	lda #ZEROPAGE_BASE
	tad									; return direct page to real zero page

clearpal:
	setaxy8

	stz CGADDR
	ldx #0
@1:
	stz CGDATA
	stz CGDATA

	inx
	bne @1

clearvram:
	setaxy16

	stz PPUADDR
	ldx #32768
@1:
	stz PPUDATA
	dex
	bne @1

clearram:
	lda #0
	ldx #0
@1:
	sta $7e0000,x
	sta $7f0000,x
	inx
	inx
	bne @1

	lda #(__BSS_START__+__BSS_SIZE__)
	sta sp

	setaxy8

	; copy our CODE bank to BSS
	lda #<__CODE_RUN__
	sta WMADDL
	lda #>__CODE_RUN__
	sta WMADDM
	stz WMADDH

	lda #(DMA_00) ; write from cpu to io, increasing, in 8bit units
	sta DMAMODE
	lda #$80 ; to ram
	sta DMAPPUREG
	lda #<__CODE_LOAD__
	sta DMAADDR
	lda #>__CODE_LOAD__
	sta DMAADDRHI
	lda #$80 ; from the default lorom bank (with access to low ram)
	sta DMAADDRBANK
	lda #<__CODE_SIZE__
	sta DMALEN
	lda #>__CODE_SIZE__
	sta DMALENHI
	lda #1
	sta MDMAEN

	lda #$80
	sta _curbank

	setaxy16

	; sound init
	lda f:spc_code
	sta ptr2 ; size

	lda f:spc_code+14
	sta _spc_music_load_adr

	stz _spc_stream_enable

	lda #$200
	sta _spc_dstaddr

	lda ptr2
	cmp #32766
	bcs @largedata
@smalldata:

	lda #.loword(spc_code+2)
	sta _sneslib_ptr
	lda #.hiword(spc_code+2)
	sta _sneslib_ptr+2

	lda ptr2
	sta _spc_size

	jsr _spc_load_data

	jmp @datadone
@largedata:
	lda #.loword(spc_code+2)
	sta _sneslib_ptr
	lda #.hiword(spc_code+2)
	sta _sneslib_ptr+2

	lda #32766
	sta _spc_size

	jsr _spc_load_data

	lda #SCMD_LOAD
	jsr _spc_command

	lda #.loword(spc_code2)
	sta _sneslib_ptr
	lda #.hiword(spc_code2)
	sta _sneslib_ptr+2

	lda #$81fe
	sta _spc_dstaddr

	lda ptr2
	sec
	sbc #32766
	sta _spc_size

	jsr _spc_load_data

@datadone:
	lda #SCMD_INITIALIZE
	jsr _spc_command

	setaxy8

	lda #$fd
	sta <RAND_SEED
	sta <RAND_SEED+1

	; Detect multitap
	stz HASMULTITAP

	lda #1
	sta JOY0
	ldx #8
@multidetect:
	lda JOY1
	lsr
	lsr
	rol HASMULTITAP

	dex
	bne @multidetect

	stz JOY0

	; HASMULTITAP is now 0xff if a multitap is connected
	lda HASMULTITAP
	cmp #$ff
	beq @yestap
	lda #0
	bra @savetap
@yestap:
	lda #1
@savetap:
	sta HASMULTITAP


	; Normal brightness, screen off
	lda #$8f
	sta PPU_MASK_VAR

	; Enable nmi, autoread
	lda #(VBLANK_NMI | AUTOREAD)
	sta PPUNMI

	; Mode 1 (16col, 16col, 4col), all with 8px tiles
	;bg3 in front of everything added
	lda #%00001001
	sta BGMODE

	; BG1 tiles start at 0x1000, BG2 tiles start at 0x3000
	lda #%00110001
	sta BGCHRADDR
	
	; BG3 tiles at 5000
	lda #5
	sta $210c
	
	; 2108 set for bg2 map base address, start at 400
	lda #%00000100
	sta $2108
	
	;2109 for bg3 map, start at $5400
	lda #%01010100
	sta $2109

	; sprite tiles start at 6000 alternative sprite set starts at
	lda #3
	sta OBSEL

	; enable sprites and bgs ; changed by me
	lda #$17
	sta BLENDMAIN

	
	; set the data bank to our rodata bank
	lda #$83
	pha
	plb

	; Unlike on the NES, we don't have to wait 2 vblanks to do
	; any of the following remaining tasks.
	; * Fill or clear areas of VRAM that will be used
	; * Clear areas of WRAM that will be used
	; * Load palette data into CGRAM
	; * Fill shadow OAM and then copy it to OAM
	; * Boot the S-SMP
	; The main routine can do these in any order.
	jml _main
.endproc

