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
#include <device.h>
#include <panel.h>

Panel
*defpanel();

main() 
{
  Actuator *a;
  Panel *panel;
  
  foreground();
  noport();
  winopen("demo");
  
  doublebuffer();
  gconfig();
  
  panel=defpanel();
  
  for (;;) {
    pnl_dopanel();
    swapbuffers();
  }
}

void cleanexit() {exit(0);}

#define MKACT(_a, _type,_label)	\
  a=(_a)=pnl_mkact(_type);		\
  (_a)->label=_label

#define ADDACT \
  a->x=x;			\
  a->y=(y-=a->h+dly+PNL_DIM_1);	\
  pnl_addact(a, p)

#define OVER	\
  x+=a->w+dlx+PNL_DIM_1;	\
  y+=a->h+dly+PNL_DIM_1

Panel
*defpanel()
{
  Actuator *a;
  Panel *p;
  Coord y, x, d=1.0, dlx, dly;

  p=pnl_mkpanel();
  p->label="labels";

/* the buttons */

  x=y=0;
  dlx=dly=0.0;

  MKACT(a, pnl_puck, "upper left");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_UPPER_LEFT;
  ADDACT; OVER;
  MKACT(a, pnl_puck, "top left");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_TOP_LEFT;
  ADDACT; OVER;
  MKACT(a, pnl_puck, "top");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_TOP;
  ADDACT; OVER;
  MKACT(a, pnl_puck, "top right");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_TOP_RIGHT;
  ADDACT; OVER;
  MKACT(a, pnl_puck, "upper right");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_UPPER_RIGHT;
  ADDACT;
  MKACT(a, pnl_puck, "right top");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_RIGHT_TOP;
  ADDACT;
  MKACT(a, pnl_puck, "right");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_RIGHT;
  ADDACT;
  MKACT(a, pnl_puck, "right bottom");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_RIGHT_BOTTOM;
  ADDACT;
  MKACT(a, pnl_puck, "lower right");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_LOWER_RIGHT;
  ADDACT;
  
  x=0;
  y=0-(a->h+dly+PNL_DIM_1);
  
  MKACT(a, pnl_puck, "left top");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_LEFT_TOP;
  ADDACT;
  MKACT(a, pnl_puck, "left");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_LEFT;
  ADDACT;
  MKACT(a, pnl_puck, "left bottom");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_LEFT_BOTTOM;
  ADDACT;
  MKACT(a, pnl_puck, "lower left");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_LOWER_LEFT;
  ADDACT; OVER;
  MKACT(a, pnl_puck, "bottom left");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_BOTTOM_LEFT;
  ADDACT; OVER;
  MKACT(a, pnl_puck, "bottom");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_BOTTOM;
  ADDACT; OVER;
  MKACT(a, pnl_puck, "bottom right");
  a->w=a->h=2;
  a->labeltype=PNL_LABEL_BOTTOM_RIGHT;
  ADDACT; OVER;
  
  return p;
}

