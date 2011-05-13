#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <fcntl.h>
#include <errno.h>

#include "font.h"
#include "fntdefs.h"
#include "pixop.h"


static int code_first = 0, code_last = 255 ;

pixmap_t * getfngfont(char *path)
{
	int fd ;
	unsigned long flen ;
	unsigned char *pfng ;
	pixmap_t * rc = &font_pixmap ;

	if (path == NULL)
		return rc ;		// default font is defined statically

	if ((fd = open(path, O_RDONLY)) >= 0)
	{
		flen = lseek(fd, 0, SEEK_END) ;
		lseek(fd, 0, SEEK_SET) ;

		printf("%s: %d bytes\n", path, (int)flen) ;

		if ((pfng=malloc(flen)) != NULL)
		{
			if ((read(fd,pfng,flen)) == flen)
			{
				if (memcmp(pfng,"FNG ", 4) == 0)
				{
					code_first = ((struct fngheader *)pfng)->firstcode ;
					code_last = ((struct fngheader *)pfng)->lastcode ;

					rc->surf = (char *)pfng + sizeof(struct fngheader) ; // skip the fng header
					rc->width = ((struct fngheader *)pfng)->wd ;
					rc->height = ((struct fngheader *)pfng)->ht ;
					nbytesperchar = rc->height * (((rc->width*1)*4)/8) ;
				}
				else
				{
					printf("%s -- wrong format\n", path) ;
					free(pfng) ;
					close(fd) ;
				}
			}
			else
			{
				printf("Can't read %d bytes\n", (int) flen) ;
				free(pfng) ;
				close(fd) ;
			}
		}
		else
		{
			printf("Can't allocate %d bytes\n", (int)flen) ;
			close(fd) ;
		}
	}
	else
		perror(path) ;

	return rc ;
}

int get_char_pixmap(int code, pixmap_t *ppx)
{
	const char *p ;

	code &= 0xff ;


	if ((code < code_first) || (code > code_last))
		code = code_first ;

	p = font_pixmap.surf + (code-code_first)*nbytesperchar ;

	ppx->width = font_pixmap.width ;
	ppx->height = font_pixmap.height ;
	ppx->surf = (char *) p ;

	return (code) ;
}

void get_font_metrics(int *pw, int *ph)
{
	*pw = font_pixmap.width ;
	*ph = font_pixmap.height ;
}

void freefngfont(pixmap_t *p)
{
	if (p)
	{
		if (p->surf != (char *)&default_font[0])
		free(p - sizeof(struct fngheader)) ;
	}
}
