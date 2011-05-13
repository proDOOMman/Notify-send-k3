#ifndef _FNTDEFS_H_
#define _FNTDEFS_H_

#include "pixop.h"

struct fngheader
{
	char magic[4] ;
	short wd ;
	short ht ;
	short bl ;
	short firstcode ;
	short lastcode ;
};

extern unsigned char default_font[] ;
extern pixmap_t font_pixmap ;
extern int nbytesperchar ;

void prepare_font() ;
int get_char_pixmap(int code, pixmap_t *ppx) ;

pixmap_t * getfngfont(char *path) ;
void freefngfont(pixmap_t *p) ;
void get_font_metrics(int *pw, int *ph) ;

#endif
