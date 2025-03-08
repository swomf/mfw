PROG := mfw
PREFIX ?= /usr/local

all: $(PROG)

install: $(PROG)
	install -Dm755 $(PROG) $(DESTDIR)$(PREFIX)/bin/$(PROG)

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/$(PROG)

fmt:
	clang-format -i $(PROG)

.PHONY: all install uninstall fmt
