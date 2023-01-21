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

void
_newvalgraphframe(a,p,x,y)
     Actuator *a;
     Panel *p;
     Coord x, y;
{
  Graphframe *ad=(Graphframe *)a->data;
  /* frame coords (relative to origin of this frame */
  Coord fx=(x-a->x)/a->scalefactor;
  Coord fy=(y-a->y)/a->scalefactor;

  ad->x=fx;
  ad->y=fy;
}

void
_addsubgraphframe(sa, a)
     Actuator *sa, *a;
{
  Graphframe *ad=(Graphframe *)a->data;

  fprintf(stderr, "panellib:  warning, can't add subacts to Graphframe\n");
}

void
_drawgraphframe(a, p)
     Actuator *a;
     Panel *p;
{
  Actuator *sa;
  Graphframe *ad=(Graphframe *)a->data;
  Screencoord left, right, bottom, top;
  
  if (!a->dirtycnt) return;

  left=(a->x-p->minx)*p->ppu;
  right=(a->x+a->w-p->minx)*p->ppu;
  bottom=(a->y-p->miny)*p->ppu;
  top=(a->y+a->h-p->miny)*p->ppu;
  
  pushmatrix();
  pushviewport();
  viewport(left, right, bottom, top);

  color(pnl_background_color);
  clear();
  
  if (ad->userdrawfunc) (*ad->userdrawfunc)(a, p);

  popviewport();
  popmatrix();
  
  if (a->beveled) pnl_drawbevel(a, p);
  if (a->label) pnl_drawlabel(a, p);
}

void
pnl_graphframe(a)
     Actuator *a;
{
  Graphframe *ad;
  a->type=PNL_FRAME;
  
  a->data = (char *) pnl_alloc(sizeof(Graphframe));
  a->datasize = sizeof(Graphframe);
  ad=(Graphframe *)a->data;
  ad->mode=PNL_FM_FREE;

  a->w=PNL_DIM_1;
  a->h=PNL_DIM_1;

  a->labeltype=PNL_LABEL_TOP;
  a->newvalfunc=_newvalgraphframe;
  a->addsubfunc=_addsubgraphframe;
  a->drawfunc=_drawgraphframe;
}

