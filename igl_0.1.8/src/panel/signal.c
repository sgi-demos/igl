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
#include <setjmp.h>
#include <signal.h>
#include <gl.h>
#include <panel.h>

/* this file implements the signal actuator */

extern jmp_buf func_return;

void
pnl_sighandler(sig)
     int sig;
{
#if 0
  signal(sig, SIG_IGN);

  if (pnl_sl[sig]->downfunc==SIG_DFL) exit(2);
  if (pnl_sl[sig]->downfunc==SIG_IGN) ;	/* nothin' */ 
  else
    if (pnl_sl[sig]->downfunc) {
      pnl_funcmode=PNL_FCNM_DOWN;
      (*pnl_sl[sig]->downfunc)(pnl_sl[sig]);
      pnl_funcmode=PNL_FCNM_NONE;
    }
  
  qenter(PNL_TOKEN_SIGNAL, sig);

  signal(sig, pnl_sighandler);

  if (pnl_funcmode!=PNL_FCNM_NONE) {
    longjmp(func_return);
  }
#endif
}

void
_newvalsignal(a, p, x, y)
     Actuator *a;
     Panel *p;
     Coord x, y;
{
  Signal *ad=(Signal *)a->data;
/*
  a->active=ad->activate;
  ad->activate=FALSE;
*/
}

void
_addsignal(a, p)
Actuator *a;
Panel *p;
{
  Signal *ad=(Signal *)a->data;

  if (pnl_sl[ad->signal]) {
    fprintf (stdout,"panellib: replacing signal actuator for signal %d\n", signal);
    delact(pnl_sl[ad->signal]);
  }
      
  pnl_sl[ad->signal]=a;
  signal(ad->signal, pnl_sighandler);
}

void
_delsignal(a, p)
Actuator *a;
Panel *p;
{
  Signal *ad=(Signal *)a->data;

  pnl_sl[ad->signal]=NULL;
  signal(ad->signal, SIG_DFL);
}

void
pnl_signal(a)
Actuator *a;
{
  Signal *ad;
  a->type=PNL_SIGNAL;

  ad=(Signal *)(a->data=(char *)pnl_alloc(sizeof(Signal)));
  a->datasize=sizeof(Signal);
  ad->signal=SIGINT;		/* default signal */
  a->newvalfunc=_newvalsignal;
  a->addfunc=_addsignal;
  a->delfunc=_delsignal;
  a->downfunc=SIG_IGN;
  a->visible=FALSE;
}
