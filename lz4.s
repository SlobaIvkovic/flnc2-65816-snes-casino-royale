;
; Lauri Kasanen
; (C) Mega Cat Studios
; An optimized LZ4 decompressor, SNES banked version
;

	.importzp	sp, sreg, regsave, regbank
	.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
	.importzp	_sneslib_ptr
	.macpack	longbranch
	.export		_decompress_lz4_bank

.smart
.include "snes.sinc"

out = _sneslib_ptr
written = sreg
tmp = tmp1
token = tmp2
offset = ptr3
in = regsave
outlen = ptr4

; ---------------------------------------------------------------
; void decompress_lz4_bank (const u8 srcbank, const u8 *in,
;                           const u8 dstbank, u8 * const out,
;                           const u16 outlen)
; ---------------------------------------------------------------

.segment	"BSS"
mvnram:	.res 4

.segment	"CODE"

; size in ptr3, src in ptr1
; don't touch size
memcpy_r2v:
	lda	in+2
	sta	mvnram+2
	lda	out+2
	sta	mvnram+1

	bra inner

memcpy_v2v:
	lda	out+2
	sta	mvnram+2
	sta	mvnram+1
inner:
	phb

	setaxy16

	; dest is always out[written]
	lda	out
	clc
	adc	written
	tay

	lda	ptr3
	dea

	ldx	ptr1

	jsr	mvnram

	plb
	rts

; memcpy drops through in 16-bit mode
.a8
.i8

.proc	_decompress_lz4_bank: near

	sta	outlen
	stx	outlen+1

	lda	#$60 ; rts
	sta	mvnram+3
	lda	#$54 ; mvn
	sta	mvnram

	; popax, popa, popax, popa replacement
	seta16

	lda	(sp)
	sta	out

	ldy	#2
	lda	(sp),y
	sta	out+2

	iny

	lda	(sp),y
	sta	in

	iny

	lda	(sp),y
	sta	in+1

	lda	sp
	clc
	adc	#6
	sta	sp

	seta8
;
; written = 0;
;
	lda     #$00
	sta     written
;
; while (written < outlen) {
;
	jmp     L0046
;
; token = *in++;
;
L0004:
	lda     [in]
	sta	token

	inc	in
	bne	L000A
	inc	in+1
L000A:
;
; offset = token >> 4;
;
	ldx     #$00
	lsr     a
	lsr     a
	lsr     a
	lsr     a
	sta     offset
	stx     offset+1
;
; token &= 0xf;
; token += 4; // Minmatch
;
	lda     token
	and     #$0F
	clc
	adc	#$04
	sta     token
;
; if (offset == 15) {
;
	lda     offset
	cmp     #$0F
L0013:	bne     L001A
;
; tmp = *in++;
;
	lda	[in]
	sta	tmp

	inc	in
	bne	L0017
	inc	in+1
L0017:
;
; offset += tmp;
;
	clc
	adc     offset
	sta     offset
	lda     #$00
	adc     offset+1
	sta     offset+1
;
; if (tmp == 255)
;
	lda     tmp
	cmp     #$FF
;
; goto moreliterals;
;
	jmp     L0013
;
; if (offset) {
;
L001A:	lda     offset
	ora     offset+1
	beq     L001C
;
; memcpy(&out[written], in, offset);
;
	lda     in
	ldx     in+1
	sta	ptr1
	stx	ptr1+1
	jsr     memcpy_r2v
;
; written += offset;
;
	lda     offset
	clc
	adc     written
	sta     written
;
; in += offset;
;
	lda     offset
	clc
	adc     in
	sta     in

	setaxy8
;
; if (written >= outlen)
;
L001C:	lda     written
	cmp     outlen
	lda     written+1
	sbc     outlen+1
;
; return;
;
	bcc     L0047
	rts
;
; memcpy(&offset, in, 2);
;
L0047:
	seta16

	lda     [in]
	sta	offset
;
; in += 2;
;
	inc	in
	inc	in
;
; copysrc = out + written - offset;
;
	lda     out
	clc
	adc     written
	sec
	sbc     offset
	sta     ptr1

	seta8
;
; offset = token;
;
	lda     #$00
	sta     offset+1
	lda     token
	sta     offset
;
; if (token == 19) {
;
	cmp     #$13
L0045:	bne     L003C
;
; tmp = *in++;
;
	ldy	#$00
	lda	[in]
	sta	tmp

	inc	in
	bne	L0039
	inc	in+1
L0039:
;
; offset += tmp;
;
	clc
	adc     offset
	sta     offset
	tya
	adc     offset+1
	sta     offset+1
;
; if (tmp == 255)
;
	lda     tmp
	cmp     #$FF
;
; goto morematches;
;
	jmp     L0045
;
; memcpy(&out[written], copysrc, offset);
;
L003C:
	jsr     memcpy_v2v

	setaxy8
;
; written += offset;
;
	lda     offset
	clc
	adc     written
	sta     written
	lda     offset+1
	adc     written+1
L0046:	sta     written+1
;
; while (written < outlen) {
;
	lda     written
	cmp     outlen
	lda     written+1
	sbc     outlen+1
	jcc     L0004

	rts

.endproc

