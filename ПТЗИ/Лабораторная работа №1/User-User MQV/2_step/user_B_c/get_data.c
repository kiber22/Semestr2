#include "libraries.h"

void get_data(mpz_t *p, mpz_t *q, mpz_t *g, mpz_t *b, mpz_t *A, mpz_t *B) {
  mpz_inits(*p, *g, *q, *b, *A, *B, 0);
  FILE *user_b;
  user_b = fopen("user_B_p.txt", "r");
  mpz_inp_str(*p, user_b, 16);
  fclose(user_b);
  user_b = fopen("user_B_g.txt", "r");
  mpz_inp_str(*g, user_b, 16); // Генератор
  fclose(user_b);
  user_b = fopen("user_B_q.txt", "r");
  mpz_inp_str(*q, user_b, 16);
  fclose(user_b);
  user_b = fopen("user_B_b.txt", "r");
  mpz_inp_str(*b, user_b, 16); // Секретное число пользователя B
  fclose(user_b);
  user_b = fopen("user_B_A.txt", "r");
  mpz_inp_str(*A, user_b, 16); // Открытое число пользователя A
  fclose(user_b);
  user_b = fopen("user_B_B.txt", "r");
  mpz_inp_str(*B, user_b, 16); // Открытое число пользователя B
  fclose(user_b);
}
