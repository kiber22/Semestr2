#include "28147_14.h"

int main() {

  unsigned char master_key[] = "6133291755541ec8a2c05f6600db0286";
  unsigned char original_text[16];
  memset(original_text, 16, sizeof(original_text));
  unsigned char chipher_text[16];
  memset(chipher_text, 16, sizeof(chipher_text));
  unsigned char round_keys[160];

  printf("Ожидание сообщения\n");

  struct sockaddr_in user_addr_b_recv;
  user_addr_b_recv.sin_family = AF_INET;
  user_addr_b_recv.sin_addr.s_addr = htonl(INADDR_ANY);
  user_addr_b_recv.sin_port = htons(13002);
  int sock_recv;
  if ((sock_recv = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    perror("Не получен socket\n");
  if (bind(sock_recv, (struct sockaddr *)&user_addr_b_recv,
           sizeof(user_addr_b_recv)))
    perror("Вызов bind от сервера неудачен\n");

  int msgLength;
  while (1) {
    if ((msgLength = recvfrom(sock_recv, chipher_text, 16, 0, NULL, NULL)) < 0)
      perror("Плохой socket клиента.\n");
    else
      break;
  }

  ExpandKey(master_key, round_keys);
  Decrypt_14(chipher_text, original_text, round_keys);

  printf("----------------------------------------\n");
  printf("Принятое зашифрованное сообщение: ");
  for (size_t i = 0; i < sizeof(chipher_text) / 4; i++)
    printf("%i", (int)chipher_text[i]);
  printf("\nРасшифрованоое сообщение: %s\n", original_text);
  printf("----------------------------------------\n");

  return 0;
}
