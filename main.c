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

#include <math.h>
#include "fusepr.h"

struct fuse_operations FuseProblem_operations = {
 .getattr = FuseProblem_getattr,
 .open = FuseProblem_open,
 .read = FuseProblem_read,
 .readdir = FuseProblem_readdir,
 .write = FuseProblem_write
};

int rosenbrock_f(Problem *s) {
  s->y[0] = pow(s->p[0] - s->x[0], 2.0) +
            s->p[1] * pow((s->x[1] - pow(s->x[0], 2.0)), 2.0);
  s->y[1] = pow(s->p[0] - s->x[1], 2.0) +
            s->p[1] * pow((s->x[0] - pow(s->x[1], 2.0)), 2.0);
  return 0;
}

/*   __  __      _
 * |  \/  |__ _(_)_ _
 * | |\/| / _` | | ' \
 * |_|  |_\__,_|_|_||_|
 */
int main(int argc, char *argv[]) {

  Problem * pr = Problem_init(2, 2, 2);
  pr->f = rosenbrock_f;

  return fuse_main(argc, argv, &FuseProblem_operations, (void*)pr);
}
