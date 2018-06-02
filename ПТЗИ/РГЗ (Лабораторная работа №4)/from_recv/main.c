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
  char recv_message[257];
  memset(recv_message, 0, 257);
  while (1) {
    if ((msgLength = recvfrom(sock_recv, recv_message, 257, 0, NULL, NULL)) < 0)
      perror("Плохой socket клиента.\n");
    else
      break;
  }
  mpz_t Y;
  mpz_init(Y);
  mpz_set_str(Y, recv_message, 16);
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
  printf("Расчитал общий ключ\n");
  printf("-----------------------------------------------------------------\n");;
  gmp_printf("Общий ключ: %Zx\n", sa);
  printf("-----------------------------------------------------------------\n");

  // Ожидание дополнительной информации от абонента B
  struct additional_data *additional_data =
      (struct additional_data *)calloc(1, sizeof(struct additional_data));
  while (1) {
    if ((msgLength = recvfrom(sock_recv, additional_data,
                        sizeof(struct additional_data), 0, NULL, NULL)) < 0)
      perror("Плохой socket клиента.\n");
    else
      break;
  }
  printf("Принял дополнительные данные\n");
  printf("-----------------------------------------------------------------\n");
  printf("Инициализирующий вектор: %s\n", additional_data->iv);
  printf("-----------------------------------------------------------------\n");
  printf("Длина зашифрованного сообщения: %i\n",
         additional_data->length_message);
  printf("-----------------------------------------------------------------\n");

  // Ожидание зашифрованного сообщения от абонента B
  u8 *ciphertext = (u8 *)calloc(additional_data->length_message, sizeof(u8));
  while (1) {
    if ((msgLength = recvfrom(sock_recv, ciphertext,
                        additional_data->length_message, 0, NULL, NULL)) < 0)
      perror("Плохой socket клиента.\n");
    else
      break;
  }
  printf("Принял зашифрованное сообщение: ");
  for (size_t i = 0; i < additional_data->length_message; i++) {
    printf("%x", ciphertext[i]);
  }
  printf("\n");
  printf("-----------------------------------------------------------------\n");

  // Вычисление HASH функции
  char key_sa[256];
  memset(key_sa, 0, 256);
  mpz_get_str(key_sa, 16, sa);
  GOST34112012Context CTX;
  u8 key[16];
  memset(key, 0, 16);
  GOST34112012Init(&CTX, 256);
  GOST34112012Update(&CTX, (unsigned char *)key_sa, strlen(key_sa));
  GOST34112012Final(&CTX, key);
  printf("HASH функция вычислена\n");
  printf("-----------------------------------------------------------------\n");
  printf("HASH функция: ");
  for (int i = 0; i < 32; i++)
    printf("%0X", key[i]);
  printf("\n");
  printf("-----------------------------------------------------------------\n");

  // Инициалицация структуры состояния генератора
  ECRYPT_ctx ctx;
  ECRYPT_init();
  printf("Структура состояния генератора инициализирована\n");
  printf("-----------------------------------------------------------------\n");

  // Установка секретного ключа в структуру
  ECRYPT_keysetup(&ctx, key, 128, 128);
  printf("Секретный ключ установлен в структуру\n");
  printf("-----------------------------------------------------------------\n");

  // Установка инициализируещего вектора
  ECRYPT_ivsetup(&ctx, additional_data->iv);
  printf("Инициализирующий вектор установлен в структуру\n");
  printf("-----------------------------------------------------------------\n");

  // Расшифровка
  u8 *plaintext = (u8 *)calloc(additional_data->length_message, sizeof(u8));
  ECRYPT_encrypt_bytes(&ctx, ciphertext, plaintext,
                       additional_data->length_message);
  printf("Расшифрованный текст: ");
  for (int i = 0; i < additional_data->length_message; i++)
    printf("%c", plaintext[i]);
  printf("\n");
  printf("-----------------------------------------------------------------\n");
}
