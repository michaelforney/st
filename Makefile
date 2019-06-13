# st - simple terminal
# See LICENSE file for copyright and license details.

include config.mk

SRC = st.c wl.c xdg-shell-protocol.c
OBJ = ${SRC:.c=.o}

all: options st-wl

options:
	@echo st-wl build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

config.h:
	cp config.def.h config.h

xdg-shell-protocol.c:
	@echo GEN $@
	@wayland-scanner private-code ${XDG_SHELL_PROTO} $@

xdg-shell-client-protocol.h:
	@echo GEN $@
	@wayland-scanner client-header ${XDG_SHELL_PROTO} $@

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

st.o: st.h xdg-shell-client-protocol.h
wl.o: st.h xdg-shell-client-protocol.h

${OBJ}: config.h config.mk

st-wl: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f st-wl ${OBJ} st-wl-${VERSION}.tar.gz xdg-shell-protocol.c xdg-shell-client-protocol.h

dist: clean
	@echo creating dist tarball
	@mkdir -p st-wl-${VERSION}
	@cp -R LICENSE Makefile README config.mk config.def.h st-wl.info st-wl.1 arg.h ${SRC} st-wl-${VERSION}
	@tar -cf st-wl-${VERSION}.tar st-wl-${VERSION}
	@gzip st-wl-${VERSION}.tar
	@rm -rf st-wl-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f st-wl ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/st-wl
	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@sed "s/VERSION/${VERSION}/g" < st-wl.1 > ${DESTDIR}${MANPREFIX}/man1/st-wl.1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/st-wl.1
	@echo Please see the README file regarding the terminfo entry of st-wl.
	@tic -sx st-wl.info

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/st-wl
	@echo removing manual page from ${DESTDIR}${MANPREFIX}/man1
	@rm -f ${DESTDIR}${MANPREFIX}/man1/st-wl.1

.PHONY: all options clean dist install uninstall
