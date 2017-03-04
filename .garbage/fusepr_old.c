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

int FuseProblem_getattr(const char *path, struct stat *stbuf) {

  int path_id = problem_path(path);
  if (path_id < FunctionOutput::root)
    return -ENOENT;

  memset(stbuf, 0, sizeof(struct stat));

  // User space configuration
  stbuf->st_uid = getuid();
  stbuf->st_gid = getgid();
  stbuf->st_atime = stbuf->st_mtime = time(NULL);

  switch(path_id) {
  case FunctionOutput::root:
    stbuf->st_mode = S_IFDIR | S_IRUSR | S_IRGRP;
    stbuf->st_nlink = 2;
    break;
  case FunctionOutput::xo:
    stbuf->st_mode = S_IFREG | S_IWUSR | S_IWGRP | S_IRUSR | S_IRGRP;
    stbuf->st_nlink = 1;
    stbuf->st_size = problem_xfilesize();
    break;
  case FunctionOutput::po:
    stbuf->st_mode = S_IFREG | S_IWUSR | S_IWGRP | S_IRUSR | S_IRGRP;
    stbuf->st_nlink = 1;
    // stbuf->st_size = GET_PROBLEM()->y_buf_size;
    stbuf->st_size = problem_pfilesize();
    break;
  case FunctionInput::info:
    stbuf->st_mode = S_IFREG | S_IRUSR | S_IRGRP;
    stbuf->st_nlink = 1;
    // stbuf->st_size = GET_PROBLEM()->y_buf_size;
    stbuf->st_size = problem_infofilesize();
    break;
  case P_PATH:
    stbuf->st_mode = S_IFREG | S_IWUSR | S_IWGRP;
    stbuf->st_nlink = 1;
    stbuf->st_size = 0;
    break;
  default:
    if (path_id >= 0 && path_id < problem_fsize()) {
      stbuf->st_mode = S_IFREG | S_IRUSR | S_IRGRP;
      stbuf->st_nlink = 1;
      stbuf->st_size = problem_ffilesize(path_id);
    }
    break;
  }
  return 0;
}

int FuseProblem_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                        off_t offset, struct fuse_file_info *fi) {
  int path_id = problem_path(path);
  if (path_id < FunctionOutput::root)
    return 0;

  (void)offset;
  (void)fi;

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);

  switch (path_id) {
  case FunctionOutput::root:
    filler(buf, "x", NULL, 0);
    filler(buf, "y", NULL, 0);
    for (int i = 0; i < problem_fsize(), i++) {
      string s("/y");
      filler(buf, (s + to_string(i)).c_str(), NULL, 0);
    }
    break;
  case FunctionOutput::info:
    filler(buf, "info", NULL, 0);
    break;
  case FunctionOutput::xo:
    filler(buf, "x", NULL, 0);
    break;
  case FunctionOutput::po:
    filler(buf, "x", NULL, 0);
    break;
  default:
    if (path_id >= 0 && path_id < problem_fsize()) {
      string s("/y");
      filler(buf, (s + to_string(path_id)).c_str(), NULL, 0);
    }
    break;
  }
  return 0;
}

int FuseProblem_open(const char *path, struct fuse_file_info *fi) {
  return 0;

  /*
  fi->direct_io = 1;

  if (checkPath(path) >= 0)
    return 0;
  return -ENOENT;
  */s
}

int FuseProblem_write(const char *path, const char *buf, size_t size,
                      off_t offset, struct fuse_file_info *fi) {
  /*
  if (strcmp(path, FuseProblem_p_path) == 0) {
    Problem_write_p(GET_PROBLEM(), buf);
    return size;
  }
  if (strcmp(path, FuseProblem_x_path) == 0) {
    Problem_write_x(GET_PROBLEM(), buf);
    return size;
  }
  */
  return size;
}

int FuseProblem_read(const char *path, char *buf, size_t size, off_t offset,
                     struct fuse_file_info *fi) {
  /*
  if (strcmp(path, FuseProblem_y_path) == 0) {
    return Problem_read(GET_PROBLEM(), buf, size, offset);
  }
  */
  return 0;
}

int FuseProblem_truncate(const char *path, off_t size) {
  /*
  if (strcmp(path, FuseProblem_x_path) == 0 ||
      strcmp(path, FuseProblem_p_path) == 0)
    return 0;
  */
  return -ENOENT;
}

int FuseProblem_flush(const char *path, struct fuse_file_info *fi) { return 0; }

int FuseProblem_access(const char *path, int mask) {
//  switch (checkPath(path)) {
//  case ROOT_PATH:
//  case Y_PATH:
//    if (mask & R_OK)
//      return 0;
//  case X_PATH:
//  case P_PATH:
//    if (mask & W_OK)
//      return 0;
//  //default:
//  //  if (mask & R_OK)
//  //    return 0;
//  }
//  return -EACCES;
  return 0;
}

int FuseProblem_setxattr(const char *path, const char *name, const char *value,
                         size_t size, int flags) {
  return -ENOTSUP;
}

int FuseProblem_getxattr(const char *path, const char *name, char *value,
                         size_t size) {
  return -ENOTSUP;
}

int FuseProblem_listxattr(const char *path, const char *list, size_t size) {
  return -ENOTSUP;
}