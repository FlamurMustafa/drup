CC=gcc
CFLAGS=-I

all: server client

server: server.c
	$(CC) -o server server.c -lpthread

client: client.c
	$(CC) -o client client.c

clean: 
	rm -rf *.o game



