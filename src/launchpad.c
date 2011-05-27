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


#include <sys/types.h>
#include <aio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

#include <sys/time.h>

#include "launchpad.h"
#include "statusbar.h"

static fbscreen_t *pfbscreen = NULL ;

int launchpad_init(void)
{
    pfbscreen = fbscreen_open() ;
    return -1;
}

void launchpad_deinit(void)
{
	if (pfbscreen)
	{
		fbscreen_close(pfbscreen) ;
		pfbscreen = NULL ;
	}
}

int show_message(char* string, int time)
{
	if(time < 1)
	    time = 1;
	int len = strlen(string);
	int count = (int)len/MAXLENGTH+1;
	pixmap_t *statusbar ;
	statusbar = statusbar_show(pfbscreen, count);
	statusbar_text(pfbscreen, statusbar, string, count);
	msdelay(400);
	statusbar_flash(pfbscreen, statusbar, 1, 800, 1, count);
	msdelay(time*1000);
	statusbar_hide(pfbscreen, statusbar, count);
	return 0;
}

