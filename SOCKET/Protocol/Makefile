all: server client
server:Server.cc
	g++ -o $@ $^ -ljsoncpp

client:Client.cc
	g++ -o $@ $^ -ljsoncpp

.PHONY:clean
clean:
	rm -rf server client