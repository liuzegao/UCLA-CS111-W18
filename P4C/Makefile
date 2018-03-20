# NAME: Quentin Truong
# EMAIL: quentintruong@gmail.com
# ID: 404782322

.SILENT:

CC = gcc
FLAGS = -Wall -Wextra
FILES = lab4c_tcp.c lab4c_tls.c Makefile README

default:
	$(CC) -o lab4c_tcp $(FLAGS) -lmraa -lm lab4c_tcp.c
	$(CC) -o lab4c_tls $(FLAGS) -lmraa -lm -lssl -lcrypto lab4c_tls.c

clean:
	rm -f lab4c_tcp lab4c_tls lab4c-404782322.tar.gz 

dist:
	tar -czf lab4c-404782322.tar.gz $(FILES)