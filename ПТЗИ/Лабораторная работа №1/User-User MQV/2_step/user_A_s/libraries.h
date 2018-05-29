#include <arpa/inet.h>
#include <errno.h>
#include <gmp.h>
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

void get_data(mpz_t *p, mpz_t *g, mpz_t *q, mpz_t *a, mpz_t *A, mpz_t *B);
void gen_X(mpz_t p, mpz_t g, mpz_t q, mpz_t *x, mpz_t *X);
void gen_sa(mpz_t p, mpz_t X, mpz_t Y, mpz_t B, mpz_t a, mpz_t x, mpz_t q,
            mpz_t *sa);
