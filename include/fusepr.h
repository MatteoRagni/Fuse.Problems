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

#ifndef _FUSER_H_
#define _FUSER_H_

#define FUSE_USE_VERSION 26
#define _XOPEN_SOURCE 500

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fuse.h>
#include <string.h>
#include "problem.h"

#define GET_PROBLEM() ((Problem *)(fuse_get_context()->private_data))

// Fuse configuration
static const char *FuseProblem_x_path = "/x";
static const char *FuseProblem_y_path = "/y";
static const char *FuseProblem_p_path = "/p";

static int FuseProblem_getattr(const char *path, struct stat *stbuf);
static int FuseProblem_readdir(const char *path, void *buf,
                               fuse_fill_dir_t filler, off_t offset,
                               struct fuse_file_info *fi);
static int FuseProblem_open(const char *path, struct fuse_file_info *fi);
static int FuseProblem_read(const char *path, char *buf, size_t size,
                            off_t offset, struct fuse_file_info *fi);
static int FuseProblem_write(const char *path, const char *buf, size_t size,
                             off_t offset, struct fuse_file_info *fi);

#endif /* _FUSER_H_ */
