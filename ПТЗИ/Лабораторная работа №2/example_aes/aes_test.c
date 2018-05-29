#include <memory.h>
#include <stdio.h>
#include "rdtsc.h"

#include "rijndael-alg-fst.h"

static void pba (const char* text, unsigned char* value, unsigned int valueSize)
  {
  int i;
  printf ("%s", text);
  for (i = 0; i < valueSize; i++) printf ("%02x ", value[i]);
  }

static void pua (const char* text, unsigned int* value, unsigned int valueSize)
  {
  int i;
  printf ("%s", text);
  for (i = 0; i < valueSize; i++) printf ("%08x ", value[i]);
  }

int main ()
  {
  unsigned char K [32];
  u32 RK [60];
  unsigned char P[100], C[16];
  int t1, t2;
  memset (K, 0, 32);
  printf ("KEY: "), scanf ("%s", K);
  memset (P, 0, 100);
  printf ("TEXT: "), scanf ("%s", P);
  rijndaelKeySetupEnc (RK, K, 256);
  printf ("K: %s\nT: %s\n", K, P);
  rijndaelEncrypt (RK, 14, P, C);
  t1 = CC ();
  rijndaelEncrypt (RK, 14, P, C);
  t2 = CC ();
  pba ("C: ", C, 16);  
  printf ("\ncycles = %d", t2 - t1);
  memset (P, 0, 100);
  rijndaelKeySetupDec (RK, K, 256);
  rijndaelDecrypt (RK, 14, C, P);
  t1 = CC ();
  rijndaelDecrypt (RK, 14, C, P);
  t2 = CC ();
  printf ("\nT: %s\n", P);
  printf ("cycles = %d\n", t2 - t1);
  return 0;
  }
