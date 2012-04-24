CC = gcc
CFLAGS = -g -Wall
GMPFLAG = -lgmp

all: test_div_mod test_so_long so_long

test_div_mod: so_long test_div_mod.c
	$(CC) $(CFLAGS) $(GMPFLAG) -o test_div_mod so_long.o test_div_mod.c

test_so_long: so_long test_so_long.c
	$(CC) $(CFLAGS) $(GMPFLAG) -o test_so_long so_long.o test_so_long.c

so_long: so_long.c so_long.h
	$(CC) $(CFLAGS) -c -o so_long.o so_long.c

clean:
	rm so_long.o test_so_long test_div_mod
