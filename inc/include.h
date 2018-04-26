#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include "esp8266.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

/********define in playlist.c******/
struct filelist
{
	char data[50];
	int lenth;
	struct filelist* next;
};

#define NET_LIST 0
#define FILE_LIST 1

extern struct filelist *Songid_HEAD ;
extern struct filelist *Songid_END ;

extern struct filelist *Playlist_HEAD;
extern struct filelist *Playlist_END;

extern void filelist_init();
extern void filelist_add(uint8_t list_id,char* id_data,int lenth);
extern void filelist_delete(uint8_t list_id);


/***********define in main.c ************/
extern int32_t error_num ;


/***********define in MUSIC_task.c ************/
extern uint8_t *file_buff; 
extern uint8_t *raw_buff;

/***********define in mem.c ************/
extern void readout_file(char* music_name);


/***********define in music.c ************/
extern DEV_SPI_PTR spi;
extern volatile uint8_t flag_dma_finish;
extern void spi_dma_test();

/***********define in dma2spi.c*********/
extern void spi_dma_prepare(void);
extern int32_t spi_writeraw(const void *data);


/**************define in mp3api.c************/
extern void play_mp3(int filelenth);
// extern void send2spi();
extern void playlist_init();
/**************define in iosignal.c**********/

extern void iosignal_init();
extern void iosignal_ctrl(uint8_t val,uint8_t num);
extern uint8_t iosignal_read(uint8_t num);


/*************define in net.c****************/

#define SONG_ID 0
#define SONG_INFO 1

extern char dllink[500];
extern char songpoint[50];
extern uint8_t flag_netpoll ;
extern char *net_buff;
extern uint32_t bypass_cnt;
extern ESP8266_DEF_PTR ESP8266_A;
extern void net_init();
extern int socket_request(unsigned char option);
extern void download_mp3();

#endif










