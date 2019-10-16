## Compile

rm server && g++ -g -x c src/server.c -o server
rm client && g++ -g -x c src/client.c -o client

**This command makes the machine available on the internal network**

`docker run -it --security-opt seccomp=unconfined -v <path_to_directory>:/home/ alpine_devel:latest`

`docker run -it --net container:<CONTAINER NAME>  nicolaka/netshoot`

`docker run -it --security-opt seccomp=unconfined --mount`

## TCPDUMP

 tcpdump -i eth0 port 8080 -c 1 -Xvv

<https://hackertarget.com/tcpdump-examples/>

 tcpdump -i eth0 port 8080 -Xvv -A

## alpine

apk add gdb

## CMake
Clean cmake rubbish
`find . -iwholename '*cmake*' -not -name CMakeLists.txt -delete`

`alias clean='find . -iwholename '*cmake*' -not -name CMakeLists.txt -delete && cmake . && rm *.out'`
