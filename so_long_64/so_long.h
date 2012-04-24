/**
 * so_long.h
 * a big number library created for passing problems on uva.onlinejudge.org
 * implemented with statically allocated int nodes.
 * passed 10007, 10213
 * Please report bugs to MickeyJu@gmail.com
 * Mickey Ju on November 24, 2011, and 2012.
 */
#include <stdio.h>

/**
 * the number of int nodes used.
 * because in intact_mul(), target[i + j] will be accessed, to prevent
 * segmentation faults, please set MAX_NUM_NODES to the number of nodes
 * that the largest possible target can have plus one.  For example, if the max
 * possible target uses four nodes, then if lhs uses also four nodes, and rhs
 * uses two nodes, then i + j may equal 1 + 3 = 4, which will cause segmentation
 * faults.  In that case, the result can fit into four nodes when rhs is small,
 * and lhs is large, or the other way around.
 */
#define NUM_NODES_SO_LONG 3000
/**
 * the maximux of an int is 2,147,483,647, the left-most digit is not usable.
 * only 9 digits of an int are used.
 * if BN_WIDTH is modified, BN_WIDTH_STR AND BN_DIVIDER have to be modified all
 * together.
 */
#define WIDTH_SO_LONG 18/*for unsigned long long*/
/**
 * is used for printing so_long's
 */
#define WIDTH_STR_SO_LONG "%018llu"
/**
 * the greatest value a node can hold.
 */
#define MAX_VALUE_NODE_SO_LONG 999999999999999999ULL
/**
 * is defined by the greatest number a node can hold plus one.
 */
#define DIVIDER_SO_LONG 1000000000000000000ULL/*for unsigned long long*/
/**
 *  The code relies on the definition below.  If they are to be changed, please
 *  read the code and do the necessary changes as well.
 */
#define SIGNED_SO_LONG 1
#if SIGNED_SO_LONG ==1
# define PLUS 1
# define ZERO 0
# define MINUS -1
#endif

struct so_long {
#if SIGNED_SO_LONG == 1
  /**
   * MINUS if negative, PLUS if positive, ZERO if it is zero.
   */
  char sign;
#endif
  int num_nodes;
  /**
   * nodes[0] is the least significant digit, and nodes[num_nodes - 1] is the
   * most significant digit.
   */
  unsigned long long nodes[NUM_NODES_SO_LONG];
};

/**
 * these functions create and initialize so_long
 */
struct so_long* make_s_len_so_long(struct so_long* lhs, char* str, int len);
/**
 * @param str should be 0-terminated.
 */
struct so_long* make_s_len_so_long(struct so_long* sl, char* s, int len);
struct so_long* make_s_so_long(struct so_long* lhs, char* str);
struct so_long* make_u_so_long(struct so_long* sl, unsigned u);
struct so_long* make_d_so_long(struct so_long* sl, int d);
struct so_long* make_llu_so_long(struct so_long* sl, unsigned long long llu);
struct so_long* make_ll_so_long(struct so_long* sl, long long ll);

/**
 * @param target and source can be aliases, but it does not make sense at all
 */
struct so_long* copy_so_long(struct so_long* target, struct so_long* source);
void swap_so_long(struct so_long** lhs, struct so_long** rhs);
/**
 * returns one if sl is zero, return zero otherwise.
 * The sign does not matter here.
 */
int is_zero_so_long(struct so_long* sl);
/**
 * defines what zero is for so_long.  Please call this function to get zero
 * so_long's. Zero is treated to be unsigned, non-negtive in the code.
 */
struct so_long* zero_so_long(struct so_long* sl);
/**
 * @return 1 when lhs > rhs, 0 when lhs == rhs, -1 when lhs < rhs.
 */
int cmp_so_long(struct so_long* lhs, struct so_long* rhs);
/**
 * @return 1 if sl is an odd number, 0 otherwise.
 */
int is_odd(struct so_long* sl);

#if SIGNED_SO_LONG == 1
struct so_long* neg_so_long(struct so_long* sl);
int is_positive_so_long(struct so_long* sl);
struct so_long* positive_so_long(struct so_long* sl);
int is_negative_so_long(struct so_long* sl);
struct so_long* negative_so_long(struct so_long* sl);
#endif

/**
 * @param lhs holds the result.  lhs and rhs can be aliases, but if that is
 *   the case, rhs wil be changed as well.
 */
struct so_long* add2_so_long(struct so_long* lhs, struct so_long* rhs);
/**
 * This function does basically the same thing as does add2_so_long(), but just
 * the process starts adding from the pos'th element of lhs.
 * @lhs and rhs will be treated to be unsigned.
 */
struct so_long* add2_pos_unsigned_so_long(
  struct so_long* lhs, struct so_long* rhs, int pos
);
/**
 * copies lhs to sum and then calls add2_so_long() internally.
 * use add2_so_long() when possible for better performance.
 * @param sum holds the result.  lhs and rhs can be aliases.
 *   sum and lhs can be aliases.  sum and rhs cannot be aliases.
 */
struct so_long* add3_so_long(
  struct so_long* sum, struct so_long* lhs, struct so_long* rhs
);

/**
 * @param lhs holds the result.  lhs and rhs can be aliases.
 */
struct so_long* sub2_so_long(struct so_long* lhs, struct so_long* rhs);
/**
 * copies lhs to diff and then calls sub2_so_long() internally.
 * use sub2_so_long() when possible for better performance.
 * @param diff holds the result. lhs and rhs can be aliases.
 *   diff and lhs can be aliases.  diff and rhs cannot be aliases.
 */
struct so_long* sub3_so_long(
  struct so_long* diff, struct so_long* lhs, struct so_long* rhs
);

/**
 * creates a local so_long, and calls mul3_so_long(), and then copies the
 * result back to lhs.
 * use mul3_so_long() when possible for better performance.
 * @param lhs holds the result.  lhs and rhs can be aliases.
 */
struct so_long* mul2_so_long(struct so_long* lhs, struct so_long* rhs);
/**
 * @param product holds the result.  lhs and rhs can be aliases.  But product
 *  cannot be an alias of either lhs or rhs.
 */
struct so_long* mul3_so_long(
  struct so_long* product, struct so_long* lhs, struct so_long* rhs
);
/**
 * This function does product = lhs * rhs.
 * @param rhs should be no less than zero.
 */
struct so_long* mul3_ull_so_long(
  struct so_long* product, struct so_long* lhs, unsigned long long rhs
);

/**
 * creates a local so_long, and calls div3_mod_so_long(), and then copies the
 * result back to lhs.
 * use div3_mod_so_long() when possible for better performance.
 * @param lhs holds the result.  lhs and rhs can be aliases.  No other aliasing
 *   is okay.
 * @return NULL if rhs is zero, which should not be the case.
 */
struct so_long* div2_mod_so_long(
  struct so_long* lhs, struct so_long* rhs, struct so_long* remainder
);
/**
 * This function basically implements long-division, but just it uses binary
 * search together with mul2_so_long() to find out the quotients for each
 * round.
 * @param quotient holds the result.  Only lhs and rhs can be aliases.
 * @return NULL if rhs is zero, which should not be the case.
 */
struct so_long* div3_mod_so_long(
  struct so_long* quotient,
  struct so_long* lhs,
  struct so_long* rhs,
  struct so_long* remainder
);

void print_so_long(struct so_long* sl);
/**
 * @param NULL-terminated buffer.
 * @param size is the size of the buf.
 * @return the number of characters used in buf, not including the ending NULL.
 */
int snprint_so_long(char* buf, int size, struct so_long* sl);
