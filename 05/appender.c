/*
 * Question: Write a program that opens an existing file for writing with the
 * O_APPEND flag, and then seeks to the beginning of the file before
 * writing some data. Where does the data append in the file? Why?
 *
 * Answer:
 * It appends the data to the end of the file.
 * From the open(2) manpage: The file is opened in append
 * mode.  Before each write(2), the file offset is positioned at the
 * end of the file, as if with lseek(2).
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
  if (argc != 2 || strcmp(argv[1], "--help") == 0) {
    printf("usage: %s <file>\n", argv[0]);
    exit(1);
  }

  int fd;
  fd = open(argv[1], O_APPEND | O_WRONLY);
  if (fd == -1) {
    perror("error opening file");
    exit(1);
  }

  if (lseek(fd, 0, SEEK_SET) == -1) {
    perror("lseek failed");
  }

  if (write(fd, "where does it append :)", 24) == -1) {
    perror("could not write bytes to file");
  }

  if (close(fd) == -1) {
    perror("error closing output file");
    exit(1);
  }

  return EXIT_SUCCESS;
}
