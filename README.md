# MUGbar

Mugbar for _CoffeeSYS_ systems

## Config

Configs are placed in the user .config/coffeesys dir, the file name is mugbar.lua

Default contents of this file are

```lua
-- ~/.config/coffeesys/mugbar.lua
palette = {background = "#101010", accent = "#33b5e5"}
elements = {panel_size = "20"}
keys = {reload = "r"}
icons = {term = "st &", param = "st -e nano ~/.config/coffeesys/mugbar.lua &", vol = "pavucontrol &"}
-- After saving changes press reload key to apply them
```

As provided in the code Changes can be applied by using Current `reload` key

## Compilation

This program is very compact and can be complied on *almost* any Unix-family os, it can be complied easily using `make` command

```Makefile
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
```

else, if you cant use make you can just reconstruct this command from `Makefile` file like so:

```cc -std=c99 -Wall -Wextra -O2 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/lua5.3 Mugbar.c -o mugb -lX11 -lXft -lfontconfig -lfreetype -llua5.3 ``` (for Void Linux)
