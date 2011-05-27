/*
* Copyright (C) 2010 Andy M. aka h1uke	h1ukeguy @ gmail.com
* Copyright (C) 2011 Stanislav (proDOOMman) Kosolapov <prodoomman@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <stddef.h>

#include "fntdefs.h"
#include "screen.h"
#include "pixop.h"
#include "statusbar.h"

pixmap_t * statusbar_show(fbscreen_t *pscreen, int lines)
{
	pixmap_t *psav ;

	psav = pixbuf_alloc(pscreen->surface.width, STATUSBAR_HEIGHT*lines) ;
	screen_get_pixmap(pscreen, 0, 0, psav) ;

	screen_rect_at(pscreen, 0, 0,
			pscreen->surface.width, STATUSBAR_HEIGHT*lines, 0) ;
	screen_update_area(pscreen, UMODE_PARTIAL, 0, 0,
			pscreen->surface.width, STATUSBAR_HEIGHT*lines, NULL) ;

	return psav ;
}

void statusbar_hide(fbscreen_t *pscreen, pixmap_t *psav, int lines)
{

	screen_put_pixmap(pscreen, 0, 0, psav ) ;
	screen_update_area(pscreen, UMODE_FULL, 0, 0,
			pscreen->surface.width, STATUSBAR_HEIGHT*lines, NULL) ;
	pixbuf_free(psav) ;
}

void statusbar_text(fbscreen_t *pscreen, pixmap_t *psb, char *p, int lines)
{
	screen_rect_at(pscreen, 0, 0,
			pscreen->surface.width, STATUSBAR_HEIGHT*lines, 0) ;
	int count = 0;
	while(count<strlen(p))
	{
	    char str[MAXLENGTH+1];
	    str[MAXLENGTH] = '\0';
	    strncpy(str,p+count,MAXLENGTH);
	    screen_string_at(pscreen, 30, 10+STATUSBAR_HEIGHT*count/MAXLENGTH, str);
	    count += MAXLENGTH;
	}
}

void statusbar_flash(fbscreen_t *pscreen, pixmap_t *psb, int count, int tmo, int waitflag, int lines)
{
	screen_flash_area(pscreen, count, 0, 0,
		pscreen->surface.width, STATUSBAR_HEIGHT*lines, tmo, waitflag) ;
}
