#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#define PORT 8080

int main(int argc, char const *argv[]) {
  int opt = 1;
  int server_fd;
  int new_socket;
  int valread;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  char* hello = "Hello from server";

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket failed\n");
    exit(EXIT_FAILURE);
  }

  // Gak pake SO_REUSEADDR | SO_REUSEPORT. Jadinya cuma SO_REUSEADDR
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0) {
    perror("Bind failed\n");
    exit(EXIT_FAILURE);
  }

  connection_invalid:
    if (listen(server_fd, 3) < 0) {
      perror("Listen failed\n");
      exit(EXIT_FAILURE);
    }

  if ((new_socket = accept(server_fd, (struct sockaddr*) &address, (socklen_t*) &addrlen)) < 0) {
    printf("Accept failed, go to listening again\n");
    goto connection_invalid;

    perror("Accept failed\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    printf("Input>> ");
    scanf("%s", hello);
    send(new_socket, hello, strlen(hello), 0);

    if (recv(new_socket, buffer, 1024, 0) == 0) goto connection_invalid;

    printf("From client: %s\n", buffer);
  }

  return EXIT_SUCCESS;
}
