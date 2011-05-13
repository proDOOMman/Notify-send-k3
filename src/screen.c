#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include "pixop.h"
#include "fntdefs.h"
#include "screen.h"

typedef unsigned char u8 ;

#include "linux/einkfb.h"

extern void msdelay(int) ;

#ifndef i386

static fbscreen_t fbscreen ;

#ifdef DEBUG_VINFO
static void show_info(struct fb_fix_screeninfo *pfinfo, struct fb_var_screeninfo *pvinfo)
{
	struct fb_var_screeninfo vinfo = *pvinfo;
	struct fb_fix_screeninfo finfo = *pfinfo;

	printf("finfo: \n") ;
	printf("   id=%s\n", &finfo.id[0]) ;
	printf("   smem_start=%08x\n", (unsigned int)finfo.smem_start) ;
	printf("   smem_len=%d\n", finfo.smem_len) ;
	printf("   type=%d\n", finfo.type) ;
	printf("   type_aux=%d\n", finfo.type_aux) ;
	printf("   visual=%d\n", finfo.visual) ;
	printf("   xpanstep=%d\n", finfo.xpanstep) ;
	printf("   ypanstep=%d\n", finfo.ypanstep) ;
	printf("   ywrapstep=%d\n", finfo.ywrapstep) ;
	printf("   line_length=%d\n", finfo.line_length) ;
	printf("   mmio_start=%08x\n", (unsigned int)finfo.mmio_start) ;
	printf("   mmio_len=%d\n", finfo.mmio_len) ;
	printf("   accel=%d\n", finfo.accel) ;

	printf("vinfo: \n") ;
	printf("   xres=%d\n", vinfo.xres) ;
	printf("   yres=%d\n", vinfo.yres) ;
	printf("   xres_virt=%d\n", vinfo.xres_virtual) ;
	printf("   yres_virt=%d\n", vinfo.yres_virtual) ;
	printf("   xoffset=%d\n", vinfo.xoffset) ;
	printf("   yoffset=%d\n", vinfo.yoffset) ;

	printf("   bitsperpixel=%d\n", vinfo.bits_per_pixel) ;
	printf("   grayscale=%d\n", vinfo.grayscale) ;

/***
	struct fb_bitfield red) ;
	struct fb_bitfield green) ;
	struct fb_bitfield blue) ;
	struct fb_bitfield transp) ;
***/
	printf("   nonstd=%d\n", vinfo.nonstd) ;

	printf("   activate=%d\n", vinfo.activate) ;

	printf("   height=%d\n", vinfo.height) ;
	printf("   width=%d\n", vinfo.width) ;

	printf("   flags=%d\n", vinfo.accel_flags) ;

	/* Timing: All values in pixclocks, except pixclock (of course) */
	printf("   pixclk=%d\n", vinfo.pixclock) ;
	printf("   lmargin=%d\n", vinfo.left_margin) ;
	printf("   rmargin=%d\n", vinfo.right_margin) ;
	printf("   umargin=%d\n", vinfo.upper_margin) ;
	printf("   bmargin=%d\n", vinfo.lower_margin) ;
	printf("   hsync_len=%d\n", vinfo.hsync_len) ;
	printf("   vsync_len=%d\n", vinfo.vsync_len) ;
	printf("   sync=%d\n", vinfo.sync) ;
	printf("   vmode=%d\n", vinfo.vmode) ;
	printf("   rotate=%d\n", vinfo.rotate) ;


}
#endif
#endif

fbscreen_t *fbscreen_open()
{
	fbscreen_t *pfbuf = NULL ;
#ifndef i386
	int fbfd = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize = 0;
	char *fbp = 0;

	fbscreen.fd = -1 ;
	fbscreen.screensize = 0 ;
	memset(&fbscreen.surface, 0, sizeof(pixmap_t)) ;

	/* Open the file for reading and writing */
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd != -1)
	{
		/* Get fixed screen information */
		if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
			printf("Error reading fixed screen information.\n");
		}

		/* Get variable screen information */
		if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
			printf("Error reading variable screen information.\n");
			/* assume we are on K3, if so */
			vinfo.xres=600;
			vinfo.yres=800;
			vinfo.bits_per_pixel=4 ;
		}

		/* Figure out the size of the screen in bytes */
		screensize = (vinfo.xres * vinfo.yres * vinfo.bits_per_pixel) / 8;

		/* Map the device to memory */
		fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
		if (fbp != ((void*) -1))
		{
			fbscreen.fd = fbfd ;
			fbscreen.screensize = screensize ;
			fbscreen.surface.width = vinfo.xres ;
			fbscreen.surface.height = vinfo.yres ;
			fbscreen.surface.surf = (char *) fbp ;
			pfbuf = &fbscreen ;
#ifdef DEBUG_VINFO
			show_info(&finfo, &vinfo) ;
#endif
		}
		else
		{
			close(fbfd) ;
			printf("Error: failed to map framebuffer device to memory.\n");
		}
	}
	else
	{
		printf("Error: cannot open framebuffer device.\n");
	}
#endif
	return pfbuf ;
}

void fbscreen_close(fbscreen_t *pfbuf)
{
#ifndef i386
	if (pfbuf)
	{
		if ((pfbuf->fd != -1) && (pfbuf->surface.surf) && (pfbuf->screensize != 0))
		{
			munmap(pfbuf->surface.surf, pfbuf->screensize);
			close(pfbuf->fd);
			pfbuf->fd = -1 ;
			pfbuf->screensize = 0 ;
			memset(&pfbuf->surface, 0, sizeof(pixmap_t)) ;
		}
		else
		{
			fbscreen.fd = -1 ;
			fbscreen.screensize = 0 ;
			memset(&fbscreen.surface, 0, sizeof(pixmap_t)) ;
		}
	}
#endif
}

void screen_update_area(fbscreen_t *pfbuf, int mode, int x0, int y0, int w, int h, void *pbuf)
{
	update_area_t ua;

	ua.x1 = x0 ;
	ua.y1 = y0 ;
	ua.x2 = x0 + w  ;
	ua.y2 = y0 + h  ;
	ua.which_fx = mode ;
	ua.buffer = pbuf ;

#ifndef i386
	ioctl(pfbuf->fd, FBIO_EINK_UPDATE_DISPLAY_AREA, &ua);
#endif
}

struct flash_param
{
	fbscreen_t *pfbuf ;
	int count ;
	int x0 ;
	int y0 ;
	int w ;
	int h ;
	int delay ;
};

static void* flash_thread(void* param)
{
	update_area_t ua;
	pixmap_t *psave ;
	int n ;
	struct flash_param p ;

	p = *((struct flash_param *) param) ;

	psave = pixbuf_alloc(p.w, p.h) ;

	if (psave)
	{
		n = pixmap_xtract(psave, p.x0, p.y0, &(p.pfbuf->surface)) ;

		while (p.count > 0)
		{
			ua.x1 = p.x0 ;
			ua.y1 = p.y0 ;
			ua.x2 = p.x0 + p.w ;
			ua.y2 = p.y0 + p.h ;
			ua.which_fx = UMODE_INVERT ; // UMODE_PARTIAL ;
			ua.buffer = NULL ;

			//pixmap_invert(psave) ;
			//pix_blt(&(pfbuf->surface), x0, y0, psave) ;

#ifndef i386
			ioctl(p.pfbuf->fd, FBIO_EINK_UPDATE_DISPLAY_AREA, &ua);
#endif

			msdelay(p.delay) ;

			//pixmap_invert(psave) ;
			pix_blt(&(p.pfbuf->surface), p.x0, p.y0, psave) ;

			ua.which_fx = UMODE_PARTIAL ;
			ua.buffer = NULL ;

#ifndef i386
			ioctl(p.pfbuf->fd, FBIO_EINK_UPDATE_DISPLAY_AREA, &ua);
#endif

			msdelay(p.delay) ;

			--p.count ;
		}
		pixbuf_free(psave) ;
	}

	return NULL ;
}

void screen_flash_area(fbscreen_t *pfbuf, int count, int x0, int y0, int w, int h, int delay, int waitflag)
{
	pthread_t	thstat ;
	static struct flash_param p ;

	p.pfbuf = pfbuf ;
	p.count = count ;
	p.x0 = x0 ;
	p.y0 = y0 ;
	p.w = w ;
	p.h = h ;
	p.delay = delay ;

	if (waitflag)
		flash_thread((void *) &p) ;
	else
		pthread_create(&thstat, NULL, &flash_thread, (void *) &p) ;
}

int screen_string_at(fbscreen_t *pscreen, int x0, int y0, char *p)
{
	pixmap_t char_pixmap ;
	int nchars=0;
	int len ;
	int n, nlines ;
	int x ;

	len = strlen(p) ;

	get_char_pixmap((int) *p, &char_pixmap) ;
	nlines = char_pixmap.height ;

	if ((y0 + nlines) <= (pscreen->surface.height))
	{
		if ((x0 + char_pixmap.width) <= (pscreen->surface.width))
		{
			// number of chars that still fits
			n = ((pscreen->surface.width) - x0) / (char_pixmap.width-2) ;
			if (n>len)
				n = len ;
			nchars = n ;

			x = x0 ;
			do
			{
				pix_blt(&pscreen->surface, x, y0, &char_pixmap) ;
				++p ;
				x += char_pixmap.width ;
				get_char_pixmap((int) *p, &char_pixmap) ;
			} while (--n > 0) ;

			////screen_update_area(pscreen, UMODE_PARTIAL, x0, y0, nchars*(char_pixmap.width), nlines, NULL) ;
		}
	}

	return nchars ;
}

int screen_rect_at(fbscreen_t *pscreen, int x0, int y0, int w, int h, int color)
{
	return pix_fill(&(pscreen->surface), x0, y0, w, h, color) ;
}

int screen_get_pixmap(fbscreen_t *pscreen, int x0, int y0, pixmap_t *dst)
{
	return pixmap_xtract(dst, x0, y0, &(pscreen->surface));
}

int screen_put_pixmap(fbscreen_t *pscreen, int x0, int y0, pixmap_t *src)
{
	return pix_blt(&(pscreen->surface), x0, y0, src) ;
}
