# NAME: Quentin Truong
# EMAIL: quentintruong@gmail.com
# ID: 404782322

.SILENT:

CC = gcc
FLAGS = -Wall -Wextra -lz

default: 
	$(CC) $(FLAGS) lab1b-client.c -o lab1b-client
	$(CC) $(FLAGS) lab1b-server.c -o lab1b-server

lab1b-client:
	$(CC) $(FLAGS) lab1b-client.c -o lab1b-client

lab1b-server:
	$(CC) $(FLAGS) lab1b-server.c -o lab1b-server

clean:
	rm -f lab1b-client lab1b-server lab1b-404782322.tar.gz

dist:
	tar -czf lab1b-404782322.tar.gz lab1b-client.c lab1b-server.c Makefile README
