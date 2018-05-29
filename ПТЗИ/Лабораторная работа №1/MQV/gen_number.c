#include "libraries.h"

void gen_p_q_g_t(int size_p, int size_q, mpz_t *p, mpz_t *q, mpz_t *g,
                 mpz_t *t) {
  char *buf_t_, *buf_q;
  int r = 2;
  mpz_t r_mpz;
  int size_t_ = size_p - size_q;
  mpz_inits(*p, *q, *g, *t, r_mpz, 0);
  size_t_ /= 8;
  size_q /= 8;
  srand(time(0));
  buf_t_ = (char *)malloc(size_t_ * sizeof(char));
  buf_q = (char *)malloc(size_q * sizeof(char));
  do {
    for (int i = 0; i < size_t_; i++)
      buf_t_[i] = rand();
    buf_t_[0] |= 0x80;
    for (int i = 0; i < size_q; i++)
      buf_q[i] = rand();
    buf_q[0] |= 0x80;
    mpz_import(*t, size_t_, 1, 1, 0, 0, buf_t_);
    mpz_import(*q, size_q, 1, 1, 0, 0, buf_q);
    mpz_nextprime(*q, *q);
    mpz_mul(*p, *q, *t);
    mpz_add_ui(*p, *p, 1);
  } while (!mpz_probab_prime_p(*p, 25));
  while (1) {
    mpz_set_ui(r_mpz, r);
    mpz_powm(*g, r_mpz, *t, *p);
    if (mpz_cmp_ui(*g, 1) > 0) {
      break;
    }
  }
  free(buf_t_);
  free(buf_q);
}

void gen_a_b(mpz_t q, mpz_t *a, mpz_t *b) {
  gmp_randstate_t state_2;
  gmp_randinit_mt(state_2);
  gmp_randseed_ui(state_2, time(NULL));
  mpz_inits(*a, *b, 0);
  mpz_urandomm(*a, state_2, q);
  mpz_urandomm(*b, state_2, q);
  sleep(1);
}

void gen_A_B(mpz_t p, mpz_t g, mpz_t a, mpz_t b, mpz_t *A, mpz_t *B) {
  mpz_inits(*A, *B, 0);
  mpz_powm(*A, g, a, p);
  mpz_powm(*B, g, b, p);
}

void gen_x_y(mpz_t q, mpz_t *x, mpz_t *y) {
  gmp_randstate_t state;
  gmp_randinit_mt(state);
  gmp_randseed_ui(state, time(NULL));
  mpz_inits(*x, *y, 0);
  mpz_urandomm(*x, state, q);
  mpz_urandomm(*y, state, q);
}

void gen_X_Y(mpz_t p, mpz_t g, mpz_t x, mpz_t y, mpz_t *X, mpz_t *Y) {
  mpz_inits(*X, *Y, 0);
  mpz_powm(*X, g, x, p);
  mpz_powm(*Y, g, y, p);
}

void gen_d_e(int size_q, mpz_t X, mpz_t Y, mpz_t *d, mpz_t *e) {
  mpz_inits(*d, *e, 0);
  mpz_set(*d, X);
  mpz_set(*e, Y);
  for (size_t i = 126; i < size_q * 4; i++) {
    mpz_clrbit(*d, i);
    mpz_clrbit(*e, i);
  }
  mpz_setbit(*d, (size_q / 2) - 1);
  mpz_setbit(*e, (size_q / 2) - 1);
}

void gen_sa_sb(mpz_t p, mpz_t X, mpz_t Y, mpz_t A, mpz_t B, mpz_t d, mpz_t e,
               mpz_t a, mpz_t b, mpz_t x, mpz_t y, mpz_t q, mpz_t *sa,
               mpz_t *sb) {
  mpz_t power_sa, power_sb;
  mpz_inits(power_sb, power_sa, *sa, *sb, 0);
  mpz_set(power_sa, x);
  mpz_addmul(power_sa, d, a);
  mpz_mod(power_sa, power_sa, q);
  mpz_powm(*sa, B, e, p);
  mpz_mul(*sa, Y, *sa);
  mpz_mod(*sa, *sa, p);
  mpz_powm(*sa, *sa, power_sa, p);
  mpz_set(power_sb, y);
  mpz_addmul(power_sb, e, b);
  mpz_mod(power_sb, power_sb, q);
  mpz_powm(*sb, A, d, p);
  mpz_mul(*sb, X, *sb);
  mpz_mod(*sb, *sb, p);
  mpz_powm(*sb, *sb, power_sb, p);
}
