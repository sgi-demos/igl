INCDIR=include
SRCDIR=src
LIBDIR=lib
EXAMPLEDIR=examples

CC=gcc
CPP=g++
LD=ld
LN=ln
AR=ar
RANLIB=ranlib
CFLAGS=-fPIC
CFLAGS+=-g	# debug version
#CFLAGS+=-O2	# release version
LDFLAGS=-shared
INCLUDES=-I/usr/include -I$(INCDIR)
LIBS=-L/usr/lib -L/usr/X11R6/lib/ -lGL -lGLU -lX11 -lXext -lm
LIB_OBJS=$(LIBDIR)/buffer.o       \
     $(LIBDIR)/color.o            \
     $(LIBDIR)/device.o           \
     $(LIBDIR)/grpos.o            \
     $(LIBDIR)/igl.o              \
     $(LIBDIR)/light.o            \
     $(LIBDIR)/matrix.o           \
     $(LIBDIR)/menu.o             \
     $(LIBDIR)/object.o           \
     $(LIBDIR)/text.o             \
     $(LIBDIR)/texture.o          \
     $(LIBDIR)/vertex.o           \
     $(LIBDIR)/window.o

EXAMPLE_OBJS=$(EXAMPLEDIR)/demo1.o    \
             $(EXAMPLEDIR)/demo2.o    \
             $(EXAMPLEDIR)/demo3.o    \
             $(EXAMPLEDIR)/demo4.o    \
             $(EXAMPLEDIR)/demo5.o

#
# targets
#
SHARED=$(LIBDIR)/libIGL.so.0.1.8
STATIC=$(LIBDIR)/libIGL.a
EXAMPLES=$(EXAMPLEDIR)/demo1 $(EXAMPLEDIR)/demo2 $(EXAMPLEDIR)/demo3 $(EXAMPLEDIR)/demo4 $(EXAMPLEDIR)/demo5

all: $(SHARED) $(STATIC) $(EXAMPLES)
shared: $(SHARED)
static: $(STATIC)
examples: $(EXAMPLES)

$(LIB_OBJS): $(LIBDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/*.h
	@echo Compiling $<...
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(SHARED): $(LIB_OBJS)
	@echo Creating shared library $@...
	@$(LD) $(LDFLAGS) $(LIBS) $(LIB_OBJS) -o $@
	@echo DONE

$(STATIC): $(LIB_OBJS)
	@echo Creating static library $@...
	@$(AR) cru $@ $(LIB_OBJS)
	@$(RANLIB) $@
	@echo DONE

$(EXAMPLE_OBJS): $(EXAMPLEDIR)/%.o: $(EXAMPLEDIR)/%.c $(LIB_OBJS)
	@echo Compiling $<...
	@$(CC) $(INCLUDES) -o $@ $< $(LIBS) -L./lib -lIGL -e igl_X11main

$(EXAMPLES): %: %.o $(SHARED) $(STATIC)
	@echo Linking $(@F)...
	@$(LD) $@.o -o $@ -L./lib -lIGL -e igl_X11main

clean:
	@echo Cleaning up...
	@rm -f $(LIB_OBJS) $(SHARED) $(STATIC)
	@rm -f $(EXAMPLE_OBJS) $(EXAMPLES)
	@echo DONE

