#include <stdio.h>
#include <stdlib.h>
#include "problem.h"

int main() {

  CProblem pr;

  pr = CProblem_new("testString");
  printf("<Problem:%#010x>\n", pr);

  char xi[] = "1.0\n 2.0";
  char pi[] = "4.0\n 5.0";
  CProblem_x_write(pr, xi);
  CProblem_p_write(pr, pi);

  char *yo;
  yo = (char*)calloc(4096, sizeof(char));
  size_t r = CProblem_read(pr, -2, yo, 4096, 0);
  printf("\n%s\n",yo);
  printf("length = %d\n", r);

  r = CProblem_read(pr, -1, yo, 4096, 0);
  printf("\n%s\n",yo);
  printf("length = %d\n", r);

  r = CProblem_read(pr, 0, yo, 4096, 0);
  printf("\n%s\n",yo);
  printf("length = %d\n", r);

  r = CProblem_read(pr, 1, yo, 4096, 0);
  printf("\n%s\n",yo);
  printf("length = %d\n", r);

  CProblem_destroy(pr);
  free(yo);
  return 0;

}
