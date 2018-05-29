#include "libraries.h"

/* Подготовительный этап для протокола MQV. Генерация q, p, секретных
 * долговременных и открытых ключей */

int main() {
  // Генерация
  mpz_t p, g, q, t, a, b, A, B;
  gen_p_q_g_t(1024, 256, &p, &q, &g, &t);
  gen_a_b(q, &a, &b);
  gen_A_B(p, g, a, b, &A, &B);
  FILE *user_a, *user_b;
  // Запись заранее подготовленных значений для пользователя A
  user_a = fopen("user_A_p.txt", "w+");
  mpz_out_str(user_a, 16, p);
  fclose(user_a);
  user_a = fopen("user_A_g.txt", "w+");
  mpz_out_str(user_a, 16, g); // Генератор
  fclose(user_a);
  user_a = fopen("user_A_q.txt", "w+");
  mpz_out_str(user_a, 16, q);
  fclose(user_a);
  user_a = fopen("user_A_a.txt", "w+");
  mpz_out_str(user_a, 16, a); // Секретное число пользователя A
  fclose(user_a);
  user_a = fopen("user_A_A.txt", "w+");
  mpz_out_str(user_a, 16, A); // Открытое число пользователя A
  fclose(user_a);
  user_a = fopen("user_A_B.txt", "w+");
  mpz_out_str(user_a, 16, B); // Открытое число пользователя B
  fclose(user_a);
  // Запись заранее подготовленных значений для пользователя B
  user_b = fopen("user_B_p.txt", "w+");
  mpz_out_str(user_b, 16, p);
  fclose(user_b);
  user_b = fopen("user_B_g.txt", "w+");
  mpz_out_str(user_b, 16, g); // Генератор
  fclose(user_b);
  user_b = fopen("user_B_q.txt", "w+");
  mpz_out_str(user_b, 16, q);
  fclose(user_b);
  user_b = fopen("user_B_b.txt", "w+");
  mpz_out_str(user_b, 16, b); // Секретное число пользователя B
  fclose(user_b);
  user_b = fopen("user_B_B.txt", "w+");
  mpz_out_str(user_b, 16, B); // Открытое число пользователя B
  fclose(user_b);
  user_b = fopen("user_B_A.txt", "w+");
  mpz_out_str(user_b, 16, A); // Открытое число пользователя A
  fclose(user_b);
  printf("Подготовительный этап завершен\n");
  return 0;
}
