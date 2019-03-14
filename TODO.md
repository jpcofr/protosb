# TODO

- Client + Server communicating OSPF packets using raw sockets
  - Keep the server listening
  - Server listen to many clients on demand
  - Server listens to many clients on different interfaces
- Send and receive OSPF packages

- Configure many interfaces on container (each of them needs an IP)
- [Mixing C and C++ code](https://isocpp.org/wiki/faq/mixing-c-and-cpp)
- [Create a daemon](http://www.microhowto.info/howto/cause_a_process_to_become_a_daemon_in_c.html)

## Posponed

- Make the build completely machine independent and autonomous
  - Separate executables for a client and a server on cmake
- Learn to install build essentials or development support directly from the dockerfile
- https://theboostcpplibraries.com/boost.graph-vertices-and-edges
- https://theboostcpplibraries.com/
- <https://hub.docker.com/_/gcc>

# Reference

[Multiple executables](https://stackoverflow.com/questions/14306642/adding-multiple-executables-in-cmake)
[Raw sockets](https://www.tenouk.com/Module43a.html)
[Basic sockets](https://www.geeksforgeeks.org/socket-programming-cc/)
[Lifecycle of a docker container](https://medium.com/@nagarwal/lifecycle-of-docker-container-d2da9f85959)
[Network analyzer docker image](https://github.com/jpcofr/netshoot)
[Cisco docs about OSPF](https://www.cisco.com/c/en/us/support/docs/ip/open-shortest-path-first-ospf/7039-1.html)
[More docs about OSPF](https://www.routeralley.com/guides/ospf.pdf)

# Notes

# Other

- <https://github.com/skydive-project/skydive>
