.export _music1, _music2, _music3, _music6, _music5, _music7, spc_code, spc_code2, _font_chr, _font_bank
.export _title_screen_1_chr, _title_screen_map
.export _title_screen_2_chr, _title_big_selector, _title_numbers

.segment "RODATA"
_font_bank: .byte ^_font_chr

.segment "CHR0"
_title_screen_1_chr:
.incbin "title_screen_bins/half.chr"                     ;21472
_title_screen_map:
.incbin "title_screen_bins/title_screen_safe.map"        ;1972


.segment "CHR1"
_title_screen_2_chr:
.incbin "title_screen_bins/half2.chr"                     ;21472
_title_big_selector:
.incbin "title_screen_bins/title_big_selector.chr"
_title_numbers:
.incbin "title_screen_bins/title_numbers.chr"

.segment "CHR2"
.incbin "betting_room_bins/betting_screen_safe.chr"      ;8288
.incbin "betting_room_bins/betting_screen_safe_new.map"  ;1794
.incbin "betting_room_bins/colored_chips.chr"     ;512
.incbin "betting_room_bins/betting_selector.chr"  ;112
.incbin "end_screen_bins/end_screen_base.chr"     ;8416
.incbin "end_screen_bins/end_screen_base.map"     ;1792
.incbin "end_screen_bins/jackpot.chr"             ;3296
.incbin "end_screen_bins/jackpot.map"             ;578
.incbin "end_screen_bins/lose_message.chr"        ;4384
.incbin "end_screen_bins/lose_message.map"        ;578
.incbin "end_screen_bins/end_selector.chr"        ;160
.incbin "end_screen_bins/end_screen_font.chr"     ;1792


.segment "CHR3"
.incbin "card_bins/cards.chr"                           ;11328
.incbin "blackjack_bins/blackjack_screen_safe.chr"      ;12352
.incbin "blackjack_bins/blackjack_screen_safe_new.map"  ;1794
.incbin "card_bins/card_set_2.chr"
.incbin "blackjack_bins/nums.chr"
.incbin "blackjack_bins/letters.chr"
.incbin "blackjack_bins/sel.chr"
.incbin "blackjack_bins/dolar.chr"  

.segment "CHR4"
.incbin "card_bins/card_back.chr"                     ;1120         
.incbin "poker_bins/showdown_font.chr"                ;1504
.incbin "multi_screen_bins/multi_select.chr"          ;4000
.incbin "multi_screen_bins/multi_select1.map"         ;1794
.incbin "multi_screen_bins/multi_field.chr"           ;384
.incbin "belote_bins/Belote_TeamSelection.chr"        ;4480
.incbin "belote_bins/Belote_TeamSelection1.map"        ;1794
.incbin "belote_bins/Belote_TakePass2.chr"             ;5536
.incbin "belote_bins/Belote_TakePass2.map"             ;1794, total 
.incbin "belote_bins/Belote_NewTeamSelection.chr"             ;5920
.incbin "belote_bins/Belote_NewTeamSelection.map"      ;1794
; ima jos prostora

.segment "CHR5"
.incbin "solitaire_bins/solitaire_table_safe.chr"      ;3744
.incbin "solitaire_bins/solitaire_table_safe_new.map"  ;1794
.incbin "solitaire_bins/solitaire_cards.chr"           ;9312
.incbin "solitaire_bins/solitaire_cards_16.chr"        ;16384

.segment "CHR6"
.incbin "solitaire_bins/sol_back.chr"                  ;768
.incbin "solitaire_bins/colored_sol_sel.chr"           ;128
.incbin "pause_dialog_bins/pause_dialog.chr"           ;128
.incbin "pause_dialog_bins/pause_choice.chr"           ;224
.incbin "poker_bins/poker_table_4.chr"                 ;4128
.incbin "poker_bins/poker_table_4_new.map"             ;1974
.incbin "poker_bins/spades_small.chr"                  ;2400
.incbin "poker_bins/poker_fields.chr"                  ;384
.incbin "poker_bins/slider_bar.chr"                    ;256
.incbin "belote_bins/Belote_Trick2.chr"              ;4768  5728
.incbin "belote_bins/Belote_Trick2.map"             ;1794     
.incbin "belote_bins/TrumpSuits.chr"             ;
.incbin "belote_bins/Belote_SetTrump2.chr"             ;6368
.incbin "belote_bins/Belote_SetTrump2.map"             ;1794

.segment "CHR7"
.incbin "achi_bins/achi_slots.chr"             ;3200
.incbin "achi_bins/achi_slots.map"             ;4096

.incbin "achi_bins/achi_text1.chr"              ;7712
.incbin "achi_bins/achi_text1.map"               ;4096

.incbin "achi_bins/achievment_background.chr"        ;7680
.incbin "achi_bins/achievment_background1.map"        ;1794
.incbin "achi_bins/fill_slot.chr"              ;192
.incbin "achi_bins/achi_digits.chr"              ;352

.segment "CHR8"
.incbin "option_screen_bins/options_screen.chr"  ;10592
.incbin "option_screen_bins/options_screen.map"  ;1792
.incbin "option_screen_bins/sound_symbols1.chr"  ;1440
.incbin "option_screen_bins/slide_sel1.chr"  ;    320
.incbin "blackjack_bins/letsel.chr" ;    
.incbin "blackjack_bins/bjpause.chr"     ;
.incbin "poker_bins/poker_pause.chr"                
.incbin "belote_bins/belote_pause.chr"        

_font_chr:
.incbin "font.chr"

.segment "RODATA6"
spc_code:
.incbin "music/spc700.bin", 0, 32768

.segment "RODATA7"
spc_code2:
.incbin "music/spc700.bin", 32768
_music1:
.incbin "music/music_1.bin"
_music2:
.incbin "music/music_2.bin"
_music3:
.incbin "music/music_3.bin"
_music6:
.incbin "music/music_6.bin"
_music5:
.incbin "music/music_5.bin"

.segment "MUSICDATA"
_music7:
.incbin "music/music_7.bin"
