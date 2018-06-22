#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

static jmp_buf env;

static void setJumpAndReturn() {
  int id = setjmp(env);
  printf("we got here, env is: %d", id);
  return;
}

int main() {
  setJumpAndReturn();
  // 👟
  longjmp(env, 1); // this segfaults :)
  return EXIT_SUCCESS;
}
