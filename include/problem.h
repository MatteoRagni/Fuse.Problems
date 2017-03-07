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

/**
 * @file problem.h
 * @brief The Problem container class
 *
 * This file describes the class that contains the problem
 */
#ifndef _FUSE_PROBLEM_PROBLEM_HH_
#define _FUSE_PROBLEM_PROBLEM_HH_

#ifndef PROBLEM_PRECISION
/**
 * @def PROBLEM_PRECISION this keyword determines the precision of the project
 * since the internal class is a templated class with respect to different
 * float or double. Please notice that if not defined it will automatically
 * defined as double. Also, the external libraries should be compiled with
 * the same precision.
 */
#define PROBLEM_PRECISION double
#endif

using namespace std;

#include <cmath>
#include <cstddef>
#include <dlfcn.h>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
/**
 * @brief Enumeration for the writing filesystem
 *
 * The enum FunctionInput is used to enumerate the elements in input, that are,
 * as for now, only the input file and the parameter file. The starting value
 * of this enum is changed in such a way it reflects the ones in FunctionOutput
 */
typedef enum FunctionInput {
  xi = -2, /**< Value for the input of variables */
  pi /**< Value for the input of parameters */
} FunctionInput;
/**
 * @brief Enumeration for the reading filesystem
 *
 * The enum FunctionOutput is used to enumerate the elements in output files.
 * The starting value is a negative number that intercepts the root of the
 * filesystem. Any value below root is consider as not part of the filesystem,
 * while a number above or equal than 0 is considered as part of the output of
 * an evaluated function.
 */
typedef enum FunctionOutput {
  root = -4, /**< value for the root of the filesystem "/" */
  info, /**< value for the info file that contains some problem info */
  xo, /**< value for reading the last input values */
  po, /**< value for reading the last parameter values */
  fo /**< value for reading an output function value */
} FunctionOutput;
/**
 * @def PROBLEM_E_NOFILE this macro define the value returned by
 * Problem<FP>::ispath method when requested file is not part of filesystem.
 */
#define PROBLEM_E_NOFILE -100

/**
 * @brief Problem Function pointer definition
 *
 * This is a definition of the function pointer that are inluded in the external
 * shared object that is loaded by the filesystem at initialization.
 */
typedef void (*Function)(void *, void *, void *);

/**
 * @brief The problem container class that abstract the device of filesystem
 *
 * This is a container of the problem. It handles loading and unloading of the
 * shared object and also parses filesystem input. It contains string representation
 * for the files and pointer to externally declared functions. The problem
 * precision is unader the template class FP, and it can be float or double.
 * Usually the real problem precision is described in PROBLEM_PRECISION macro.
 */
template <class FP> class Problem {

private:
  string dl_file; /**< Path of the shared object loaded dinamically */
  void *hdl; /**< Pointer to the handle returned by dlopen */

  string x_file; /**< container string for last variable input parsed */
  string p_file; /**< container string for last variable parameter parsed */
  string info_file; /**< container string for information file */
  vector<string> y_file; /**< vector of container string for the last evaluation */
  map<string, int> paths; /**< path to internal representation (enums) mapping */

  size_t x_s; /**< declared dimension for input x */
  size_t p_s; /**< declared dimension for parameter p */
  size_t f_s; /**< number of function declared in shared object */

  vector<Function> f; /**< vector of function pointer */

public:
  vector<FP> x; /**< last values of input x  */
  vector<FP> p; /**< last values of parameters p */
  vector<FP> y; /**< last value of last evaluated function (it is garbage) */

private:
  /**
   * @brief Loads shared object and setup the dimensions
   *
   * The shared object is loaded when this function is called. If files does
   * not exist it aborts. If it cannot load the four required symbol:
   *  - FunctionSizeX: dimension of x array
   *  - FunctionSizeP: dimension of p array
   *  - FunctionSizeF: number of function defined
   *  - FunctionArray: array of function pointer defined in shared object
   * It also initialize arrays and info file.
   */
  void init();
  /**
  * @brief evaluates function and performs conversion to string for y
  *
  * The update function actually calls the shared object external functions
  * and saves temporaneally the values in y vector. It generates the string
  * representation on the fly and returns.
  */
  void update();
  /**
  * @brief closes the external shared object
  *
  * It closes the external shared object, deleting the handle hdl, with dlclose
  */
  void close();

public:
  /**
   * @brief Initialization, calls init method
   *
   * The initialization function calls the init method and set some of
   * the internal properties to initial values.
   * @param dl path to external shared object
   * @return Problem<FP> new instance
   */
  Problem(const char *dl)
      : dl_file(dl), x_file(""), p_file(""), info_file(""), x_s(0), p_s(0),
        f_s(0) {
    init();
  };
  /**
   * @brief Destroyes the Problem<FP> class
   *
   * Calls close method.
   */
  ~Problem() { close(); };
  /**
   * @brief representation of the libfuse write operation
   *
   * It is the equivalent of the write operation from fuse. Already handles
   * size, thus it is not in parameter list. Once called, it also calls update
   * method to regenerate all internal representations.
   * TODO: must be simplified and broken in smaller pieces.
   * @param type FunctionInput enum value to understand what is being written
   * @param buf the buffer that is being written.
   * @return the number of variable updated.
   */
  size_t write(FunctionInput type, const char *buf);
  /**
   * @brief representation of the libfuse read operation
   *
   * It is the equivalent of the read operation from fuse. The first int value
   * is the file identification from pathid, and the content of the file is inserted into
   * the buffer buf, starting from the offset point, till reaching the size. Obviously,
   * file size is truncated when requested segment is bigger than content. The function
   * returns an integer, that is the number of bytes written into buf. This int format is required
   * libfuse. That means we set a direct I/O filesystem type (see fpr_read function)
   * @param type file that is currently required
   * @param buf the buffer in which informations will be copied
   * @param size the number of byte maximum for buf
   * @param offset the offset in byte since last reading
   * @return number of byte written in buf
   */
  int read(int type, char *buf, size_t size, size_t offset);
  /**
   * @brief gives a number that identifies the file in path
   *
   * Returns a number that identifies the file in path. It returns a PROBLEM_E_NOFILE when
   * the file does not exist. If number is negative, it is one of the fixed files, while if
   * it is positive is one of the output files.
   * @param path the required file path
   * @return an integer that identifies the file (w.r.t FunctionOutput)
   */
  int pathid(const char *path);
  /**
   * @brief Returns the size (dimensions) of specified element
   *
   * Returns the number of dimensions for vector x, vector p and number of
   * functions in f.
   * @param t type of vector size to query
   * @return a size_t value
   */
  size_t size(FunctionOutput t);
  /**
   * @brief number of characters in file
   *
   * Number of characters in specified file. The second argument is used to
   * specify the number of output file.
   * @param t type of file to query
   * @param i element number in y_file vector, default to 0 when not needed
   */
  size_t filesize(FunctionOutput t, int i = 0);
}; /* Problem */

#endif /* _FUSE_PROBLEM_PROBLEM_HH_ */
