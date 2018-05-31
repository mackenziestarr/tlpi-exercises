// compile with _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

  if (argc != 3 || strcmp(argv[1], "--help") == 0) {
    printf("%s pathname offset", argv[1]);
    exit(1);
  }

  int fd;
  fd = open(argv[1],
            O_CREAT | O_WRONLY,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

  if (fd == -1) {
    perror("error opening input file");
    exit(1);
  }

  off_t offset = atoll(argv[2]);
  if (lseek(fd, offset, SEEK_SET) == -1) {
    perror("lseek failed");
  }

  if (write(fd, "testing", 7) == -1) {
    perror("could not write bytes to file");
  }

  if (close(fd) == -1) {
    perror("error closing output file");
    exit(1);
  }

  return EXIT_SUCCESS;
}
