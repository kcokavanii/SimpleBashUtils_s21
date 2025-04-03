CC = gcc
CFLAGS = -Werror -Wextra -Wall -std=c11

all: s21_cat test

s21_cat: s21_cat.o
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

test:
	bash test.sh

rebuild: clean all

clean:
	rm -rf *.o
	rm -rf s21_cat
