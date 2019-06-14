# st - simple terminal
# See LICENSE file for copyright and license details.
.POSIX:

include config.mk

SRC = st.c wl.c xdg-shell-protocol.c
OBJ = $(SRC:.c=.o)

all: options st-wl

options:
	@echo st-wl build options:
	@echo "CFLAGS  = $(STCFLAGS)"
	@echo "LDFLAGS = $(STLDFLAGS)"
	@echo "CC      = $(CC)"

config.h:
	cp config.def.h config.h

xdg-shell-protocol.c:
	@echo GEN $@
	@wayland-scanner private-code $(XDG_SHELL_PROTO) $@

xdg-shell-client-protocol.h:
	@echo GEN $@
	@wayland-scanner client-header $(XDG_SHELL_PROTO) $@

.c.o:
	$(CC) $(STCFLAGS) -c $<

st.o: config.h st.h win.h
wl.o: arg.h config.h st.h win.h xdg-shell-client-protocol.h

$(OBJ): config.h config.mk

st-wl: $(OBJ)
	$(CC) -o $@ $(OBJ) $(STLDFLAGS)

clean:
	rm -f st-wl $(OBJ) st-wl-$(VERSION).tar.gz xdg-shell-*

dist: clean
	mkdir -p st-wl-$(VERSION)
	cp -R FAQ LEGACY TODO LICENSE Makefile README config.mk\
		config.def.h st-wl.info st-wl.1 arg.h st-wl.h win.h $(SRC)\
		st-wl-$(VERSION)
	tar -cf - st-wl-$(VERSION) | gzip > st-wl-$(VERSION).tar.gz
	rm -rf st-wl-$(VERSION)

install: st
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f st-wl $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/st-wl
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	sed "s/VERSION/$(VERSION)/g" < st-wl.1 > $(DESTDIR)$(MANPREFIX)/man1/st-wl.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/st-wl.1
	tic -sx st-wl.info
	@echo Please see the README file regarding the terminfo entry of st-wl.

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/st-wl
	rm -f $(DESTDIR)$(MANPREFIX)/man1/st-wl.1

.PHONY: all options clean dist install uninstall
