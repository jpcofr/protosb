// Server side C/C++ program to demonstrate Socket programming
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>

#define PORT 8080
#define MAX_EVENTS 10

void server_error(std::string msg)
{
  std::cerr << msg;
  exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[])
{
  // Creating socket file descriptor
  int server_fd;
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    server_error("socket failed");

  // Forcefully attaching socket to the port 8080
  int socket_options = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                 &socket_options, sizeof(socket_options)))
    server_error("setsockopt");

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(PORT);

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&server_address,
           sizeof(server_address)) < 0)
    server_error("bind failed");

  if (listen(server_fd, 3) < 0)
    server_error("listen");

  struct epoll_event ev, events[MAX_EVENTS];
  int nfds, epollfd;

  epollfd = epoll_create1(0);
  if (epollfd == -1)
    server_error("epoll_create1");

  ev.events = EPOLLIN;
  ev.data.fd = server_fd;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_fd, &ev) == -1)
    server_error("epoll_ctl: server_fd");

  int incoming_connection_socket;
  int server_address_length = sizeof(server_address);
  for (;;)
  {
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1)
      server_error("epoll_wait");

    for (int n = 0; n < nfds; ++n)
    {
      if (events[n].data.fd == server_fd)
      {
        incoming_connection_socket = accept(server_fd, (struct sockaddr *)&server_address, (socklen_t *)&server_address_length);
        if (incoming_connection_socket == -1)
          server_error("accept");

        int flags = 1;
        fcntl(incoming_connection_socket, O_NONBLOCK, &flags); // the socket must not block the program.
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = incoming_connection_socket;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, incoming_connection_socket, &ev) == -1)
          server_error("epoll_ctl: incoming_connection_socket");
      }
      else
      {
        int err;
        char buf[1024];
        std::fill(buf, buf + sizeof(buf), 0);

        if (err = read(events[n].data.fd, buf, 1024) < 0)
          server_error("read");

        std::cout << buf << std::endl;
      }
    }
  }

  return 0;
}
