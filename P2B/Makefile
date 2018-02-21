# NAME: Quentin Truong
# EMAIL: quentintruong@gmail.com
# ID: 404782322

.SILENT:

CC = gcc
FLAGS = -Wall -Wextra

default:
	$(CC) -o lab2_list $(FLAGS) -pthread -g -lprofiler SortedList.c lab2_list.c

tests: default
	chmod u+x generate_data.sh
	./generate_data.sh

profile: default
	rm -f ./raw.gperf
	CPUPROFILE=./raw.gperf ./lab2_list --threads=12 --iterations=1000 --sync=s 
	pprof --text ./lab2_list ./raw.gperf > profile.gperf > profile.out
	pprof --list=insert_length_delete ./lab2_list ./raw.gperf >> profile.out
	rm -f ./raw.gperf ./profile.gperf

graphs: tests
	gnuplot lab2b_list.gp

dist: graphs profile
	tar -czf lab2b-404782322.tar.gz Makefile README lab2_list.c SortedList.c SortedList.h generate_data.sh profile.out lab2b_list.csv lab2b_list.gp *.png

clean:
	rm -f lab2_list lab2b-404782322.tar.gz
