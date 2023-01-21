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
#include <panel.h>

main()
{
int i;
char *s;

    foreground();
    winopen();
    doublebuffer();
    gconfig();
    drawit();
    for (;;) {
	cmov2i(100,100);
	s=g_getstring(WHITE, BLUE, RED, "Edit this string", 30);
	(void) fprintf (stdout,"s=%x  ",s);
	for (i=0;i<10;i++) (void) fprintf (stdout,"[%3d] ",s[i]); 
	(void) fprintf (stdout,"\n");
        if (s) (void) fprintf (stdout,"%s\n",s);
	drawit();
    }
}

drawit()
{
    color(BLACK);
    clear();
    cmov2i(100,100);
    color(RED);
    charstr("Edit this string");
    swapbuffers();
}
