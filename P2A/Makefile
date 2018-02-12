# NAME: Quentin Truong
# EMAIL: quentintruong@gmail.com
# ID: 404782322

.SILENT:

CC = gcc
FLAGS = -Wall -Wextra

build:
	$(CC) $(FLAGS) -pthread lab2_add.c -o lab2_add
	$(CC) $(FLAGS) -pthread lab2_list.c SortedList.c -o lab2_list

tests: build
	chmod u+x generate_data.sh
	./generate_data.sh

graphs: tests
	gnuplot lab2_add.gp
	gnuplot lab2_list.gp

dist: graphs
	tar -czf lab2a-404782322.tar.gz Makefile README lab2_add.c lab2_list.c SortedList.c SortedList.h generate_data.sh *.png *.csv *.gp

clean:
	rm -f lab2_add lab2_list lab2a-404782322.tar.gz