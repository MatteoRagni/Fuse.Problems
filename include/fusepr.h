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

#ifndef _FUSEPR_H_
#define _FUSEPR_H_

#define FUSE_USE_VERSION 30
#define _XOPEN_SOURCE 500

#include <string>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "problem.h"

#include <stdio.h>
#define LOG(m) printf("====> FUSE_PROBLEM :: %s\n", m);

#define GET_PROBLEM() ((CProblem)(fuse_get_context()->private_data))
#include "fusepr_utils.h"

#ifdef __cpluplus
extern "C" {
#endif /* __cplusplus */

int FuseProblem_getattr(const char *path, struct stat *stbuf);
int FuseProblem_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                        off_t offset, struct fuse_file_info *fi);
int FuseProblem_open(const char *path, struct fuse_file_info *fi);
int FuseProblem_read(const char *path, char *buf, size_t size, off_t offset,
                     struct fuse_file_info *fi);
int FuseProblem_write(const char *path, const char *buf, size_t size,
                      off_t offset, struct fuse_file_info *fi);
int FuseProblem_truncate(const char *path, off_t size);

int FuseProblem_flush(const char *path, struct fuse_file_info *fi);
int FuseProblem_access(const char* path, int mask);
int FuseProblem_setxattr(const char *path, const char *name, const char *value,
                         size_t size, int flags);
int FuseProblem_getxattr(const char *path, const char *name, char *value,
                         size_t size);
int FuseProblem_listxattr(const char *path, const char *list, size_t size);

#ifdef __cpluplus
}
#endif /* __cplusplus */

#endif /* _FUSEPR_H_ */
