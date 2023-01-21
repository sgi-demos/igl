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
  
static void _newvaltypein(a,p,x,y)
Actuator *a;
Panel *p;
Coord x, y;
{
  pnl_setdirty(a);
  if (!a->active) return;
  
  frontbuffer(TRUE);	/* hack hack hack */
  (*a->drawfunc)(a,p);
  frontbuffer(FALSE);
  cmov2((Coord)(a->x+PNL_DIM_3),
	(Coord)(a->y+PNL_DIM_3)+(Coord)getdescender()/p->ppu);
  (void) strcpy(PNL_ACCESS(Typein,a,str),
		g_mode_getstring(pnl_white_color,
				 pnl_highlight_color,
				 pnl_normal_color,
				 PNL_ACCESS(Typein,a,str),
				 PNL_ACCESS(Typein,a,len),
				 PNL_ACCESS(Typein,a,mode)));


  if (PNL_ACCESS(Typein,a,mode)&PNL_TIM_TERM_ENTER) {
    /* simulate a mouseup and shut this thing off */
    pnl_justup=TRUE;	      /* hack hack hack */
    pnl_setinactive(pnl_ca,pnl_cp); /* hackus maximus */
  }

#if 0	/* 3000's seem to work now */
#ifndef IRIS_4D
  pnl_justup=TRUE;	      /* hack hack hack */
  pnl_setinactive(pnl_ca,pnl_cp); /* hackus maximus */
  
  /* the 4d version of g_gets() seems to preserve mouse up events
     better than the 3000 does */
#endif	IRIS_4D	
#endif 0
}

static void
  _addtypein(a, p)
Actuator *a;
Panel *p;
{
  int len;
  char *str = (char *)pnl_alloc(PNL_TYPEIN_MAX_STRING);
  
  if (PNL_ACCESS(Typein, a, str))
    (void) strcpy(str,PNL_ACCESS(Typein, a, str));
  PNL_ACCESS(Typein, a, str)=str;
  
  len = (PNL_ACCESS(Typein, a, len)?
	 PNL_ACCESS(Typein, a, len):
	 PNL_TYPEIN_LENGTH);
  PNL_ACCESS(Typein, a, len)=len;
  a->w=(float)len*(float)strwidth("a")/p->ppu+2.0*PNL_DIM_3;
  a->h=2.0*PNL_DIM_3+(Coord)getheight()/p->ppu;
  a->datasize=sizeof(Typein)+PNL_TYPEIN_MAX_STRING;
}

static void
  _fixtypein(a)
Actuator *a;
{
  Typein *ad = (Typein *)a->data;
  
  a->w=(float)ad->len*(float)strwidth("a")/a->p->ppu+2.0*PNL_DIM_3;
}


static void _dumptypein(a, fd)
     Actuator *a;
     int fd;
{
  Typein *ad=(Typein *)a->data;
  static int msgtype=PNL_MT_STATE;
  
  (void) write(fd, (char *) &msgtype, sizeof(msgtype));
  (void) write(fd, (char *) &a->id, sizeof(a->id));
  (void) write(fd, (char *) a, sizeof(Actuator));
  (void) write(fd, (char *) &a->datasize, sizeof(int));
  (void) write(fd, a->data, sizeof(Typein));
  (void) write(fd, ad->str, PNL_TYPEIN_MAX_STRING);
}

static void _loadtypein(a, fd)
     Actuator *a;
     int fd;
{
  Typein *ad=(Typein *)a->data;
  
  (void) read(fd, (char *) a, sizeof(Actuator));
  (void) read(fd, (char *) &a->datasize, sizeof(int));
  (void) read(fd, a->data, sizeof(Typein));
  (void) read(fd, ad->str, PNL_TYPEIN_MAX_STRING);
}

static void
  _drawtypein(a, p)
Actuator *a;
Panel *p;
{
  if (!a->dirtycnt) return;
  
#ifdef DEBUG
  fprintf (stdout,"drawing typein %s, active:%d cnt:%d\n",
	 a->label?a->label:"<no label>", a->active, a->dirtycnt);
#endif DEBUG
  if (a->active) color(pnl_highlight_color);
  else	   color(pnl_normal_color);
  rectf(a->x,a->y,a->x+a->w,a->y+a->h);
  color(pnl_black_color);
  rect(a->x,a->y,a->x+a->w,a->y+a->h);
  if (a->active) color(pnl_white_color);
  else	   color(pnl_label_color);
  cmov2(a->x+PNL_DIM_3,a->y+PNL_DIM_3+(Coord)getdescender()/p->ppu);
  charstr(PNL_ACCESS(Typein,a,str));
  if (a->beveled) pnl_drawbevel(a, p);
  if (a->label) pnl_drawlabel(a, p);
}

void
  pnl_oldtypein(a)
Actuator *a;
{
  a->type=PNL_TYPEIN;
  
  a->labeltype=PNL_LABEL_BOTTOM;
  a->data = (char *) pnl_alloc(sizeof(Typein));
  a->datasize=sizeof(Typein);
  PNL_ACCESS(Typein, a, len) = 0;
  PNL_ACCESS(Typein, a, str) = "";
  a->addfunc=_addtypein;
  a->fixfunc=_fixtypein;
  a->newvalfunc=_newvaltypein;
  a->dumpfunc=_dumptypein;
  a->drawfunc=_drawtypein;
}

