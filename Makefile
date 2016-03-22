all: server

server: src/server.c
	gcc src/server.c -o src/server

run: server
	src/server

clean:
	rm src/server
