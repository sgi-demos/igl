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

Actuator *mslider, *freqslider, *ulbar, *valbar, *llbar;
  
Panel *defpanel();

main() 
{
  Panel *p;

  foreground();
  noport();
  winopen("demo");
  
  doublebuffer();
  gconfig();
  
  p=defpanel();
  
  for (;;) {
    pnl_dopanel();
    animatesliders();
    swapbuffers();
  }
}

animatesliders()
{
  static float sense=1;
  float dx=sense*freqslider->val*
    (ulbar->extval-llbar->extval)/(mslider->maxval-mslider->minval);

  if (freqslider->val==0.0) return;

  valbar->extval+=dx;

  if (valbar->extval<llbar->extval) {
    valbar->extval= -valbar->extval+2*llbar->extval;
    sense=1;
  } else if (valbar->extval>ulbar->extval) {
    valbar->extval= -valbar->extval+2*ulbar->extval;
    sense= -1;
  }

  valbar->extval=RANGE(valbar->extval, llbar->extval, ulbar->extval);
  pnl_fixact(mslider); 
}

float range, center;

void saverange(a)
Actuator *a;
{
  range=ulbar->extval-llbar->extval;
  center=ulbar->extval-range/2.0;
}

void setcenter(a)
Actuator *a;
{
  llbar->extval=ulbar->extval-range;
  llbar->extval=RANGE(llbar->extval, mslider->minval, ulbar->extval);
  valbar->extval=RANGE(valbar->extval, llbar->extval, ulbar->extval);
  saverange(a);
  pnl_fixact(mslider);
}

void setrange(a)
Actuator *a;
{
  ulbar->extval=2*center-llbar->extval;
  ulbar->extval=RANGE(ulbar->extval, llbar->extval, mslider->maxval);
  valbar->extval=RANGE(valbar->extval, llbar->extval, ulbar->extval);
  saverange(a);
  pnl_fixact(mslider);
}

Panel 
*defpanel()
{
  Actuator *a;
  Panel *p;
  Coord x, y, d=1.0, dl=0.5;

  p=pnl_mkpanel();
  p->label="modulation";

  mslider=a=pnl_mkact(pnl_multislider);
  PNL_ACCESS(Multislider, a, mode)=PNL_MSM_ORDERED;
  PNL_ACCESS(Multislider, a, n)=0;
  a->label="mslider";
  a->maxval=1;
  a->minval= -1;
  a->val=0;
  pnl_addact(a, p);

  ulbar=a=pnl_mkact(pnl_multislider_bar);
  a->extval= 1;
  a->label="< hi";
  a->labeltype=PNL_LABEL_RIGHT;
  a->downfunc=saverange;
  a->activefunc=setcenter;
  pnl_addsubact(a, mslider);

  valbar=a=pnl_mkact(pnl_multislider_bar);
  a->extval=0;
  pnl_addsubact(a, mslider);

  llbar=freqslider=a=pnl_mkact(pnl_hslider);
  a->extval= -1;
  a->maxval=(mslider->maxval-mslider->minval)/2.0;
  a->label="< lo";
  a->labeltype=PNL_LABEL_RIGHT;
  PNL_ACCESS(Slider, a, mode)|=PNL_SM_NOSNAP;
  a->downfunc=saverange;
  a->activefunc=setrange;
  pnl_addsubact(a, mslider);

  return p;
}

