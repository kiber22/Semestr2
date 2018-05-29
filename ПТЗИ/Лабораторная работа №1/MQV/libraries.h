#include <gmp.h>
#include <math.h>
#include <pthread.h>
#include <rdtsc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void gen_p_q_g_t(int size_p, int size_q, mpz_t *p, mpz_t *q, mpz_t *g,
                 mpz_t *t);
void gen_a_b(mpz_t q, mpz_t *a, mpz_t *b);
void gen_A_B(mpz_t p, mpz_t g, mpz_t a, mpz_t b, mpz_t *A, mpz_t *B);
void gen_x_y(mpz_t q, mpz_t *x, mpz_t *y);
void gen_X_Y(mpz_t p, mpz_t g, mpz_t x, mpz_t y, mpz_t *X, mpz_t *Y);
void gen_d_e(int size_q, mpz_t X, mpz_t Y, mpz_t *d, mpz_t *e);
void gen_sa_sb(mpz_t p ,mpz_t X, mpz_t Y, mpz_t A, mpz_t B, mpz_t d, mpz_t e, mpz_t a,
               mpz_t b, mpz_t x, mpz_t y, mpz_t q, mpz_t *sa,
               mpz_t *sb);
void compar_ka_kb(mpz_t ka, mpz_t kb);
void print_info(mpz_t p, mpz_t g, mpz_t q, mpz_t t, mpz_t x, mpz_t y, mpz_t X,
                mpz_t Y, mpz_t a, mpz_t b, mpz_t A, mpz_t B, mpz_t d, mpz_t e,
                mpz_t sa, mpz_t sb);
