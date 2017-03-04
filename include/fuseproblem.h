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

#ifndef _FUSE_PROBLEM_FUSE_HH_
#define _FUSE_PROBLEM_FUSE_HH_

#define _FILE_OFFSET_BITS 64
#define FUSE_USE_VERSION 30
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void * problem;  /**< external global variable that contains the problem lib path */

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief initializer for the fuse.problem filesystem
 *
 * This function creates a new pointer for the fuse problem that will be used
 * throughout the library
 * @param path path of the shared library that contains the shared object
 */
void fpr_init(const char * path);
/**
 * @brief destroyer for the fuse.problem filesystem.
 *
 * Destroys the fuse Problem class
 */
void fpr_destroy();
/**
 * @brief getattr callback for fuse.problem
 *
 * The getattr callback should fill a stat struct that contains the
 * times for the different files and access attributes. This is a
 * callback.
 * @param path the path of the inode that is stat
 * @param stbuf the struct stat that must be filled
 * @return a 0 int when succeed, and a -ENOENT on error (path unknown)
 */
int fpr_getattr(const char *path, struct stat *stbuf);
/**
* @brief fill the stat struct for the filesystem
*
* Fills recursively the stat struct that contains the access informations
* and the contained file system. This is a callback.
* @param path the current file path
* @param buf an internal buffer required by the callback
* @param filler
* @param offset
* @param fi
* @return a 0 on success, -ENOENT il not found
 */
int fpr_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi);
/**
 * @brief
 *
 * @param path
 * @param fi
 * @return
 */
int fpr_open(const char *path, struct fuse_file_info *fi);
/**
 * @brief
 *
 * @param path
 * @param buf
 * @param size
 * @param offset
 * @param fi
 * @return
 */
int fpr_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi);

int fpr_write(const char *path, const char *buf, size_t size,
                      off_t offset, struct fuse_file_info *fi);

int fpr_truncate(const char *path, off_t size);

int fpr_flush(const char *path, struct fuse_file_info *fi);

int fpr_access(const char *path, int mask);

int fpr_setxattr(const char *path, const char *name, const char *value,
                         size_t size, int flags);

int fpr_listxattr(const char *path, char *list, size_t size);

int fpr_getxattr(const char *path, const char *name, char *value,
                         size_t size);

#ifdef __cplusplus
}
#endif

#endif /* _FUSE_PROBLEM_FUSE_HH_ */
