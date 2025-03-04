# μfilewatch

A microscopic file watch for Linux.

`mfw` watches a file for updates, and runs a command when one is detected.

This can be considered a minimal version of
[inotifywait](https://linux.die.net/man/1/inotifywait). See [LIMITATIONS](#limitations).

## example usage

```bash
# Run pdflatex on file change
mfw homework.tex pdflatex homework.tex
```

If the file disappears, the `mfw` process disappears as well.

## installation

Build and install to the default `$PREFIX`.

```bash
make
sudo make install
```

If `$PREFIX` is unset, `mfw` installs to `/usr/local`.  
You can also specify a prefix via environment variable:

```bash
# installs to ~/.local/bin/mfw
PREFIX=~/.local make install
```

## features

- [x] Watch for file change to run a shell command
- [x] Kill process if file disappears
- [ ] Recursively watch a directory

## limitations

### use of inotify

This only watches a single file using the Linux kernel `inotify` library.

By using `inotify`, we inherit some limitations of `inotify`:
* Nonrecursive (to implement recursion, all of a directory's
subdirectories need manual assignment of `inotify` descriptors)
* Requires support by the Linux kernel (reasonable; `inotify` was added in [2.6.13](https://en.wikipedia.org/wiki/Inotify#:~:text=2.6.13))
* May lose events due to race conditions, e.g. time-to-check-to-time-to-use
([article](https://www.wingolog.org/archives/2018/05/21/correct-or-inotify-pick-one))
* Has a maximum amount of simultaneous watches dictated by the kernel (see [SUSE docs](https://www.suse.com/support/kb/doc/?id=000020048#:~:text=cat%20/proc))

However, since `mfw` does not implement recursion (not included in `inotify` API),
the issue of $O(n)$ memory scaling to the amount of directories watched does not apply.
This also means buffer overflows are unlikely.

> [!NOTE]
> Although `inotify` isn't very efficient, alternatives such as `fanotify` require root.

### comparison of similar software

* swomf/mfw
  * 🟩 Implemented in a single source file in pure C.
  * 🟩 Minimal memory for the job (uses about 1MB)
  * 🟥 Only watches a single file in Linux via `inotify`.
* [emcrisostomo/fswatch](https://github.com/emcrisostomo/fswatch)
  * 🟩 Written with minimal dependencies in C++11.
  * 🟩 Avoids `inotify`.
  * 🟩 Reasonably memory-efficient (starts at around 5MB; uses 42MB to watch my whole home directory)
  * Uses autoconf
* [watchexec](https://github.com/watchexec/watchexec)
  * 🟩 Written in Rust as both a library and a CLI (`cargo install watchexec-cli`)
  * 🟩 Avoids `inotify`
  * Somewhat memory-efficient (starts at around 10MB; uses 42MB to watch my whole home directory)
* [inotify-tools: inotifywait](https://github.com/inotify-tools/inotify-tools)
  * 🟥 Not as simple as the above three software
  * 🟥 Uses `inotify` for recursion
  * Uses autoconf
* [inotify_dtree.c](https://www.man7.org/tlpi/code/online/dist/inotify/inotify_dtree.c.html)
  * 🟩 Implemented as a single (big) source file in pure C
  * 🟥 Written only as a literature example for `inotify`
  * 🟥 Incredibly memory-inefficient during recursion (uses 300MB+ to watch my whole home directory)
