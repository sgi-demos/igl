# /* IGL: Makefile rewritten, original is in Makefile.ORIG */
INCDIR=../../include
SRCDIR=./
LIBDIR=lib

CC=gcc
LD=ld
LN=ln
AR=ar
RANLIB=ranlib
CFLAGS=-g
#CFLAGS+=-O2
INCLUDES=-I$(INCDIR) -I./ -I/usr/include
LIBS=-L/usr/lib -L/usr/X11R6/lib/ -lGL -lGLU -lX11 -lXext -lm
OBJS=$(LIBDIR)/panel.o		\
     $(LIBDIR)/puck.o		\
     $(LIBDIR)/slider.o		\
     $(LIBDIR)/g_gets.o         \
     $(LIBDIR)/strip.o          \
     $(LIBDIR)/button.o         \
     $(LIBDIR)/typein.o         \
     $(LIBDIR)/typeout.o        \
     $(LIBDIR)/mouse.o          \
     $(LIBDIR)/slideroid.o      \
     $(LIBDIR)/palette.o        \
     $(LIBDIR)/dial.o           \
     $(LIBDIR)/script.o         \
     $(LIBDIR)/mslider.o        \
     $(LIBDIR)/menu.o           \
     $(LIBDIR)/iconmenu.o       \
     $(LIBDIR)/icon.o           \
     $(LIBDIR)/frame.o          \
     $(LIBDIR)/cycle.o          \
     $(LIBDIR)/submenu.o        \
     $(LIBDIR)/list.o           \
     $(LIBDIR)/scroll.o         \
     $(LIBDIR)/viewframe.o      \
     $(LIBDIR)/polyframe.o      \
     $(LIBDIR)/mapwfind.o       \
     $(LIBDIR)/signal.o         \
     $(LIBDIR)/oldtypein.o      \
     $(LIBDIR)/graphframe.o     \
     $(LIBDIR)/user.o

#
# targets
#
STATIC=$(LIBDIR)/libpanel.a

all: $(STATIC)
static: $(STATIC)

$(OBJS): $(LIBDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/*.h
	@echo Compiling $<...
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(STATIC): $(OBJS)
	@echo Creating static library $@...
	@$(AR) cru $@ $(OBJS)
	@$(RANLIB) $@
	@echo DONE

clean:
	@echo Cleaning...
	@rm -f $(LIBDIR)/*
	@echo DONE

