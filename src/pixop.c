#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>

#include "pixop.h"

// fill rectangle in pixmap "dst" with color "color"
// bx, by = upper-left coord in dst pixmap (pixels)
// wd, ht = width and height of rectangle (pixels)
int pix_fill(pixmap_t* dst, int bx, int by, int wd, int ht, int color) {
	char* dp;
	int dst_stride;		// dst pixmap width in bytes
	int fill_stride;	// fill width in bytes
	int n;

	if (dst == NULL) return 0;
	bx &= ~1;	// make even
	if (bx < 0) { wd += bx; bx = 0; }
	if (by < 0) { ht += by; by = 0; }
	if (bx + wd > dst->width) wd = dst->width - bx;
	if (by + ht > dst->height) ht = dst->height - by;
	if (wd <= 0 || ht <= 0) return 0;
	color &= 0x0F;
	color |= (color << 4);		// two color values in a single byte
	dp = dst->surf;
	dst_stride = (dst->width + 1)/2;
	fill_stride = (wd + 1)/2;
	bx /= 2;				// first byte offset in row
	dp += by*dst_stride;	// start row
	dp += bx;				// first pixel to fill
	n = 0;
	while (ht-- > 0) {
		memset(dp, color, fill_stride);
		dp += dst_stride;	// first pixel to fill in next row
		n += fill_stride;
	}
	return n*2;				// num of pixels actually filled
}

// transfer pixmap "src" to pixmap "dst"
// bx, by = upper-left coord in dst pixmap (pixels)
int pix_blt(pixmap_t* dst, int bx, int by, pixmap_t* src) {
	char* dstp;
	char* srcp;
	int dst_stride, src_stride;
	int n_bytes, n_lines;
	int src_bx = 0, src_by = 0;
	int n;

	if (dst == NULL || src == NULL)
		return 0;
	bx &= ~1;	// make even
	bx /= 2;	// horz byte offset
	dst_stride = (dst->width + 1)/2;			// dst width in bytes
	n_bytes = src_stride = (src->width + 1)/2;	// src width in bytes

	if (bx < 0) {
		src_bx = -bx; n_bytes += bx; bx = 0;
	}
	if (bx + n_bytes > dst_stride)
		n_bytes = dst_stride - bx;
	if (n_bytes <= 0)
		return 0;

	n_lines = src->height;
	if (by < 0) {
		src_by = -by; n_lines += by; by = 0;
	}
	if (by + n_lines > dst->height)
		n_lines = dst->height - by;
	if (n_lines <= 0)
		return 0;

	dstp = dst->surf; dstp += by*dst_stride; dstp += bx;			// dst byte ptr
	srcp = src->surf; srcp += src_by*src_stride; srcp += src_bx;	// src byte ptr

//	printf("%02X: nl,nb=%d,%d dstp=%08X, srcp=%08X, bx,by=%d,%d, src_bx,by=%d,%d strides=%d,%d\n",
//		*srcp, n_lines, n_bytes, dstp, srcp, bx, by, src_bx, src_by, dst_stride, src_stride) ;

	n = 0;
	while (n_lines-- > 0) {
		memcpy(dstp, srcp, n_bytes);
		dstp += dst_stride;
		srcp += src_stride;
		n += n_bytes;
	}
	return n*2;		// num of pixels transferred
}

// transfer a part of pixmap "src" to pixmap "dst"
// bx, by = upper-left coord in src pixmap (pixels)
int pixmap_xtract(pixmap_t* dst, int bx, int by, pixmap_t* src) {
	char* dstp;
	char* srcp;
	int dst_stride, src_stride;
	int n_bytes, n_lines;
	int n;

	if (dst == NULL || src == NULL)
		return 0;
	if ((bx < 0) || (by < 0))
		return 0 ;

	bx &= ~1;	// make even
	bx /= 2;	// horz byte offset
	dst_stride = (dst->width + 1)/2;		// dst width in bytes
	src_stride = (src->width + 1)/2 ;		// available src width in bytes

	n_bytes = (dst_stride > (src_stride-bx)) ? (src_stride-bx) : dst_stride ;

	if (n_bytes <= 0)
		return 0;

	n_lines = (dst->height <= (src->height - by)) ? dst->height : src->height - by ;

	if (n_lines <= 0)
		return 0;

	dstp = dst->surf; 			// dst byte ptr
	srcp = src->surf; srcp += by*src_stride; srcp += bx;	// src byte ptr

//	printf("%02X: nl,nb=%d,%d dstp=%08X, srcp=%08X, bx,by=%d,%d, src_bx,by=%d,%d strides=%d,%d\n",
//		*srcp, n_lines, n_bytes, dstp, srcp, bx, by, src_bx, src_by, dst_stride, src_stride) ;

	n = 0;
	while (n_lines-- > 0) {
		memcpy(dstp, srcp, n_bytes);
		dstp += dst_stride;
		srcp += src_stride;
		n += n_bytes;
	}
	return n*2;		// num of pixels transferred
}

void pixmap_invert(pixmap_t *p)
{
	int nbytes ;
	char *tmp ;

	if (p)
	{
		nbytes = (p->width+1 /2) * (p->height) ;
		tmp = p->surf ;
		while (nbytes--)
		{
			*tmp = ~(*tmp) ;
			++tmp ;
		}
	}
}

pixmap_t * pixbuf_alloc(int w, int h)
{
	pixmap_t * p = NULL ;
	int size ;

	size = ((w+1)/2)*h + sizeof(pixmap_t) ;

	if ( (p = (pixmap_t *) malloc(size)) != NULL)
	{
		memset(p, 0, size) ;

		p->width = w ;
		p->height = h ;
		p->surf = ((char *)p) + sizeof(pixmap_t) ;
	}

	return p ;
}

void pixbuf_free(pixmap_t *p)
{
	if (p)
		free(p) ;
}
