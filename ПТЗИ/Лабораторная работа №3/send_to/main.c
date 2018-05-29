#include ".h/libraries.h"

int main(int argc, char const *argv[]) {

  // Создание сокета для отправки сообщений пользователю A
  struct sockaddr_in user_addr_b_send;
  user_addr_b_send.sin_family = AF_INET;
  user_addr_b_send.sin_addr.s_addr = htonl(INADDR_ANY);
  user_addr_b_send.sin_port = htons(13002);
  int sock_send;
  if ((sock_send = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    perror("Не получен socket\n");
  if (bind(sock_send, (struct sockaddr *)&user_addr_b_send,
           sizeof(user_addr_b_send)))
    perror("Вызов bind от сервера неудачен\n");

  // Создание сокета для приема сообщений от пользователя A
  struct sockaddr_in user_addr_b_recv;
  user_addr_b_recv.sin_family = AF_INET;
  user_addr_b_recv.sin_addr.s_addr = htonl(INADDR_ANY);
  user_addr_b_recv.sin_port = htons(13003);
  int sock_recv;
  if ((sock_recv = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    perror("Не получен socket\n");
  if (bind(sock_recv, (struct sockaddr *)&user_addr_b_recv,
           sizeof(user_addr_b_recv)))
    perror("Вызов bind от сервера неудачен\n");

  // Данные пользователя A для отправки сообщения
  struct sockaddr_in user_addr_a;
  user_addr_a.sin_family = AF_INET;
  user_addr_a.sin_addr.s_addr = htonl(INADDR_ANY);
  user_addr_a.sin_port = htons(13001);

  // Считывание данных для пользователя B, полученные на первом шаге
  mpz_t p, g, q, b, A, B;
  get_data(&p, &q, &g, &b, &A, &B);
  printf("-----------------------------------------------------------------\n");
  printf("Считал заранее подготовленные данные\n");

  // Генерация открыто сеансокого ключа
  mpz_t y, Y;
  gen_Y(p, g, q, &y, &Y);
  printf("-----------------------------------------------------------------\n");
  printf("Сгенерировал свой открытый сеансовый ключ\n");

  // Отправка открытого сеансокого ключа пользователю A
  char *send_message = (char *)calloc(257, sizeof(char));
  mpz_get_str(send_message, 16, Y);
  if (sendto(sock_send, send_message, 257, 0, (struct sockaddr *)&user_addr_a,
             sizeof(user_addr_a)) < 0)
    perror("Проблема с sendto.\n");
  free(send_message);
  printf("-----------------------------------------------------------------\n");
  printf("Отправил свой открытый сеансовый ключ\n");

  // Получение открытого сеансокого ключа от пользователя A
  int msgLength;
  char recv_message[257];
  while (1) {
    if ((msgLength = recvfrom(sock_recv, recv_message, 257, 0, NULL, NULL)) < 0)
      perror("Плохой socket клиента.\n");
    else
      break;
  }
  mpz_t X;
  mpz_init(X);
  mpz_set_str(X, recv_message, 16);
  printf("-----------------------------------------------------------------\n");
  printf("Получил чужой открытый сеансовый ключ\n");

  // Вычисление числа Sb
  mpz_t sb;
  gen_sb(p, X, Y, A, b, y, q, &sb);
  printf("-----------------------------------------------------------------\n");
  printf("Расчитал общий ключ\n");
  printf("-----------------------------------------------------------------\n");
  gmp_printf("Общий ключ: %Zx\n", sb);
  printf("-----------------------------------------------------------------\n");

  // Инициалицация структуры состояния генератора
  ECRYPT_ctx ctx;
  ECRYPT_init();
  printf("Структура состояния генератора инициализирована\n");
  printf("-----------------------------------------------------------------\n");

  // Установка секретного ключа в структуру
  u8 key[16];
  memset(key, 0, 16);
  char key_sb[256];
  memset(key_sb, 0, 256);
  mpz_get_str(key_sb, 16, sb);
  for (size_t i = 0; i < 16; i++)
    key[i] = (key_sb[i * 2] << 4) + key_sb[i * 2 + 1];
  ECRYPT_keysetup(&ctx, key, 128, 128);
  printf("Секретный ключ установлен в структуру\n");
  printf("-----------------------------------------------------------------\n");

  // Установка инициализируещего вектора
  u8 iv[16];
  memset(iv, 0, 16);
  printf("Введите инициализирующий вектор: ");
  scanf("%16s", iv);
  printf("-----------------------------------------------------------------\n");
  ECRYPT_ivsetup(&ctx, iv);
  printf("Инициализирующий вектор установлен в структуру\n");
  printf("-----------------------------------------------------------------\n");

  // Шифрование
  u8 plaintext[25];
  memset(plaintext, 0, 25);
  printf("Введите шифруемый текст (не более 24 символов): ");
  scanf("%24s", plaintext);
  int num_symbols = strlen((char *)plaintext);
  printf("-----------------------------------------------------------------\n");
  u8 *ciphertext = (u8 *)calloc((num_symbols + 1), sizeof(u8));
  ECRYPT_encrypt_bytes(&ctx, plaintext, ciphertext, (num_symbols + 1));
  printf("Шифрование %i символов завершено\n", num_symbols);
  printf("-----------------------------------------------------------------\n");
  printf("Зашифрованный текст: ");
  for (int i = 0; i < (num_symbols+1); i++)
    printf("%x", ciphertext[i]);
  printf("\n");
  printf("-----------------------------------------------------------------\n");

  // Отправка инициализируещего вектора абоненту A
  send_message = (char *)calloc(16, sizeof(char));
  for (size_t i = 0; i < 16; i++)
    send_message[i] = iv[i];
  if (sendto(sock_send, send_message, 16, 0, (struct sockaddr *)&user_addr_a,
             sizeof(user_addr_a)) < 0)
    perror("Проблема с sendto.\n");
  free(send_message);
  printf("Отправил инициализирующий вектор\n");
  printf("-----------------------------------------------------------------\n");

  // Отправка количества символов в сообщении абоненту A
  send_message = (char *)calloc(2, sizeof(char));
  send_message[0] = (char)num_symbols;
  send_message[1] = '\0';
  if (sendto(sock_send, send_message, 2, 0, (struct sockaddr *)&user_addr_a,
             sizeof(user_addr_a)) < 0)
    perror("Проблема с sendto.\n");
  free(send_message);
  printf("Отправил количество символов в сообщении\n");
  printf("-----------------------------------------------------------------\n");

  // Отправка зашифрованного сообщения абоненту A
  send_message = (char *)calloc((num_symbols + 1), sizeof(char));
  for (size_t i = 0; i < (num_symbols + 1); i++)
    send_message[i] = ciphertext[i];
  if (sendto(sock_send, send_message, (num_symbols + 1), 0,
             (struct sockaddr *)&user_addr_a, sizeof(user_addr_a)) < 0)
    perror("Проблема с sendto.\n");
  printf("Отправил зашифрованное сообщение\n");
  printf("-----------------------------------------------------------------\n");
  free(ciphertext);
}
