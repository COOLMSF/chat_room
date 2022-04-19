CC=gcc
CFLAG=-g -o
OBJS=libclient.o libserver.o
LIB=-lpthread
OUT=client server

all: server client

libclient.o: libclient.c
	$(CC) -c -o $@ $? $(LIB)

libserver.o: libserver.c
	$(CC) -c -o $@ $? $(LIB)

# libclient.so: libclient.o
# 	$(CC) -c -fPIC -o $@ $?
# 
# libserver.so: libserver.o
# 	$(CC) -c -fPIC -o $@ $?

chatroom_client.o:chatroom_client.c
	$(CC) -g -c -o $@ $?

chatroom_server.o:chatroom_server.c
	$(CC) -g -c -o $@ $?

client:chatroom_client.o libclient.o
	$(CC) -g -o $@ $? -lpthread -L.

server:chatroom_server.o libserver.o
	$(CC) -g -o $@ $? -lpthread -L.

clean:
	@rm -f *.o *.so
	@rm -f server client
