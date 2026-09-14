PREFIX = /usr/local
CC     = cc
CFLAGS = -std=c99 -Wall -Wextra -O2

UNAME_S := $(shell uname -s)

X11INC = $(shell pkg-config --cflags x11 xft fontconfig freetype2 lua53 2>/dev/null || pkg-config --cflags x11 xft fontconfig freetype2 lua 2>/dev/null)
X11LIB = $(shell pkg-config --libs x11 xft fontconfig freetype2 lua53 2>/dev/null || pkg-config --libs x11 xft fontconfig freetype2 lua 2>/dev/null)
RPATH  =

ifeq ($(UNAME_S),NetBSD)
    export PKG_CONFIG_PATH = /usr/pkg/lib/pkgconfig:/usr/pkg/share/pkgconfig:/usr/X11R7/lib/pkgconfig
    X11INC = $(shell pkg-config --cflags x11 xft fontconfig freetype2 lua53 2>/dev/null || pkg-config --cflags x11 xft fontconfig freetype2 lua 2>/dev/null)
    X11LIB = $(shell pkg-config --libs x11 xft fontconfig freetype2 lua53 2>/dev/null || pkg-config --libs x11 xft fontconfig freetype2 lua 2>/dev/null)
    RPATH  = -Wl,-R/usr/pkg/lib -Wl,-R/usr/X11R7/lib
endif

CPPFLAGS = $(X11INC)
LDFLAGS  = $(X11LIB) $(RPATH)

all: mugb

mugb: Mugbar.c
	$(CC) $(CFLAGS) $(CPPFLAGS) Mugbar.c -o mugb $(LDFLAGS)

run: mugb
	./mugb

clean:
	rm -f mugb

.PHONY: all run clean