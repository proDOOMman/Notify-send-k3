/*
* Copyright (C) 2010 Andy M. aka h1uke	h1ukeguy @ gmail.com
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


#ifndef _LAUNCHPAD_H_
#define _LAUNCHPAD_H_

#include <stdio.h>

extern FILE * get_diag_fp() ;

#define DIAG_PRINT(format, ...)  fprintf(get_diag_fp(), format, ##__VA_ARGS__)

#define KEYBOARD_INPUT	"/dev/input/event0"
#define FIVEWAY_INPUT	"/dev/input/event1"

#define K3_VOL_INPUT	"/dev/input/event2"

#define PROC_KEYPAD_FILE	"/proc/keypad"
#define PROC_FIVEWAY_FILE	"/proc/fiveway"

/* there can be up to MAXKEY keys
 * each generating a code in range of 0..MAXCODE
 */
#define MAXKEYS	96
#define MAXCODE 255

#define MAXPATH 256
#define MAXNAME 16

/* longest hotkey sequence supported */
#define MAXSEQ	8

#define ACTION_SENDKEYS			0
#define ACTION_SHELL_COMMAND	1
#define ACTION_KINDLE_KEYS		2
#define ACTION_KINDLE_SCRIPT	3

#define SENDMODE_NORMAL	0
#define SENDMODE_SHIFT	1
#define SENDMODE_ALT	2


struct input_key
{
	int input_key_code ;
	char input_key_name[MAXNAME] ;
} ;

struct input_key_alias
{
	int input_key_code ;
	int substitute_code ;
} ;

/*
 * this supports navigation within the kindle symbols input popup.
 * When opened, the popup shows 33 symbols in a table with cursor pointing to the comma.
 * signed hsteps and vsteps values below contain number of horizontal and vertical steps
 * needed to navigate to a particular symbol
 */
struct kindle_syms
{
	int hsteps ;
	int vsteps ;
};

/*
 * this structure is used when simulating ascii data input on Kindle
 */
struct ascii_char
{
	int code ;						/* keypad code */
	int sendmode ;					/* choice of "send" "sendhift" and "sendalt" when sending directly */
	struct kindle_syms *kindle_fwork_sequence ;	/* specific code sequence to be used when sending to kindle framework */
} ;

struct action
{
	int type ;
	int len ;	/* number of key codes in sequence */
	short sequence[MAXSEQ] ;
	char command[MAXPATH] ;
	char dispname[MAXNAME] ;
};

int has_console(void) ;

int launchpad_init(void) ;
void launchpad_deinit(void) ;
int show_message(char* string, int time);

extern void msdelay(int) ;

#endif
