ifndef LUA_INC
	LUA_INC= lua5.1
endif

WARN= -Wall -Wmissing-prototypes -Wmissing-declarations -ansi -pedantic -fPIC
INCS= -I/usr/include/$(LUA_INC)
CFLAGS= -O2 $(WARN) $(INCS) $(DEFS) `pkg-config --cflags --libs gtk+-2.0 libnotify` 
CXXFLAGS= -O2 $(WARN) $(INCS) $(DEFS)
CC= gcc

# OS dependent
LIB_OPTION= -shared #for Linux
#LIB_OPTION= -bundle -undefined dynamic_lookup #for MacOS X

LIBNAME= notify.so

OBJS= notify.o
SRCS= notify.c
AR= ar rcu
RANLIB= ranlib

EXTRA_DIST=Makefile test/application-zip.svg test/document-print.svg test/example.lua test/gnome-mime-application-pdf.svg test/system-shutdown.svg test/example.lua

DISTFILES=$(SRCS) $(EXTRA_DIST)
all: $(LIBNAME)

$(LIBNAME): $(OBJS)
	$(CC) -o $@ $(LIB_OPTION) $(OBJS) $(CFLAGS)

clean:
	rm -f $(LIBNAME) *.o $(TARBALL)

LIBVERSION=`cat VERSION`

PKG=notify-$(LIBVERSION)
TARBALL= $(PKG).tar.bz2

dist: $(TARBALL)	

$(TARBALL):
	rm -rf $(PKG)
	mkdir $(PKG)
	cp -a $(DISTFILES) $(PKG)
	tar -jcf $(PKG).tar.bz2 $(DIST_FILE) $(PKG) && rm -rf $(PKG)

