#include "libraries.h"

void gen_p_g(int size, mpz_t *p, mpz_t *g) {
  char *buf;
  mpz_t q, t;
  mpz_inits(q, *p, *g, t, 0);
  size = size / 8;
  buf = (char *)malloc(size * sizeof(char));
  srand(time(0));
  do {
    for (size_t i = 0; i < size; i++)
      buf[i] = rand();
    buf[0] &= 0x7f;
    buf[0] |= 0x40;
    mpz_import(q, size, 1, 1, 0, 0, buf);
    mpz_nextprime(q, q);
    mpz_mul_ui(*p, q, 2);
    mpz_add_ui(*p, *p, 1);
  } while (!mpz_probab_prime_p(*p, 25));
  mpz_set_ui(*g, 1);
  do {
    mpz_add_ui(*g, *g, 1);
    mpz_powm(t, *g, q, *p);
  } while (!mpz_cmp_ui(t, 1));
  free(buf);
}

void gen_x_y(mpz_t p, mpz_t *x, mpz_t *y) {
  gmp_randstate_t state;
  gmp_randinit_mt(state);
  gmp_randseed_ui(state, time(NULL));
  mpz_inits(*x, *y, 0);
  mpz_urandomm(*x, state, p);
  mpz_urandomm(*y, state, p);
}

void gen_X_Y(mpz_t p, mpz_t g, mpz_t x, mpz_t y, mpz_t *X, mpz_t *Y) {
  mpz_inits(*X, *Y, 0);
  mpz_powm(*X, g, x, p);
  mpz_powm(*Y, g, y, p);
}

void gen_ka_kb(mpz_t x, mpz_t y, mpz_t p, mpz_t X, mpz_t Y, mpz_t *ka,
               mpz_t *kb) {
  mpz_inits(*ka, *kb, 0);
  mpz_powm(*ka, Y, x, p);
  mpz_powm(*kb, X, y, p);
}
