CC = gcc
CFLAGS = -Werror -Wextra -Wall -std=c11

all: s21_grep test

s21_grep: s21_grep.o
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

test:
	bash test.sh

rebuild: clean all

clean:
	rm -rf *.o
	rm -rf s21_grep