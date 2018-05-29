#include ".h/libraries.h"

int main(int argc, char const *argv[]) {

  // Создание сокета для отправки сообщений пользователю B
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

  // Создание сокета для приема сообщений от пользователя B
  struct sockaddr_in user_addr_a_recv;
  user_addr_a_recv.sin_family = AF_INET;
  user_addr_a_recv.sin_addr.s_addr = htonl(INADDR_ANY);
  user_addr_a_recv.sin_port = htons(13001);
  int sock_recv;
  if ((sock_recv = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    perror("Не получен socket\n");
  if (bind(sock_recv, (struct sockaddr *)&user_addr_a_recv,
           sizeof(user_addr_a_recv)))
    perror("Вызов bind от сервера неудачен\n");

  // Данные пользователя B для отправки сообщения
  struct sockaddr_in user_addr_b;
  user_addr_b.sin_family = AF_INET;
  user_addr_b.sin_addr.s_addr = htonl(INADDR_ANY);
  ;
  user_addr_b.sin_port = htons(13003);

  // Считывание данных для пользователя A, полученные на первом шаге
  mpz_t p, g, q, a, A, B;
  get_data(&p, &g, &q, &a, &A, &B);
  printf("-----------------------------------------------------------------\n");
  printf("Считал заранее подготовленные данные\n");

  // Генерация открыто сеансокого ключа
  mpz_t x, X;
  gen_X(p, g, q, &x, &X);
  printf("-----------------------------------------------------------------\n");
  printf("Сгенерировал свой открытый сеансовый ключ\n");

  // Ожидание открытого сеансокого ключа от пользователя B
  int msgLength;
  char *recv_message = (char *)calloc(257, sizeof(char));
  while (1) {
    if ((msgLength = recvfrom(sock_recv, recv_message, 257, 0, NULL, NULL)) < 0)
      perror("Плохой socket клиента.\n");
    else
      break;
  }
  mpz_t Y;
  mpz_init(Y);
  mpz_set_str(Y, recv_message, 16);
  free(recv_message);
  printf("-----------------------------------------------------------------\n");
  printf("Получил чужой открытый сеансовый ключ\n");

  // Отправка открытого сеансокого ключа пользователю B
  char send_message[257];
  memset(send_message, 0, sizeof(send_message));
  mpz_get_str(send_message, 16, X);
  if (sendto(sock_send, send_message, 257, 0, (struct sockaddr *)&user_addr_b,
             sizeof(user_addr_b)) < 0)
    perror("Проблема с sendto.\n");
  printf("-----------------------------------------------------------------\n");
  printf("Отправил свой открытый сеансовый ключ\n");

  // Вычисление числа Sa
  mpz_t sa;
  gen_sa(p, X, Y, B, a, x, q, &sa);
  printf("-----------------------------------------------------------------\n");
  gmp_printf("Общий ключ: %Zx\n", sa);
  printf("-----------------------------------------------------------------\n");

  // Ожидание инициализирующего вектора, число символов и зашифрованное
  // сообщение от пользователя B
  recv_message = (char *)calloc(16, sizeof(char));
  char *recv_message_2;
unsigned char *recv_message_3;
  while (1) {
    if ((msgLength = recvfrom(sock_recv, recv_message, 16, 0, NULL, NULL)) < 0)
      perror("Плохой socket клиента.\n");
    else {
      printf("Получил инициализирующий вектор\n");
      printf("-----------------------------------------------------------------"
             "\n");
      recv_message_2 = (char *)calloc(2, sizeof(char));
      while (1) {
        if ((msgLength =
                 recvfrom(sock_recv, recv_message_2, 2, 0, NULL, NULL)) < 0)
          perror("Плохой socket клиента.\n");
        else {
          printf("Получил длину сообщения\n");
          printf("-------------------------------------------------------------"
                 "----\n");
          recv_message_3 =
              (unsigned char *)calloc(((int)recv_message_2[0] + 1), sizeof(char));
          while (1) {
            if ((msgLength =
                     recvfrom(sock_recv, recv_message_3,
                              ((int)recv_message_2[0] + 1), 0, NULL, NULL)) < 0)
              perror("Плохой socket клиента.\n");
            else {
              printf("Получил зашифрованное сообщение\n");
              printf("---------------------------------------------------------"
                     "--------\n");
              break;
            }
          }
          break;
        }
      }
      break;
    }
  }
  printf("Полученый инициализирующий вектор: ");
  for (size_t i = 0; i < 16; i++)
    printf("%c", recv_message[i]);
  printf("\n");
  printf("-----------------------------------------------------------------\n");
  int num_symbols = (int)recv_message_2[0];
  free(recv_message_2);
  printf("Число зашифрованных символов: %i\n", num_symbols);
  printf("-----------------------------------------------------------------\n");
  printf("Полученное зашифрованное сообщение: ");
  for (size_t i = 0; i < (num_symbols + 1); i++)
    printf("%x", recv_message_3[i]);
  printf("\n");
  printf("-----------------------------------------------------------------\n");

  // Инициалицация структуры состояния генератора
  ECRYPT_ctx ctx;
  ECRYPT_init();
  printf("Структура состояния генератора инициализирована\n");
  printf("-----------------------------------------------------------------\n");

  // Установка секретного ключа в структуру
  u8 key[16];
  memset(key, 0, 16);
  char key_sa[256];
  memset(key_sa, 0, 256);
  mpz_get_str(key_sa, 16, sa);
  for (size_t i = 0; i < 16; i++)
    key[i] = (key_sa[i * 2] << 4) + key_sa[i * 2 + 1];
  ECRYPT_keysetup(&ctx, key, 128, 128);
  printf("Секретный ключ установлен в структуру\n");
  printf("-----------------------------------------------------------------\n");

  // Установка инициализируещего вектора
  u8 iv[16];
  memset(iv, 0, 16);
  for (size_t i = 0; i < 16; i++)
    iv[i] = recv_message[i];
  free(recv_message);
  ECRYPT_ivsetup(&ctx, iv);
  printf("Инициализирующий вектор установлен в структуру\n");
  printf("-----------------------------------------------------------------\n");

  // Расшифровка
  u8 *ciphertext = (u8 *)calloc((num_symbols + 1), sizeof(u8));
  for (size_t i = 0; i < (num_symbols + 1); i++)
    ciphertext[i] = recv_message_3[i];
  free(recv_message_3);
  u8 *plaintext = (u8 *)calloc((num_symbols + 1), sizeof(u8));
  ECRYPT_encrypt_bytes(&ctx, ciphertext, plaintext, num_symbols + 1);
  printf("Расшифрованный текст: ");
  for (int i = 0; i < num_symbols; i++)
    printf("%c", plaintext[i]);
  printf("\n");
  printf("-----------------------------------------------------------------\n");
}
