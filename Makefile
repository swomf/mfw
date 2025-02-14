PROG := mfw
DESTDIR ?= /usr/local/bin

all: $(PROG)

install: $(PROG)
	install -Dm755 $(PROG) $(DESTDIR)

uninstall:
	$(RM) $(DESTDIR)/$(PROG)

.PHONY: all install
