# /* IGL: Makefile rewritten, original is in Makefile.ORIG */
INCDIR=../
SRCDIR=./
LIBDEP=../lib/libpanel.a ../../../lib/libIGL.a

CC=gcc
LD=ld
LN=ln
AR=ar
RANLIB=ranlib
CFLAGS=-g
#CFLAGS+=-O2
LDFLAGS=-shared
INCLUDES=-I$(INCDIR) -I../../../include -I/usr/include
LIBS=-L/usr/lib -L/usr/X11R6/lib/ -lGL -lGLU -lX11 -lXext -lm $(LIBDEP) -e igl_X11main

#
# targets
#
all: ep
ep:	ep.o colormod.o fade.o $(LIBDEP)
	$(CC) $(CFLAGS) $(INCLUDES) -o ep ep.o colormod.o fade.o $(LIBS)
	@echo DONE

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean:
	@echo Cleaning...
	@rm -f ep *.o
	@echo DONE

