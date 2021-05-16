#include <stdio.h>
#include <stdlib.h>

void explode_bomb() {
  printf("Bomb Exploded!!");
  exit(1);
}

void read_six_numbers(char* input, int* o) {
  int count = sscanf(input, "%d %d %d %d %d %d", o, o + 1, o + 2, o + 3, o + 4, o + 5);
  if (count <= 5) explode_bomb();
}

void phase_6(char* input) {
  int array[6];
  read_six_numbers(input, array);

  // input 1~6 inclusive, and no equal number
  int i = 0, r12 = 0;
  do {
    unsigned int tmp = array[i] - 1;
    if (tmp > 5) {
      explode_bomb();
    }
    r12++;
    if (r12 == 6) break;
    for (int rbx = r12; rbx < 6; rbx++) {
      int ret = array[rbx];
      if (ret == array[i]) {
        explode_bomb();
      }
    }
    i++;
  } while (1);

  // flip
  for (i = 0; i < 6; i++) {
    array[i] = 7 - array[i];
  }

  long *dict[6] = {
      (long*) 0x6032d0,
      (long*) 0x6032e0L,
      (long*) 0x6032f0L,
      (long*) 0x603300L,
      (long*) 0x603310L,
      (long*) 0x603320L
  };
  long *addr[6]; // start from array + 32;

  for (i = 0; i < 6; i++) {
    addr[i] = dict[array[i] - 1];
  }

  for (i = 0; i < 6; i++) {
    printf("%lx\n", (long) addr[i]);
  }

  for (i = 0; i < 5; i++) {
    *(addr[i] + 1) = (long) addr[i + 1];
  }

  *(addr[5] + 1) = 0;
  long *rbx = addr[0];
  int rbp = 5;
  do {
    int *a = (int*) *(rbx + 1);
    int *b = (int*) rbx;
    if (*b < *a) {
      explode_bomb();
    }
    rbx = (long*) *(rbx + 1);
    rbp--;
  } while (rbp != 0);
}

int main() {
  char* input = NULL;
  size_t len = 100;
  getline(&input, &len, stdin);
  phase_6(input);
}

