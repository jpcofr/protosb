// TCP Client side for test purposes
#include <arpa/inet.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#define PORT 8080
#define SERVER_IP "172.17.0.2"

void client_error(std::string msg)
{
  std::cerr << msg;
  exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[])
{
  int local_socket = 0;
  if ((local_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    client_error("\n Socket creation error \n");

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  if (
      // Convert IPv4 and IPv6 addresses from text to binary form
      inet_pton(AF_INET,                 // Address family intended for the structure
                SERVER_IP,               // String to be converted into a network address structure
                &server_address.sin_addr // Destiny of the converted network
                ) <= 0)
    client_error("\nInvalid address/ Address not supported\n");

  // Initiate a connection on a socket
  if (connect(local_socket,                       // connects the socket referred to by the file
                                                  // descriptor sockfd to the specified address
              (struct sockaddr *)&server_address, // Address to which datagrams are sent; the only address
                                                  //from which datagrams are received.
              sizeof(server_address)              //
              ) < 0)
    client_error("\nConnection Failed\n");

  std::vector<std::string> messages({"This is a sample ", "to check out ", "client/server interaction ", "using non-blocking ", "mechanisms.\n"});
  for (int i = 0; i < 100; i++)
  {
    for (auto msg : messages)
    {
      // Send a message on a socket
      send(local_socket, // Destiny's socket/file descriptor
           msg.c_str(),  // Message content buffer
           msg.length(), // Message length
           0             // Possible options MSG_NOSIGNAL
      );

      std::cout << msg.c_str() << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }

  return 0;
}
