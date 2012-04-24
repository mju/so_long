/**
 * so_long.c
 * Please report bugs to MickeyJu@gmail.com
 * Mickey Ju on November 24, 2011, and 2012.
 */
#include "so_long.h"
#include <stdio.h>
#include <stdlib.h>

struct so_long*
make_s_len_so_long(struct so_long* sl, char* s, int len) {
  int i, j;
  /* the left most digit */
  int pos_first_digit = 0;
  /* the left most non-zero digit */
  int pos_left_most_digit;
  unsigned long long tmp;
  unsigned long long tens;
  sl->num_nodes = 0;

#if SIGNED_SO_LONG == 1
  if (len > 0) {
    sl->sign = PLUS;
    if (s[0] == '-') {
      sl->sign = MINUS;
      pos_first_digit = 1;
    } else if (s[0] == '+') {
      pos_first_digit = 1;
    }
  }
#endif

  /* deals with leading zeros */
  for (i = pos_first_digit; s[i] == '0' && i < len; i++);
  pos_left_most_digit = i;

  /* gets the right-most digit */
  len--;

  for (j = 0, tmp = 0, tens = 1; len >= pos_left_most_digit; len--, j++) {
    if (j == WIDTH_SO_LONG) {
      j = 0;
      tens = 1;
      sl->nodes[sl->num_nodes++] = tmp;
      tmp = 0;
    }
    tmp += (s[len] - '0') * tens;
    tens *= 10;
  }
  if (tmp != 0) {
    sl->nodes[sl->num_nodes++] = tmp;
  }

  if (sl->num_nodes == 0) {
    zero_so_long(sl);
  }

  return sl;
}

struct so_long*
make_s_so_long(struct so_long* sl, char* s) {
  int i;
  for (i = 0; s[i] != '\0'; i++);
  return make_s_len_so_long(sl, s, i);
}

struct so_long*
make_u_so_long(struct so_long* sl, unsigned u) {
#if SIGNED_SO_LONG == 1
  sl->sign = PLUS;
#endif
  sl->nodes[0] = (unsigned long long)u;
  sl->num_nodes = 1;

  return sl;
}

struct so_long*
make_d_so_long(struct so_long* sl, int d) {
  if (d < 0) {
    make_u_so_long(sl, -d);
#if SIGNED_SO_LONG == 1
    sl->sign = MINUS;
#endif
  } else {
    make_u_so_long(sl, d);
  }

  return sl;
}

struct so_long*
make_llu_so_long(struct so_long* sl, unsigned long long llu) {
  if (llu == 0) {
    return zero_so_long(sl);
  }

#if SIGNED_SO_LONG == 1
  sl->sign = PLUS;
#endif
  sl->num_nodes = 0;
  while (llu != 0) {
    sl->nodes[sl->num_nodes++] = llu % DIVIDER_SO_LONG;
    llu /= DIVIDER_SO_LONG;
  }
  return sl;
}

struct so_long*
make_ll_so_long(struct so_long* sl, long long ll) {
  if (ll < 0) {
    make_llu_so_long(sl, -ll);
#if SIGNED_SO_LONG == 1
    sl->sign = MINUS;
#endif
  } else {
    make_llu_so_long(sl, ll);
  }

  return sl;
}

struct so_long*
copy_so_long(struct so_long* target, struct so_long* source) {
#if SIGNED_SO_LONG == 1
  target->sign = source->sign;
#endif
  target->num_nodes = source->num_nodes;
  int i;
  for (i = 0; i < source->num_nodes; i++) {
    target->nodes[i] = source->nodes[i];
  }
  return target;
}

void
swap_so_long(struct so_long** lhs, struct so_long** rhs) {
  struct so_long* tmp = *lhs;
  *lhs = *rhs;
  *rhs = tmp;
}

int
is_zero_so_long(struct so_long* sl) {
#if SIGNED_SO_LONG == 1
  return sl->sign == ZERO;
#else
  return sl->num_nodes == 1 && sl->nodes[0] == 0;
#endif
}

struct so_long*
zero_so_long(struct so_long* sl) {
#if SIGNED_SO_LONG == 1
  sl->sign = ZERO;
#endif

  sl->nodes[0] = 0;
  sl->num_nodes = 1;
  return sl;
}

int
cmp_so_long(struct so_long* lhs, struct so_long* rhs) {
#if SIGNED_SO_LONG == 1
  //if (lhs->sign == PLUS && rhs->sign == MINUS) {
  if (lhs->sign > rhs->sign) {
    return 1;
  }
  //if (lhs->sign == MINUS && rhs->sign == PLUS) {
  if (rhs->sign > lhs->sign) {
    return -1;
  }
  /* otherwise they have the same sign */
#endif

  int r = 0;
  if (lhs->num_nodes > rhs->num_nodes) {
    r = 1;
  } else if (rhs->num_nodes > lhs->num_nodes) {
    r = -1;
  } else {
    /* lhs->num_nodes == rhs->num_nodes */
    int i;
    for (i = lhs->num_nodes - 1; i > 0 && lhs->nodes[i] == rhs->nodes[i]; i--);
    if (lhs->nodes[i] > rhs->nodes[i]) {
      r = 1;
    }
    if (lhs->nodes[i] < rhs->nodes[i]) {
      r =  -1;
    }
  }

#if SIGNED_SO_LONG == 1
  /* in case lhs and rhs are both negative */
  if (lhs->sign == MINUS) {
    r *= -1;
  }
#endif

  return r;
}

int
is_odd(struct so_long* sl) {
  return sl->nodes[0] & 0x1;
}

#if SIGNED_SO_LONG == 1
struct so_long*
neg_so_long(struct so_long* sl) {
  sl->sign = -sl->sign;
  return sl;
}

int
is_positive_so_long(struct so_long* sl) {
  return sl->sign == PLUS;
}

struct so_long*
positive_so_long(struct so_long* sl) {
  sl->sign = PLUS;
  return sl;
}

int
is_negative_so_long(struct so_long* sl) {
  return sl->sign == MINUS;
}

struct so_long*
negative_so_long(struct so_long* sl) {
  sl->sign = MINUS;
  return sl;
}
#endif

struct so_long*
add2_pos_unsigned_so_long(struct so_long* lhs, struct so_long* rhs, int pos) {
  if (is_zero_so_long(rhs) == 1) {
    return lhs;
  }

  int i;

  if (is_zero_so_long(lhs) == 1) {
    for (i = 0; i < pos; i++) {
      lhs->nodes[i] = 0;
    }
    for (i = 0; i < rhs->num_nodes; i++) {
      lhs->nodes[pos + i] = rhs->nodes[i];
    }
    lhs->num_nodes = pos + rhs->num_nodes;
    return lhs;
  }

  /* padding lhs with zeros if needed. */
  if (pos >= lhs->num_nodes) {
    for (i = lhs->num_nodes; i < pos; i++) {
      lhs->nodes[i] = 0;
    }
    for (i = 0; i < rhs->num_nodes; i++) {
      lhs->nodes[pos + i] = rhs->nodes[i];
    }
    lhs->num_nodes = pos + rhs->num_nodes;
    return lhs;
  }

  struct so_long* longer;
  int pos_end;
  if (lhs->num_nodes > pos + rhs->num_nodes) {
    longer = lhs;
    pos_end = pos + rhs->num_nodes;
  } else {
    longer = rhs;
    pos_end = lhs->num_nodes;
  }

  int carry;
  for (i = pos, carry = 0; i < pos_end; i++) {
    lhs->nodes[i] += rhs->nodes[i - pos] + carry;
    if (lhs->nodes[i] > MAX_VALUE_NODE_SO_LONG) {
      lhs->nodes[i] -= DIVIDER_SO_LONG;
      carry = 1;
    } else {
      carry = 0;
    }
  }

  if (longer == lhs) {
    for (; carry == 1 && i < lhs->num_nodes; i++) {
      if (lhs->nodes[i] == MAX_VALUE_NODE_SO_LONG) {
        lhs->nodes[i] = 0;
      } else {
        lhs->nodes[i]++;
        break;
      }
    }
    if (i == lhs->num_nodes) {
      lhs->nodes[i] = 1;
      lhs->num_nodes++;
    }
  } else {/* it goes here when lhs->num_nodes == pos + rhs->num_nodes */
    for (; i - pos < rhs->num_nodes; i++) {
      if (carry == 1) {
        if (rhs->nodes[i - pos] == MAX_VALUE_NODE_SO_LONG) {
          lhs->nodes[i] = 0;
        } else {
          lhs->nodes[i] = rhs->nodes[i - pos] + 1;
          carry = 0;
        }
      } else {
        lhs->nodes[i] = rhs->nodes[i - pos];
      }
    }
    lhs->num_nodes = pos + rhs->num_nodes;
    if (carry == 1) {
      lhs->nodes[lhs->num_nodes] = 1;
      lhs->num_nodes++;
    }
  }

  return lhs;
}

struct so_long*
add2_so_long(struct so_long* lhs, struct so_long* rhs) {
  if (is_zero_so_long(rhs) == 1) {
    return lhs;
  }
  if (is_zero_so_long(lhs) == 1) {
    return copy_so_long(lhs, rhs);
  }

#if SIGNED_SO_LONG == 1
  /* the arguments for sub2_so_long() in this block should all be positive */
  //if (lhs->sign == PLUS && rhs->sign == MINUS) {
  if (lhs->sign > rhs->sign) {
    rhs->sign = PLUS;
    sub2_so_long(lhs, rhs);
    rhs->sign = MINUS;
    return lhs;
  }
  //if (lhs->sign == MINUS && rhs->sign == PLUS) {
  if (lhs->sign < rhs->sign) {
    lhs->sign = PLUS;
    sub2_so_long(lhs, rhs);
    if (is_zero_so_long(lhs) == 1) {
      return lhs;
    }
    /* flips the sign */
    lhs->sign = -lhs->sign;
    return lhs;
  }
#endif

  struct so_long* longer;
  struct so_long* shorter;
  if (lhs->num_nodes >= rhs->num_nodes) {
    longer = lhs;
    shorter = rhs;
  } else {
    longer = rhs;
    shorter = lhs;
  }

  unsigned long long carry;
  int i;
  for (i = 0, carry = 0; i < shorter->num_nodes; i++) {
    carry += longer->nodes[i];
    carry += shorter->nodes[i];
    if (carry < DIVIDER_SO_LONG) {
      lhs->nodes[i] = carry;
      carry = 0;
    } else {
      lhs->nodes[i] = carry - DIVIDER_SO_LONG;
      carry = 1;
    }
  }
  if (carry == 1) {
    for (; i < longer->num_nodes; i++) {
      carry = 1 + longer->nodes[i];/* the one is carried over */
      if (carry < DIVIDER_SO_LONG) {
        lhs->nodes[i] = carry;
        break;
      } else {
        lhs->nodes[i] = carry - DIVIDER_SO_LONG;
      }
    }
    if (i == longer->num_nodes) {
      lhs->nodes[i] = 1;
      lhs->num_nodes = longer->num_nodes + 1;
      return lhs;
    }
    /* if it gets to here, lhs->nodes[i] has been assigned in the for-loop. */
    /* and i should always point to the next node which is not yet processed. */
    i++;
  }
  if (shorter == lhs) {
    for (; i < longer->num_nodes; i++) {
      lhs->nodes[i] = longer->nodes[i];
    }
    lhs->num_nodes = longer->num_nodes;
  }

  return lhs;
}

struct so_long*
add3_so_long(struct so_long* sum, struct so_long* lhs, struct so_long* rhs) {
  return add2_so_long(copy_so_long(sum, lhs), rhs);
}

/**
 * this function is written this way to support negative numbers.  In unsigned
 * context, it should always be true that lhs >= rhs.
 */
struct so_long*
sub2_so_long(struct so_long* lhs, struct so_long* rhs) {
  int cmp = cmp_so_long(lhs, rhs);
  if (cmp == 0) {
    return zero_so_long(lhs);
  }

#if SIGNED_SO_LONG == 1
  /* the arguments for ***2_so_long() in this block should all be positive */
  //if (lhs->sign == PLUS && rhs->sign == MINUS) {
  if (lhs->sign > rhs->sign) {
    rhs->sign = PLUS;
    add2_so_long(lhs, rhs);
    rhs->sign = MINUS;
    return lhs;
  }
  if (lhs->sign == MINUS && rhs->sign == MINUS) {
    lhs->sign = PLUS;
    rhs->sign = PLUS;
    sub2_so_long(lhs, rhs);
    /* it will not be zero here since the comparison at top */
    /* flips the sign */
    lhs->sign = -lhs->sign;
    return lhs;
  }
  //if (lhs->sign == MINUS && rhs->sign == PLUS) {
  if (lhs->sign < rhs->sign) {
    lhs->sign = PLUS;
    add2_so_long(lhs, rhs);
    lhs->sign = MINUS;
    return lhs;
  }
#endif

  /**
   * the function from here on assumes both operands are non-negative
   * and this is the pre-condition for the code from here on
   */
  if (is_zero_so_long(lhs) == 1) {
    copy_so_long(lhs, rhs);
#if SIGNED_SO_LONG == 1
    lhs->sign = MINUS;
#endif
    return lhs;
  }
  if (is_zero_so_long(rhs) == 1) {
    return lhs;
  }

  int i;
  struct so_long* smaller;
  struct so_long* larger;
  int most_significant_digit;
  int borrow;/* used purely as a flag */

  if (cmp > 0) {
    larger = lhs;
    smaller = rhs;
  } else {
    smaller = lhs;
    larger = rhs;
#if SIGNED_SO_LONG == 1
    /* both lhs and rhs should be positive when it gets to here */
    lhs->sign = MINUS;
#endif
  }
  for (i = 0, borrow = 0; i < smaller->num_nodes; i++) {
    if (smaller->nodes[i] > larger->nodes[i] - borrow) {
      lhs->nodes[i] =
        larger->nodes[i] - borrow + DIVIDER_SO_LONG - smaller->nodes[i];
      borrow = 1;
    } else {
      lhs->nodes[i] = larger->nodes[i] - borrow - smaller->nodes[i];
      borrow = 0;
    }
    if (lhs->nodes[i] != 0) {
      most_significant_digit = i;
    }
  }
  if (borrow == 1) {
    for (; i < larger->num_nodes; i++) {
      if (larger->nodes[i] == 0) {
        lhs->nodes[i] = MAX_VALUE_NODE_SO_LONG;
        most_significant_digit = i;
      } else {
        lhs->nodes[i] = larger->nodes[i] - 1;/* -1 is carried over */
        if (lhs->nodes[i] != 0) {
          most_significant_digit = i;
        }
        /* i should always point to the next node which is not yet processed. */
        i++;
        break;
      }
    }
  }
  /* when it gets this point, there will be no more borrowing situations */
  if (i < larger->num_nodes) {
    if (smaller == lhs) {
      /* just copies the rest of digits from the rhs */
      for (; i < larger->num_nodes; i++) {
        lhs->nodes[i] = larger->nodes[i];
      }
    }
    most_significant_digit = larger->num_nodes - 1;
  }
  lhs->num_nodes = most_significant_digit + 1;

  return lhs;
}

struct so_long*
sub3_so_long(struct so_long* diff, struct so_long* lhs, struct so_long* rhs) {
  return sub2_so_long(copy_so_long(diff, lhs), rhs);
}

struct so_long*
mul2_so_long(struct so_long* lhs, struct so_long* rhs) {
  if (is_zero_so_long(lhs) == 1) {
    return lhs;
  }
  if (is_zero_so_long(rhs) == 1) {
    return zero_so_long(lhs);
  }

  struct so_long product;
  return copy_so_long(lhs, mul3_so_long(&product, lhs, rhs));
}

struct so_long*
mul3_so_long(
  struct so_long* product, struct so_long* lhs, struct so_long* rhs
) {
  if (is_zero_so_long(lhs) == 1 || is_zero_so_long(rhs) == 1) {
    return zero_so_long(product);
  }

  zero_so_long(product);

#if SIGNED_SO_LONG == 1
  product->sign = lhs->sign * rhs->sign;
#endif

  struct so_long tmp;
  int i;
  for (i = 0; i < rhs->num_nodes; i++) {
    if (rhs->nodes[i] == 0) {
      continue;
    }
    add2_pos_unsigned_so_long(
      product, mul3_ull_so_long(&tmp, lhs, rhs->nodes[i]), i
    );
  }

  return product;
}

struct so_long*
mul3_ull_so_long(
  struct so_long* product, struct so_long* lhs, unsigned long long rhs
) {
  if (rhs == 0) {
    zero_so_long(product);
    return product;
  }
  if (rhs == 1) {
    return copy_so_long(product, lhs);
  }
  mul3_ull_so_long(product, lhs, rhs >> 1);
  add2_so_long(product, product);
  if ((rhs & 0x1) == 1) {
    add2_so_long(product, lhs);
  }
  return product;
}

struct so_long*
div2_mod_so_long(
  struct so_long* lhs, struct so_long* rhs, struct so_long* remainder
) {
  struct so_long quotient;
  return copy_so_long(lhs, div3_mod_so_long(&quotient, lhs, rhs, remainder));
}

struct so_long*
div3_mod_so_long(
  struct so_long* quotient,
  struct so_long* lhs,
  struct so_long* rhs,
  struct so_long* remainder
) {
  if (is_zero_so_long(rhs) == 1) {
    perror("div3_mod_so_long: divide-by-zero\n");
    return NULL;
  }

  copy_so_long(remainder, lhs);

#if SIGNED_SO_LONG == 1
  /* rhs * quotient + remainder has to be equal to rhs */
  quotient->sign = lhs->sign * rhs->sign;
  remainder->sign = lhs->sign;
  char sign_lhs = lhs->sign;
  char sign_rhs = rhs->sign;
  lhs->sign = PLUS;
  rhs->sign = PLUS;
#endif

  if (cmp_so_long(lhs, rhs) < 0) {
    return zero_so_long(quotient);
  }

  struct so_long tmp_lhs;
  struct so_long tmp;
  struct so_long tmp_remainder;
  int i;
  int is_quotient_set = 0;
  int pos_quotient;
  int no_padding = 0;

#if SIGNED_SO_LONG == 1
  positive_so_long(&tmp_lhs);
  positive_so_long(&tmp_remainder);
#endif

  while (remainder->num_nodes >= rhs->num_nodes) {
    for (i = 0; i < rhs->num_nodes; i++) {
      tmp_lhs.nodes[rhs->num_nodes - 1 - i] =
        remainder->nodes[remainder->num_nodes - 1 - i];
      tmp_lhs.num_nodes = rhs->num_nodes;
    }
    if (is_quotient_set == 0) {
      quotient->num_nodes = lhs->num_nodes - rhs->num_nodes + 1;
    }
    if (cmp_so_long(&tmp_lhs, rhs) < 0) {
      if (remainder->num_nodes == rhs->num_nodes) {
        break;
      }
      for (i = 0; i <= rhs->num_nodes; i++) {
        tmp_lhs.nodes[rhs->num_nodes - i] =
          remainder->nodes[remainder->num_nodes - 1 - i];
        tmp_lhs.num_nodes = rhs->num_nodes + 1;
      }
      if (is_quotient_set == 0) { 
        quotient->num_nodes--;
      } else {
        if (no_padding == 1) {
          no_padding = 0;
        } else {
          quotient->nodes[pos_quotient--] = 0;
        }
      }
    }
    if (is_quotient_set == 0) {
      is_quotient_set = 1;
      pos_quotient = quotient->num_nodes - 1;
    }

    /* do bynary search to figure the quotient of tmp_lhs / rhs */
    /**
     * MAKE tmp_quotient, lower_bound, and upper_bound long long's after
     * making nodes an array of long long's.
     */
    unsigned long long tmp_quotient;
    unsigned long long lower_bound = 1;
    unsigned long long upper_bound = MAX_VALUE_NODE_SO_LONG;
    tmp_quotient = (lower_bound + upper_bound) >> 1;
    while (1) {
      /* MAKE it make_llu_so_long if nodes are long long's */
      make_llu_so_long(&tmp, tmp_quotient);
      mul2_so_long(&tmp, rhs);
      /* gets the remainder. */
      if (cmp_so_long(&tmp_lhs, &tmp) < 0) {
        upper_bound = tmp_quotient - 1;
      } else {
        sub3_so_long(&tmp_remainder, &tmp_lhs, &tmp);
        if (cmp_so_long(rhs, &tmp_remainder) > 0) {
          break;
        }
        lower_bound = tmp_quotient + 1;
      }
      tmp_quotient = (lower_bound + upper_bound) >> 1;
    }
    /* tmp_quotient holds the quotient and tmp_remainder holds the remainder */
    quotient->nodes[pos_quotient--] = tmp_quotient;

    /* adjusts remainder. */
    int _num_nodes;
    if (is_zero_so_long(&tmp_remainder)) {
      _num_nodes = 0;
    } else {
      _num_nodes = tmp_remainder.num_nodes;
    }

    remainder->num_nodes -= tmp_lhs.num_nodes - _num_nodes;
    for (i = 0; i < _num_nodes; i++) {
      remainder->nodes[remainder->num_nodes - 1 - i] =
        tmp_remainder.nodes[_num_nodes - 1 - i];
    }

    /* pads zeros for quotient if necessary. */
    if (rhs->num_nodes == _num_nodes) {
      /**
       * if this is the case, in next round/iteration, one more digit must be
       * used
       */
      no_padding = 1;
    } else if (pos_quotient >= 0) {
      /**
       * use example to understand this.  Pad zero only when
       * rhs->num_nodes - _num_nodes > 1.
       */
      for (i = 0; i < rhs->num_nodes - _num_nodes - 1; i++) {
        quotient->nodes[pos_quotient--] = 0;
      }
    }
  }
  while (pos_quotient >= 0) {
    quotient->nodes[pos_quotient--] = 0;
  }
  if (remainder->num_nodes == 0) {
    zero_so_long(remainder);
  }

#if SIGNED_SO_LONG == 1
  lhs->sign = sign_lhs;
  rhs->sign = sign_rhs;
#endif

  return quotient;
}

void
print_so_long(struct so_long* sl) {
#if SIGNED_SO_LONG == 1
  if (sl->sign == MINUS) {
    printf("-");
  }
#endif

  int i;
  printf("%llu", sl->nodes[sl->num_nodes - 1]);
  for (i = sl->num_nodes - 2; i >= 0; i--) {
    printf(WIDTH_STR_SO_LONG, sl->nodes[i]);
  }
}

int
snprint_so_long(char* buf, int size, struct so_long* sl) {
  int r = 0;

#if SIGNED_SO_LONG == 1
  if (sl->sign == MINUS) {
    r += snprintf(buf, size, "%s", "-");
  }
#endif

  int i;
  r += snprintf(buf + r, size - r, "%llu", sl->nodes[sl->num_nodes - 1]);
  for (i = sl->num_nodes - 2; i >= 0; i--) {
    r += snprintf(buf + r, size - r, WIDTH_STR_SO_LONG, sl->nodes[i]);
  }

  return r;
}

/*
int
main(int argc, char** argv) {
  char str[1000];
  struct so_long lhs, rhs;

  while (1) {
    printf("Please enter lhs\n");
    scanf("%s", str);
    make_s_so_long(&lhs, str);

    printf("Please enter rhs\n");
    scanf("%s", str);
    make_s_so_long(&rhs, str);

    print_so_long(&lhs);
    printf("\n");
    printf("rhs:\n");
    print_so_long(&rhs);
    printf("\n");
    mul2_so_long(&lhs, &rhs);
    printf("product:\n");
    print_so_long(&lhs);
    printf("\n");
  }

  return 0;
}
*/
