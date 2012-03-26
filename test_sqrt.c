/**
 * for using open.
 */
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>
#include "so_long.h"

/**
 * N is used to generate a integer number between 0 and 2^N - 1
 * N cannot be zero since SIZE is undefined with log(0).
 */
#define N 10000
/**
 * defines the size of char* buf for holding random integer numbers.
 * SIZE = (floor(log (2^N - 1)) + 1) + 1 + 1
 * the two trailing +1's are for potential sign bit and the ending NULL.
 */
#define SIZE 3013
/**
 * defines the size of the char* holding the result of arithmetic operations.
 * it has to be twice as large due to multiplication.
 */
#define SIZE_TARGET ((SIZE >> 1) + 1)
#define BASE 10

#define NUM_RUNS 100

int
main(int argc, char** argv) {
  printf("Start testing sqrt_so_long()\n");
  /**
   * initializes the random number generator.
   */
  unsigned long int seed;
  int fd = open("/dev/random", O_RDONLY);
  read(fd, &seed, sizeof(seed));
  close(fd);
  gmp_randstate_t state;
  gmp_randinit_default(state);
  gmp_randseed_ui(state, seed);

  char op[SIZE];
  char sqrt0[SIZE_TARGET];
  char sqrt1[SIZE_TARGET];
  struct so_long op_so_long;
  struct so_long _sqrt_so_long;
  mpz_t op_mpz;
  mpz_t sqrt_mpz;
  mpz_init(op_mpz);
  mpz_init(sqrt_mpz);

  int i;
  for (i = 0; i < NUM_RUNS; i++) {
printf("%d\n", i);
    mpz_rrandomb(op_mpz, state, N);

    mpz_get_str(op, BASE, op_mpz);
    make_s_so_long(&op_so_long, op);

    sqrt_so_long(&_sqrt_so_long, &op_so_long);
    mpz_sqrt(sqrt_mpz, op_mpz);

    snprint_so_long(sqrt0, SIZE_TARGET, &_sqrt_so_long);
    mpz_get_str(sqrt1, BASE, sqrt_mpz);

    if (strcmp(sqrt0, sqrt1) != 0) {
      printf("run #%d\n", i);
      printf("op_char: %s\n", op);
      printf("\n");

      gmp_printf("op_mpz: %Zd\n", op_mpz);
      gmp_printf("sqrt_mpz: %Zd\n", sqrt_mpz);
      printf("\n");

      printf("op_so_long: ");
      print_so_long(&op_so_long);
      printf("\n");
      printf("sqrt_sol: ");
      print_so_long(&_sqrt_so_long);
      printf("\n");

      break;
    }
  }

  mpz_clear(op_mpz);
  mpz_clear(sqrt_mpz);

  gmp_randclear(state);

  printf(
    "End of tests!  There were no errors if there was not extra output.\n"
  );

  return 0;
}
