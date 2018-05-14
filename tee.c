#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

int main (int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "must give file argument\n");
    exit(1);
  }

  int output_fd;
  output_fd = open(argv[1],
                   O_WRONLY | O_CREAT | O_TRUNC,
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

  char buffer[BUFFER_SIZE + 1];
  ssize_t bytes_read;

  while (bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) {
    // print bytes to stdout
    buffer[bytes_read] = '\0';
    printf("%s", buffer);
    // write bytes to file
    if (write(output_fd, buffer, bytes_read) != bytes_read) {
      perror("write() failed");
      exit(1);
    }
  }

  if (bytes_read == -1) {
    perror("read() failed");
  }

  if (close(output_fd) == -1) {
    perror("close() failed");
  }

  return EXIT_SUCCESS;
}
