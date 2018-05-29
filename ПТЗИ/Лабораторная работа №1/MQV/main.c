#include "libraries.h"
#include <inttypes.h>

uint64_t get_time() {
  uint32_t low, high;
  __asm__ __volatile__("rdtsc\n" : "=a"(low), "=d"(high));
  return ((uint64_t)high << 32) | low;
}

int main() {
  mpz_t p, g, q, t, a, b, A, B, x, y, X, Y, d, e, sa, sb;
  gen_p_q_g_t(1024, 256, &p, &q, &g, &t);
  gen_a_b(q, &a, &b);
  gen_A_B(p, g, a, b, &A, &B);
  uint64_t start = get_time();
  gen_x_y(q, &x, &y);
  gen_X_Y(p, g, x, y, &X, &Y);
  gen_d_e(256, X, Y, &d, &e);
  gen_sa_sb(p, X, Y, A, B, d, e, a, b, x, y, q, &sa, &sb);
  uint64_t end = get_time();
  print_info(p, g, q, t, x, y, X, Y, a, b, A, B, d, e, sa, sb);
  printf("-----------------------------------------------------------------\n");
  printf("Время выболнения (в тактах) %" PRId64 "\n", (end - start));
  printf("-----------------------------------------------------------------\n");
  return 0;
}
