# NAME: Quentin Truong
# EMAIL: quentintruong@gmail.com
# ID: 404782322

.SILENT:

CC = gcc
FLAGS = -Wall -Wextra
FILES = lab4c_tcp.c Makefile README

default:
	$(CC) -o lab4c_tcp $(FLAGS) -lmraa -lm lab4c_tcp.c
	$(CC) -o lab4c_tls $(FLAGS) -lmraa -lm -lssl -lcrypto lab4c_tls.c

#check: default
#	{ echo "START"; echo "PERIOD=2"; echo "SCALE=C"; sleep 3; echo "STOP"; sleep 1; echo "OFF"; } | ./lab4b --log=LOGFILE --period=1 --scale=F

clean:
	rm -f lab4c_tcp lab4c-404782322.tar.gz 

dist:
	tar -czf lab4c-404782322.tar.gz $(FILES)