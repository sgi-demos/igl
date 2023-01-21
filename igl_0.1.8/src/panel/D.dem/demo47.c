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

Actuator *ms;
Panel
*defpanel();

Actuator *add_button, *delete_button,
  *free_slider, *ordered_slider, *constrained_slider;
  
main() 
{
  foreground();
  noport();
  winopen("demo");
  
  doublebuffer();
  gconfig();
  
  defpanel();
  
  for (;;) {
    pnl_dopanel();
    swapbuffers();
  }
}

void
setaddmode(a)
Actuator *a;
{
  if (a->val==1.0) {
    PNL_ACCESS(Multislider, free_slider, mode) |= PNL_MSM_ADD;
    PNL_ACCESS(Multislider, ordered_slider, mode) |= PNL_MSM_ADD;
    PNL_ACCESS(Multislider, constrained_slider, mode) |= PNL_MSM_ADD;
    PNL_ACCESS(Multislider, free_slider, mode) &= ~PNL_MSM_DELETE;
    PNL_ACCESS(Multislider, ordered_slider, mode) &= ~PNL_MSM_DELETE;
    PNL_ACCESS(Multislider, constrained_slider, mode) &= ~PNL_MSM_DELETE;
  } else {
    PNL_ACCESS(Multislider, free_slider, mode) &= ~PNL_MSM_ADD;
    PNL_ACCESS(Multislider, ordered_slider, mode) &= ~PNL_MSM_ADD;
    PNL_ACCESS(Multislider, constrained_slider, mode) &= ~PNL_MSM_ADD;
  }
}

void
setdeletemode(a)
Actuator *a;
{
  if (a->val==1.0) {
    PNL_ACCESS(Multislider, free_slider, mode) |= PNL_MSM_DELETE;
    PNL_ACCESS(Multislider, ordered_slider, mode) |= PNL_MSM_DELETE;
    PNL_ACCESS(Multislider, constrained_slider, mode) |= PNL_MSM_DELETE;
    PNL_ACCESS(Multislider, free_slider, mode) &= ~PNL_MSM_ADD;
    PNL_ACCESS(Multislider, ordered_slider, mode) &= ~PNL_MSM_ADD;
    PNL_ACCESS(Multislider, constrained_slider, mode) &= ~PNL_MSM_ADD;
  } else {
    PNL_ACCESS(Multislider, free_slider, mode) &= ~PNL_MSM_DELETE;
    PNL_ACCESS(Multislider, ordered_slider, mode) &= ~PNL_MSM_DELETE;
    PNL_ACCESS(Multislider, constrained_slider, mode) &= ~PNL_MSM_DELETE;
  }
}

void
unsetmodes()
{
  add_button->val=delete_button->val=0.0;
  pnl_fixact(add_button);
  pnl_fixact(delete_button);
}

void
unsetmodesifoff(a)
     Actuator *a;
{
  if (a->val==0.0) unsetmodes(a);
}

void
_newvaltoggleradiobutton(a,p,x,y)
Actuator *a;
Panel *p;
Coord x,y;
{
  float saveval=a->val;
  void _newvaltogglebutton();
  void _newvalradiobutton();


  _newvalradiobutton(a,p,x,y);
  if (pnl_justdown) a->val=(saveval?0.0:1.0);
}


#define MKACT(_a, _type,_label)	\
  a=(_a)=pnl_mkact(_type);		\
  (_a)->label=_label

#define ADDACT \
  a->x=x;			\
  a->y=(y-=a->h+dl+PNL_DIM_1);	\
  pnl_addact(a, p)

#define OVER	\
  x+=a->w+PNL_DIM_1;	\
  y+=a->h+dl+PNL_DIM_1


Panel
*defpanel()
{
  Actuator *a;
  Panel *p;
  Coord x, y, d=1.0, dl=1.0;

  p=pnl_mkpanel();
  p->label="horizontal multisliders";

  MKACT(a, pnl_hmultislider, "free");
  a->selectable=0;
  free_slider=a;
  PNL_ACCESS(Multislider, a, mode)=PNL_MSM_FREE;
  ADDACT;

  MKACT(a, pnl_hmultislider, "ordered");
  ordered_slider=a;
  PNL_ACCESS(Multislider, a, mode)=PNL_MSM_ORDERED;
  ADDACT;

  MKACT(a, pnl_hmultislider, "constrained");
  constrained_slider=a;
  PNL_ACCESS(Multislider, a, mode)=PNL_MSM_CONSTRAINED;
  ADDACT;

  y-=1;

  MKACT(a, pnl_radio_button, "add");
  add_button=a;
  a->newvalfunc=_newvaltoggleradiobutton;
  a->downfunc=setaddmode;
  ADDACT;

  dl=0;

  MKACT(a, pnl_radio_button, "delete");
  delete_button=a;
  a->newvalfunc=_newvaltoggleradiobutton;
  a->downfunc=setdeletemode;
  ADDACT;

  return p;
}

