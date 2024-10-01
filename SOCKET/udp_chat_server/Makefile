all:server client
server:main.cc 
	g++ -o $@ $^ -std=c++14
client:UdpClient.cc
	g++ -o $@ $^ -std=c++14
.PHONY:clean
clean:
	rm -rf server client