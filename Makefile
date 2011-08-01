LUA_INC= /usr/include/lua5.1

WARN= -Wall -Wmissing-prototypes -Wmissing-declarations -ansi -pedantic -fPIC
INCS= -I$(LUA_INC)
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

EXTRA_DIST=Makefile /test/application-zip.svg test/document-print.svg test/example.lua \ test/gnome-mime-application-pdf.svg test/system-shutdown.svg /test/example.lua

DISTFILES=$(SRCS) $(EXTRA_DIST)

all: $(LIBNAME)

$(LIBNAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(LIB_OPTION) $(OBJS)

#~ $(COMPAT_DIR)/compat-5.1.o: $(COMPAT_DIR)/compat-5.1.c
#~ $(CC) -c $(CFLAGS) -o $@ $(COMPAT_DIR)/compat-5.1.c

#~ install:
#~ mkdir -p $(LUA_LIBDIR)/notify
#~ cp src/$(LIBNAME) $(LUA_LIBDIR)/notify

clean:
	rm -f $(LIBNAME) *.o $(TARBALL)

PKG= notify-$(LIBVERSION)
TARBALL= $(PKG).tar.bz2

dist: $(TARBALL)	

$(TARBALL): $(DISTFILES)
	rm -rf $(PKG)
	mkdir $(PKG)
	cp -a $(DISTFILES) $(PKG)
	tar -jcf $@ $(PKG) && rm -rf $(PKG)

