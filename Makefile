CC = gcc
CFLAGS = -g -Wall
GMPFLAG = -lgmp

# put the requried library after the source so the linker can link correctly.
# for example gmp_is_used.c -lgmp.
# -lgmp gmp_is_used.c sometimes doesn't work in some environments.

all: test_sqrt test_pow_u test_div_mod test_so_long so_long

test_sqrt: so_long test_sqrt.c
	$(CC) $(CFLAGS) test_sqrt.c $(GMPFLAG) -o test_sqrt so_long.o

test_pow_u: so_long test_pow_u.c
	$(CC) $(CFLAGS) test_pow_u.c $(GMPFLAG) -o test_pow_u so_long.o

test_div_mod: so_long test_div_mod.c
	$(CC) $(CFLAGS) test_div_mod.c $(GMPFLAG) -o test_div_mod so_long.o

test_so_long: so_long test_so_long.c
	$(CC) $(CFLAGS) test_so_long.c $(GMPFLAG) -o test_so_long so_long.o

so_long: so_long.c so_long.h
	$(CC) $(CFLAGS) -c -o so_long.o so_long.c

clean:
	rm so_long.o test_so_long test_div_mod test_pow_u test_sqrt
