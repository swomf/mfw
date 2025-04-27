#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define EVENT_SIZE (sizeof(struct inotify_event))

static inline void clear() {
  if (isatty(fileno(stdout)))
    system("clear");
}

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Usage: mfw <filename> <command> [args...]\n");
    exit(EXIT_FAILURE);
  }
  const char *file_path = argv[1];
  const char *file_name = basename(argv[1]);
  const char *dir_name = dirname(argv[1]);

  size_t cmd_length = 0;
  for (int i = 2; i < argc; i++)
    cmd_length += strlen(argv[i]) + 1; /* +1 for space and null terminator */

  char *cmd = malloc(cmd_length);
  if (cmd == NULL) {
    perror("Couldn't malloc for cmd_length");
    exit(EXIT_FAILURE);
  }

  /* unload variable user input args into 1 str */
  for (int i = 2; i < argc; i++) {
    strcat(cmd, argv[i]);
    if (i < argc - 1)
      strcat(cmd, " ");
  }

  /* filename is: (1) an existing file (2) readable (3) not a directory */
  /* FIXME: does not pass 01-is-file-not-dir test */
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    free(cmd);
    fprintf(stderr, "Couldn't stat %s as file\n", file_path);
    exit(EXIT_FAILURE);
  }
  fclose(file);

  int length, i = 0;
  int wd, fd;
  char buf[EVENT_SIZE + NAME_MAX];

  fd = inotify_init();
  if (fd < 0) {
    free(cmd);
    perror("Couldn't initialize inotify");
    exit(EXIT_FAILURE);
  }

  /* Watch its dir instead. https://unix.stackexchange.com/a/312359 */
  wd = inotify_add_watch(fd, dir_name, IN_MODIFY | IN_DELETE);

  clear();
  system(cmd);

  while (1) {
    i = 0;
    length = read(fd, buf, EVENT_SIZE + NAME_MAX);
    if (length < 0) {
      free(cmd);
      perror("Couldn't read file descriptor");
      exit(EXIT_FAILURE);
    }

    while (i < length) {
      struct inotify_event *event = (struct inotify_event *)&buf[i];
      if (!event->len)
        continue;
      i += EVENT_SIZE + event->len;

      int is_file_arg = strcmp(event->name, file_name) == 0 ? 1 : 0;
      if (event->mask & IN_MODIFY && !(event->mask & IN_ISDIR) && is_file_arg) {
        clear();
        system(cmd);
      }

      /* FIXME: doesn't detect mv */
      if (event->mask & IN_DELETE && !(event->mask & IN_ISDIR) && is_file_arg) {
        fprintf(stderr, "File deleted. Closing.\n");
        free(cmd);
        exit(EXIT_SUCCESS);
      }
    }
  }

  /*
   * Unreachable, because the loop above can only return, not break.
   * Thus we defer the following to the system
   *
   * inotify_rm_watch(fd, wd);
   * close(fd);
   * free(cmd);
   */

  return EXIT_SUCCESS;
}
