#include <stdio.h>

void main(int argc, char** argv) {
  int i, j;

  if (argc != 3) {
    printf(" usage : %s [Max i] [Max j]\n"
	   " @ find value i%%j=0.\n", argv[0]);
    return;
  }
  int max_i = atoi(argv[1]);
  int max_j = atoi(argv[2]);

  for (  i=max_j; i<=max_i;   i++)
    for (j=1;     j<=max_j; j++) {
      if (i%j != 0) break;
      if (i%j == 0 && j != max_j) continue;
      
      printf("j=%d answer = %d\n", j, i);
      return;
    }
  printf("no answer, i=%d\n", i);
}
