#include <stdio.h>

void main(int argc, char** argv) {
  int i, j;

  if (argc != 3) {
    printf(" usage : %s [value] [#.yakusu]\n", argv[0]);
    return;
  }
  int valu = atoi(argv[1]);
  int Nyakusu = atoi(argv[2]);

  int tmp_j = 2;

  for (  i=0;     i<=Nyakusu; i++)
    for (j=tmp_j; j<valu;    j++) {
      if (valu%j == 0) {
	printf(" %d", j);
	tmp_j = j+1;
	break;
      }
    }
  printf("\n");
}
