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
 * Right-hand-side should be much smaller than the lhs in most cases, so
 * most tests make sense.
 */
#define N_RHS 100
/**
 * defines the size of char* buf for holding random integer numbers.
 * SIZE = (floor(log (2^N - 1)) + 1) + 1 + 1
 * the two trailing +1's are for potential sign bit and the ending NULL.
 */
#define SIZE 3013
#define SIZE_RHS 33
/**
 * defines the size of the char* holding the result of arithmetic operations.
 * it has to be twice as large due to multiplication.
 */
#define SIZE_TARGET (SIZE)
#define BASE 10

#define NUM_RUNS 100000

/**
 * prints everything related out.
 */
void
print_ops(
  char* lhs,
  char* rhs,
  mpz_t lhs_mpz,
  mpz_t rhs_mpz,
  mpz_t q_mpz,
  mpz_t r_mpz,
  struct so_long* lhs_so_long,
  struct so_long* rhs_so_long,
  struct so_long* q_so_long,
  struct so_long* r_so_long
) {
  printf("lhs_cha: %s\n", lhs);
  printf("rhs_cha: %s\n", rhs);
  printf("\n");

  gmp_printf("lhs_mpz: %Zd\n", lhs_mpz);
  gmp_printf("rhs_mpz: %Zd\n", rhs_mpz);
  gmp_printf("quotient_mpz: %Zd\n", q_mpz);
  gmp_printf("remainder_mpz: %Zd\n", r_mpz);
  printf("\n");

  printf("lhs_sol: ");
  print_so_long(lhs_so_long);
  printf("\n");
  printf("rhs_sol: ");
  print_so_long(rhs_so_long);
  printf("\n");
  printf("quotient_sol: ");
  print_so_long(q_so_long);
  printf("\n");
  printf("remainder_sol: ");
  print_so_long(r_so_long);
  printf("\n");
}

/**
 * @return 0 on success.
 */
int
run_test(
  char* lhs,
  char* rhs,
  mpz_t lhs_mpz,
  mpz_t rhs_mpz,
  mpz_t q_mpz,
  mpz_t r_mpz,
  struct so_long* lhs_so_long,
  struct so_long* rhs_so_long,
  struct so_long* q_so_long,
  struct so_long* r_so_long,
  char* q0,
  char* r0,
  char* q1,
  char* r1
) {
  make_s_so_long(lhs_so_long, lhs);
  make_s_so_long(rhs_so_long, rhs);
  gmp_sscanf(lhs, "%Zd", lhs_mpz);
  gmp_sscanf(rhs, "%Zd", rhs_mpz);
  div3_mod_so_long(q_so_long, lhs_so_long, rhs_so_long, r_so_long);
  mpz_tdiv_qr(q_mpz, r_mpz, lhs_mpz, rhs_mpz);
  snprint_so_long(q0, SIZE_TARGET, q_so_long);
  mpz_get_str(q1, BASE, q_mpz);
  snprint_so_long(r0, SIZE_TARGET, r_so_long);
  mpz_get_str(r1, BASE, r_mpz);

  if (strcmp(q0, q1) != 0 || strcmp(r0, r1) != 0) {
    print_ops(
      lhs,
      rhs,
      lhs_mpz,
      rhs_mpz,
      q_mpz,
      r_mpz,
      lhs_so_long,
      rhs_so_long,
      q_so_long,
      r_so_long
    );
    return 1;
  }
  return 0;
}

int
main(int argc, char** argv) {
  printf("Start testing div3_div_mod_so_long()\n");
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
  char rhs[SIZE_RHS];
  char q0[SIZE_TARGET];
  char q1[SIZE_TARGET];
  char r0[SIZE_TARGET];
  char r1[SIZE_TARGET];
  struct so_long lhs_so_long;
  struct so_long rhs_so_long;
  struct so_long quotient_so_long;
  struct so_long remainder_so_long;
  mpz_t lhs_mpz;
  mpz_t rhs_mpz;
  mpz_t quotient_mpz;
  mpz_t remainder_mpz;
  mpz_init(lhs_mpz);
  mpz_init(rhs_mpz);
  mpz_init(quotient_mpz);
  mpz_init(remainder_mpz);

  /* corner-case tests. */
  int rc;
  printf("Corner-case test starts\n");
  /* lhs / 1 */
  sprintf(lhs, "%s", "1000000000000000000000000000000000000000000000001");
  sprintf(rhs, "%s", "1");
  rc = run_test(
    lhs,
    rhs,
    lhs_mpz,
    rhs_mpz,
    quotient_mpz,
    remainder_mpz,
    &lhs_so_long,
    &rhs_so_long,
    &quotient_so_long,
    &remainder_so_long,
    q0,
    r0,
    q1,
    r1
  );
  if (rc != 0) {
    return 1;
  }

  /* even / 2 */
  sprintf(lhs, "%s", "1000000000000000000000000000000000000000000000000");
  sprintf(rhs, "%s", "2");
  rc = run_test(
    lhs,
    rhs,
    lhs_mpz,
    rhs_mpz,
    quotient_mpz,
    remainder_mpz,
    &lhs_so_long,
    &rhs_so_long,
    &quotient_so_long,
    &remainder_so_long,
    q0,
    r0,
    q1,
    r1
  );
  if (rc != 0) {
    return 1;
  }

  /* x000 / x */
  sprintf(lhs, "%s", "123456789012345678901234567890000000000000000000000000");
  sprintf(rhs, "%s", "12345678901234567890123456789");
  rc = run_test(
    lhs,
    rhs,
    lhs_mpz,
    rhs_mpz,
    quotient_mpz,
    remainder_mpz,
    &lhs_so_long,
    &rhs_so_long,
    &quotient_so_long,
    &remainder_so_long,
    q0,
    r0,
    q1,
    r1
  );
  if (rc != 0) {
    return 1;
  }

  /* lhs / something greater than lhs */
  sprintf(lhs, "%s", "1");
  sprintf(rhs, "%s", "10000000000000000000000000000000");
  rc = run_test(
    lhs,
    rhs,
    lhs_mpz,
    rhs_mpz,
    quotient_mpz,
    remainder_mpz,
    &lhs_so_long,
    &rhs_so_long,
    &quotient_so_long,
    &remainder_so_long,
    q0,
    r0,
    q1,
    r1
  );
  if (rc != 0) {
    return 1;
  }

  /* lhs / something greater than lhs */
  sprintf(lhs, "%s", "1234567891123456789112345678911234567891");
  sprintf(rhs, "%s", "1234567891");
  rc = run_test(
    lhs,
    rhs,
    lhs_mpz,
    rhs_mpz,
    quotient_mpz,
    remainder_mpz,
    &lhs_so_long,
    &rhs_so_long,
    &quotient_so_long,
    &remainder_so_long,
    q0,
    r0,
    q1,
    r1
  );
  if (rc != 0) {
    return 1;
  }

  /* lhs / something greater than lhs */
  sprintf(lhs, "%s", "99999999999999999999999999999999999999999999999999999");
  sprintf(rhs, "%s", "1");
  rc = run_test(
    lhs,
    rhs,
    lhs_mpz,
    rhs_mpz,
    quotient_mpz,
    remainder_mpz,
    &lhs_so_long,
    &rhs_so_long,
    &quotient_so_long,
    &remainder_so_long,
    q0,
    r0,
    q1,
    r1
  );
  if (rc != 0) {
    return 1;
  }

  printf("Corner-case test has ended\n");

  int i;
  for (i = 0; i < NUM_RUNS; i++) {
    mpz_rrandomb(lhs_mpz, state, N);
    while (1) {
      mpz_rrandomb(rhs_mpz, state, N_RHS);
      if (mpz_cmp_si(rhs_mpz, 0) != 0) {
        break;
      }
    }
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

    div3_mod_so_long(
      &quotient_so_long, &lhs_so_long, &rhs_so_long, &remainder_so_long
    );
    mpz_tdiv_qr(quotient_mpz, remainder_mpz, lhs_mpz, rhs_mpz);

    snprint_so_long(q0, SIZE_TARGET, &quotient_so_long);
    mpz_get_str(q1, BASE, quotient_mpz);
    snprint_so_long(r0, SIZE_TARGET, &remainder_so_long);
    mpz_get_str(r1, BASE, remainder_mpz);

    if (strcmp(q0, q1) != 0 || strcmp(r0, r1) != 0) {
      printf("run #%d\n", i);
      print_ops(
        lhs,
        rhs,
        lhs_mpz,
        rhs_mpz,
        quotient_mpz,
        remainder_mpz,
        &lhs_so_long,
        &rhs_so_long,
        &quotient_so_long,
        &remainder_so_long
      );
      break;
    }
  }

  mpz_clear(lhs_mpz);
  mpz_clear(rhs_mpz);
  mpz_clear(quotient_mpz);
  mpz_clear(remainder_mpz);

  gmp_randclear(state);

  printf(
    "End of tests!  There were no errors if there was not extra output.\n"
  );

  return 0;
}
