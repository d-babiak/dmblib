#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BACK_LOG 5

ssize_t send_uint32(int sock_fd, uint32_t n) {
  uint32_t big_n = htonl(n);
  return write(sock_fd, &big_n, sizeof(uint32_t));
}

ssize_t recv_uint32(int sock_fd) {
  uint32_t net_long;
  ssize_t n_read = read(sock_fd, &net_long, sizeof(uint32_t));
  assert(n_read == 4);
  return ntohl(net_long);
}

int main(int argc, char *argv[]) {
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  assert(listen_fd >= 0);

  struct sockaddr_in serv_addr = {
    .sin_family = AF_INET,
    .sin_port = htons(atoi(argv[1]))
  };

  int rc = inet_pton(AF_INET, "0.0.0.0", &serv_addr.sin_addr);
  assert(rc == 1);

  int err = bind(listen_fd, (struct sockaddr *) &serv_addr, sizeof serv_addr);
  assert(!err);

  err = listen(listen_fd, BACK_LOG);
  assert(!err);

  struct sockaddr_in client_addr = { .sin_family = AF_INET };
  socklen_t client_len = sizeof client_addr;

  int client_fd = accept(
     listen_fd, 
    (struct sockaddr *) &client_addr, 
    &client_len
  );
  assert(client_fd >= 0);

  char buf[4096];
  for (;;) {
    memset(buf, 0, sizeof buf);
    uint32_t msg_len = recv_uint32(client_fd);
    ssize_t n = read(client_fd, buf, msg_len);
    buf[msg_len] = '\0';
    printf("  (%u) %s\n", msg_len, buf);
  }
}
