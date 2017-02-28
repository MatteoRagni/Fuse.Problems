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

#include "problem.h"
#include <errno.h>
#include <fuse.h>

int rosenbrock_f(testProblem *s) {
  s->y[0] = pow(s->p[0] - s->x[0], 2.0) +
            s->p[1] * pow((s->x[1] - pow(s->x[0], 2.0)), 2.0);
  s->y[1] = pow(s->p[0] - s->x[1], 2.0) +
            s->p[1] * pow((s->x[0] - pow(s->x[1], 2.0)), 2.0);
  return 0;
}

Problem *pr;

// Fuse configuration
static const char *FuseProblem_x_path = "/x";
static const char *FuseProblem_y_path = "/y";
static const char *FuseProblem_p_path = "/p";

static int FuseProblem_getattr(const char *path, struct stat *stbuf);
static int FuseProblem_readdir(const char *path, void *buf,
                               fuse_fill_dir_t filler, off_t offset,
                               struct fuse_file_info *fi);
static int FuseProblem_open(const char *path, struct fuse_file_info *fi);
static int FuseProblem_read(onst char *path, char *buf, size_t size,
                            off_t offset, struct fuse_file_info *fi);

struct fuse_operations testFnc_operations = {
  .getattr = FuseProblem_getattr,
  .open = FuseProblem_open,
  .read = FuseProblem_read,
  .readdir = FuseProblem_readdir
};


static int FuseProblem_getattr(const char *path, struct stat *stbuf) {
  memset(stbuf, 0, sizeof(struct stat));

  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | S_IRUSR;
    stbuf->st_nlink = 2;
    return 0;
  }

  if (strcmp(path, FuseProblem_x_path) == 0) {
    stbuf->st_mode = S_IFDIR | S_IRUSR;
    stbuf->st_nlink = 2;
    stbuf->st_size = 
    return 0;
  }

  if (strcmp(path, FuseProblem_y_path) == 0) {
    stbuf->st_mode = S_IFDIR | S_IRUSR;
    stbuf->st_nlink = 2;
    stbuf->st_size =
    return 0;
  }

  if (strcmp(path, FuseProblem_p_path) == 0) {
    stbuf->st_mode = S_IFDIR | S_IRUSR;
    stbuf->st_nlink = 2;
    stbuf->st_size =
    return 0;
  }

  return -ENOENT;
}

/*   __  __      _
 * |  \/  |__ _(_)_ _
 * | |\/| / _` | | ' \
 * |_|  |_\__,_|_|_||_|
 */
int main(int argc, char *argv[]) {

  pr = Problem_init(2, 2, 2);
  pr->f = rosenbrock_f;
}
