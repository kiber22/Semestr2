#include "28147_14.h"

uint64_t get_time() {
  uint32_t low, high;
  __asm__ __volatile__("rdtsc\n" : "=a"(low), "=d"(high));
  return ((uint64_t)high << 32) | low;
}

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
  ExpandKey(master_key, round_keys);
  /*Шифровка текста*/
  unsigned char chipher_text[16]; // зашифрованный текст
  uint64_t start_encrypt = get_time();
  Encrypt_14(original_text, chipher_text, round_keys);
  uint64_t end_encrypt = get_time();
  /*Расшифровка текста*/
  uint64_t start_decrypt = get_time();
  Decrypt_14(chipher_text, original_text, round_keys);
  uint64_t end_decrypt = get_time();
  /*Результат*/
  printf("-------------------------------\n");
  printf("ENCRYPTED TEXT: ");
  for (size_t i = 0; i < sizeof(chipher_text) / 4; i++)
    printf("%i", (int)chipher_text[i]);
  printf("\nDECRYPTED TEXT: %s\n", original_text);
  printf("-------------------------------\n");
  printf("TIME ENCRYPT: %" PRId64 "\n", (end_encrypt - start_encrypt));
  printf("TIME DECRYPT: %" PRId64 "\n", (end_decrypt - start_decrypt));
  printf("-------------------------------\n");
  return 0;
}
