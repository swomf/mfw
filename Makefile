PROG := mfw
PREFIX ?= /usr/local
ZSHCOMPDIR ?= $(PREFIX)/share/zsh/site-functions

TESTS = $(patsubst %.in,%,$(wildcard tests/[0-9][0-9]-*.in))

.PHONY: all
all: $(PROG)

.PHONY: install
install: $(PROG)
	install -Dm755 $(PROG) $(DESTDIR)$(PREFIX)/bin/$(PROG)
	install -Dm644 completions/zsh/_$(PROG) $(DESTDIR)$(ZSHCOMPDIR)/_$(PROG)

.PHONY: uninstall
uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/$(PROG)
	$(RM) $(ZSHCOMPDIR)/_$(PROG)

.PHONY: fmt
fmt: $(TESTS)
	clang-format -i *.c
	shellcheck $(TESTS)

.PHONY: test
test: $(TESTS)
	@find tests -type f -executable | xargs -n1 -P $(shell nproc) sh

tests/%: tests/%.in tests/common $(PROG)
	@m4 -Itests -P $< >$@
	@chmod +x "$@"
	@printf '  \033[1;94mGEN\033[0m     '$@'\n'

.PHONY: clean
clean:
	$(RM) $(PROG)
	$(RM) $(TESTS) tests/*tmp tests/*out
