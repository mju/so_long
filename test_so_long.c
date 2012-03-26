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
#define SIZE_TARGET (SIZE << 1)
#define BASE 10

#define NUM_RUNS 10000

enum ops {
  FIRST,
  ADD,
  SUB,
  MUL,
  END
};

const int NUM_OPS = END - FIRST - 1;

int
main(int argc, char** argv) {
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

#if SIGNED_SO_LONG == 1
  srand(time(NULL));
#endif

  char lhs[SIZE];
  char rhs[SIZE];
  char target[SIZE_TARGET];
  char tmp[SIZE_TARGET];
  struct so_long lhs_so_long;
  struct so_long rhs_so_long;
  struct so_long target_so_long;
  mpz_t lhs_mpz;
  mpz_t rhs_mpz;
  mpz_t target_mpz;
  mpz_init(lhs_mpz);
  mpz_init(rhs_mpz);
  mpz_init(target_mpz);

  /**
   * offset is used to generate negative numbers.
   */
  //mpz_t offset;
  //mpz_init(offset);
  /* random_mpz - offset makes half of random numbers negative. */
  //mpz_ui_pow_ui(offset, 2, N >> 1);

  struct so_long* (*op_so_long)(
    struct so_long*, struct so_long*, struct so_long*
  );
  /* please refer to gmp.h directly */
  void (*op_mpz)(mpz_ptr, mpz_srcptr, mpz_srcptr);

  char op;
  int i, j;
  for (i = FIRST + 1; i < END; i++) {
    switch (i) {
    case ADD:
      printf("Testing ADD\n");
      op = '+';
      op_so_long = add3_so_long;
      op_mpz = mpz_add;
      break;
    case SUB:
      printf("Testing SUB\n");
      op = '-';
      op_so_long = sub3_so_long;
      op_mpz = mpz_sub;
      break;
    case MUL:
      printf("Testing MUL\n");
      op = '*';
      op_so_long = mul3_so_long;
      op_mpz = mpz_mul;
      break;
    default:
      printf("ERROR reaching default\n");
      return 1;
    }
    for (j = 0; j < NUM_RUNS; j++) {
#if SIGNED_SO_LONG == 1
      mpz_rrandomb(lhs_mpz, state, N);
      mpz_rrandomb(rhs_mpz, state, N);
#else
      mpz_rrandomb(lhs_mpz, state, N);
      mpz_rrandomb(rhs_mpz, state, N);
      while (i == SUB && mpz_cmp(lhs_mpz, rhs_mpz) < 0) {
        mpz_rrandomb(lhs_mpz, state, N);
        mpz_rrandomb(rhs_mpz, state, N);
      }
#endif
/*
      mpz_sub(lhs_mpz, lhs_mpz, offset);
      mpz_sub(rhs_mpz, rhs_mpz, offset);
*/
#if SIGNED_SO_LONG == 1
      if ((rand() & 0x1) == 0) {
        mpz_neg(lhs_mpz, lhs_mpz);
      }
      if ((rand() & 0x1) == 0) {
        mpz_neg(rhs_mpz, rhs_mpz);
      }
#endif
      mpz_get_str(lhs, BASE, lhs_mpz);
      mpz_get_str(rhs, BASE, rhs_mpz);
      make_s_so_long(&lhs_so_long, lhs);
      make_s_so_long(&rhs_so_long, rhs);

      op_so_long(&target_so_long, &lhs_so_long, &rhs_so_long);
      op_mpz(target_mpz, lhs_mpz, rhs_mpz);

      snprint_so_long(target, SIZE_TARGET, &target_so_long);
      mpz_get_str(tmp, BASE, target_mpz);

      if (strcmp(target, tmp) != 0) {
        printf("run #%d\n", j);
        printf("operator: %c\n", op);

        printf("lhs_cha: %s\n", lhs);
        printf("rhs_cha: %s\n", rhs);
        printf("\n");

        gmp_printf("lhs_mpz: %Zd\n", lhs_mpz);
        gmp_printf("rhs_mpz: %Zd\n", rhs_mpz);
        gmp_printf("target_mpz: %Zd\n", target_mpz);
        printf("\n");

        printf("lhs_sol: ");
        print_so_long(&lhs_so_long);
        printf("\n");
        printf("rhs_sol: ");
        print_so_long(&rhs_so_long);
        printf("\n");
        printf("target_sol: ");
        print_so_long(&target_so_long);
        printf("\n");

        mpz_clear(lhs_mpz);
        mpz_clear(rhs_mpz);
        mpz_clear(target_mpz);
        gmp_randclear(state);
        return 1;
      }
    }
  }

  mpz_clear(lhs_mpz);
  mpz_clear(rhs_mpz);
  mpz_clear(target_mpz);

  gmp_randclear(state);

  return 0;
}
