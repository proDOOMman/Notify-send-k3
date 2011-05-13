#ifndef _PIXOP_H_
#define _PIXOP_H_

typedef struct pixmap_str {
	int		width;
	int		height;
	char*	surf;		// 1 byte = 2 pixels
} pixmap_t;


int pix_fill(pixmap_t* dst, int bx, int by, int wd, int ht, int color) ;
int pix_blt(pixmap_t* dst, int bx, int by, pixmap_t* src) ;
int pixmap_xtract(pixmap_t* dst, int bx, int by, pixmap_t* src);
void pixmap_invert(pixmap_t *p) ;

pixmap_t * pixbuf_alloc(int w, int h) ;
void pixbuf_free(pixmap_t *p) ;

#endif
