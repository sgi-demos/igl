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
#include <fcntl.h>
#include <gl.h>
#include <panel.h>

Actuator *messagewindow,*gridfiletypein,*solnfiletypein;
Panel *defpanel();

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
cancelfunc()
{
    exit(0);
}

void
openfiles(a)
Actuator *a;
{
int gridfd, solnfd;
int n=0;
Typein *adgrid=(Typein *)gridfiletypein->data;
Typein *adsoln=(Typein *)solnfiletypein->data;
Typeout *adout=(Typeout *)messagewindow->data;
char *p=adout->buf;
    
    if (!strcmp(adgrid->str,"")) {
	ringbell();
	tprint(messagewindow,"null grid file name");
	return;
    }

    if (!strcmp(adsoln->str,"")) {
	ringbell();
	tprint(messagewindow,"null solution file name");
	return;
    }

    gridfd=open(adgrid->str, O_RDONLY);
    if (gridfd<0) {
	tprint(messagewindow,"can't open grid file");
	return;
    }

    solnfd=open(adsoln->str, O_RDONLY);
    if (solnfd<0) {
	tprint(messagewindow,"can't open solution file");
	return;
    }
}

Panel
*defpanel()
{
Actuator *a;
Panel *p;

    p=pnl_mkpanel();
    p->label="file window";
    p->x=200;
    p->y=100;

    a=pnl_mkact(pnl_wide_button);
    a->x=1.0;
    a->y=0.0;
    a->label="ok";
    a->upfunc=openfiles;
    pnl_addact(a,p);

    a=pnl_mkact(pnl_wide_button);
    a->x=6.0;
    a->y=0.0;
    a->label="cancel";
    a->upfunc=cancelfunc;
    pnl_addact(a,p);

    gridfiletypein=pnl_mkact(pnl_typein);
    gridfiletypein->x=1.0;
    gridfiletypein->y=1.0;
    gridfiletypein->label="grid file";
    gridfiletypein->labeltype=PNL_LABEL_LEFT;
    PNL_ACCESS(Typein,gridfiletypein,len)=60;
    pnl_addact(gridfiletypein,p);

    solnfiletypein=pnl_mkact(pnl_typein);
    solnfiletypein->x=1.0;
    solnfiletypein->y=1.5;
    solnfiletypein->label="solution file";
    solnfiletypein->labeltype=PNL_LABEL_LEFT;
    PNL_ACCESS(Typein,solnfiletypein,len)=60;
    pnl_addact(solnfiletypein,p);

    messagewindow=pnl_mkact(pnl_typeout);
    messagewindow->x=0.0;
    messagewindow->y=2.5;
    messagewindow->label="messages";
    PNL_ACCESS(Typeout,messagewindow,col)=80;
    PNL_ACCESS(Typeout,messagewindow,lin)=20;
    pnl_addact(messagewindow,p);
}

