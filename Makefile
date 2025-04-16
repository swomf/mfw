PROG := mfw
PREFIX ?= /usr/local
ZSHCOMPDIR ?= $(PREFIX)/share/zsh/site-functions

all: $(PROG)

install: $(PROG)
	install -Dm755 $(PROG) $(DESTDIR)$(PREFIX)/bin/$(PROG)
	install -Dm644 completions/$(PROG)-completion.zsh $(DESTDIR)$(ZSHCOMPDIR)/_$(PROG)

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/$(PROG)
	$(RM) $(ZSHCOMPDIR)/_$(PROG)

fmt:
	clang-format -i $(PROG)

.PHONY: all install uninstall fmt
