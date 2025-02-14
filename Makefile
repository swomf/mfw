PROG := mfw
DESTDIR ?= /usr/local/bin

all: $(PROG)

install: $(PROG)
	install -Dm755 $(PROG) $(DESTDIR)

uninstall:
	$(RM) $(DESTDIR)/$(PROG)

fmt:
	clang-format -i $(PROG)

.PHONY: all install uninstall fmt
