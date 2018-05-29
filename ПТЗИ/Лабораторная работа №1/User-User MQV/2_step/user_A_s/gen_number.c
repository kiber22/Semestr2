#include "libraries.h"

void gen_X(mpz_t p, mpz_t g, mpz_t q, mpz_t *x , mpz_t *X) {
  gmp_randstate_t state;
  gmp_randinit_mt(state);
  gmp_randseed_ui(state, time(NULL));
  mpz_init(*x);
  mpz_urandomm(*x, state, q);
  mpz_init(*X);
  mpz_powm(*X, g, *x, p);
}

void gen_sa(mpz_t p, mpz_t X, mpz_t Y, mpz_t B, mpz_t a, mpz_t x, mpz_t q,
            mpz_t *sa) {
  mpz_t e, d;
  mpz_inits(d, e, 0);
  mpz_set(d, X);
  mpz_set(e, Y);
  mpz_fdiv_q_2exp(d, X, 128);
  mpz_fdiv_q_2exp(e, Y, 128);
  mpz_setbit(d, 127);
  mpz_setbit(e, 127);
  mpz_t power_sa;
  mpz_inits(power_sa, *sa, 0);
  mpz_set(power_sa, x);
  mpz_addmul(power_sa, d, a);
  mpz_mod(power_sa, power_sa, q);
  mpz_powm(*sa, B, e, p);
  mpz_mul(*sa, Y, *sa);
  mpz_mod(*sa, *sa, p);
  mpz_powm(*sa, *sa, power_sa, p);
}
