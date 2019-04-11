#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// what to do here?
int takewhile(char *buf, bool(*pred)(int), FILE *stream) {
  int c;
  char *cp = buf;
  while ((c = fgetc(stream)) > 0 && pred(c)) {
    *cp++ = c;
  }

  if (c != EOF) {
    ungetc(c, stream);
  }

  *cp = '\0';
  return cp - buf; 
}

int dropwhile(bool(*pred)(int), FILE *stream) {
  uint32_t n = 0;
  int c;
  while ((c = fgetc(stream)) > 0 && pred(c)) {
    n++;
  }
  return n;
}

bool is_numeric(int c) {
  return c == '+' || c == '-' || ('0' <= c && c <= '9');
}

bool not_numeric(int c) {
  return !is_numeric(c);
}

// this needs to return an err...
int *read_int(int *n, FILE *stream) {
  // BUFFER OVERFLOW
  char buf[256];
  dropwhile(not_numeric, stream);
  takewhile(buf, is_numeric, stream);
  errno = 0;
  *n = strtol(buf, NULL, 10);
  assert(errno == 0);
  return n;
}

int main(int argc, char *argv[]) {
  FILE *stream = fopen("input.txt", "r");
  int i = 0;
  int n;
  while (read_int(&n, stream) != NULL) {
    printf("%d n: %d\n", i++, n);
  }
}
