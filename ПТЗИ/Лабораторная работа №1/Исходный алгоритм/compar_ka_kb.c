#include "libraries.h"

void compar_ka_kb(mpz_t ka, mpz_t kb) {
  if (!mpz_cmp(ka, kb))
    printf("Ключи ka и kb равны\n");
  else
    printf("Ключи ka и kb неравны\n");
}
