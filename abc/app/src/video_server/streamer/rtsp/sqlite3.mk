prefix = ./bin
exec_prefix = $(prefix)
includedir = ./include
libdir = $(exec_prefix)/lib

CFLAGS = -Wall -g -O2 -fPIC -D SQLITE_DEFAULT_FOREIGN_KEYS=1

THREAD_LIB = -lpthread
LDLIBS = $(THREAD_LIB) -ldl


.DEFAULT_GOAL := all

all: libsqlite3.so install

objects := $(patsubst %.c,%.o,$(wildcard sqlite3/*.c))


libsqlite3.so: $(objects)
	gcc -shared -o $@ -D SQLITE_DEFAULT_FOREIGN_KEYS=1 $(LDLIBS) $^

libsqlite3.a: $(objects)
	$(AR) -rcs $@ $^

clean:
	rm -f sqlite3/*.o libsqlite3.so libsqlite3.a

install: libsqlite3.so libsqlite3.a sqlite3/*.h
	mkdir -p $(DESTDIR)$(libdir)
	install sqlite3/*.h $(DESTDIR)$(includedir)/
	install libsqlite3.so $(DESTDIR)$(libdir)/libsqlite3.so
	install libsqlite3.a $(DESTDIR)$(libdir)/libsqlite3.a
	rm -f libsqlite3.so libsqlite3.a
	$(POST_INSTALL)

uninstall:
	rm -f $(DESTDIR)$(includedir)/sqlite3.h
	rm -f $(DESTDIR)$(libdir)/libsqlite3.so
	rm -f $(DESTDIR)$(libdir)/libsqlite3.a

.PHONY: clean uninstall



