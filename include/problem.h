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
#ifndef _FUSE_PROBLEM_PROBLEM_HH_
#define _FUSE_PROBLEM_PROBLEM_HH_

#ifndef PROBLEM_PRECISION
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

typedef enum FunctionInput { xi = -2, pi } FunctionInput;
typedef enum FunctionOutput { root = -4, info, xo, po, fo } FunctionOutput;
#define PROBLEM_E_NOFILE -100

typedef void (*Function)(void *, void *, void *);

template <class FP> class Problem {

private:
  string dl_file;
  void *hdl;
  string x_file, p_file, info_file;
  vector<string> y_file;
  size_t x_s, p_s, f_s;
  vector<Function> f;
  map<string, int> paths;

public:
  vector<FP> x;
  vector<FP> y;
  vector<FP> p;

private:
  void update();
  void init();
  void close();

public:
  Problem(const char *dl)
      : dl_file(dl), x_file(""), p_file(""), info_file(""), x_s(0), p_s(0),
        f_s(0) {
    init();
  };
  ~Problem() { close(); };

  /* Fuse interface */
  size_t write(FunctionInput type, const char *buf);
  int read(int type, char *buf, size_t size, size_t offset);
  int pathid(const char *path);

  /* Fuse Utils */
  size_t size(FunctionOutput t);
  size_t filesize(FunctionOutput t, int i = 0);

}; /* Problem */

#endif /* _FUSE_PROBLEM_PROBLEM_HH_ */
