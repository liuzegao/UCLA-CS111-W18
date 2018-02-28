# NAME: Quentin Truong
# EMAIL: quentintruong@gmail.com
# ID: 404782322

.SILENT:

CC = gcc
FLAGS = -Wall -Wextra
FILES = lab4b.c Makefile README

default:
	$(CC) -o lab4b $(FLAGS) -lmraa -lm lab4b.c

check: default
	./lab4b --bad > /dev/null; 
	if [ $? = 1 ]; then echo "Success bad_arg"; fi


clean:
	rm -f lab4b lab4b-404782322.tar.gz 

dist:
	tar -czf lab4b-404782322.tar.gz $(FILES)