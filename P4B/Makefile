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
	{ echo "START"; echo "PERIOD=2"; echo "SCALE=C"; sleep 3; echo "STOP"; sleep 1; echo "OFF"; } | ./lab4b --log=LOGFILE --period=1 --scale=F

clean:
	rm -f lab4b lab4b-404782322.tar.gz 

dist:
	tar -czf lab4b-404782322.tar.gz $(FILES)