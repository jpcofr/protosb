// Server side example showing the use of epoll
#include <iostream>
#include <netinet/in.h> // The header file in.h contains constants and structures
                        // needed for internet domain addresses. Includes socket.h
#include <sys/epoll.h>
#include <unistd.h> // Header file that provides access to the POSIX operating system API
#include <fcntl.h>

#define PORT 8080
#define MAX_EVENTS 10

void server_error(std::string msg)
{
  std::cerr << msg;
  exit(EXIT_FAILURE);
}
// change 2
int main(int argc, char const *argv[])
{
  // Creating socket file descriptor (See https://bit.ly/2z6zTiG)
  int server_file_descriptor; // May also be called 'socket descriptor'
  if ((server_file_descriptor =
           socket(AF_INET,     // Domain: Protocol family. (See https://bit.ly/2Fijxrh)
                  SOCK_STREAM, // Type: Communication semantics (e.gr. SOCK_RAW Raw network protocol access)
                  0)           // Protocol: Many protocols may exist. Otherwise, the protocol is 0.
                               // (See '/etc/protocols' and GETPROTOENT(3))
       ) == 0)
    server_error("socket failed");

  int socket_options = 1; // The optlen argument of getsockopt() and setsockopt() is in reality an int [*]
  if (
      // getsockopt, setsockopt - get and set options on sockets (See https://bit.ly/2W75Jpk)
      // The level at which the option resides and the name of the option must be specified.
      setsockopt(server_file_descriptor,      // Socket/file descriptor.
                 SOL_SOCKET,                  // Level: Manipulate options at the sockets API level.
                                              // E.gr. if an option is to be interpreted by the TCP protocol,
                                              // level should be set to the protocol number of TCP
                 SO_REUSEADDR | SO_REUSEPORT, // Optname: options are passed uninterpreted to the
                                              // appropriate protocol module for interpretation (See socket(7) https://bit.ly/2TNUNzr).
                                              // SO_REUSEPORT Permits multiple AF_INET or AF_INET6
                                              // sockets to be bound to an identical socket address.
                 &socket_options,             // (const void *) Access option values for setsockopt. The argument should
                                              // be nonzero to enable a boolean option, or zero if the option is to be disabled.
                 sizeof(socket_options)       //
                 ))
    server_error("setsockopt");

  struct sockaddr_in server_address;           // (See https://bit.ly/2TGyP27)
  server_address.sin_family = AF_INET;         // (sa_family_t) Address family: AF_INET
  server_address.sin_port =                    // (in_port_t) Port in network byte order
      htons(PORT);                             // Convert values between host and network byte order (See https://bit.ly/2FhI9AC)
  server_address.sin_addr.s_addr = INADDR_ANY; // (struct in_addr) Internet address, a plain integer.
                                               // INADDR_ANY =(u_int32_t)0x00000000

  if (
      // Bind a name to a socket (See https://bit.ly/2Fjvv40)
      bind(server_file_descriptor,             // Server's socket/file descriptor.
           (struct sockaddr *)&server_address, // Server's address data. The structure depends on the address family.
                                               // The only purpose of this structure is to cast the structure pointer
                                               // passed in addr in order to avoid compiler warnings.
           sizeof(server_address)              //
           ) < 0)
    server_error("bind failed");

  if (
      // Listen for connections on a socket (See https://bit.ly/2CoGLKF)
      // Listen marks the referred socket as a passive socket it will accept
      // incoming connection requests using accept(2).
      listen(server_file_descriptor, // Server's socket/file descriptor.
                                     // Refers to a socket of type SOCK_STREAM or SOCK_SEQPACKET.
             3                       // Backlog: Defines queue of pending connections maximum length.
                                     // pending connections for sockfd may grow.
             ) < 0)
    server_error("listen");

  // epoll_event structure  (See https://bit.ly/2CosMEK)
  //  (uint32_t)events: 	Epoll events
  //  (epoll_data_t) data:  User data variable (void *ptr, int file_descriptor, uint32_t u32, uint64_t u64)
  struct epoll_event server_epoll_config;
  int ready_file_descriptors, epoll_file_descriptor;
  epoll_file_descriptor = epoll_create1(0);
  if (
      // Opens an epoll file descriptor (See https://bit.ly/2W6TdGk)
      // epoll_create() creates an epoll(7) instance (See https://bit.ly/2TXXWwm)
      epoll_file_descriptor == -1)
    server_error("epoll_create1");

  server_epoll_config.events =
      EPOLLIN; // Is a bit mask to describe the file/socket descriptor status.
               // EPOLLIN The descriptor is available for read(2) operations. Is a bit mask.
               // The memory area pointed to by events will contain the events
               // that will be available for the caller.
  server_epoll_config.data.fd =
      server_file_descriptor;
  if (
      // Control interface for an epoll file descriptor. Its use is to add, modify, or remove entries in the
      // interest list of the epoll(7) instance referred to by the file descriptor(See https://bit.ly/2Y3iH9d)
      epoll_ctl(epoll_file_descriptor,  // epoll file descriptor.
                EPOLL_CTL_ADD,          // Sets up the operation to be done on the server descriptor
                                        // EPOLL_CTL_ADD, EPOLL_CTL_MOD, EPOLL_CTL_DEL
                server_file_descriptor, // requests that the operation op be performed for the target file
                                        // descriptor, in this case the server's socket/file descriptor.
                &server_epoll_config    // The configuration to be applied to the target descriptor.
                ) == -1)
    server_error("epoll_ctl: server_file_descriptor");

  struct epoll_event epoll_events[MAX_EVENTS];
  int incoming_connection_socket_descriptor;
  int server_address_length = sizeof(server_address);
  struct sockaddr_in client_address;
  int client_address_length = sizeof(client_address);
  while (true)
  {
    ready_file_descriptors =
        // Wait for an I/O event on an epoll file descriptor.
        // Up to maxevents are returned. The argument must be greater than zero.
        // This system call waits for events on the epoll(7) instance
        // referred to by the file descriptor. (See https://bit.ly/2CosMEK)
        epoll_wait(epoll_file_descriptor, // epoll's file descriptor.
                   epoll_events,          // The memory area pointed to by the events will contain
                                          // the events that will be available for the caller.
                   MAX_EVENTS,            // Maximum number of events that will be attended.
                   -1                     // The timeout argument specifies the number of milliseconds that
                                          // epoll_wait() will block. The call will block until either:
                                          //  *  a file descriptor delivers an event;
                                          //  *  the call is interrupted by a signal handler; or
                                          //  *  the timeout expires.
                                          // a timeout of -1 causes epoll_wait() to block indefinitely.
        );

    if (ready_file_descriptors == -1)
      server_error("epoll_wait");

    for (int n = 0; n < ready_file_descriptors; ++n) // Check if any event is available from the caller
    {
      if (epoll_events[n].data.fd == server_file_descriptor) // Is the server ready to be consumed?
      {                                                      // Yes, consume the event and start observing its descriptor...

        // accept a connection on a socket. This system call is used with
        // connection-based socket types (SOCK_STREAM, SOCK_SEQPACKET).
        incoming_connection_socket_descriptor =
            //accept(server_file_descriptor,         	// Already binded socket that is listening.
            //   	(struct sockaddr *)&server_address, // This structure is filled in with the address of the peer socket
            //   	(socklen_t *)&server_address_length //
            //);
            accept(server_file_descriptor,             // Already binded socket that is listening.
                   (struct sockaddr *)&client_address, // This structure is filled in with peer's socket address
                   (socklen_t *)&client_address_length //
            );
        if (incoming_connection_socket_descriptor == -1)
          server_error("accept");

        int flags_for_fcntl = 1;
        // The socket must not block the program...
        // fcntl - manipulate file descriptor
        fcntl(incoming_connection_socket_descriptor, // Socket/file descriptor to be modified
              O_NONBLOCK,                            // The operation that will be applied to the socket
              &flags_for_fcntl                       //
        );

        server_epoll_config.events = EPOLLIN | EPOLLET; // EPOLLET: Sets the Edge Triggered behavior (See https://bit.ly/2Cpwqy1)
        server_epoll_config.data.fd = incoming_connection_socket_descriptor;
        if (
            // Make the incoming connection descriptor events observable
            epoll_ctl(epoll_file_descriptor,
                      EPOLL_CTL_ADD,
                      incoming_connection_socket_descriptor,
                      &server_epoll_config) == -1)
          server_error("epoll_ctl: incoming_connection_socket_descriptor");
      }
      else
      {
        char buffer[1024];
        std::fill(buffer, buffer + sizeof(buffer), 0);

        if (read(epoll_events[n].data.fd, // Socket/file descriptor to be read
                 buffer,                  // The buffer in which the data is going to be put on.
                 1024                     // Number of bytes to be read
                 ) < 0)
          server_error("read");

        std::cout << buffer << std::endl;
      }
    }
  }

  return 0;
}
