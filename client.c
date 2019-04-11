#include <arpa/inet.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BACK_LOG 5

int main(int argc, char *argv[]) {
  int port = atoi(argv[2]);

  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_addr = {
    .sin_family = AF_INET,
    .sin_port = htons(port)
  };

  int rc = inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
  assert(rc == 1);

  int err = connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof serv_addr);
  assert(!err);

  char buf[4096];
  memset(buf, 0, sizeof buf);
  ssize_t n;

  while ((n = read(STDIN_FILENO, buf, sizeof buf)) > 0) {
    ssize_t N = write(sock_fd, buf, n);
    assert(N == n);
  }

  memset(buf, 0, sizeof buf);

  // why does this loop forever?
  while ((n = read(sock_fd, buf, sizeof buf)) > 0) {
    ssize_t N = write(STDOUT_FILENO, buf, n);
    assert(N == n);
  }

  close(sock_fd);
}
