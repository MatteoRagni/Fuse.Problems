/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* Copyright (c) 2017 - Matteo Ragni, University of Trento                   */
/*                                                                           */
/* Permission is hereby granted, free of charge, to any person               */
/* obtaining a copy of this software and associated documentation            */
/* files (the "Software"), to deal in the Software without                   */
/* restriction, including without limitation the rights to use,              */
/* copy, modify, merge, publish, distribute, sublicense, and/or sell         */
/* copies of the Software, and to permit persons to whom the                 */
/* Software is furnished to do so, subject to the following                  */
/* conditions:                                                               */
/*                                                                           */
/* The above copyright notice and this permission notice shall be            */
/* included in all copies or substantial portions of the Software.           */
/*                                                                           */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,           */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES           */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                  */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT               */
/* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,              */
/* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING              */
/* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR             */
/* OTHER DEALINGS IN THE SOFTWARE.                                           */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <math.h>
#include "fusepr.h"

struct fuse_operations FuseProblem_operations = {
 .getattr = FuseProblem_getattr,
 .open = FuseProblem_open,
 .read = FuseProblem_read,
 .readdir = FuseProblem_readdir,
 .write = FuseProblem_write,
 .truncate = FuseProblem_truncate,
 .getxattr = FuseProblem_getxattr,
 .setxattr = FuseProblem_setxattr,
 .access = FuseProblem_access
};

/*   __  __      _
 * |  \/  |__ _(_)_ _
 * | |\/| / _` | | ' \
 * |_|  |_\__,_|_|_||_|
 */
int main(int argc, char *argv[]) {

  CProblem * pr = CProblem_new("test/libtest.so");

  int ret = fuse_main(argc, argv, &FuseProblem_operations, (void*)pr);

  //char x_test[] = "2.0E00\n2.1E00";
  //char p_test[] = "1.0E00\n1.0E02\n";

  //Problem_write_x(pr, x_test);
  //Problem_write_p(pr, p_test);

  //printf("%s\n", pr->y_buf);
  //printf("%d\n", pr->y_buf_size);

  //printf("x = %lf, %lf\n", pr->x[0], pr->x[1]);
  //printf("y = %lf, %lf\n", pr->y[0], pr->y[1]);
  //printf("p = %lf, %lf\n", pr->p[0], pr->p[1]);

  CProblem_destroy(pr);
  //return ret;
  return 0;
}
