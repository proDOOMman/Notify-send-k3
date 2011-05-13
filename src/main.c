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


#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>

#include <linux/input.h>
#include <libgen.h>

#include <sys/file.h>
#include <poll.h>

#include "version.h"

#include "launchpad.h"
#include "screen.h"
#include "fntdefs.h"

static char proc_dir[MAXPATH] ;

static FILE * diag_fp = NULL ;

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

void close_diag_fp() ;

static volatile int keepgoing = 1 ;
static volatile int reinit = 0 ;

char *my_process_directory(void)
{
	return &proc_dir[0] ;
}

int main(int argc, char **av)
{
	launchpad_init();
	if(argc>1)
		show_message(av[1]);
	return EXIT_SUCCESS;
}

void msdelay(int msecs)
{
	struct timespec tv , rem;
	unsigned long v = msecs ;

	tv.tv_nsec = (v%1000) * 1000000 ;
	tv.tv_sec = v/1000 ;

	while (nanosleep(&tv, &rem) == EINTR)
		tv = rem ;
}
