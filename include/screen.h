#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "pixop.h"

/* defines below simply mimic values of fx_type from einkfb.h */

#define UMODE_MASK 11
#define UMODE_BUFISMASK 14
#define UMODE_NONE -1
#define UMODE_FLASH 20
#define UMODE_INVERT 21
#define UMODE_PARTIAL 0
#define UMODE_FULL 0

typedef struct fbscreen
{
	int fd ;
	int screensize ;
	pixmap_t surface ;
} fbscreen_t;

fbscreen_t *fbscreen_open() ;
void fbscreen_close(fbscreen_t *pfbuf) ;
void screen_update_area(fbscreen_t *pfbuf, int mode, int x0, int y0, int x1, int y1, void *pbuf) ;
void screen_flash_area(fbscreen_t *pfbuf, int count, int x0, int y0, int w, int h, int delay, int waitflag) ;
int screen_string_at(fbscreen_t *pscreen, int x0, int y0, char *p) ;
int screen_rect_at(fbscreen_t *pscreen, int x0, int y0, int w, int h, int color) ;
int screen_get_pixmap(fbscreen_t *pscreen, int x0, int y0, pixmap_t *dst) ;
int screen_put_pixmap(fbscreen_t *pscreen, int x0, int y0, pixmap_t *dst) ;
#endif
