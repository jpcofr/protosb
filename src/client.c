#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv)
{

  struct sockaddr_in servaddr;
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (sock == -1)
    perror("Socket");

  bzero((void *)&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(6782);
  char *ip_str = "172.17.0.2";
  servaddr.sin_addr.s_addr = inet_addr(ip_str);

  if (-1 == connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)))
    perror("Connect");

  while (1)
  {

    char message[6];
    fgets(message, 6, stdin);

    message[5] = '\0';

    send(sock, message, 6, 0);
  }

  close(sock);
}

/*
// Client side C/C++ program to send meaningless OSPF packages

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    printf("Client started...\n");
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "172.17.0.2", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    //for (;;)
    {
        send(sock, hello, strlen(hello), 0);
        printf("Hello message sent to server\n");
        valread = read(sock, buffer, 1024);
        printf("Got a message!\n");
        printf("%s\n", buffer);
    }
    return 0;
}
*/
