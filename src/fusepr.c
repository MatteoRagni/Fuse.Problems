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

#include "fusepr.h"

static int FuseProblem_getattr(const char *path, struct stat *stbuf) {
  memset(stbuf, 0, sizeof(struct stat));

  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | S_IRUSR;
    stbuf->st_nlink = 2;
    return 0;
  }

  if (strcmp(path, FuseProblem_x_path) == 0) {
    stbuf->st_mode = S_IFREG | S_IWUSR;
    stbuf->st_nlink = 1;
    stbuf->st_size = 0;
    return 0;
  }

  if (strcmp(path, FuseProblem_y_path) == 0) {
    stbuf->st_mode = S_IFREG | S_IRUSR;
    stbuf->st_nlink = 1;
    stbuf->st_size = GET_PROBLEM()->y_buf_size;
    return 0;
  }

  if (strcmp(path, FuseProblem_p_path) == 0) {
    stbuf->st_mode = S_IFREG | S_IWUSR;
    stbuf->st_nlink = 1;
    stbuf->st_size = 0;
    return 0;
  }

  return -ENOENT;
}

static int FuseProblem_readdir(const char *path, void *buf,
                               fuse_fill_dir_t filler, off_t offset,
                               struct fuse_file_info *fi) {
  if (strcmp(path, "/") != 0)
    return 0;

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);
  filler(buf, FuseProblem_x_path, NULL, 0);
  filler(buf, FuseProblem_y_path, NULL, 0);
  filler(buf, FuseProblem_p_path, NULL, 0);
  return 0;
}

static int FuseProblem_open(const char *path, struct fuse_file_info *fi) {
  return 0;
}

static int FuseProblem_write(const char *path, const char *buf, size_t size,
                             off_t offset, struct fuse_file_info *fi) {
  if (strcmp(path, FuseProblem_p_path) == 0) {
    if (Problem_write_p(GET_PROBLEM(), buf) == 0)
      return size;
    else
      return 0;
  }
  if (strcmp(path, FuseProblem_x_path) == 0) {
    if (Problem_write_x(GET_PROBLEM(), buf) == 0)
      return size;
    else
      return 0;
  }

  return size;
}

static int FuseProblem_read(const char *path, char *buf, size_t size,
                            off_t offset, struct fuse_file_info *fi) {
  if (strcmp(path, FuseProblem_y_path) == 0) {
    return Problem_read(GET_PROBLEM(), buf, size, offset);
  }
  return 0;
}