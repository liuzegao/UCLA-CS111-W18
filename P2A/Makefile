# NAME: Quentin Truong
# EMAIL: quentintruong@gmail.com
# ID: 404782322

.SILENT:

CC = gcc
FLAGS = -Wall -Wextra

default:
	$(CC) $(FLAGS) -pthread lab2_add.c -o lab2_add
	$(CC) $(FLAGS) -pthread lab2_list.c SortedList.c -o lab2_list

tests:
	chmod u+x generate_data.sh
	./generate_data.sh