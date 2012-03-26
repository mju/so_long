/**
 * so_long.c
 * Please report bugs to MickeyJu@gmail.com
 * Mickey Ju on November 24, 2011, and 2012.
 */
#include "so_long.h"

struct so_long*
make_s_len_so_long(struct so_long* sl, char* s, int len) {
  int i, j;
  /* the left most digit */
  int pos_first_digit = 0;
  /* the left most non-zero digit */
  int pos_left_most_digit;
  int tmp;
  int tens;
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
  sl->num_nodes = 0;
  if (u < DIVIDER_SO_LONG) {
    sl->nodes[0] = (int)u;
    sl->num_nodes = 1;
  } else {
    sl->nodes[1] = (int)(u / DIVIDER_SO_LONG);
    sl->nodes[0] = (int)(u % DIVIDER_SO_LONG);
    sl->num_nodes = 2;
  }
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
    sl->nodes[sl->num_nodes++] = (int)(llu % DIVIDER_SO_LONG);
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
  /*if (lhs->sign == PLUS && rhs->sign == MINUS) {*/
  if (lhs->sign > rhs->sign) {
    return 1;
  }
  /*if (lhs->sign == MINUS && rhs->sign == PLUS) {*/
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
is_odd_so_long(struct so_long* sl) {
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
add2_so_long(struct so_long* lhs, struct so_long* rhs) {
  if (is_zero_so_long(rhs) == 1) {
    return lhs;
  }
  if (is_zero_so_long(lhs) == 1) {
    return copy_so_long(lhs, rhs);
  }

#if SIGNED_SO_LONG == 1
  /* the arguments for sub2_so_long() in this block should all be positive */
  /*if (lhs->sign == PLUS && rhs->sign == MINUS) {*/
  if (lhs->sign > rhs->sign) {
    rhs->sign = PLUS;
    sub2_so_long(lhs, rhs);
    rhs->sign = MINUS;
    return lhs;
  }
  /*if (lhs->sign == MINUS && rhs->sign == PLUS) {*/
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

  int carry;
  int i;
  for (i = 0, carry = 0; i < shorter->num_nodes; i++) {
    lhs->nodes[i] = longer->nodes[i] + shorter->nodes[i] + carry;
    if (lhs->nodes[i] < DIVIDER_SO_LONG) {
      carry = 0;
    } else {
      lhs->nodes[i] -= DIVIDER_SO_LONG;
      carry = 1;
    }
  }
  if (carry == 1) {
    for (; i < longer->num_nodes; i++) {
      if (longer->nodes[i] < MAX_VALUE_NODE_SO_LONG) {
        lhs->nodes[i] = longer->nodes[i] + 1;/* 1 is carried over */
        break;
      } else {
        lhs->nodes[i] = 0;
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
  /*if (lhs->sign == PLUS && rhs->sign == MINUS) {*/
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
  /*if (lhs->sign == MINUS && rhs->sign == PLUS) {*/
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

#if SIGNED_SO_LONG == 1
  product->sign = lhs->sign * rhs->sign;
#endif

  long long carry;/* sizeof(long long) should be 2 * sizeof(int) */
  int i, j;

  /**
   * initializes product.  It could have lhs->num_nodes + rhs->num_nodes - 1 or
   * lhs->num_nodes + rhs->num_nodse nodes
   */
  product->num_nodes = lhs->num_nodes + rhs->num_nodes - 1;/* be conservetive */
  /* assumes it will use lhs->num_nodes + rhs->num_nodes nodes */
  for (i = 0; i <= product->num_nodes; i++) {
    product->nodes[i] = 0;
  }

  for (i = 0; i < rhs->num_nodes; i++) {
    if (rhs->nodes[i] == 0) {
      continue;
    }
    for (j = 0, carry = 0; j < lhs->num_nodes; j++) {
      carry +=
        (long long)rhs->nodes[i] * (long long)lhs->nodes[j] +
        (long long)product->nodes[i + j];
      product->nodes[i + j] = (int)(carry % DIVIDER_SO_LONG);
      carry /= DIVIDER_SO_LONG;
    }
    if (carry != 0) {
      product->nodes[i + j] = (int)carry;
      /* happens when product->num_nodes == lhs->num_nodes + rhs->num_nodes */
      if (i + j == product->num_nodes) {
        product->num_nodes++;
      }
    }
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
  struct so_long tmp_remainder;
  struct so_long tmp;
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
    int tmp_quotient;
    int lower_bound = 1;
    int upper_bound = MAX_VALUE_NODE_SO_LONG;
    tmp_quotient = (lower_bound + upper_bound) >> 1;
    while (1) {
      /* MAKE it make_ll_so_long if nodes are long long's */
      make_d_so_long(&tmp, tmp_quotient);
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

    /**
     * if there are leading zeros in remainder, skip them.  They are there
     * because last round of division produced no remainder, and the following
     * digits are zeros.
     */
    for (
      i = remainder->num_nodes - 1; i >= 0 && remainder->nodes[i] == 0; i--
    ) {
      remainder->num_nodes--;
      quotient->nodes[pos_quotient--] = 0;
    }

    /* pads zeros for quotient if necessary. */
    if (rhs->num_nodes == _num_nodes) {
      /**
       * if this is the case, in next round/iteration, one more digit must be
       * used.
       */
      no_padding = 1;
    } else {
      /**
       * use examples to understand this.  Pad zero only when
       * rhs->num_nodes - _num_nodes > 1.
       */
      for (
        i = 0; i < rhs->num_nodes - _num_nodes - 1 && pos_quotient >= 0; i++
      ) {
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

struct so_long*
div_by_two_so_long(struct so_long* quotient, struct so_long* lhs) {
  if (lhs->num_nodes == 1) {
    quotient->num_nodes = 1;
    quotient->nodes[0] = lhs->nodes[0] >> 1;
    return quotient;
  }

  int tmp_lhs;
  int i;

  if (lhs->nodes[lhs->num_nodes - 1] == 1) {
    i = lhs->num_nodes - 2;
    quotient->num_nodes = lhs->num_nodes - 1;
    tmp_lhs = lhs->nodes[i] + DIVIDER_SO_LONG;
  } else {
    i = lhs->num_nodes - 1;
    quotient->num_nodes = lhs->num_nodes;
    tmp_lhs = lhs->nodes[i];
  }

  quotient->nodes[i--] = tmp_lhs >> 1;
  while (i >= 0) {
    if ((tmp_lhs & 0x1) == 1) {
      tmp_lhs = lhs->nodes[i] + DIVIDER_SO_LONG;
    } else {
      tmp_lhs = lhs->nodes[i];
    }
    quotient->nodes[i--] = tmp_lhs >> 1;
  }

  return quotient;
}

struct so_long*
pow_u_so_long(
  struct so_long* result, struct so_long* base, unsigned exp
) {
  if (is_zero_so_long(base) == 1) {
    return zero_so_long(result);
  }
  if (exp == 0) {
#if SIGNED_SO_LONG == 1
    result->sign = PLUS;
#endif
    result->nodes[0] = 1;
    result->num_nodes = 1;
    return result;
  }

  return _pow_u_so_long(result, base, exp);
}

struct so_long*
_pow_u_so_long(
  struct so_long* result, struct so_long* base, unsigned exp
) {
  if (exp == 1) {
    return copy_so_long(result, base);
  }
  _pow_u_so_long(result, base, exp >> 1);
  mul2_so_long(result, result);
  if ((exp & 0x1) == 1) {
    mul2_so_long(result, base);
  }
  return result;
}

struct so_long*
sqrt_so_long(struct so_long* sqrt, struct so_long* op) {
  if (is_zero_so_long(op)) {
    return zero_so_long(sqrt);
  }
  /* for the case where op is less than 4.  This is to save a division */
  if (op->num_nodes == 1 && op->nodes[0] < 4) {
    return make_u_so_long(sqrt, 1);
  }
  /* for the case where op is less than 5.  This is to save a division */
  if (op->num_nodes == 1 && op->nodes[0] < 6) {
    return make_u_so_long(sqrt, 2);
  }

  struct so_long _lower_bound, _upper_bound;
  struct so_long* lower_bound = &_lower_bound;
  struct so_long* upper_bound = &_upper_bound;
  struct so_long tmp;
  struct so_long _mid;
  struct so_long* mid = &_mid;

  /* do binary search, using sqrt as a tmp. */
  /* takes the floor. */
  int num_nodes_sqrt = (op->num_nodes >> 1) + (op->num_nodes & 0x1);
  int i;
  if (op->num_nodes == 1) {
    /* in this case, use op / 2 as the upper bound. */
    make_u_so_long(lower_bound, 1);
    copy_so_long(upper_bound, op);
    div_by_two_so_long(upper_bound, op);
  } else {
    lower_bound->num_nodes = num_nodes_sqrt;
    upper_bound->num_nodes = num_nodes_sqrt;
#if SIGNED_SO_LONG
    lower_bound->sign = PLUS;
    upper_bound->sign = PLUS;
#endif
    for (i = 0; i < num_nodes_sqrt; i++) {
      lower_bound->nodes[i] = 0;
      upper_bound->nodes[i] = MAX_VALUE_NODE_SO_LONG;
    }
    lower_bound->nodes[num_nodes_sqrt - 1] = 1;
  }

  int cmp;
  add3_so_long(mid, lower_bound, upper_bound);
  div_by_two_so_long(mid, mid);
  while (1) {
    if (
      upper_bound->nodes[0] - lower_bound->nodes[0] == 1 ||
      (lower_bound->nodes[0] - upper_bound->nodes[0] == MAX_VALUE_NODE_SO_LONG)
    ) {
      sub3_so_long(&tmp, upper_bound, lower_bound);
      if (tmp.num_nodes == 1 && tmp.nodes[0] <= 1) {
        break;
      }
    }

    if (
      mid->nodes[mid->num_nodes - 1] * mid->nodes[mid->num_nodes - 1] >=
        DIVIDER_SO_LONG &&
      op->num_nodes < (mid->num_nodes << 1)
    ) {
      cmp = 1;
    } else {
      mul3_so_long(&tmp, mid, mid);
      cmp = cmp_so_long(&tmp, op);
    }

    if (cmp == 0) {
      return copy_so_long(sqrt, mid);
    }
    if (cmp > 0) {
      swap_so_long(&upper_bound, &mid);
    } else {
      swap_so_long(&lower_bound, &mid);
    }

    add3_so_long(mid, lower_bound, upper_bound);
    div_by_two_so_long(mid, mid);
  }

  /* Newton's method */
  /**
   * Newton's method is much faster when the input size goes up.  However,
   * for 10023 on uva.onlinejudge.org, binary search won.  This function can
   * be further optimized.
   * To use this, break out of the while loop for binary search when
   * upper_bound is for sure greater than the square root of op.  Or skip
   * the binary search part totally.
   */
/*
  while (1) {
    div3_mod_so_long(lower_bound, op, upper_bound, mid);
    add2_so_long(lower_bound, upper_bound);
    div_by_two_so_long(lower_bound, lower_bound);
    if (cmp_so_long(lower_bound, upper_bound) >= 0) {
      break;
    }
    swap_so_long(&lower_bound, &upper_bound);
  }
  swap_so_long(&lower_bound, &upper_bound);
*/

  /* lower_bound holds the result. */
  return copy_so_long(sqrt, lower_bound);
}

void
print_so_long(struct so_long* sl) {
#if SIGNED_SO_LONG == 1
  if (sl->sign == MINUS) {
    printf("-");
  }
#endif

  int i;
  printf("%d", sl->nodes[sl->num_nodes - 1]);
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
  r += snprintf(buf + r, size - r, "%d", sl->nodes[sl->num_nodes - 1]);
  for (i = sl->num_nodes - 2; i >= 0; i--) {
    r += snprintf(buf + r, size - r, WIDTH_STR_SO_LONG, sl->nodes[i]);
  }

  return r;
}

/*
int
main(int argc, char** argv) {
  char str[1000];
  struct so_long target, lhs;

  while (1) {
    scanf("%s", str);
    make_s_so_long(&lhs, str);

    printf("lhs:\n");
    print_so_long(&lhs);
    printf("\n");
    sqrt_so_long(&target, &lhs);
    printf("sqrt:\n");
    print_so_long(&target);
    printf("\n");
  }

  return 0;
}
*/
