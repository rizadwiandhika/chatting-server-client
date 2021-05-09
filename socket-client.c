#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

int main(int argc, char const *argv[]) {
  struct sockaddr_in address;
  int sock = 0;
  int valread;
  struct sockaddr_in serv_addr;
  char* hello = "Hello from client";
  char buffer[1024] = {0};

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket failed\n");
    return -1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("Invalid address or address not supported\n");
    return -1;
  }

  if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))) {
    printf("Connection failed\n");
    return -1;
  }

  while (1) {
    if (recv(sock, buffer, 1024, 0) == 0) goto jump;
    printf("From server: %s\n", buffer);
    scanf("Inpu>>: %s", hello);
    send(sock, hello, strlen(hello), 0);
  }

  jump:
    return 0;
}
