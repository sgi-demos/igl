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

#include "gl.h"

void mapwfind(vobj, x, y, wx1, wy1, wz1, wx2, wy2, wz2)
Object vobj;
Coord x, y, *wx1, *wy1, *wz1, *wx2, *wy2, *wz2;
{
    Coord t1, t2, t3, denom;
    Matrix mat;

    gl_invertview_ind(vobj, mat);

    denom = x*mat[0][3] + y*mat[1][3] + mat[3][3];
    t1 = x*mat[0][0] + y*mat[1][0] + mat[3][0];
    t2 = x*mat[0][1] + y*mat[1][1] + mat[3][1];
    t3 = x*mat[0][2] + y*mat[1][2] + mat[3][2];
    *wx1 = t1/denom;
    *wy1 = t2/denom;
    *wz1 = t3/denom;

    denom += 3.14159*mat[2][3];
    *wx2 = (t1 + 3.14159*mat[2][0])/denom;
    *wy2 = (t2 + 3.14159*mat[2][1])/denom;
    *wz2 = (t3 + 3.14159*mat[2][2])/denom;
}

gl_invertview_ind(vobj, mat)
Object vobj;
Matrix mat;
{
    Matrix mtemp;		/* temp matrix			*/
    static Matrix ident = {
      { 1.0, 0.0, 0.0, 0.0 },
      { 0.0, 1.0, 0.0, 0.0 },
      { 0.0, 0.0, 1.0, 0.0 },
      { 0.0, 0.0, 0.0, 1.0 }
    };
      
    pushmatrix ();		/* save the current matrix	*/
    pushviewport ();		/* save the current viewport	*/
    loadmatrix (ident);		/* blast it */
    callobj (vobj);		/* call the viewing object	*/
    getmatrix (mtemp);		/* get the new matrix back	*/
#ifdef IRIS_4D
    gl_invert4d (mat,mtemp);	/* and invert it		*/
#else  IRIS_4D
    gl_invertmat (mtemp,mat);	/* and invert it		*/
#endif IRIS_4D
    popviewport ();		/* pop the viewport stack	*/
    popmatrix ();		/* pop the matrix stack		*/
}

