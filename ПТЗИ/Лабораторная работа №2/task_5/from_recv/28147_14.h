#include "rdtsc.h"
#include <arpa/inet.h>
#include <errno.h>
#include <gmp.h>
#include <inttypes.h>
#include <memory.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int funcX(unsigned char *a, unsigned char *b, unsigned char *outdata);
int funcS(unsigned char *indata, unsigned char *outdata);
int funcReverseS(unsigned char *indata, unsigned char *outdata);
int funcR(unsigned char *indata, unsigned char *outdata);
int funcReverseR(unsigned char *indata, unsigned char *outdata);
int funcL(unsigned char *indata, unsigned char *outdata);
int funcReverseL(unsigned char *indata, unsigned char *outdata);
int funcLSX(unsigned char *a, unsigned char *b, unsigned char *outdata);
int funcReverseLSX(unsigned char *a, unsigned char *b, unsigned char *outdata);
int funcF(unsigned char *inputKey, unsigned char *inputKeySecond,
          unsigned char *iterationConst, unsigned char *outputKey,
          unsigned char *outputKeySecond);
int funcC(unsigned char number, unsigned char *output);
int ExpandKey(unsigned char *masterKey, unsigned char *keys);
int Encrypt_14(unsigned char *plainText, unsigned char *chipherText,
               unsigned char *keys);
int Decrypt_14(unsigned char *chipherText, unsigned char *plainText,
               unsigned char *keys);
