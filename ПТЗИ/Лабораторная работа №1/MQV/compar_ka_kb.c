#include "libraries.h"

void compar_ka_kb(mpz_t ka, mpz_t kb) {
  if (!mpz_cmp(ka, kb))
    printf("Ключи sa и sb равны\n");
  else
    printf("Ключи sa и sb неравны\n");
}
