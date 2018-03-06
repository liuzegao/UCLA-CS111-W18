# NAME: Quentin Truong
# EMAIL: quentintruong@gmail.com
# ID: 404782322

.SILENT:

CC = gcc
FLAGS = -Wall -Wextra

default:
	$(CC) -o lab3a $(FLAGS) lab3a.c

dist:
	tar -czf lab3a-404782322.tar.gz Makefile README lab3a.c ext2_fs.h

clean:
	rm -f lab3a lab3a-404782322.tar.gz
