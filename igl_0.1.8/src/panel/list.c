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

void
pnl_listadd(p, list)
     Alist *p, **list;
{
  p->next=(*list);
  (*list)=p;
}

void
pnl_listdelete(p, list)
     Alist *p, **list;
{     
  Alist *e;

  if (p==(*list)) {
    (*list)=(*list)->next;
  } else for (e=(*list);e;e=e->next) {
    if (e->next==p) {
      e->next=p->next;
      break;
    }
  }
}

Boolean
pnl_listin(p, list)
     Alist *p, *list;
{
  Alist *e;

  for (e=list;e;e=e->next) if (e->a==p->a) break;
  return (Boolean) e;
}

