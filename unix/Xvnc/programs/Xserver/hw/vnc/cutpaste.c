/*
 * cutpaste.c - routines to deal with cut & paste buffers / selection.
 */

/*
 *  Copyright (C) 1999 AT&T Laboratories Cambridge.  All Rights Reserved.
 *  Copyright (C) 2014 D. R. Commander.  All Rights Reserved.
 *
 *  This is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
 *  USA.
 */

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <stdio.h>
#include <X11/X.h>
#include <X11/Xproto.h>
#include "rfb.h"
#include "selection.h"
#include "property.h"
#include <X11/Xatom.h>

extern Selection *CurrentSelections;
extern int NumCurrentSelections;


static Bool inSetXCutText = FALSE;

/*
 * rfbSetXCutText sets the cut buffer to be the given string.  We also clear
 * the primary selection.  Ideally we'd like to set it to the same thing, but I
 * can't work out how to do that without some kind of helper X client.
 */

void
rfbSetXCutText(char *str, int len)
{
    inSetXCutText = TRUE;
    ChangeWindowProperty(screenInfo.screens[0]->root, XA_CUT_BUFFER0,
                         XA_STRING, 8, PropModeReplace, len,
                         (pointer)str, TRUE);
    inSetXCutText = FALSE;
}


void rfbGotXCutText(char *str, int len)
{
    if (!inSetXCutText)
        rfbSendServerCutText(str, len);
}
