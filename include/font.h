#ifndef _FONT_H_
#define _FONT_H_

#include "pixop.h"

#include "fnt12x22.h"

pixmap_t font_pixmap = {
		FNTWIDTH,
		FNTHEIGHT,
		(char *) &default_font[0]
};

int nbytesperchar = FNTHEIGHT * (((FNTWIDTH+1)*4)/8) ;

#endif
