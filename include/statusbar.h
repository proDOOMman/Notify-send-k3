#ifndef _STATUSBAR_H_
#define _STATUSBAR_H_

#include "fntdefs.h"
#include "screen.h"
#include "pixop.h"

#define STATUSBAR_HEIGHT 40

#define MAXLENGTH 45

pixmap_t * statusbar_show(fbscreen_t *pscreen, int lines) ;
void statusbar_hide(fbscreen_t *pscreen, pixmap_t *psav, int lines) ;
void statusbar_text(fbscreen_t *pscreen, pixmap_t *psb, char *p, int lines) ;
void statusbar_flash(fbscreen_t *pscreen, pixmap_t *psb, int count, int tmo, int waitflag, int lines) ;

#endif
