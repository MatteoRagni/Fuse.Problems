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

#define FUSE_USE_VERSION 30
#define _XOPEN_SOURCE 500

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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This definition queries the fuse context to get the private data.
 * Once is obtained reinterpret it to a Problem pointer of the initially
 * created instance
 */
#define get_problem() reinterpret_cast< Problem<PROBLEM_PRECISION>* >(fuse_get_context()->private_data)

extern char * dl_path; /**< external global variable that contains the shared lib path */


/**
 * @brief initializer for the fuse.problem filesystem
 *
 * This function creates a new pointer for the fuse problem that will be saved in the
 * context of the fuse library
 * @param conn struct with some informations on the filesystem configuration (unused)
 * @return a void pointer that is a reinterpret cast from Problem class
 */
void * fpr_init(struct fuse_conn_info *conn);
/**
 * @brief destroyer for the fuse.problem filesystem.
 *
 * The function takes as input the content of the context, that is as void pointer.
 * With a reinterpret cast the class instance is reconstructed and deleted.
 * @param private_data the pointer void to our instance
 * @return nothing
 */
void fpr_destroy(void * private_data);
/**
 * @brief getattr callback for fuse.problem
 *
 * The getattr callback should fill a stat struct that contains the
 * times for the different files and access attributes.
 * @param path the path of the inode that is stat
 * @param stbuf the struct stat that must be filled
 * @return a 0 int when succeed, and a -ENOENT on error (path unknown)
 */
int fpr_getattr(const char *path, struct stat *stbuf);
/**
* @brief
*
* @param path
* @param buf
* @param filler
* @param offset
* @param fi
* @return
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

#ifdef __cplusplus
}
#endif

#endif /* _FUSE_PROBLEM_FUSE_HH_ */
