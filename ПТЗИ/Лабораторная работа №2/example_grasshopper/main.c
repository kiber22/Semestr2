#include "28147_14.h"
#include "callback_print.h"
#include "rdtsc.h"
#include <memory.h>
#include <stdio.h>

int main() {
  /*Входные данные*/
  unsigned char master_key[32];    // ключ 256 бит
  unsigned char original_text[16]; // шифруемый текст 128 бит
  memset(master_key, 0, 32);
  printf("MASTER KEY: "), scanf("%s", master_key);
  memset(original_text, 0, 16);
  printf("TEXT FOR ENCRYPT: "), scanf("%s", original_text);
  /*Выработка раундовых ключей по мастер-ключу*/
  unsigned char round_keys[160]; // раундовые ключи 1280 бит
  ExpandKey(master_key, round_keys, 0);
  /*Шифровка текста*/
  unsigned char chipher_text[16]; // зашифрованный текст
  Encrypt_14(original_text, chipher_text, round_keys, 0, 0);
  /*Расшифровка текста*/
  Decrypt_14(chipher_text, original_text, round_keys, 0, 0);
  /*Результат*/
  printf("ENCRYPTED TEXT: ");
  for (size_t i = 0; i < sizeof(chipher_text) / 4; i++)
    printf("%i", (int)chipher_text[i]);
  printf("\nDECRYPTED TEXT: %s\n", original_text);
  return 0;
}
