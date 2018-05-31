#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

void usage(int exit_code) {
  fprintf(stderr, "tee [-a <append>] <file>\n");
  exit(exit_code);
}

int main (int argc, char *argv[]) {

  int append_mode = 0;

  int arg;
  while ((arg = getopt(argc, argv, "ha")) != -1) {
    switch (arg) {
    case 'a':
      append_mode = 1;
      break;
    case 'h': usage(0);
    case '?':
    default: usage(1);
  }


  if (argv[optind] == NULL) {
    fprintf(stderr, "must specify a file as an argument\n");
    exit(1);
  }

  char *filename = argv[optind];
  int output_fd;
  output_fd = open(filename,
                   O_WRONLY | O_CREAT | (append_mode ? O_APPEND : O_TRUNC),
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
