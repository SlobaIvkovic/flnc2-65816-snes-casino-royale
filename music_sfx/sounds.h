//this file generated with SNES GSS tool

#define SOUND_EFFECTS_ALL	12

#define MUSIC_ALL	7

//sound effect aliases

enum {
	SFX_VICTORY_SFX=0,
	SFX_FAILURE_SFX=1,
	SFX_MOVE_CURSOR=2,
	SFX_SELECT_CONFIRM=3,
	SFX_CANCEL_GO_BACK=4,
	SFX_CARD_DEALING=5,
	SFX_TURN_OVER_NEW_CARD_1=6,
	SFX_TURN_OVER_NEW_CARD_2=7,
	SFX_FOLD_CARDS=8,
	SFX_CHIPS_SMALL=9,
	SFX_CHIPS_MEDIUM=10,
	SFX_CHIPS_LARGE=11
};

//sound effect names

const char* const soundEffectsNames[SOUND_EFFECTS_ALL]={
	"VICTORY SFX",	//0
	"FAILURE SFX",	//1
	"MOVE CURSOR",	//2
	"SELECT\CONFIRM",	//3
	"CANCEL\GO BACK",	//4
	"CARD DEALING",	//5
	"TURN OVER NEW CARD 1",	//6
	"TURN OVER NEW CARD 2",	//7
	"FOLD CARDS",	//8
	"CHIPS SMALL",	//9
	"CHIPS MEDIUM",	//10
	"CHIPS LARGE"	//11
};

//music effect aliases

enum {
	MUS_GAMEPLAY_5=0,
	MUS_GAMEPLAY_1__POKER_THEME_2_=1,
	MUS_GAMEPLAY_2=2,
	MUS_GAMEPLAY_3=3,
	MUS_GAMEPLAY_4=4,
	MUS_MAIN_MENU=5,
	MUS_VICTORY=6
};

//music names

const char* const musicNames[MUSIC_ALL]={
	"GAMEPLAY 5",	//0
	"GAMEPLAY 1 (POKER THEME 2)",	//1
	"GAMEPLAY 2",	//2
	"GAMEPLAY 3",	//3
	"GAMEPLAY 4",	//4
	"MAIN MENU",	//5
	"VICTORY"	//6
};

extern const unsigned char spc700_code_1[];
extern const unsigned char spc700_code_2[];
extern const unsigned char music_1_data[];
extern const unsigned char music_2_data[];
extern const unsigned char music_3_data[];
extern const unsigned char music_4_data[];
extern const unsigned char music_5_data[];
extern const unsigned char music_6_data[];
extern const unsigned char music_7_data[];

const unsigned char* const musicData[MUSIC_ALL]={
	music_1_data,
	music_2_data,
	music_3_data,
	music_4_data,
	music_5_data,
	music_6_data,
	music_7_data
};
