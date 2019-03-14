#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/select.h>

int main(int argc, char **argv)
{

  fd_set fds, readfds;
  int i, clientaddrlen;
  int clientsock[2], rc, numsocks = 0, maxsocks = 2;

  int serversock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serversock == -1)
    perror("Socket");

  struct sockaddr_in serveraddr, clientaddr;
  bzero(&serveraddr, sizeof(struct sockaddr_in));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(6782);

  if (-1 == bind(serversock, (struct sockaddr *)&serveraddr,
                 sizeof(struct sockaddr_in)))
    perror("Bind");

  if (-1 == listen(serversock, SOMAXCONN))
    perror("Listen");

  FD_ZERO(&fds);
  FD_SET(serversock, &fds);

  while (1)
  {

    readfds = fds;
    rc = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);

    if (rc == -1)
    {
      perror("Select");
      break;
    }

    for (i = 0; i < FD_SETSIZE; i++)
    {
      if (FD_ISSET(i, &readfds))
      {
        if (i == serversock)
        {
          if (numsocks < maxsocks)
          {
            clientsock[numsocks] = accept(serversock,
                                          (struct sockaddr *)&clientaddr,
                                          (socklen_t *)&clientaddrlen);
            if (clientsock[numsocks] == -1)
              perror("Accept");
            FD_SET(clientsock[numsocks], &fds);
            numsocks++;
          }
          else
          {
            printf("Ran out of socket space.\n");
          }
        }
        else
        {
          int messageLength = 5;
          char message[messageLength + 1];
          int in, index = 0, limit = messageLength + 1;

          while ((in = recv(i, &message[index], limit, 0)) > 0)
          {
            index += in;
            limit -= in;
          }

          printf("%d\n", index);
          printf("%s\n", message);
        }
      }
    }
  }

  close(serversock);
  return 0;
}

/*
// Server side C/C++ program to listen meaningless OSPF packages
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
  printf("Server started...\n");
  int server_fd, new_socket, valread;
  struct sockaddr_in address;

  int opt = 1;
  int addrlen = sizeof(address);

  char buffer[1024] = {0};
  char *hello = "Hello from server";

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET,
           SO_REUSEADDR | SO_REUSEPORT,
           &opt, sizeof(opt)))
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address,
       sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
               (socklen_t *)&addrlen)) < 0)
  {

    perror("accept");
    exit(EXIT_FAILURE);
  }

  printf("I am listening\n");
  for (;;)
  {
    //valread = read(new_socket, buffer, 1024);
    recv(new_socket, &buffer, 1, MSG_PEEK | MSG_DONTWAIT);
    printf("Got a message!\n");
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent to client\n");
  }

  return 0;
}*/
