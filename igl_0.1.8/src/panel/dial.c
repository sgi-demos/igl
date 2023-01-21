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
#include <math.h>
#include <gl.h>
#include <panel.h>

void _newvaldial(a,p,x,y)
     Actuator *a;
     Panel *p;
     Coord x, y;
{    
  Dial *ad=(Dial *)a->data;
  float theta, dtheta;
  
  if (!a->active) return;
  
  theta = atan2(x-a->x-a->w/2.0, y-a->y-a->h/2.0);

  if (pnl_justdown) {
    ad->wsave = theta;
    if (!pnl_controlkey&&!ad->mode&PNL_DM_NOSNAP)
      a->val=PNL_WORLD_TO_VAL(theta, (a->minval+a->maxval)/2.0,
			      a->maxval, ad->winds*PI);
  }

  dtheta=theta-ad->wsave;
  if (dtheta> PI) dtheta-=2.0*PI;;
  if (dtheta<-PI) dtheta+=2.0*PI;

  if (pnl_controlkey)
    a->val+=ad->finefactor
      *PNL_WORLD_TO_VAL(dtheta, 0.0, a->maxval-a->minval, ad->winds*2.0*PI);
  else
    a->val+=PNL_WORLD_TO_VAL(dtheta, 0.0,
			     a->maxval-a->minval, ad->winds*2.0*PI);

  if (ad->mode&PNL_DM_WRAP) {
    if (a->val>a->maxval) a->val-=(a->maxval-a->minval);
    if (a->val<a->minval) a->val+=(a->maxval-a->minval);
  } else 
    a->val=RANGE(a->val, a->maxval, a->minval);

  ad->wsave=theta;
  pnl_setdirty(a);
}

void
_drawdial(a, p)
Actuator *a;
Panel *p;
{
  float thetadeg;
  Dial *ad=(Dial *)a->data;

  if (!a->dirtycnt) return;

  pushmatrix();
  
  translate(a->x,a->y,0.0);
  
  color(pnl_other_color);
  rectf(0.0,0.0,a->w,a->h);
  color(pnl_black_color);
  rect(0.0,0.0,a->w,a->h);
  color(pnl_normal_color);
  circf(a->w/2.0, a->h/2.0, MIN(a->w/2.0, a->h/2.0)-PNL_DIM_3);
  color(pnl_black_color);
  circ(a->w/2.0, a->h/2.0, MIN(a->w/2.0, a->h/2.0)-PNL_DIM_3);

  translate(a->w/2.0, a->h/2.0, 0.0);

  thetadeg=PNL_VAL_TO_WORLD(RANGE(a->val, a->minval, a->maxval),
			    a->minval, a->maxval,
			    ad->winds*2.0*PI)*180.0/PI;
  rot(-thetadeg+ad->winds*180.0+90.0, 'z');

  color(pnl_highlight_color);
  rectf(0.0,-PNL_DIM_4, MIN(a->w/2.0, a->h/2.0)-PNL_DIM_3, PNL_DIM_4);
  color(pnl_black_color);
  rect(0.0,-PNL_DIM_4, MIN(a->w/2.0, a->h/2.0)-PNL_DIM_3, PNL_DIM_4);
  
  popmatrix();
  if (a->beveled) pnl_drawbevel(a, p);
  if (a->label) pnl_drawlabel(a, p);
}

void
_hitdial()
{
}

void
pnl_dial(a)
Actuator *a;
{
  Dial *ad;

  a->type=PNL_DIAL;

  a->data = (char *)pnl_alloc(sizeof(Dial));
  a->datasize = sizeof(Dial);
  ad=(Dial *)a->data;
  ad->wsave=0.0;
  ad->finefactor=PNL_FINE_CONTROL_FACTOR;
  ad->winds=PNL_DIAL_WINDS;

  a->labeltype=PNL_LABEL_BOTTOM;

  a->w=PNL_DIAL_EDGE;
  a->h=PNL_DIAL_EDGE;
  a->newvalfunc=_newvaldial;
  a->drawfunc=_drawdial;
}

