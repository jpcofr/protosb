// Client side C/C++ program to demonstrate Socket programming
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

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0)
    client_error("\nInvalid address/ Address not supported\n");

  if (connect(local_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    client_error("\nConnection Failed\n");

  std::vector<std::string> messages({"This is a test ", "to check out ", "the interaction ", "between C++ code and ", "plain C code.\n"});
  for (int i = 0; i < 100; i++)
  {
    for (auto msg : messages)
    {
      send(local_socket, msg.c_str(), msg.length(), 0);

      std::cout << msg.c_str() << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }

  return 0;
}
