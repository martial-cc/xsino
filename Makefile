# Based on the Makefile from suckless dwm, MIT licensed
.POSIX:

# xsino version
VERSION = 0.1.0

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# includes and libs
INCS = -I${X11INC}
LIBS = -L${X11LIB} -lX11

# flags
CPPFLAGS = -D_BSD_SOURCE -DVERSION=\"${VERSION}\" 
CFLAGS   = -std=c99 -pedantic -Wall -O0 ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS}

# compiler and linker
CC = cc

SRC = xsino.c
OBJ = ${SRC:.c=.o}

all: options xsino

options:
	@echo xsino build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

xsino: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f xsino ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f xsino ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/xsino
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < xsino.1 > ${DESTDIR}${MANPREFIX}/man1/xsino.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/xsino.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/xsino\
		${DESTDIR}${MANPREFIX}/man1/xsino.1

.PHONY: all options clean install uninstall
