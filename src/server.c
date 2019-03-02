// c++ -I ./include/ src/server.cpp -o example
// c++ -I include src/server.cpp -o example
// c++ -v -I include src/server.cpp -o example
// c++ -v -I include -lboost_thread src/server.cpp -o example
// c++ -Iinclude_test/boost_1_69_0/  src/server.cpp -o example
// c++ -Iinclude_test/boost_1_69_0/  src/server.cpp -o example
// c++ -std=c++11 src/server.cpp -o example
// c++ -x c src/server.cpp -o example
// c++ -I /Users/jpcf/Desktop/test/boost_1_65_0 /Users/jpcf/swAtelier/src/showcase/showcase_sandbox/cpp/casual/KnowYourTrade.cpp  -o example

// Server side C/C++ program to demonstrate Socket programming
//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux

#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>    //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define TRUE 1
#define FALSE 0
#define PORT 8888

int main(int argc, char *argv[])
{
  int opt = TRUE;
  int master_socket, addrlen, new_socket, client_socket[30],
      max_clients = 30, activity, i, valread, sd;
  int max_sd;
  struct sockaddr_in address;

  char buffer[1025]; //data buffer of 1K

  //set of socket descriptors
  fd_set readfds;

  //a message
  char *message = "ECHO Daemon v1.0 \r\n";

  //initialise all client_socket[] to 0 so not checked
  for (i = 0; i < max_clients; i++)
  {
    client_socket[i] = 0;
  }

  //create a master socket
  if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  //set master socket to allow multiple connections ,
  //this is just a good habit, it will work without this
  if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                 sizeof(opt)) < 0)
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  //type of socket created
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  //bind the socket to localhost port 8888
  if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  printf("Listener on port %d \n", PORT);

  //try to specify maximum of 3 pending connections for the master socket
  if (listen(master_socket, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  //accept the incoming connection
  addrlen = sizeof(address);
  puts("Waiting for connections ...");

  while (TRUE)
  {
    //clear the socket set
    FD_ZERO(&readfds);

    //add master socket to set
    FD_SET(master_socket, &readfds);
    max_sd = master_socket;

    //add child sockets to set
    for (i = 0; i < max_clients; i++)
    {
      //socket descriptor
      sd = client_socket[i];

      //if valid socket descriptor then add to read list
      if (sd > 0)
        FD_SET(sd, &readfds);

      //highest file descriptor number, need it for the select function
      if (sd > max_sd)
        max_sd = sd;
    }

    //wait for an activity on one of the sockets , timeout is NULL ,
    //so wait indefinitely
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    if ((activity < 0) && (errno != EINTR))
    {
      printf("select error");
    }

    //If something happened on the master socket ,
    //then its an incoming connection
    if (FD_ISSET(master_socket, &readfds))
    {
      if ((new_socket = accept(master_socket,
                               (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
      {
        perror("accept");
        exit(EXIT_FAILURE);
      }

      //inform user of socket number - used in send and receive commands
      printf("New connection , socket fd is %d , ip is : %s , port : %d \n " , new_socket , inet_ntoa(address.sin_addr) , ntohs
             (address.sin_port));

      //send new connection greeting message
      if (send(new_socket, message, strlen(message), 0) != strlen(message))
      {
        perror("send");
      }

      puts("Welcome message sent successfully");

      //add new socket to array of sockets
      for (i = 0; i < max_clients; i++)
      {
        //if position is empty
        if (client_socket[i] == 0)
        {
          client_socket[i] = new_socket;
          printf("Adding to list of sockets as %d\n", i);

          break;
        }
      }
    }

    //else its some IO operation on some other socket
    for (i = 0; i < max_clients; i++)
    {
      sd = client_socket[i];

      if (FD_ISSET(sd, &readfds))
      {
        //Check if it was for closing , and also read the
        //incoming message
        if ((valread = read(sd, buffer, 1024)) == 0)
        {
          //Somebody disconnected , get his details and print
          getpeername(sd, (struct sockaddr *)&address,
                      (socklen_t *)&addrlen);
          printf("Host disconnected , ip %s , port %d \n",
                 inet_ntoa(address.sin_addr), ntohs(address.sin_port));

          //Close the socket and mark as 0 in list for reuse
          close(sd);
          client_socket[i] = 0;
        }

        //Echo back the message that came in
        else
        {
          //set the string terminating NULL byte on the end
          //of the data read
          buffer[valread] = '\0';
          send(sd, buffer, strlen(buffer), 0);
        }
      }
    }
  }
  return 0;
}

/*
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/array.hpp>
#include <array>
#include <utility>
#include <iterator>
#include <algorithm>
#include <iostream>

int main()
{
  enum { topLeft, topRight, bottomRight, bottomLeft };

  std::array<std::pair<int, int>, 4> edges({
    std::make_pair(topLeft, topRight),
    std::make_pair(topRight, bottomRight),
    std::make_pair(bottomRight, bottomLeft),
    std::make_pair(bottomLeft, topLeft)
  });

  typedef boost::adjacency_list<boost::setS, boost::vecS,
    boost::undirectedS> graph;
  graph g{edges.begin(), edges.end(), 4};

  boost::array<int, 4> distances{{0}};

  boost::breadth_first_search(g, topLeft,
    boost::visitor(
      boost::make_bfs_visitor(
        boost::record_distances(distances.begin(),
          boost::on_tree_edge{}))));

  std::copy(distances.begin(), distances.end(),
    std::ostream_iterator<int>{std::cout, "\n"});
}*/

/*
int main()
{
  boost::asio::io_context io;

  // Construct a timer without setting an expiry time.
  boost::asio::steady_timer timer(io);

  // Set an expiry time relative to now.
  timer.expires_after(boost::chrono::steady_clock(boost::chrono::seconds(10)));

  // Wait for the timer to expire.
  timer.wait();
  std::cout << "Hello, world!" << std::endl;

  return 0;
}*/
// grep -nr "chrono::seconds" /usr/local/Cellar/boost/HEAD-c6f08f6_1/include
/*
#include <iostream>
#include <boost/asio.hpp>

int main()
{
  boost::asio::io_context io;

  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
  t.wait();

  std::cout << "Hello, world!" << std::endl;

  return 0;
} */

// export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/Cellar/boost/HEAD-c6f08f6  _1/lib/
// The includes work  c++ -Iinclude_test/boost_1_69_0/  src/server.cpp -o example
/*#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " " );
}*/

/* #include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

int main()
{
    boost::asio::io_context io;

    boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
    t.wait();

    std::cout << "Hello, world!" << std::endl;

    return 0;
} */

/*#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/find_not.hpp>

int main()
{
    boost::asio::io_service io;

    boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
    t.wait();

    std::cout << "Hello, world!" << std::endl;

    return 0;
}*/
/*
#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " " );
}
*/
