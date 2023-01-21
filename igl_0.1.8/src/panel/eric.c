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

Actuator *hs, *vs;

Panel
*defpanel1(), *defpanel2(), *defpanel3();

main () 
{
  short col;

  foreground ();
  winopen ("demo");
  doublebuffer ();
  gconfig ();

  ortho2 (-1.0,1.0,-1.0,1.0);

  defpanel1 ();
  defpanel2 ();
  defpanel3 ();

  color (RED);

  for (;;) {
    (void) pnl_dopanel ();
    pushmatrix ();
    translate (hs->val,vs->val,0.0);
    col = getcolor ();
    color (BLACK);
    clear ();
    color (col);
    rectf (-.10,-.10,.10,.10);
    swapbuffers ();
    popmatrix ();
  }
}


typedef void (*PFV)();

struct button_hook
{
  Actuator act;
  Actuator *button;
};

set_my_button (b)
     struct button_hook *b;
{
  b->button->val = b->act.val;
  pnl_fixact(b->button);
}

Panel
*defpanel1 ()
{
  Panel *panel;
  Actuator *slave;
  struct button_hook *master;

  panel=pnl_mkpanel ();
  panel->label="panel one";
  panel->ppu=50.0;

  hs = pnl_mkact (PNL_HSLIDER);
  hs->label	= "y position";
  hs->x		=  1.0;
  hs->y		=  0.0;
  hs->minval	= -1.0;
  hs->maxval	=  1.0;
  pnl_addact (hs, panel);

  slave = pnl_mkact (PNL_TOGGLE_BUTTON);
  slave->label	= "I push myself";
  slave->x	= 1.0;
  slave->y	= 2.0;
  pnl_addact (slave, panel);

  master = PNL_MKUSERACT (button_hook, PNL_TOGGLE_BUTTON);
  master->button	= slave;
  master->act.label	= "push me";
  master->act.x	 	= 1.0;
  master->act.y	 	= 1.5;
  master->act.downfunc	= (PFV) set_my_button;
  pnl_addact (master, panel);

  return panel;
}

clean_exit ()
{
  exit (0);
}

Panel
*defpanel2 ()
{
  Panel *panel;
  Actuator *tem;

  panel = pnl_mkpanel ();
  panel->label	= "panel two";
  panel->ppu	= 50.0;

  vs = pnl_mkact (PNL_VSLIDER);
  vs->label	= "x position";
  vs->x		= 0.0;
  vs->y		= 0.0;
  vs->minval	=  -1.0;
  vs->maxval	= 1.0;
  pnl_addact (vs, panel);

  tem = pnl_mkact (PNL_BUTTON);
  tem->label	= "dump panel";
  tem->x	= 1.0;
  tem->y	= 1.5;
  tem->downfunc = (void (*)()) pnl_dumppanel;
  pnl_addact (tem, panel);

  tem = pnl_mkact (PNL_BUTTON);
  tem->label	= "exit";
  tem->x	= 1.0;
  tem->y	= 1.0;
  tem->downfunc = (void (*)()) clean_exit;
  pnl_addact (tem, panel);

  return panel;
}

struct color_struct
{
  Actuator act;
  Colorindex color;
} *cs;

set_color_func (cs)
 struct color_struct *cs;
{
  color (cs->color);
}

Panel
*defpanel3 ()
{
  Panel *panel;

  panel	= pnl_mkpanel ();
  panel->label	= "panel three";
  panel->ppu	= 50.0;

#define MAKE_COLOR_BUTTON(_color, _label, _x, _y, _val)	\
  cs = PNL_MKUSERACT (color_struct, PNL_RADIO_BUTTON);	\
  cs->color	= _color;				\
  cs->act.label	= _label;				\
  cs->act.x	= _x;					\
  cs->act.y	= _y;					\
  cs->act.val	= _val;					\
  cs->act.downfunc = (void (*)()) set_color_func;	\
  pnl_addact (cs, panel)

  MAKE_COLOR_BUTTON (RED,   "red",   1.0, 2.0, 1.0);
  MAKE_COLOR_BUTTON (GREEN, "green", 1.0, 1.5, 0.0);
  MAKE_COLOR_BUTTON (CYAN, " cyan",  1.0, 1.0, 0.0);

  return panel;
}
