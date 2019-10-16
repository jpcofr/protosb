# TODO

- Implement the protocol over TCP (It should be possible to list the routes from this router to other routers)
  - Implement flooding (maintain database synchronization)
    - serialize a map and transmit it from a client from a server
  - Routes should age and refreshed

- Understand the protocol
  - Technical reference
  - Read the RFC <https://www.ietf.org/rfc/rfc2328.txt>

  - Raw socket development
    - Client + Server communicating OSPF packets using raw sockets
    - Listen to artificial TCP packets
    - [Listening on raw sockets](https://stackoverflow.com/questions/13082023/raw-socket-listener)
    - [IP SPOOFING with BSD RAW SOCKETS INTERFACE](http://www.enderunix.org/docs/en/rawipspoof/)
    - [Raw socket tutorial](http://www.cs.binghamton.edu/~steflik/cs455/rawip.txt)
    - Server listens to many clients on different interfaces
    - Send and receive OSPF packages

- Configure many interfaces on container (each of them needs an IP)
- Create a daemon

## Posponed

- [Mixing C and C++ code](https://isocpp.org/wiki/faq/mixing-c-and-cpp)
- Make the build completely machine independent and autonomous
  - Separate executables for a client and a server on cmake
- Learn to install build essentials or development support directly from the dockerfile
- https://theboostcpplibraries.com/boost.graph-vertices-and-edges
- https://theboostcpplibraries.com/
- <https://hub.docker.com/_/gcc>

# Reference

<https://www.tcpdump.org/manpages/tcpdump.1.html>
<https://hackertarget.com/tcpdump-examples/>
<https://dzone.com/articles/parallel-tcpip-socket-server-with-multi-threading>
<https://codereview.stackexchange.com/questions/143286/multithreaded-client-server-communication>
<https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/>

[Multiple executables](https://stackoverflow.com/questions/14306642/adding-multiple-executables-in-cmake)
[Raw sockets](https://www.tenouk.com/Module43a.html)
[Basic sockets](https://www.geeksforgeeks.org/socket-programming-cc/)
[Lifecycle of a docker container](https://medium.com/@nagarwal/lifecycle-of-docker-container-d2da9f85959)
[Network analyzer docker image](https://github.com/jpcofr/netshoot)
[More docs about OSPF](https://www.routeralley.com/guides/ospf.pdf)

# Notes

# Other

- [skydive: An open source real-time network topology and protocols analyzer](https://github.com/skydive-project/skydive)

Versions of boost: boost 1.65 Ubuntu, boost 1.68 Macos 
