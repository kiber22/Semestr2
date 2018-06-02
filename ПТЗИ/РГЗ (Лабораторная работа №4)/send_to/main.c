#include ".h/libraries.h"

int main(int argc, char const *argv[]) {

  srand(time(NULL));

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

  // Вычисление HASH функции
  char key_sb[256];
  memset(key_sb, 0, 256);
  mpz_get_str(key_sb, 16, sb);
  GOST34112012Context CTX;
  u8 key[16];
  memset(key, 0, 16);
  GOST34112012Init(&CTX, 256);
  GOST34112012Update(&CTX, (unsigned char *)key_sb, strlen(key_sb));
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
  struct additional_data *additional_data =
      (struct additional_data *)calloc(1, sizeof(struct additional_data));
  for (size_t i = 0; i < 16; i++)
    additional_data->iv[i] = rand() % 50 + 50;
  printf("Инициализирующий вектор: %s\n", additional_data->iv);
  printf("-----------------------------------------------------------------\n");
  ECRYPT_ivsetup(&ctx, additional_data->iv);
  printf("Инициализирующий вектор установлен в структуру\n");
  printf("-----------------------------------------------------------------\n");

  // Шифрование
  u8 plaintext[25];
  memset(plaintext, 0, 25);
  printf("Введите шифруемый текст (не более 24 символов): ");
  scanf("%24s%n", plaintext, &additional_data->length_message);
  printf("-----------------------------------------------------------------\n");
  u8 *ciphertext = (u8 *)calloc(additional_data->length_message, sizeof(u8));
  ECRYPT_encrypt_bytes(&ctx, plaintext, ciphertext,
                       additional_data->length_message);
  printf("Шифрование %i символов завершено\n", additional_data->length_message);
  printf("-----------------------------------------------------------------\n");
  printf("Зашифрованный текст: ");
  for (int i = 0; i < additional_data->length_message; i++)
    printf("%x", ciphertext[i]);
  printf("\n");
  printf("-----------------------------------------------------------------\n");

  // Отправка дополнительных данных абоненту A
  if (sendto(sock_send, additional_data, sizeof(struct additional_data), 0,
             (struct sockaddr *)&user_addr_a, sizeof(user_addr_a)) < 0)
    perror("Проблема с sendto.\n");
  free(send_message);
  printf("Отправил дополнительные данные\n");
  printf("-----------------------------------------------------------------\n");

  // Отправка зашифрованного сообщения абоненту A
  if (sendto(sock_send, ciphertext, additional_data->length_message, 0,
             (struct sockaddr *)&user_addr_a, sizeof(user_addr_a)) < 0)
    perror("Проблема с sendto.\n");
  printf("Отправил зашифрованное сообщение\n");
  printf("-----------------------------------------------------------------\n");
  free(ciphertext);
}
