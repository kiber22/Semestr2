#include "28147_14.h"

int main() {

  unsigned char master_key[] = "6133291755541ec8a2c05f6600db0286";
  unsigned char original_text[16];
  unsigned char chipher_text[16];
  unsigned char round_keys[160];

  printf("Введите передаваемое сообщение: ");
  scanf("%16s", original_text);

  struct sockaddr_in user_addr_a_send;
  user_addr_a_send.sin_family = AF_INET;
  user_addr_a_send.sin_addr.s_addr = htonl(INADDR_ANY);
  user_addr_a_send.sin_port = htons(13000);
  int sock_send;
  if ((sock_send = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    perror("Не получен socket\n");
  if (bind(sock_send, (struct sockaddr *)&user_addr_a_send,
           sizeof(user_addr_a_send)))
    perror("Вызов bind от сервера неудачен\n");

  struct sockaddr_in user_addr_b;
  user_addr_b.sin_family = AF_INET;
  user_addr_b.sin_addr.s_addr = htonl(INADDR_ANY);
  user_addr_b.sin_port = htons(13002);

  ExpandKey(master_key, round_keys);
  Encrypt_14(original_text, chipher_text, round_keys);

  if (sendto(sock_send, chipher_text, 16, 0, (struct sockaddr *)&user_addr_b,
             sizeof(user_addr_b)) < 0)
    perror("Проблема с sendto.\n");

  printf("----------------------------------------\n");
  printf("Исходное сообщение: %s\n", original_text);
  printf("Отправленное зашифрованное сообщение: ");
  for (size_t i = 0; i < sizeof(chipher_text) / 4; i++)
    printf("%i", (int)chipher_text[i]);
  printf("\n----------------------------------------\n");

  return 0;
}
