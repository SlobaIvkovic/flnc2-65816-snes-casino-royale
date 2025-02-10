/*command codes for sound driver, nnnnmmmm where nnn is channel number and mmm is command code*/

#define SCMD_NONE				0x00	/*no command, means the APU is ready for a new command*/
#define SCMD_INITIALIZE			0x01	/*initialize DSP*/
#define SCMD_LOAD				0x02 	/*load new music data*/
#define SCMD_STEREO				0x03	/*change stereo sound mode, mono by default*/
#define SCMD_GLOBAL_VOLUME		0x04	/*set global volume*/
#define SCMD_CHANNEL_VOLUME		0x05	/*set channel volume*/
#define SCMD_MUSIC_PLAY 		0x06	/*start music*/
#define SCMD_MUSIC_STOP 		0x07	/*stop music*/
#define SCMD_MUSIC_PAUSE 		0x08	/*pause music*/
#define SCMD_SFX_PLAY			0x09	/*play sound effect*/
#define SCMD_STOP_ALL_SOUNDS	0x0a	/*stop all sounds*/
#define SCMD_STREAM_START		0x0b	/*start BRR streaming*/
#define SCMD_STREAM_STOP		0x0c	/*stop streaming*/
#define SCMD_STREAM_SEND		0x0d	/*send BRR block*/

/*sound driver variables*/

extern unsigned int spc_music_load_adr;

extern const unsigned char** spc_stream_block_list;
extern const unsigned char*  spc_stream_block_src;

extern unsigned int spc_stream_block;
extern unsigned int spc_stream_block_ptr;
extern unsigned int spc_stream_block_ptr_prev;
extern unsigned int spc_stream_block_size;
extern unsigned int spc_stream_block_repeat;
extern unsigned int spc_stream_enable;
extern unsigned int spc_stream_stop_flag;

extern unsigned char spc_fadespeed; // Used in spc_global_volume
extern unsigned char spc_channelmask; // Used in spc_channel_volume
extern unsigned char spc_sfxpan; // Used in sfx_play, 0=left, 255=right, 128=mid
extern unsigned char spc_sfxvol; // Used in sfx_play
extern unsigned char spc_sfxchan; // Used in sfx_play
extern unsigned int spc_dstaddr; // Used in spc_load_data
extern unsigned int spc_size; // Used in spc_load_data

extern unsigned long sneslib_ptr; // Used in spc_load_data
#pragma zpsym("sneslib_ptr")

/*send a command to the SPC driver*/

void spc_command(unsigned char command);

/*upload data into the sound memory using IPL loader*/

void spc_load_data(unsigned int size);

/*change stereo sound mode*/

void spc_stereo(unsigned char stereo);

/*set global sound volume, target volume 0..127, fade speed 1..255 (slowest to fastest) */

void spc_global_volume(unsigned char volume);

/*set channel volume*/

void spc_channel_volume(unsigned char volume);

/*play sound effect*/

void sfx_play(unsigned char sfx);

/*play music*/
void music_play(unsigned char bank, const unsigned char *data);

/*get channel count*/
/*
unsigned int music_channel_count(const unsigned char *data)
{
	return data[2];
}
*/
/*stop music*/

void music_stop(void);

/*pause or unpause music*/

void music_pause(unsigned char pause);

/*stop all sounds*/

void sound_stop_all(void);

/*internal BRR streaming function*/
/*
void spc_stream_set_block(void)
{
	// TODO asm this, uses long ptr
	spc_stream_block_src=spc_stream_block_list[spc_stream_block];

	if(!spc_stream_block_src)
	{
		spc_stream_stop_flag=TRUE;
		return;
	}

	spc_stream_block_size=spc_stream_block_src[0]+(spc_stream_block_src[1]<<8);

	spc_stream_block_src+=2;

	spc_stream_block_ptr=0;
	spc_stream_block_ptr_prev=0;
	spc_stream_block_repeat=0;
}*/

/*start BRR streaming, it uses channels 6 and 7*/
/*these channels should not be used with any functions while the streamer is active*/
/*
void spc_stream_start(const unsigned char const **list)
{
	// TODO asm this, uses long ptr
	spc_stream_block_list=list;
	spc_stream_block=0;
	spc_stream_stop_flag=FALSE;
	spc_stream_enable=TRUE;

	spc_stream_set_block();

	spc_command(SCMD_STREAM_START);
}*/

/*stop streaming*/
/*
void spc_stream_stop(void)
{
	spc_stream_enable=FALSE;

	spc_command(SCMD_STREAM_STOP);
}
*/
/*internal BRR streaming function*/
/*
void spc_stream_advance_ptr(unsigned int i)
{
	spc_stream_block_ptr+=i;

	if(spc_stream_block_ptr>=spc_stream_block_size)
	{
		++spc_stream_block;

		spc_stream_set_block();
	}
}
*/
/*send BRR stream data when needed, should be called every TV frame*/

//void spc_stream_update(void);
