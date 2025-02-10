#define CHR0_BANK 0x89
#define CHR1_BANK 0x8A
#define CHR2_BANK 0x8B
#define CHR3_BANK 0x8C
#define CHR4_BANK 0x8D
#define CHR5_BANK 0x8E
#define CHR6_BANK 0x8F
#define CHR7_BANK 0x92
#define CHR8_BANK 0x93

#define TITLE_SCREEN_1_2_S     21472
#define TITLE_SCREEN_2_2_S     21472
#define TITLE_SCREEN_MAP_S     1792
#define TITLE_BIG_SELECTOR_S   128
#define TITLE_NUMBERS_S

#define BETTING_ROOM_S          8288
#define BETTING_ROOM_MAP_S      1794
#define BETTING_SELECTOR_S      112
#define COLORED_CHIPS_S         512    
#define BETTING_ROOM_TOTAL_S  (BETTING_ROOM_S + BETTING_ROOM_MAP_S + BETTING_SELECTOR_S + COLORED_CHIPS_S)

#define END_SCREEN_BASE_S       8416
#define END_SCREEN_BASE_MAP_S   1792
#define JACKPOT_S               3296
#define JACKPOT_MAP_S           578
#define LOSE_MESSAGE_S          4384            
#define LOSE_MESSAGE_MAP_S      578
#define END_SELECTOR_S          160
#define END_SCREEN_FONT_S       1792   

#define CARDS_S                 11328
#define BLACKJACK_TABLE_S       12352
#define BLACKJACK_TABLE_MAP_S   1794
#define CARD_SET_2_S            5312
#define NUMS_S                  480
#define LETTERS_S               416
#define SEL_S                   64
#define DOLAR_S                 48
#define CARD_BACK_S             1120


#define SOLITAIRE_TABLE_S       3744
#define SOLITAIRE_TABLE_MAP_S   1794
#define SOLITAIRE_CARDS_S       9312
#define SOLITAIRE_CARDS_16_S    16384
#define SOL_BACK_S              768
#define COLORED_SOL_SEL_S       128

#define PAUSE_DIALOG 128
#define PAUSE_CHOICE 224

#define BEFORE_POKER_S (SOL_BACK_S + COLORED_SOL_SEL_S + PAUSE_DIALOG + PAUSE_CHOICE)
#define POKER_TABLE_S           4128
#define POKER_TABLE_MAP_S       1794
#define SMALL_SPADES_S          2400
#define POKER_FIELDS_S          384
#define SLIDER_BAR_S            256
