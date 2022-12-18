# ought2make


CFLAGS += -Wall -Wextra -I./include/
LIBRARYSOURCES = $(wildcard src/*.c)
TESTSOURCES = $(wildcard tests/*.c)
OBJECTS = $(LIBRARYSOURCES:.c=.o)
TESTOBJECTS = $(TESTSOURCES:.c=)
LIBRARYNAME = $(libname)
STATICLIBRARY = lib$(LIBRARYNAME).a
DESTDIR = /
PREFIX = /lib/

LDLIBS += -L. -l$(LIBRARYNAME)

-include config.mak

all:
	@test -e config.mak || ./configure -error
	$(MAKE) static
	$(MAKE) create_test

static: $(OBJECTS)
	$(AR) -cvq $(STATICLIBRARY) $(OBJECTS)

create_test: $(TESTOBJECTS)

clean:
	$(RM) $(OBJECTS) $(TESTOBJECTS) $(STATICLIBRARY) config.mak 

install:
	mkdir -p $(DESTDIR)/$(prefix)/include $(DESTDIR)/$(prefix)/lib/
	cp $(STATICLIBRARY) $(DESTDIR)/$(prefix)/lib/
	cp -r include/$(LIBRARYNAME)/ $(DESTDIR)/$(prefix)/include

# 
