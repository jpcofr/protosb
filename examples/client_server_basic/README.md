# Compilation commands

`c++ -g -x c server.c -o server`

# Docker tips

This command runs an unsecured linux attached to the local terminal that maps the container /home directory into the developmen directory.

`docker run -it --security-opt seccomp=unconfined -v /Users/jpcf/Dropbox/protosb:/home/ alpine_devel:latest`

# Analyzing network traffic

* Runs a container that has network tools:

`docker run -it --net container:<CONTAINER NAME>  nicolaka/netshoot`

* Captures the packets on an interface and port and parses them to ascii

`tcpdump -i eth0 port 8080 -c 1 -Xvv -A`


