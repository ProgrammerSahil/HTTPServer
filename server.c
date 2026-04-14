#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(void){

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if(sockfd < 0){
    perror("Socket connection error!");
    return 1;
  }

  printf("Socket connected successfully!\n");

  struct sockaddr_in server_address;

  memset(&server_address, 0, sizeof(server_address));

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8080);
  server_address.sin_addr.s_addr = INADDR_ANY;


  


  return 0;
}
