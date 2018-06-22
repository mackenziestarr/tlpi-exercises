#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

/**
 * Implement setenv() and unsetenv() using getenv(), putenv(), and,
 * where necessary, code that directly modifies environ. Your version of
 * unsetenv() should check to see whether there are multiple definitions
 * of an environment variable, and remove them all (which is what the
 * glibc version of unsetenv() does).
 */
static void my_setenv(const char *name, const char *value, int overwrite) {
  size_t size = sizeof(char) * (strlen(name) + strlen("=") + strlen(value) + 1);
  char *environment_variable = (char *) malloc(size);
  snprintf(environment_variable, size, "%s=%s", name, value);
  putenv(environment_variable);
}

static void my_unsetenv(const char *name) {

}

int main() {
  my_setenv("HI", "mom", 0);
  assert(strcmp(getenv("HI"), "mom") == 0);
  return 0;
}
