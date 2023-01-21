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

Boolean
tread(fd, buf, len)
int fd;
char *buf;
int len;
{
  int c;

  if ((c=read(0,buf,len))!=len) {
    (void) fprintf (stdout,"<end of input>\n");
    return FALSE;
  }
  return TRUE;
}   

main() 
{
  int c, i;
  Actuator a;
  short actid;
  int msgtype, datasize, delay;
  static union {
    int i;
    float f;
  } buf[9];
  char data[16386];

  for (;;) {
    if (!tread(0,&msgtype,sizeof(msgtype))) exit(1);
    
  redo:
    switch (msgtype) {
    case PNL_MT_MOUSE:
      if (!tread(0,buf,sizeof(buf))) exit(1);
      
      pnl_cp=(Panel *)buf[0].i;
      pnl_ca=(Actuator *)buf[1].i;
      pnl_x=buf[2].f;
      pnl_y=buf[3].f;
      pnl_justup=(Boolean)buf[4].i;
      pnl_justdown=(Boolean)buf[5].i;
      pnl_mousedown=(Boolean)buf[6].i;
      pnl_shiftkey=(Boolean)buf[7].i;
      pnl_controlkey=(Boolean)buf[8].i;
      
      (void) fprintf (stdout,"MOUSE: %d %d %f %f %d %d %d\n", 
		    pnl_cp,
		    pnl_ca,
		    pnl_x,
		    pnl_y,
		    pnl_justup,
		    pnl_justdown,
		    pnl_mousedown,
		    pnl_shiftkey,
		    pnl_controlkey);
      break;
    case PNL_MT_STATE:
      tread(0,&actid,sizeof(short));
      tread(0,&a,sizeof(a));
      tread(0,&datasize,sizeof(int));
      tread(0,data,datasize);

      (void) fprintf (stdout,"STATE: %d %d %f\n", actid, a.type, a.val);
      break;
    case PNL_MT_DELAY:
      tread(0,&delay,sizeof(delay));

      (void) fprintf (stdout,"DELAY: %d\n", delay);
      break;
    default:
      (void) fprintf (stdout,"unrecognized message type\n");
      for (i=1;;i++) {
	if (!tread(0,&msgtype,sizeof(msgtype))) exit(1);
	if (  msgtype==PNL_MT_MOUSE
	    ||msgtype==PNL_MT_STATE
	    ||msgtype==PNL_MT_DELAY) break;
      }
      fprintf (stdout,"skipped %d non-tokens\n", i);
      goto redo;
    }
  }
}


