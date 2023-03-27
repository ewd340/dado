.POSIX:
CC      = cc
CFLAGS  = -pedantic -Wall -Wextra -O3 -march=native
LDFLAGS = -flto
LDLIBS  =
PREFIX = /usr/local

sources = src/dado.c 

dado: $(sources)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(sources) $(LDLIBS)

install: dado
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 755 dado $(DESTDIR)$(PREFIX)/bin

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/dado

clean:
	rm -f dado
