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

void setpaletteval(a, x, dist)
     Actuator *a;
     Coord x;
     float dist;
{
  Palette *ad=(Palette *)a->data;

  if (pnl_justdown) {
    a->val=PNL_WORLD_TO_VAL(x, a->minval, a->maxval, dist);
    ad->wsave=x;
  }
  
  if (pnl_controlkey)  a->val+= ad->finefactor
 			 *(x-ad->wsave)/dist*(a->maxval-a->minval);
  else {
    a->val=PNL_WORLD_TO_VAL(x, a->minval, a->maxval, dist);
  }
  a->val=RANGE(a->val, a->maxval, a->minval);
  
  ad->wsave=x;
}

void _newvalvpalette(a,p,x,y)
     Actuator *a;
     Panel *p;
     Coord x, y;
{    
  Palette *ad=(Palette *)a->data;
  float tmp;
  
  if (!a->active) return;
  
  setpaletteval(a, y-a->y, a->h);
  pnl_setdirty(a);
}

void _newvalhpalette(a,p,x,y)
     Actuator *a;
     Panel *p;
     Coord x, y;
{    
  Palette *ad=(Palette *)a->data;
  
  if (!a->active) return;
  
  setpaletteval(a, x-a->x, a->w);
  pnl_setdirty(a);
}

void
_drawvpalette(a, p)
Actuator *a;
Panel *p;
{
Coord y;

  if (!a->dirtycnt) return;

    pushmatrix();

    translate(a->x,a->y,0.0);

    setshade((Colorindex)a->minval);
    pmv2(0.0, 0.0);
    pdr2(a->w, 0.0);
    setshade((Colorindex)a->maxval);
    pdr2(a->w, a->h);
    pdr2(0.0,  a->h);
    spclos();

    color(pnl_black_color);
    rect(0.0,0.0,a->w,a->h);

    popmatrix();
    if (a->beveled) pnl_drawbevel(a, p);
    if (a->label) pnl_drawlabel(a, p);
}

void
_drawhpalette(a, p)
Actuator *a;
Panel *p;
{
Coord y;

  if (!a->dirtycnt) return;

    pushmatrix();

    translate(a->x,a->y,0.0);

    setshade((Colorindex)a->minval);
    pmv2(0.0, 0.0);
    pdr2(0.0, a->h);
    setshade((Colorindex)a->maxval);
    pdr2(a->w, a->h);
    pdr2(a->w, 0.0);
    spclos();
    color(pnl_black_color);
    rect(0.0,0.0,a->w,a->h);

    popmatrix();
    if (a->beveled) pnl_drawbevel(a, p);
    if (a->label) pnl_drawlabel(a, p);
}

void
pnl_palette(a)
Actuator *a;
{
  Palette *ad;

  a->type=PNL_PALETTE;

  a->data = (char *) pnl_alloc(sizeof(Palette));
  a->datasize = sizeof(Palette);
  ad=(Palette *)a->data;
  ad->wsave=0.0;
  ad->finefactor=PNL_FINE_CONTROL_FACTOR;

  a->labeltype=PNL_LABEL_BOTTOM;
  a->w=PNL_SLIDER_WIDTH;
  a->h=PNL_SLIDER_HEIGHT;
  a->newvalfunc=_newvalvpalette;
  a->drawfunc=_drawvpalette;
}

void
pnl_vpalette(a)
Actuator *a;
{
  pnl_palette(a);
  a->type=PNL_VSLIDER;

  a->w=PNL_SLIDER_WIDTH;
  a->h=PNL_SLIDER_HEIGHT;
  a->newvalfunc=_newvalvpalette;
  a->drawfunc=_drawvpalette;
}

void
pnl_hpalette(a)
Actuator *a;
{
  pnl_palette(a);
  a->type=PNL_HSLIDER;

  a->w=PNL_SLIDER_HEIGHT;
  a->h=PNL_SLIDER_WIDTH;
  a->newvalfunc=_newvalhpalette;
  a->drawfunc=_drawhpalette;
}

