;this file generated with SNES GSS tool

.define SFX_VICTORY_SFX	0
.define SFX_FAILURE_SFX	1
.define SFX_MOVE_CURSOR	2
.define SFX_SELECT_CONFIRM	3
.define SFX_CANCEL_GO_BACK	4
.define SFX_CARD_DEALING	5
.define SFX_TURN_OVER_NEW_CARD_1	6
.define SFX_TURN_OVER_NEW_CARD_2	7
.define SFX_FOLD_CARDS	8
.define SFX_CHIPS_SMALL	9
.define SFX_CHIPS_MEDIUM	10
.define SFX_CHIPS_LARGE	11

.define MUS_GAMEPLAY_5	0
.define MUS_GAMEPLAY_1__POKER_THEME_2_	1
.define MUS_GAMEPLAY_2	2
.define MUS_GAMEPLAY_3	3
.define MUS_GAMEPLAY_4	4
.define MUS_MAIN_MENU	5
.define MUS_VICTORY	6

.section ".roDataSoundCode1" superfree
spc700_code_1:	.incbin "spc700.bin" skip 0 read 32768
.ends

.section ".roDataSoundCode2" superfree
spc700_code_2:	.incbin "spc700.bin" skip 32768
.ends

.section ".roDataMusic1" superfree
music_1_data:	.incbin "music_1.bin"
.ends

.section ".roDataMusic2" superfree
music_2_data:	.incbin "music_2.bin"
.ends

.section ".roDataMusic3" superfree
music_3_data:	.incbin "music_3.bin"
.ends

.section ".roDataMusic4" superfree
music_4_data:	.incbin "music_4.bin"
.ends

.section ".roDataMusic5" superfree
music_5_data:	.incbin "music_5.bin"
.ends

.section ".roDataMusic6" superfree
music_6_data:	.incbin "music_6.bin"
.ends

.section ".roDataMusic7" superfree
music_7_data:	.incbin "music_7.bin"
.ends

