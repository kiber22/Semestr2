#include "libraries.h"

void get_data(mpz_t *p, mpz_t *g, mpz_t *q, mpz_t *a, mpz_t *A, mpz_t *B) {
  mpz_inits(*p, *g, *q, *a, *A, *B, 0);
  FILE *user_a;
  user_a = fopen("user_A_p.txt", "r");
  mpz_inp_str(*p, user_a, 16);
  fclose(user_a);
  user_a = fopen("user_A_g.txt", "r");
  mpz_inp_str(*g, user_a, 16); // Генератор
  fclose(user_a);
  user_a = fopen("user_A_q.txt", "r");
  mpz_inp_str(*q, user_a, 16);
  fclose(user_a);
  user_a = fopen("user_A_a.txt", "r");
  mpz_inp_str(*a, user_a, 16); // Секретное число пользователя A
  fclose(user_a);
  user_a = fopen("user_A_A.txt", "r");
  mpz_inp_str(*A, user_a, 16); // Открытое число пользователя A
  fclose(user_a);
  user_a = fopen("user_A_B.txt", "r");
  mpz_inp_str(*B, user_a, 16); // Открытое число пользователя B
  fclose(user_a);
}
