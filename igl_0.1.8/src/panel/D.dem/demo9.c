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

struct view    /* user-defined struct with an Actuator as its FIRST element */
{
  Actuator act;
  float x_angle;
  float y_angle;
  float z_angle;
};

func (v)
struct view *v;	    /* here the parameter is a user-defined structure */
{
  (void) fprintf (stdout,"it is %f\n", v->x_angle);
}

callit (a)
Actuator *a;	    /* and here its an Actuator */
{
  (*a->downfunc)(a);
}

main()
{
struct view *v = PNL_MKUSERACT (view, pnl_button);

  v->act.label   = "test";
  v->x_angle = 10.0;
  v->y_angle = 20.0;
  v->z_angle = 30.0;
  
  v->act.downfunc = (void (*)()) func;

  callit (v);
}


