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

openicon(a)
     Actuator *a;
{
  Icon *ad=(Icon *)a->data;

  if (a->ca) a->ca->visible=TRUE;

  ad->mode&= ~PNL_IM_STOWED;
  ad->mode|=PNL_IM_OPEN;
  a->x=ad->xopen;
  a->y=ad->yopen;
  a->w=ad->wopen;
  a->h=ad->hopen;
  ad->labelsave=a->label;	
  a->label=NULL;	 /* make pnl_labeloffsets think we have no label */
  pnl_fixpanel(a->p);
}

stowicon(a)
     Actuator *a;
{
  Icon *ad=(Icon *)a->data;

  if (a->ca) a->ca->visible=FALSE;

  ad->mode&= ~PNL_IM_OPEN;
  ad->mode|=PNL_IM_STOWED;
  a->x=ad->xstowed;
  a->y=ad->ystowed;
  a->w=ad->wstowed;
  a->h=ad->hstowed;
  a->label=ad->labelsave;
  pnl_fixpanel(a->p);
}

Boolean
_pickicon(a,p,x,y)
     Actuator *a;
     Panel *p;
     Coord x, y;
{
  Icon *ad=(Icon *)a->data;
  Actuator *sa;

  if (  PNL_HITRECT(x,y,ad->xstowed,ad->ystowed,
		    ad->xstowed+ad->wstowed,ad->ystowed+ad->hstowed)
      ||((ad->mode&PNL_IM_OPEN)
	 &&(PNL_HITRECT(x,y,ad->xopen,ad->yopen,
			ad->xopen+ad->wopen,ad->yopen+ad->hopen)
	 )))
    return TRUE;
  else
    return FALSE;
}


void
_newvalicon(a,p,x,y)
     Actuator *a;
     Panel *p;
     Coord x, y;
{
  Icon *ad=(Icon *)a->data;
  Actuator *sa;

  if (pnl_justdown) {
    openicon(a);
    return;
  }
  if (!a->active) {
    if (a->ca) { 
      a->ca->active=FALSE;
      pnl_newvalact(a->ca,p,x-a->x,y-a->y);
    }
    if (!(ad->mode&PNL_IM_STOWED)) stowicon(a);
    return;
  }

  if (!(ad->mode&PNL_IM_OPEN)) openicon(a);

  if (a->ca) {
    a->ca->active=TRUE;
    pnl_newvalact(a->ca,p,x-a->x,y-a->y);
  }

  if (a->ca) a->val=a->ca->val;
}

void
_fixicon(a)
     Actuator *a;
{
  Actuator *sa;
  Icon *ad=(Icon *)a->data;
  char *buf;

#ifdef DEBUG
  fprintf (stdout,"_fixicon: id:%d \"%s\"\n", a->id, a->label?a->label:"<no label>");
#endif DEBUG

  a->w=MAX(a->w, a->lw+2*PNL_DIM_2);
  
  ad->xstowed=a->x;
  ad->ystowed=a->y;
  ad->wstowed=a->w;
  ad->hstowed=a->h;

  if (!a->ca) return;
  ad->wopen=a->ca->w;
  ad->hopen=a->ca->h;
  ad->xopen=ad->xstowed-(ad->wopen-ad->wstowed)/2.0; /* centered horizontally */
  ad->yopen=ad->ystowed+ad->hstowed-ad->hopen;	    /* 'pop-down' */

  buf=(char *) pnl_alloc(strlen(a->ca->label)+1);
  (void) strcpy(buf, a->ca->label);
  a->label=buf;
  pnl_labeldimensions(a);
  pnl_labeloffsets(a);
}

void
_addicon(a)
     Actuator *a;
{
  Actuator *sa;
  Icon *ad=(Icon *)a->data;
 
  ad->labelsave=a->label;	
}

void
_addsubicon(sa, a)
     Actuator *sa, *a;
{
  Icon *ad=(Icon *)a->data;
  Actuator *sb, *sc;
  Coord dy;

  a->na++;
  sa->pa=a;

  sa->next=a->al;
  a->al=sa;

  a->ca=sa;

  sa->visible=FALSE;
}

void
_drawicon(a, p)
Actuator *a;
Panel *p;
{
  Coord y;
  Icon *ad=(Icon *)a->data;
  Actuator *sa;

  if (ad->mode&PNL_IM_STOWED) {
    if (!a->dirtycnt) return;
    pushmatrix();
    translate(a->x,a->y,0.0);

    color(pnl_normal_color);
    rectf(0.0,0.0,a->w,a->h);
    if (!a->beveled) {
      color(pnl_black_color);
      rect(0.0,0.0,a->w,a->h);
    }
    if (p->ppu>pnl_char_threshold) {
      color(pnl_label_color);
      cmov2(a->lx,a->ly+a->ld);
      charstr(a->label);
    } else {
      color(pnl_normal_color);
      rectf(a->lx,a->ly,a->lx+a->lw,a->ly+a->lh);
    }
    popmatrix();
    if (a->beveled) pnl_drawbevel(a, p);
  } else {
    a->ca->dirtycnt=MAX(a->ca->dirtycnt, a->dirtycnt);
    pushmatrix();
    translate(a->x,a->y,0.0);
    pnl_drawact(a->ca, a->p);
    popmatrix();
  }
}

void
pnl_icon(a)
Actuator *a;
{
  Icon *ad;

  a->type=PNL_ICON;

  a->data = (char *) pnl_alloc(sizeof(Icon));
  a->datasize = sizeof(Icon);
  ad=(Icon *)a->data;
  ad->mode=PNL_IM_STOWED;

  a->w=PNL_ICON_WIDTH;
  a->h=PNL_ICON_HEIGHT;

  a->labeltype=PNL_LABEL_CENTER;

  a->pickfunc=_pickicon;
  a->newvalfunc=_newvalicon;
  a->drawfunc=_drawicon;

  a->addfunc=_addicon;
  a->fixfunc=_fixicon;
  a->addsubfunc=_addsubicon; /* to add a sub act to this act */

  ad->xopen=ad->xstowed=0;
  ad->yopen=ad->ystowed=0;
  ad->wopen=ad->wstowed=a->w;
  ad->hopen=ad->hstowed=a->h;
  ad->labelsave=NULL;

}

