/* Exercise 4-1 cp and preserve file holes*/

/*
  gcc -Wall -Wextra -std=c99 -g cpholes.c -o cpholes

  A crappy implementation that writes bytes one by one
  and seeks for each null byte

strace:
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 63.26    0.015934           1     16328           write
 33.43    0.008421           0     20536           lseek
  1.35    0.000341          21        16        12 open
  0.82    0.000206          52         4           close
  0.45    0.000114           3        38           read
  0.19    0.000048           7         7           mmap
  0.14    0.000035           9         4           mprotect
  0.12    0.000031           4         8         7 stat
  0.07    0.000018          18         1         1 access
  0.06    0.000015          15         1           munmap
  0.06    0.000015          15         1           execve
  0.02    0.000005           3         2           fstat
  0.02    0.000004           4         1           brk
  0.01    0.000003           3         1           arch_prctl
------ ----------- ----------- --------- --------- ----------------
100.00    0.025190                 36948        20 total

For comparison so we feel bad this is cp --sparse=auto:
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 53.85    0.000664          13        53        42 open
 19.30    0.000238          10        24           mmap
 12.33    0.000152          10        16           mprotect
  4.38    0.000054           6         9           read
  3.41    0.000042           4        11           fstat
  2.51    0.000031           3        12         9 stat
  2.35    0.000029           2        14           close
  0.97    0.000012           6         2         2 access
  0.32    0.000004           1         3           brk
  0.32    0.000004           4         1           execve
  0.24    0.000003           3         1           arch_prctl
  0.00    0.000000           0         2           write
  0.00    0.000000           0         3         1 lseek
  0.00    0.000000           0         1           munmap
  0.00    0.000000           0         2           rt_sigaction
  0.00    0.000000           0         1           rt_sigprocmask
  0.00    0.000000           0         1           ioctl
  0.00    0.000000           0         1           getrlimit
  0.00    0.000000           0         1           geteuid
  0.00    0.000000           0         2           statfs
  0.00    0.000000           0         1           set_tid_address
  0.00    0.000000           0         1           fadvise64
  0.00    0.000000           0         1           set_robust_list
------ ----------- ----------- --------- --------- ----------------
100.00    0.001233                   163        54 total
*/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024 // allow cc override via -D flag
#endif

int main (int argc, char *argv[]) {

  int in_fd;
  char *in_filename = argv[1];
  in_fd = open(in_filename, O_RDONLY);
  if (in_fd == -1) {
    perror("error opening input file");
    exit(1);
  }

  int out_fd;
  char *out_filename = argv[2];
  out_fd = open(out_filename,
                O_CREAT | O_WRONLY | O_TRUNC,
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  if (out_fd == -1) {
    perror("error opening output file");
    exit(1);
  }

  ssize_t bytes_read;
  char buffer[BUFFER_SIZE];
  while ((bytes_read = read(in_fd, buffer, BUFFER_SIZE)) > 0) {
    for (int i = 0; i < bytes_read; i++) {
      if (buffer[i] != '\0') {
        if (write(out_fd, &buffer[i], 1) != 1) {
          perror("could not write byte to destination file");
        }
      } else {
        if (lseek(out_fd, 1, SEEK_CUR) == -1) {
          perror("seek");
          exit(1);
        }
      }
    }

  }
  if (bytes_read == -1) {
    perror("error while reading input file");
    exit(1);
  }

  if (close(in_fd) == -1) {
    perror("could not close input file");
  }
  if (close(out_fd) == -1) {
    perror("could not close output file");
  }

  return EXIT_SUCCESS;
}
