#include <gmp.h>
#include <pthread.h>
#include <rdtsc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void gen_p_g(int size, mpz_t *p, mpz_t *g);
void gen_x_y(mpz_t p, mpz_t *x, mpz_t *y);
void gen_X_Y(mpz_t p, mpz_t g, mpz_t x, mpz_t y, mpz_t *X, mpz_t *Y);
void gen_ka_kb(mpz_t x, mpz_t y, mpz_t p, mpz_t X, mpz_t Y, mpz_t *ka,
               mpz_t *kb);
void compar_ka_kb(mpz_t ka, mpz_t kb);
void print_info(mpz_t p, mpz_t g, mpz_t x, mpz_t y, mpz_t X, mpz_t Y, mpz_t ka,
                mpz_t kb);
