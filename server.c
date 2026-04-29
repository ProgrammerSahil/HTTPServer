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
    char method[16], path[256], protocol[16];

    sscanf(buffer, "%s %s %s", method, path, protocol);
    printf("Requested Path: %s", path);

    FILE *file_to_serve = NULL;
    char *headers = NULL;

    if(strcmp(path, "/") == 0 || strcmp(path, "/index.html") == 0){
      file_to_serve = fopen("index.html", "r");
      headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    } else if(strcmp(path, "/style.css") == 0){
      file_to_serve = fopen("style.css", "r");
      headers = "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n";
    }

    if(file_to_serve == NULL){
      char *not_found = "HTTP/1.1 404 Not Found\r\n\r\n404: File Not Found";
      send(client_fd, not_found, strlen(not_found), 0);
    } else {
      send(client_fd, headers, strlen(headers), 0);

      char file_buffer[4096];
      size_t bytes_read;
      while((bytes_read = fread(file_buffer, 1, sizeof(file_buffer), file_to_serve)) > 0){
        send(client_fd, file_buffer, bytes_read, 0);
      }
      fclose(file_to_serve);
    }
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
