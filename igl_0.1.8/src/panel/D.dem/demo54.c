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
#include <device.h>
#include <panel.h>

/* demo54 
 *
 * demonstrates use of a graphframe actuator
 *
 */

#define MAXSQUARES	64000
#define SQUARES		4
int edge, squares;

float theta[MAXSQUARES];
float thetarate[MAXSQUARES];

Actuator *speedslider, *spreadslider;

main(argc, argv)
int argc;
char **argv;
{
  Panel *p, *defpanel();
  Device dev;
  short data;
  int i;

  srand((long)time(0L));

  if (argc>1) squares=MIN(atoi(argv[1]), MAXSQUARES-1);
  else	      squares=SQUARES;

  edge=(int)sqrt((float)squares);
  if (edge*edge<squares) edge++;
  fprintf (stdout,"edge=%d\n", edge);

  foreground();
  noport();
  winopen();

  for (i=0;i<squares;i++) initsquare(i);

  p=defpanel();
  
  forever {
    pnl_dopanel();
  }
}

viewtransform()
{
  ortho(0.0, (float)edge, 0.0, (float)edge, -5.0, 5.0);
}

initsquare(i)
int i;
{
/* rando
  theta[i]=rand()%3600;
  thetarate[i]=rand()%20-5;
*/
  theta[i]=0;
  thetarate[i]=i%(edge)+i/edge+1;
}

void
drawanim(a, p)
     Actuator *a;
     Panel *p;
{
  int i, j, k;
  float thetadelta;

  pnl_setdirty(a);

  viewtransform();
  color(BLACK);
  clear();
  
  color(WHITE);
  translate(0.5, 0.5, 0.0);
  
  for (i=0, j=0, k=0; k<squares; i++, k++) {
    if (i==edge) {
      i=0;
      j++;
    }
    if (j==edge) j=0;
    
    thetadelta=INTERP(thetarate[k], edge, spreadslider->val);
    theta[k]+=thetadelta*speedslider->val*10.0;
    
    pushmatrix();
    translate((float)i, (float)j, 0.0);
    rot(theta[k], 'z');
    translate(0.4, 0.0, 0.0);
    
    if (edge>10) rectf(-0.1, -0.1, 0.1, 0.1);
    else	 circf(0.0,0.0,0.1);
      
    popmatrix();
  }
}
  
Panel
*defpanel()
{
  Panel *p;
  Actuator *a;
  Coord x,y;

  p=pnl_mkpanel();
  p->label="panel animation demo";
/*
  p->w=400;
  p->h=400;
*/

  a=pnl_mkact(pnl_graphframe);
  a->x=x;
  a->y=y;
  a->w=6-PNL_DIM_1;
  a->h=6-PNL_DIM_1;
  PNL_ACCESS(Graphframe, a, userdrawfunc)=drawanim;
  pnl_addact(a, p);
  x+=a->w+PNL_DIM_1;

  speedslider=a=pnl_mkact(pnl_slider);
  a->x=x;
  a->y=y;
  a->maxval=  1;
  a->val=     0;
  a->minval= -1;
  pnl_addact(a, p);
  x+=a->w+PNL_DIM_1;

  spreadslider=a=pnl_mkact(pnl_slider);
  a->x=x;
  a->y=y;
  a->maxval=  1;
  a->val=     0;
  a->minval=  0;
  pnl_addact(a, p);
  
  return p;
}
