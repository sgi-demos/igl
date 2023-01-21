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
#include <device.h>
#include <panel.h>

Actuator *s1, *s2, *b1, *b2, *b3;

Panel
*defpanel();

main() 
{
  Actuator *a;
  Panel *panel;
  
  fprintf(stderr, "This demo doesn't work\n");

  pnl_forkpanel();
  
  foreground();
  winopen("demo");
  
  panel=defpanel();
  pnl_startpanel();
  
  doublebuffer();
  gconfig();
  
  ortho2(-1.0,1.0,-1.0,1.0);

  forever {
    if (pnl_ca==b1) {
      exit(0);
    }
    
    if (pnl_ca==b2) {
      b3->val=b2->val;
      b3->dirtycnt=2;
    }
    
    pushmatrix();
    translate(s1->val,s2->val,0.0);
    color(BLACK);
    clear();
    color(WHITE);
    drawit();
    swapbuffers();
    popmatrix();
  }
}


drawit()
{
  rectf(-.10,-.10,.10,.10);
}


Panel
  *defpanel()
{
  Panel *panel;
  
  fprintf (stdout,"defining panel\n");
  
  panel=pnl_mkpanel();
  panel->label="demo";
  panel->ppu=50.0;
  
  s1=pnl_mkact(pnl_hslider);
  s1->label="y position";
  s1->x=1.0;
  s1->y=0.0;
  s1->minval= -1.0;
  s1->maxval=1.0;
  pnl_addact(s1, panel);
  
  s2=pnl_mkact(pnl_vslider);
  s2->label="x position";
  s2->x=0.0;
  s2->y=0.0;
  s2->minval= -1.0;
  s2->maxval=1.0;
  pnl_addact(s2, panel);
  
  b1=pnl_mkact(pnl_button);
  b1->label="exit";
  b1->x=1.0;
  b1->y=1.0;
  pnl_addact(b1, panel);
  
  b2=pnl_mkact(pnl_toggle_button);
  b2->label="push me";
  b2->x=1.0;
  b2->y=1.5;
  pnl_addact(b2, panel);
  
  b3=pnl_mkact(pnl_toggle_button);
  b3->label="I push myself";
  b3->x=1.0;
  b3->y=2.0;
  pnl_addact(b3, panel);
  
  return panel;
}

