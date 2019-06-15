# st version
VERSION = 0.8.2

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

PKG_CONFIG = pkg-config
PKGCFG = fontconfig wayland-client wayland-cursor xkbcommon wld
XDG_SHELL_PROTO = `$(PKG_CONFIG) --variable=pkgdatadir wayland-protocols`/stable/xdg-shell/xdg-shell.xml

PKG_CONFIG = pkg-config

# includes and libs
INCS = -I. -I/usr/include `$(PKG_CONFIG) --cflags ${PKGCFG}`
LIBS = -L/usr/lib -lc -lm -lrt -lutil `$(PKG_CONFIG) --libs ${PKGCFG}`

# flags
CFLAGS += -g -std=c99 -pedantic -Wall -Wvariadic-macros -Os
LDFLAGS += -g ${LIBS}
STCPPFLAGS = -DVERSION=\"$(VERSION)\" -D_XOPEN_SOURCE=600
STCFLAGS = $(INCS) $(STCPPFLAGS) $(CPPFLAGS) $(CFLAGS)
STLDFLAGS = $(LIBS) $(LDFLAGS)

# OpenBSD:
#CPPFLAGS = -DVERSION=\"$(VERSION)\" -D_XOPEN_SOURCE=600 -D_BSD_SOURCE
#LIBS = -L$(X11LIB) -lm -lX11 -lutil -lXft \
#       `pkg-config --libs fontconfig` \
#       `pkg-config --libs freetype2`

# compiler and linker
# CC = c99
