all: server  client

server:Main.cc
	g++ -o $@ $^ -lpthread 

client:TcpClient.cc
	g++ -o $@ $^ -lpthread 

.PHNOY:clean
clean:
	rm -rf server client