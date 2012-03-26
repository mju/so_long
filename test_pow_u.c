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
#define N 100
/**
 * defines the size of char* buf for holding random integer numbers.
 * SIZE = (floor(log (2^N - 1)) + 1) + 1 + 1
 * the two trailing +1's are for potential sign bit and the ending NULL.
 */
#define SIZE 33
/**
 * defines the size of the char* holding the result of arithmetic operations.
 * it has to be twice as large due to multiplication.
 */
#define SIZE_TARGET (SIZE * N)
#define BASE 10

#define NUM_RUNS 10000

int
main(int argc, char** argv) {
  printf("Start testing pow_u_so_long()\n");
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

  srand(time(NULL));

  char base[SIZE];
  char result0[SIZE_TARGET];
  char result1[SIZE_TARGET];
  struct so_long base_so_long;
  struct so_long result_so_long;
  mpz_t base_mpz;
  mpz_t result_mpz;
  mpz_init(base_mpz);
  mpz_init(result_mpz);
  unsigned pow;

  int i;
  for (i = 0; i < NUM_RUNS; i++) {
    mpz_rrandomb(base_mpz, state, N);
#if SIGNED_SO_LONG == 1
    if ((rand() & 0x1) == 0) {
      mpz_neg(base_mpz, base_mpz);
    }
#endif
    pow = rand() % 100;

    mpz_get_str(base, BASE, base_mpz);
    make_s_so_long(&base_so_long, base);

    pow_u_so_long(&result_so_long, &base_so_long, pow);
    mpz_pow_ui(result_mpz, base_mpz, pow);

    snprint_so_long(result0, SIZE_TARGET, &result_so_long);
    mpz_get_str(result1, BASE, result_mpz);

    if (strcmp(result0, result1) != 0) {
      printf("run #%d\n", i);
      printf("base_char: %s\n", base);
      printf("\n");

      gmp_printf("base_mpz: %Zd\n", base_mpz);
      gmp_printf("result_mpz: %Zd\n", result_mpz);
      printf("\n");

      printf("base_so_long: ");
      print_so_long(&base_so_long);
      printf("\n");
      printf("result_sol: ");
      print_so_long(&result_so_long);
      printf("\n");

      break;
    }
  }

  mpz_clear(base_mpz);
  mpz_clear(result_mpz);

  gmp_randclear(state);

  printf(
    "End of tests!  There were no errors if there was not extra output.\n"
  );

  return 0;
}
