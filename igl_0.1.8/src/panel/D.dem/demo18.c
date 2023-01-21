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
#include <stdio.h>
#include <gl.h>
#include <panel.h>

Actuator *filewindow;
Panel *defpanel();

#define FILE_WINDOW_BUFSIZ  16386

main() 
{
    foreground();
    noport();
    winopen("typeout demo");
    winconstraints();
    winconstraints();

    doublebuffer();
    gconfig();

    defpanel();

    for (;;) {
        pnl_dopanel();
	swapbuffers();
    }
}

void
cleanexit()
{
    exit(0);
}

void
getfile(a)
Actuator *a;
{
FILE *stream;
int n=0;
Typein *adin=(Typein *)a->data;
Typeout *adout=(Typeout *)filewindow->data;
char *p=adout->buf;
    
    stream=fopen(adin->str, "r");
    if (!stream) {
	(void) strcpy(adout->buf,"can't open file");
	goto end;
    }

    do {
	n=fread(p,sizeof(char),
	    FILE_WINDOW_BUFSIZ-(p-adout->buf),stream);
    	p+=n;
    } while (n>0);
    *p='\0';	/* mark the end */
    fclose(stream);

end:
    adout->start=0;
    adout->dot=0;
    pnl_fixact(filewindow);		    /* bake changes, mark dirty */
}

Panel
*defpanel()
{
Actuator *a;
Panel *p;

    p=pnl_mkpanel();
    p->label="file viewer";
    p->x=200;
    p->y=100;

    a=pnl_mkact(pnl_wide_button);
    a->x=6.0;
    a->y=0.0;
    a->label="exit";
    a->upfunc=cleanexit;
    pnl_addact(a,p);

    a=pnl_mkact(pnl_typein);
    a->x=2.0;
    a->y=1.0;
    a->upfunc=getfile;
    a->label="file";
    a->labeltype=PNL_LABEL_LEFT;
    PNL_ACCESS(Typein,a,len)=60;
    pnl_addact(a,p);

    filewindow=pnl_mkact(pnl_typeout);
    filewindow->x=0.0;
    filewindow->y=2.0;
    PNL_ACCESS(Typeout,filewindow,col)=80;
    PNL_ACCESS(Typeout,filewindow,lin)=40;
    PNL_ACCESS(Typeout,filewindow,size)=FILE_WINDOW_BUFSIZ;
    pnl_addact(filewindow,p);
}

