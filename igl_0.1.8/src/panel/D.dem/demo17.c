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

Actuator *poemwindow;
Panel *defpanel();

#define POEM_WINDOW_BUFSIZ  16386
#define POEM \
"Mary had a little spam.\n\
It felt as good as gold.\n\
And when she left it out at night,\n\
It shivered in the cold."


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
addtext(a)
Actuator *a;
{
    strcat(PNL_ACCESS(Typeout, poemwindow, buf), "\n");
    strcat(PNL_ACCESS(Typeout, poemwindow, buf),
	   PNL_ACCESS(Typein, a, str));

    pnl_fixact(poemwindow);
}

void
setdelim(a)
Actuator *a;
{
  if (a->val==1.0) PNL_ACCESS(Typeout, poemwindow, delimstr) = "\t\n ";
  else		   PNL_ACCESS(Typeout, poemwindow, delimstr) = NULL;
}

void
setnocursor(a)
Actuator *a;
{
  if (a->val==1.0) PNL_ACCESS(Typeout, poemwindow, mode) |= PNL_TOM_NOCURSOR;
  else		   PNL_ACCESS(Typeout, poemwindow, mode) &= ~PNL_TOM_NOCURSOR;
  pnl_fixact(poemwindow);
}

void
setnoregion(a)
Actuator *a;
{
  if (a->val==1.0) PNL_ACCESS(Typeout, poemwindow, mode) |= PNL_TOM_NOREGION;
  else		   PNL_ACCESS(Typeout, poemwindow, mode) &= ~PNL_TOM_NOREGION;
  pnl_fixact(poemwindow);
}

Panel
*defpanel()
{
Actuator *a;
Panel *p;

    p=pnl_mkpanel();
    p->label="typeout demo";
    p->x=200;
    p->y=100;

    a=pnl_mkact(pnl_wide_button);
    a->x=1.5;
    a->y= -2.0;
    a->label="exit";
    a->upfunc=cleanexit;
    pnl_addact(a,p);

    a=pnl_mkact(pnl_toggle_button);
    a->x=4.5;
    a->y= -2.0;
    a->label="auto word select";
    a->downfunc=setdelim;
    pnl_addact(a,p);

    a=pnl_mkact(pnl_toggle_button);
    a->x=4.5;
    a->y= -2.5;
    a->label="no cursor";
    a->downfunc=setnocursor;
    pnl_addact(a,p);

    a=pnl_mkact(pnl_toggle_button);
    a->x=4.5;
    a->y= -3.0;
    a->label="no region";
    a->downfunc=setnoregion;
    pnl_addact(a,p);

    a=pnl_mkact(pnl_typein);
    a->x=1.0;
    a->y=0.0;
    a->upfunc=addtext;
    a->label="a typein actuator";
    PNL_ACCESS(Typein,a,len)=60;
    PNL_ACCESS(Typein,a,str)="Edit This String";
    PNL_ACCESS(Typein,a,mode)=PNL_TIM_TERM_ENTER;

    pnl_addact(a,p);

    poemwindow=pnl_mkact(pnl_typeout);
    poemwindow->x=1.0;
    poemwindow->y=1.0;
    poemwindow->labeltype=PNL_LABEL_TOP;
    poemwindow->label="a typeout actuator";
    PNL_ACCESS(Typeout,poemwindow,size)=POEM_WINDOW_BUFSIZ;
    pnl_addact(poemwindow,p); /* addact does the malloc */
    (void) strcpy(PNL_ACCESS(Typeout,poemwindow,buf), POEM);
    pnl_fixact(poemwindow);	    /* bake in the string */
}

