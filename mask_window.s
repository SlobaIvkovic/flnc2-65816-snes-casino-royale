.p816
.smart
.include "snes.sinc"
.export _driveMask, _SetupColorWindow, _disableColorWindow, _driveAchiMask, _disableBG13Window, _disableHDMA
.segment "ZEROPAGE"
MASK_POS: .res 1
.segment "CODE"
_driveMask:
	sta MASK_POS
	;Reset the HDMA
   LDA #0 
   STA $420C         ;Turn off HDMA
   lda MASK_POS
   JSR SetupHDMA     ;Setup the HDMA transfers on Channels 6 and 7
   LDA #$C0 
   STA $420C         ;Activate HDMA channels 6 and 7

   ;lda $4210         ;Clear NMI flag
   ;setaxy8
   RTS              ;Done!
   
;============================================================================
; SetupHDMA -- Setup the HDMA for the fixed color subtract window
;----------------------------------------------------------------------------
; In: None
;----------------------------------------------------------------------------
; Out: None
;----------------------------------------------------------------------------
; Modifies: none
;----------------------------------------------------------------------------
SetupHDMA:
   
   CPA #0
   BNE SecondTable
   LDX #<HDMATableRight1
   STX $4362
   LDX #>HDMATableRight1
   STX $4363
   LDX #$83
   STX $4364
   
   LDX #<HDMATableLeft1
   STX $4372
   LDX #>HDMATableLeft1
   STX $4373
   LDX #$83
   STX $4374

SecondTable:
	CPA #1
   BNE ThirdTable
   LDX #<HDMATableRight2
   STX $4362
   LDX #>HDMATableRight2
   STX $4363
   LDX #$83
   STX $4364
   
   LDX #<HDMATableLeft2
   STX $4372
   LDX #>HDMATableLeft2
   STX $4373
   LDX #$83
   STX $4374
   
ThirdTable:
	CPA #2
   BNE FourthTable
   LDX #<HDMATableRight3
   STX $4362
   LDX #>HDMATableRight3
   STX $4363
   LDX #$83
   STX $4364
   
   LDX #<HDMATableLeft3
   STX $4372
   LDX #>HDMATableLeft3
   STX $4373
   LDX #$83
   STX $4374   

FourthTable:
	CPA #3
	BNE MaskEnd   
	LDX #<HDMATableRight4
   STX $4362
   LDX #>HDMATableRight4
   STX $4363
   LDX #$83
   STX $4364
   
   LDX #<HDMATableLeft4
   STX $4372
   LDX #>HDMATableLeft4
   STX $4373
   LDX #$83
   STX $4374      
   
MaskEnd:
  
   ;LDA #$00
   ;STA $4374
   ;STA $4364
   LDA #$26
   STA $4371
   LDA #$27
   STA $4361
   LDA #$00
   STA $4370
   STA $4360
   RTS


;============================================================================

;============================================================================
; SetupColorWindow -- setup the fixed color subtraction window
;----------------------------------------------------------------------------
; In: None
;----------------------------------------------------------------------------
; Out: None
;----------------------------------------------------------------------------
; Modifies: none
;----------------------------------------------------------------------------
_SetupColorWindow:
	lda #$20
   sta $2125      ;Enable Color Window 1
   lda #$FF
   sta $2126      ;Set Window 1 left position
   lda #$D0
   sta $2127      ;Set Window 1 right position
   lda #$10
   sta $2130      ;Color addition selection
   lda #$01
   sta $2131      ;Add/Sub
   lda #$E3
   sta $2132      ;Enable R, G, B at dim color constant
   RTS
;============================================================================

_disableColorWindow:
	lda #$00
	sta $2125
	RTS
;============================================================================

_driveAchiMask:
	jsr BG13WindowsSetup
	LDA #0 
   STA $420C         ;Turn off HDMA
   JSR AchiHDMAsetup    ;Setup the HDMA transfers on Channels 6 and 7
   LDA #$f0 
   STA $420C         ;Activate HDMA channels 6 and 7 and 5 and 4

   RTS              ;Done!

BG13WindowsSetup:
lda #$03
   sta $2123      ;Enable Color Window 1 BG1 inverted
   sta $2124      ;Enable Color Window 1 BG3 inverted
   lda #$00
   sta $2126      ;Set Window 1 left position
   lda #$ff
   sta $2127      ;Set Window 1 right position
   
   lda #$05
   sta $212E      ;Enable Window 1 for BG1 BG3
   RTS

AchiHDMAsetup:
   ;channel 6 BG1 right
   LDX #<BGTableRight
   STX $4362
   LDX #>BGTableRight
   STX $4363
   LDX #$83
   STX $4364
   ;channel 7 BG1 left
   LDX #<BGTableLeft
   STX $4372
   LDX #>BGTableLeft
   STX $4373
   LDX #$83
   STX $4374
   ;channnel 4 BG3 right
   LDX #<BGTableRight
   STX $4342
   LDX #>BGTableRight
   STX $4343
   LDX #$83
   STX $4344
   ;channel 5 BG3 left
   LDX #<BGTableLeft
   STX $4352
   LDX #>BGTableLeft
   STX $4353
   LDX #$83
   STX $4354

   LDA #$26
   STA $4371
   LDA #$27
   STA $4361
   
   LDA #$26
   STA $4351
   LDA #$27
   STA $4341
   
   
   LDA #$00
   STA $4370
   STA $4360
   
   LDA #$00
   STA $4340
   STA $4350
   
   RTS

_disableBG13Window:
	lda #$00
	sta $2123
	sta $2124
	RTS

_disableHDMA:
   LDA #0 
   STA $420C         ;Turn off HDMA
   RTS

.segment "RODATA"
;This HDMA table controls the Left side of the color window
HDMATableLeft:
   .byte $28,$ff    ;Set left=255, skip to line 40
   .byte $64,$20    ;Set left=32, skip to line 140
   .byte $01,$ff    ;Set left=255
   .byte 0

;This HDMA table controls the Right side of the color window
HDMATableRight:
   .byte $28,$00    ;Set right=0, skip to line 40
   .byte $64,$D0    ;Set right=208, skip to line 140
   .byte $01,$00    ;Set right=0
   .byte 0
   
HDMATableLeft1:
   .byte $5E,$ff    ;Set left=255, skip to line 94
   .byte $50,$0B    ;Set left=11, height = 80
   .byte $01,$ff    ;Set left=255
   .byte 0

;This HDMA table controls the Right side of the color window
HDMATableRight1:
   .byte $5E,$00    ;Set right=0, skip to line 40
   .byte $50,$43    ;Set right=67, height = 80
   .byte $01,$00    ;Set right=0
   .byte 0

HDMATableLeft2:
   .byte $5E,$ff    ;Set left=255, skip to line 94
   .byte $50,71    ;Set left=71, height = 80
   .byte $01,$ff    ;Set left=255
   .byte 0

;This HDMA table controls the Right side of the color window
HDMATableRight2:
   .byte $5E,$00    ;Set right=0, skip to line 40
   .byte $50, 127    ;Set right=127, height = 80
   .byte $01,$00    ;Set right=0
   .byte 0         

HDMATableLeft3:
   .byte $5E,$ff    ;Set left=255, skip to line 94
   .byte $50,131    ;Set left=131, height = 80
   .byte $01,$ff    ;Set left=255
   .byte 0

;This HDMA table controls the Right side of the color window
HDMATableRight3:
   .byte $5E,$00    ;Set right=0, skip to line 40
   .byte $50, 187    ;Set right=187, height = 80
   .byte $01,$00    ;Set right=0
   .byte 0         
   
HDMATableLeft4:
   .byte $5E,$ff    ;Set left=255, skip to line 94
   .byte $50,191    ;Set left=191, height = 80
   .byte $01,$ff    ;Set left=255
   .byte 0

;This HDMA table controls the Right side of the color window
HDMATableRight4:
   .byte $5E,$00    ;Set right=0, skip to line 40
   .byte $50, 247    ;Set right=247, height = 80
   .byte $01,$00    ;Set right=0
   .byte 0            
   
BGTableLeft:
	.byte 72, 255
	.byte 128, 0
	.byte 3, 0
	.byte 1, 255
	.byte 0
	
BGTableRight:
	.byte 72, 0
	.byte 128, 255
	.byte 3, 255
	.byte 1, 0
	.byte 0	  
