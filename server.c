#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int setSocket(int sockfd, int opt);
int bindSocket(struct sockaddr_in server_address, int sockfd);

int main(void) {

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  int opt = 1;

  int setSocketOutput = setSocket(sockfd, opt);

  struct sockaddr_in server_address;

  int bindingOutput = bindSocket(server_address, sockfd);

  int listeningOutput = listen(sockfd, 10);

  if (listeningOutput == -1) {
    printf("Listening error!");
    return 1;
  }

  printf("successfully accepting requests!\n");

  while (1) {
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);

    int client_fd = accept(sockfd, (struct sockaddr *)&client_address,
                           (socklen_t *)&client_len);

    if (client_fd == -1) {
      printf("accept() error!");
      return 1;
    }

    char buffer[4096];

    memset(&buffer, 0, sizeof(buffer));

    int result = recv(client_fd, buffer, sizeof(buffer), 0);

    if (result < 0) {
      printf("recv failure!");
      return 1;
    }

    printf("%s\n", buffer);
    char *outBuffer =
        "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!\r\n";

    int sendOutput = send(client_fd, outBuffer, (int)strlen(outBuffer), 0);

    close(client_fd);
  }

  return 0;
}

int setSocket(int sockfd, int opt) {
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if (sockfd < 0) {
    perror("Socket connection error!");
    return 1;
  }

  printf("Socket connected successfully!\n");
  return 0;
}

int bindSocket(struct sockaddr_in server_address, int sockfd) {
  memset(&server_address, 0, sizeof(server_address));

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8080);
  server_address.sin_addr.s_addr = INADDR_ANY;

  int bindOutput =
      bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));

  if (bindOutput == -1) {
    printf("Error binding socket");
    return 1;
  }

  printf("Binding successful!\n");
  return 0;
}
