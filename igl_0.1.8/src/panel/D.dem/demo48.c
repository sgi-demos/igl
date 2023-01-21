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

Panel *panel;
Actuator *vf[3], *svf[3];
  
Panel *defpanel();

main() 
{
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

void
  updaterotx1(a)
  Actuator *a;
{
  int i;
  Viewframe *ad;

  for (i=0;i<3;i++) {
    ad=(Viewframe *)vf[i]->data;
    ad->rotx1=a->val*360.0;
    pnl_fixact(vf[i]);
    panel->dirtycnt=2;
  }
}

void
  updaterotz(a)
  Actuator *a;
{
  int i;
  Viewframe *ad;

  for (i=0;i<3;i++) {
    ad=(Viewframe *)vf[i]->data;
    ad->rotz=a->val*360.0;
    pnl_fixact(vf[i]);
    panel->dirtycnt=2;
  }
}

void
  updaterotx2(a)
  Actuator *a;
{
  int i;
  Viewframe *ad;

  for (i=0;i<3;i++) {
    ad=(Viewframe *)vf[i]->data;
    ad->rotx2=a->val*360.0;
    pnl_fixact(vf[i]);
    panel->dirtycnt=2;
  }
}

void
  updatetran1(a)
  Actuator *a;
{
  int i;
  Viewframe *ad;

  for (i=0;i<3;i++) {
    ad=(Viewframe *)vf[i]->data;
    ad->tx1=ad->ty1=a->val;
    pnl_fixact(vf[i]);
    panel->dirtycnt=2;
  }
}

void
  updatetran2(a)
  Actuator *a;
{
  int i;
  Viewframe *ad;

  for (i=0;i<3;i++) {
    ad=(Viewframe *)vf[i]->data;
    ad->tx2=ad->ty2=a->val;
    pnl_fixact(vf[i]);
    panel->dirtycnt=2;
  }
}

void
  updatetranz(a)
  Actuator *a;
{
  int i;
  Viewframe *ad;

  for (i=0;i<3;i++) {
    ad=(Viewframe *)vf[i]->data;
    ad->tz2=a->val;
    pnl_fixact(vf[i]);
    panel->dirtycnt=2;
  }
}

void
  updatescale(a)
  Actuator *a;
{
  int i;
  Viewframe *ad;

  for (i=0;i<3;i++) {
    ad=(Viewframe *)vf[i]->data;
    ad->sx=ad->sy=a->val;
    pnl_fixact(vf[i]);
    panel->dirtycnt=2;
  }
}

void
  updatesubtranxy(a)
  Actuator *a;
{
  int i;
  Viewframe *ad;

  for (i=0;i<3;i++) {
    ad=(Viewframe *)svf[i]->data;
    ad->tx2=ad->ty2=a->val*vf[i]->scalefactor;
    pnl_fixact(svf[i]);
    panel->dirtycnt=2;
  }
}

void
  updatesubtranz(a)
  Actuator *a;
{
  int i;
  Viewframe *ad;

  for (i=0;i<3;i++) {
    ad=(Viewframe *)svf[i]->data;
    ad->tz1=a->val*vf[i]->scalefactor;
    pnl_fixact(svf[i]);
    panel->dirtycnt=2;
  }
}

void
  updatesubrotx1(a)
  Actuator *a;
{
  int i;
  Viewframe *ad;

  for (i=0;i<3;i++) {
    ad=(Viewframe *)svf[i]->data;
    ad->rotx1=a->val*360.0;
    pnl_fixact(svf[i]);
    panel->dirtycnt=2;
  }
}

void
  my_fixpanel(p)
Panel *p;
{
  p->dirtycnt=2;
}

Actuator
*defviewframe(p)
   Panel *p;
{
  Actuator *viewframe, *subviewframe, *a;
  Coord x=0, y=0;
  Coord d=1.0, dl=0.5;
  int i;

  viewframe=pnl_mkact(pnl_viewframe);
/*viewframe->beveled=FALSE;	*/
  pnl_addact(viewframe, p);

  a=pnl_mkact(pnl_slider);
  a->x=x; x+=a->w+PNL_DIM_1;
/*a->beveled=FALSE;	*/
  pnl_addsubact(a, viewframe);

  a=pnl_mkact(pnl_slider);
  a->x=x; x+=a->w+PNL_DIM_1;
/*a->beveled=FALSE;	*/
  pnl_addsubact(a, viewframe);

  {
    subviewframe=pnl_mkact(pnl_viewframe);
    subviewframe->label="sub viewframe";
/*  subviewframe->beveled=FALSE;	*/
    subviewframe->x=x;
    PNL_ACCESS(Viewframe, subviewframe, tx1)=1.0;
    PNL_ACCESS(Viewframe, subviewframe, ty1)=2.0;
    PNL_ACCESS(Viewframe, subviewframe, tz1)=3.0;
    PNL_ACCESS(Viewframe, subviewframe, rotz)=30;
    pnl_addsubact(subviewframe, viewframe);
    
    a=pnl_mkact(pnl_slider);
    a->x=10;
    a->y=3;
    a->label="slider in viewframe";
/*  a->beveled=FALSE;	*/
    pnl_addsubact(a, subviewframe);
    
    x+=subviewframe->w+PNL_DIM_1;
  }

  viewframe->u=(char *)subviewframe;

  for (i=0;i<8;i++) {
    a=pnl_mkact(pnl_button);
    a->x=x;
    a->y=y; y+=a->h+PNL_DIM_1;
/*  a->beveled=FALSE;	*/
    pnl_addsubact(a, viewframe);
  }

  x+=a->w+PNL_DIM_1;

  a=pnl_mkact(pnl_slider);
  a->x=x; x+=a->w+PNL_DIM_1;
/*a->beveled=FALSE;	*/
  pnl_addsubact(a, viewframe);

  return viewframe;
}

Panel 
*defpanel()
{
  Actuator *a;
  Panel *p;
  Coord x, y, d=1.0, dl=0.5;
  float sf;

  p=pnl_mkpanel();
  p->label="viewframe geometry";

  a=pnl_mkact(pnl_slider);
  a->label="tran1";
  a->x=x;
  a->minval= -10;
  a->maxval=10;
  a->activefunc=updatetran1;
  pnl_addact(a, p);
  x+=a->w+PNL_DIM_1;

  a=pnl_mkact(pnl_slider);
  a->label="rotx1";
  a->x=x;
  a->activefunc=updaterotx1;
  pnl_addact(a, p);
  x+=a->w+PNL_DIM_1;

  a=pnl_mkact(pnl_slider);
  a->label="rotz";
  a->x=x;
  a->activefunc=updaterotz;
  pnl_addact(a, p);
  x+=a->w+PNL_DIM_1;

  a=pnl_mkact(pnl_slider);
  a->label="rotx2";
  a->x=x;
  a->activefunc=updaterotx2;
  pnl_addact(a, p);
  x+=a->w+PNL_DIM_1;

  a=pnl_mkact(pnl_slider);
  a->label="tran2";
  a->x=x;
  a->minval= -10;
  a->maxval=10;
  a->activefunc=updatetran2;
  pnl_addact(a, p);
  x+=a->w+PNL_DIM_1;

  a=pnl_mkact(pnl_slider);
  a->label="tranz";
  a->x=x;
  a->minval= -10;
  a->maxval=10;
  a->activefunc=updatetranz;
  pnl_addact(a, p);
  x+=a->w+PNL_DIM_1;

  a=pnl_mkact(pnl_slider);
  a->label="tran2";
  a->x=x;
  a->minval= -10;
  a->maxval=10;
  a->activefunc=updatesubtranxy;
  pnl_addact(a, p);
  x+=a->w+PNL_DIM_1;

  a=pnl_mkact(pnl_slider);
  a->label="rotx1";
  a->x=x;
  a->activefunc=updatesubrotx1;
  pnl_addact(a, p);
  x+=a->w+PNL_DIM_1;

  a=pnl_mkact(pnl_slider);
  a->label="tran2";
  a->x=x;
  a->minval= -10;
  a->maxval=10;
  a->activefunc=updatesubtranz;
  pnl_addact(a, p);
  x+=a->w+PNL_DIM_1;

  a=pnl_mkact(pnl_slider);
  a->label="scale";
  a->x=x;
  a->val=1;
  a->minval=0;
  a->maxval=5;
  a->activefunc=updatescale;
  pnl_addact(a, p);
  x+=a->w+PNL_DIM_1;

  p=pnl_mkpanel();
  p->label="viewframes";
  p->activefunc=my_fixpanel;

  vf[0]=a=defviewframe(p);
  svf[0]=(Actuator *)a->u;
  a->label="viewframe one";
  a->x=x;
  a->scalefactor=sf=1.0;
  pnl_fixact(a);
  x+=a->w+PNL_DIM_1;

  vf[1]=a=defviewframe(p);
  svf[1]=(Actuator *)a->u;
  a->label="viewframe two";
  a->x=x;
  a->scalefactor=(sf*=0.66);
  pnl_fixact(a);
  x+=a->w+PNL_DIM_1;

  vf[2]=a=defviewframe(p);
  svf[2]=(Actuator *)a->u;
  a->label="viewframe three";
  a->x=x;
  a->scalefactor=(sf*=0.66);
  pnl_fixact(a);
  x+=a->w+PNL_DIM_1;

  return p;
}

