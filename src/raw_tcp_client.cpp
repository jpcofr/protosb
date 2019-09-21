// TCP client that sends raw
// Run as root or SUID 0, just datagram no data/payload
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>
#include <iostream>

// Packet length
#define PACKET_LENGTH 8192

#define SOURCE_IP "172.17.0.3"
#define SOURCE_PORT 8080
#define DESTINATION_IP "172.17.0.2"
#define DESTINATION_PORT 8080

void client_error(const std::string &msg)
{
  std::cerr << msg;
  exit(EXIT_FAILURE);
}

struct ipheader
{
  unsigned char iph_internet_header_length : 5; // Little-endian. The constant-expression specifies the width of the field in bits.
  unsigned char iph_version : 4;
  unsigned char iph_type_of_service;     // Differentiated Services Code Point.
  unsigned short int iph_length;         // Packet size in bytes.
  unsigned short int iph_identification; // Identify uniquely the group of fragments of a single IP datagram.
  unsigned char iph_flags;               // Three-bit field used to identify fragments. From most significant to least significant)
  // bit 0: Reserved; must be zero, bit 1: Don't Fragment (DF), bit 2: More Fragments (MF).
  unsigned short int iph_fragment_offset; //Specifies the offset of a fragment relative to the beginning of the original
  // unfragmented IP datagram. Measured in units of eight-byte blocks. It is 13 bits long.
  unsigned char iph_ttl;         // Time to live in seconds or hops.
  unsigned char iph_protocol;    // The protocol used in the data portion of the IP datagram.
  unsigned short int iph_chksum; // Error-checking of the header
  unsigned int iph_sourceip;
  unsigned int iph_destinationip;
};

struct tcpheader
{
  unsigned short int tcph_source_port;
  unsigned short int tcph_destination_port;
  unsigned int tcph_sequence_number;
  unsigned int tcph_acknowledgment_number;
  unsigned char tcph_offset : 4;   // Data offset (4 bits): TCP header's size in 32-bit words
  unsigned char tcph_reserved : 4; // TCP header's size in 32-bit words.
  unsigned int                     // Control bits
      tcp_res1 : 4,                // Little-endian
      tcph_hlen : 4,               // length of tcp header in 32-bit words
      tcph_fin : 1,                // Finish flag "fin"
      tcph_syn : 1,                // Synchronize sequence numbers to start a connection
      tcph_rst : 1,                // Reset flag
      tcph_psh : 1,                // Push, sends data to the application
      tcph_ack : 1,                // acknowledge
      tcph_urg : 1,                // urgent pointer
      tcph_res2 : 2;
  unsigned short int tcph_window_size;    //number of bytes the sender of this segment willing to receive
  unsigned short int tcph_checksum;       // Error-checking of the header, the payload and a pseudo-header
  unsigned short int tcph_urgent_pointer; // 16-bit field is an offset from the sequence number indicating the last urgent data byte.
};

unsigned short generate_checksum(unsigned short *buf, int len)
{
  unsigned long sum;
  for (sum = 0; len > 0; len--)
    sum += *buf++;

  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);

  return (unsigned short)(~sum);
}

int main(int argc, char *argv[])
{
  std::string dfadfasf;

  int client_socket_descriptor;

  // No data, just datagram
  char datagram_buffer[PACKET_LENGTH];

  struct ipheader *ip_header = (struct ipheader *)datagram_buffer;
  struct tcpheader *tcp_header = (struct tcpheader *)(datagram_buffer + sizeof(struct ipheader));
  struct sockaddr_in source_address, destination_address; // Socket address internet style

  std::fill(datagram_buffer, datagram_buffer + sizeof(datagram_buffer), 0);

  client_socket_descriptor = socket(
      AF_INET,    // Protocol family
      SOCK_RAW,   // Raw socket type: (See: https://bit.ly/2TxL10c)
      IPPROTO_TCP // Protocol: TCP
  );

  if (client_socket_descriptor < 0)
    client_error("socket() error");
  else
    std::cout << "socket()-SOCK_RAW and tcp protocol is OK.\n";

  // Address family
  source_address.sin_family = AF_INET;
  destination_address.sin_family = AF_INET;

  // Source port, can be any, modify as needed
  source_address.sin_port = htons(SOURCE_PORT);
  destination_address.sin_port = htons(DESTINATION_PORT);

  // Source IP, can be any, modify as needed
  source_address.sin_addr.s_addr = inet_addr(SOURCE_IP);
  destination_address.sin_addr.s_addr = inet_addr(DESTINATION_IP);

  // IP structure
  ip_header->iph_internet_header_length = 5;
  ip_header->iph_version = 4;
  ip_header->iph_type_of_service = 16;
  ip_header->iph_length = sizeof(struct ipheader) + sizeof(struct tcpheader);
  ip_header->iph_identification = htons(54321);
  ip_header->iph_fragment_offset = 0;
  ip_header->iph_ttl = 64;
  ip_header->iph_protocol = 6; // TCP
  ip_header->iph_chksum = 0;   // Done by kernel

  ip_header->iph_sourceip = inet_addr(SOURCE_IP);
  ip_header->iph_destinationip = inet_addr(DESTINATION_IP);
  tcp_header->tcph_source_port = htons(SOURCE_PORT);
  tcp_header->tcph_destination_port = htons(DESTINATION_PORT);

  tcp_header->tcph_sequence_number = htonl(1);
  tcp_header->tcph_acknowledgment_number = 0;
  tcp_header->tcph_offset = 5;
  tcp_header->tcph_syn = 1;
  tcp_header->tcph_ack = 0;
  tcp_header->tcph_window_size = htons(32767);
  tcp_header->tcph_checksum = 0; // Done by kernel
  tcp_header->tcph_urgent_pointer = 0;

  std::cout << "Destination IP: " << ip_header->iph_destinationip << std::endl;
  // IP checksum calculation
  ip_header->iph_chksum = generate_checksum((unsigned short *)datagram_buffer,
                                            (sizeof(struct ipheader) + sizeof(struct tcpheader)));

  // Inform the kernel do not fill up the headers' structure, we fabricated our own
  const int one = 1;
  const int *one_val = &one;
  if (
      setsockopt(
          client_socket_descriptor,
          IPPROTO_IP,
          IP_HDRINCL,
          one_val,
          sizeof(one) //
          ) < 0)
    client_error("setsockopt() error");
  else
    std::cout << "setsockopt() is OK\n";

  // sendto() loop, send every 2 second for 50 counts
  unsigned int count;
  for (count = 0; count < 200; count++)
  {
    if (sendto(client_socket_descriptor,
               datagram_buffer,
               sizeof(datagram_buffer)/*ip_header->iph_length*/,
               0,
               (struct sockaddr *)&source_address,
               sizeof(source_address)) < 0)
    { // Verify
      client_error("sendto() error");
    }
    else
    {
      std::cout << "Count " << count << " sendto() is OK\n";
      std::cout.write(datagram_buffer, sizeof(datagram_buffer));
    }
    sleep(2);
  }

  close(client_socket_descriptor);

  return 0;
}
