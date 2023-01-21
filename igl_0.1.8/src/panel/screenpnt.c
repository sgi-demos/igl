/*****************************************************************
    Panel Library/Electropaint Copyright (c) 1986 David A. Tristram.
    Electropaint (TM) is a Registered U.S. Trademark of Tristram Visual.
    Tristram Visual can be contacted at www.tristram.com.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
*****************************************************************/
#include <gl.h>

Boolean
screenpnt(wx, wy, wz, sx, sy)	/* returns false if pnt is clipped */
Coord wx, wy, wz;
Screencoord *sx, *sy;
{
short buf[3];

    feedback(buf, 3);
    pnt(wx, wy, wz);
    passthrough(0x99);
    endfeedback(buf);
    if (buf[0]==0x8) { /* passthrough command */
	*sx= *sy=0;
	return FALSE;
    } else {
        *sx=buf[1];
	*sy=buf[2];
	return TRUE;
    }
}

