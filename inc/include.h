#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include "embARC.h"
#include "embARC_debug.h"

#include "inc_task.h"

#include "esp8266.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

/*********Global**********************/
#define BUFF_SPACE 15728640
#define NET_LIST 0
#define FILE_LIST 1

#define IN_FILE 0
#define IN_BUFF 1

#define BIT_0	( 1 << 0 )
#define BIT_1	( 1 << 1 )
#define BIT_2	( 1 << 2 )
#define BIT_3	( 1 << 3 )

#define SONG_ID 0
#define SONG_INFO 1

#define BUFF_EMPTY 0
#define BUFF_FULL 1

/********define in filelist.c******/
struct filelist {
	char data[50];
	int32_t lenth;
	uint8_t location;
	struct filelist *next;
};



extern struct filelist *Songid_HEAD ;
extern struct filelist *Songid_END ;

extern struct filelist *Playlist_HEAD;
extern struct filelist *Playlist_END;

extern void filelist_init();
extern void filelist_add(uint8_t list_id, char *id_data, int32_t lenth, uint8_t location);
extern void filelist_delete(uint8_t list_id);


/***********define in main.c ************/

extern int32_t error_num ;

/***********define in mem.c ************/

extern void readout_file(char *music_name);

/***********define in music.c ************/
extern DEV_SPI_PTR spi;
extern int8_t file_buff[ BUFF_SPACE ];
extern void play_init();

/***********define in dma2spi.c*********/
extern void spi_dma_prepare(void);
extern int32_t spi_writeraw(const void *data);
extern int32_t Start_playing();

/**************define in mp3api.c************/
extern int32_t play_mp3(int32_t filelenth, uint8_t location);

/**************define in iosignal.c**********/

extern void iosignal_init();
extern void iosignal_ctrl(uint8_t val, uint8_t num);
extern uint8_t iosignal_read(uint8_t num);
extern void net_rst();

/*************define in net.c****************/

extern uint8_t flag_netbuff;
extern char dllink[500];
extern char songpoint[50];
extern int8_t net_buff[ BUFF_SPACE ];
extern ESP8266_DEF_PTR ESP8266_A;
extern void net_init();
extern int socket_request(uint8_t option);
extern void download_mp3();

/*********define in GUI.c************************/
struct _gui_info {
	uint8_t screen_point;
	int16_t network_speed;
	int16_t decord_speed;
	int16_t main_cycle;

	char *song_name;
	char *next_song;

	// uint8_t KEEP_ID3V2;
	uint8_t flag_next;

	uint32_t delay_cnt;
};

extern struct _gui_info gui_info;

extern void gui_init();
extern void reflash_screen();

/**********define in key.c********************/
extern void key1_isr();
extern void key2_isr();
extern void key3_isr();

#endif










