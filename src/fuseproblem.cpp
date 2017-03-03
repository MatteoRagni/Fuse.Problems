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

#include "fuseproblem.h"

//  _      _ _
// (_)_ _ (_) |_
// | | ' \| |  _|
// |_|_||_|_|\__|
void * fpr_init(struct fuse_conn_info *conn) {
  Problem<PROBLEM_PRECISION> *pr = new Problem<PROBLEM_PRECISION>(dl_path);
  return reinterpret_cast<void*>(pr);
}

//     _        _
//  __| |___ __| |_ _ _ ___ _  _
// / _` / -_|_-<  _| '_/ _ \ || |
// \__,_\___/__/\__|_| \___/\_, |
//                          |__/
void fpr_destroy(void * private_data) {
  delete reinterpret_cast< Problem<PROBLEM_PRECISION>* >(private_data);
}

//           _        _   _
//  __ _ ___| |_ __ _| |_| |_ _ _
// / _` / -_)  _/ _` |  _|  _| '_|
// \__, \___|\__\__,_|\__|\__|_|
// |___/
int fpr_getattr(const char *path, struct stat *stbuf) {
  Problem<PROBLEM_PRECISION> *pr = get_problem();
  int path_id = pr->pathid(path);

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
    stbuf->st_size = pr->filesize(FunctionOutput::xo);
    break;
  case FunctionOutput::po:
    stbuf->st_mode = S_IFREG | S_IWUSR | S_IWGRP | S_IRUSR | S_IRGRP;
    stbuf->st_nlink = 1;
    stbuf->st_size = pr->filesize(FunctionOutput::po);
    break;
  case FunctionOutput::info:
    stbuf->st_mode = S_IFREG | S_IRUSR | S_IRGRP;
    stbuf->st_nlink = 1;
    stbuf->st_size = pr->filesize(FunctionOutput::info);
    break;
  default:
    if (path_id >= 0 && path_id < pr->size(FunctionOutput::fo)) {
      stbuf->st_mode = S_IFREG | S_IRUSR | S_IRGRP;
      stbuf->st_nlink = 1;
      stbuf->st_size = pr->filesize(FunctionOutput::fo, path_id);
    }
    break;
  }
  return 0;
}

//                  _    _ _
//  _ _ ___ __ _ __| |__| (_)_ _
// | '_/ -_) _` / _` / _` | | '_|
// |_| \___\__,_\__,_\__,_|_|_|
int fpr_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
  Problem<PROBLEM_PRECISION> *pr = get_problem();
  int path_id = pr->pathid(path);

  if (path_id < FunctionOutput::root)
    return 0;

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);

  switch (path_id) {
  case FunctionOutput::root:
    filler(buf, "x", NULL, 0);
    filler(buf, "y", NULL, 0);
    for (int i = 0; i < pr->size(FunctionOutput::fo); i++) {
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
    if (path_id >= 0 && path_id < pr->size(FunctionOutput::fo)) {
      string s("/y");
      filler(buf, (s + to_string(path_id)).c_str(), NULL, 0);
    }
    break;
  }
  return 0;
}

//  ___ _ __  ___ _ _
// / _ \ '_ \/ -_) ' \
// \___/ .__/\___|_||_|
//     |_|
int fpr_open(const char *path, struct fuse_file_info *fi) {
  return 0;
}

//                  _
//  _ _ ___ __ _ __| |
// | '_/ -_) _` / _` |
// |_| \___\__,_\__,_|
int fpr_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
  return 0;
}
