#include "libraries.h"

void gen_Y(mpz_t p, mpz_t g, mpz_t q, mpz_t *y, mpz_t *Y) {
  gmp_randstate_t state;
  gmp_randinit_mt(state);
  gmp_randseed_ui(state, time(NULL));
  mpz_init(*y);
  mpz_urandomm(*y, state, q);
  mpz_init(*Y);
  mpz_powm(*Y, g, *y, p);
}

void gen_sb(mpz_t p, mpz_t X, mpz_t Y, mpz_t A, mpz_t b, mpz_t y, mpz_t q,
            mpz_t *sb) {
  mpz_t e, d;
  mpz_inits(d, e, 0);
  mpz_set(d, X);
  mpz_set(e, Y);
  mpz_fdiv_q_2exp(d, X, 128);
  mpz_fdiv_q_2exp(e, Y, 128);
  mpz_setbit(d, 127);
  mpz_setbit(e, 127);
  mpz_t power_sb;
  mpz_inits(power_sb, *sb, 0);
  mpz_set(power_sb, y);
  mpz_addmul(power_sb, e, b);
  mpz_mod(power_sb, power_sb, q);
  mpz_powm(*sb, A, d, p);
  mpz_mul(*sb, X, *sb);
  mpz_mod(*sb, *sb, p);
  mpz_powm(*sb, *sb, power_sb, p);
}
