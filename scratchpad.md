## Compile

rm server && g++ -g -x c src/server.c -o server
rm client && g++ -g -x c src/client.c -o client

**This command makes the machine available on the internal network**

docker run -it --security-opt seccomp=unconfined -v /Users/jpcf/Dropbox/protosb:/home/ alpine_devel:latest

docker run -it --net container:<CONTAINER NAME>  nicolaka/netshoot

## TCPDUMP
 tcpdump -i eth0 port 9999 -c 1 -Xvv

<https://hackertarget.com/tcpdump-examples/>

## alpine

apk add gdb
