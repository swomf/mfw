# μfilewatch

A microscopic file watch, similar to
[inotifywait](https://linux.die.net/man/1/inotifywait).

## example usage

```bash
# Run pdflatex on file change
mfw some-file.tex pdflatex some-file.tex
```

If the file disappears, the `mfw` process disappears as well.

## installation

Build and install to the default `$PREFIX`.

```bash
make
sudo make install
```

If you want to install `mfw` somewhere other than `/usr/local`,
specify a prefix via environment variable:

```bash
# installs to ~/.local/bin/mfw
PREFIX=~/.local make install
```

## features

- [x] Watch for file change to run a shell command
- [x] Kill process if file disappears
- [ ] Recursively watch a directory
