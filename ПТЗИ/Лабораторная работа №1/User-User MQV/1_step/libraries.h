#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <gmp.h>

void gen_A_B(mpz_t p, mpz_t g, mpz_t a, mpz_t b, mpz_t *A, mpz_t *B);
void gen_a_b(mpz_t q, mpz_t *a, mpz_t *b);
void gen_p_q_g_t(int size_p, int size_q, mpz_t *p, mpz_t *q, mpz_t *g,
                 mpz_t *t);
