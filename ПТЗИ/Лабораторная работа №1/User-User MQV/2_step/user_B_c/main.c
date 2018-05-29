#include "libraries.h"

uint64_t get_time() {
  uint32_t low, high;
  __asm__ __volatile__("rdtsc\n" : "=a"(low), "=d"(high));
  return ((uint64_t)high << 32) | low;
}

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
  uint64_t startTact = get_time();
  mpz_t p, g, q, b, A, B;
  get_data(&p, &q, &g, &b, &A, &B);
  uint64_t endTact = get_time();
  printf("-----------------------------------------------------------------\n");
  printf("Считал заранее подготовленные данные: %" PRId64 "\n",
         (endTact - startTact));

  // Генерация открыто сеансокого ключа
  startTact = get_time();
  mpz_t y, Y;
  gen_Y(p, g, q, &y, &Y);
  endTact = get_time();
  printf("-----------------------------------------------------------------\n");
  printf("Сгенерировал свой открытый сеансовый ключ: %" PRId64 "\n",
         (endTact - startTact));

  // Отправка открытого сеансокого ключа пользователю A
  startTact = get_time();
  char send_message[257];
  memset(send_message, 0, sizeof(send_message));
  mpz_get_str(send_message, 16, Y);
  if (sendto(sock_send, send_message, 257, 0, (struct sockaddr *)&user_addr_a,
             sizeof(user_addr_a)) < 0)
    perror("Проблема с sendto.\n");
  endTact = get_time();
  printf("-----------------------------------------------------------------\n");
  printf("Отправил свой открытый сеансовый ключ: %" PRId64 "\n",
         (endTact - startTact));

  // Получение открытого сеансокого ключа от пользователя A
  startTact = get_time();
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
  endTact = get_time();
  printf("-----------------------------------------------------------------\n");
  printf("Получил чужой открытый сеансовый ключ: %" PRId64 "\n",
         (endTact - startTact));

  // Вычисление числа Sb
  startTact = get_time();
  mpz_t sb;
  gen_sb(p, X, Y, A, b, y, q, &sb);
  endTact = get_time();
  printf("-----------------------------------------------------------------\n");
  printf("Расчитал общий ключ: %" PRId64 "\n", (endTact - startTact));
  printf("-----------------------------------------------------------------\n");
  gmp_printf("Общий ключ %Zx\n", sb);
  printf("-----------------------------------------------------------------\n");
}
