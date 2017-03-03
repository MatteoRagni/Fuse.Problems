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

typedef void *CProblem;

#ifndef PROBLEM_PRECISION
#define PROBLEM_PRECISION double
#endif

#ifdef __cplusplus

using namespace std;

#include "problemShared.hpp"
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
typedef enum FunctionOutput { root = -4, info, xo, po } FunctionOutput;
#define PROBLEM_E_NOFILE -100

// typedef void (*Function)(void*,void*,void*);

template <class FP> class Problem {

private:
  string dl_file;
  void *hdl;
  string x_file, p_file, info_file;
  vector<string> y_file;
  size_t x_s, p_s, f_s;

  vector<Function> f;

  map<string, int> paths;

  void update();
  void init();
  void close();

public:
  vector<FP> x;
  vector<FP> y;
  vector<FP> p;

  Problem(string dl) : dl_file(dl), x_file(""), p_file(""), info_file(""), x_s(0), p_s(0), f_s(0) {
    init();
  };
  virtual ~Problem() { close(); };

  /* Fuse interface */
  size_t write(FunctionInput type, const char *buf);
  size_t read(int type, char *buf, size_t size, size_t offset);
  int pathid(const char *path);

  /* Fuse Utils */
  size_t xsize() { return x_s; }
  size_t psize() { return p_s; }
  size_t fsize() { return f_s; }
  size_t xfilesize() { return x_file.size(); }
  size_t pfilesize() { return p_file.size(); }
  size_t ffilesize(size_t id) {
    if (id >= 0 && id < y_file.size())
      return y_file.size();
    else
      return 0;
  }


}; /* Problem */

/* Exception Log function */
#define ERROR_LOG(m) std::cout << "EXCEPTION RAISED: " << m << endl;

#endif /* __cplusplus */

/*   ___   ___     _            __
 *  / __|_|_ _|_ _| |_ ___ _ _ / _|__ _ __ ___
 * | (_|___| || ' \  _/ -_) '_|  _/ _` / _/ -_)
 *  \___| |___|_||_\__\___|_| |_| \__,_\__\___|
 * * * * * * * * * * * * * * * *  * * * * * * * */

#ifdef __cpluplus
extern "C" {
#endif /* __cplusplus */

CProblem CProblem_new(const char *dl);
void CProblem_destroy(CProblem self);
size_t CProblem_x_write(CProblem self, const char *buf);
size_t CProblem_p_write(CProblem self, const char *buf);
size_t CProblem_read(CProblem self, int index, char *buf, size_t size,
                     size_t offset);
int CProblem_pathid(CProblem self, const char *path);
size_t CProblem_xsize(CProblem self);
size_t CProblem_psize(CProblem self);
size_t CProblem_fsize(CProblem self);
size_t CProblem_xfilesize(CProblem self);
size_t CProblem_pfilesize(CProblem self);
size_t CProblem_ffilesize(CProblem self, size_t i);

#ifdef __cpluplus
}
#endif /* __cplusplus */

#endif /* _FUSE_PROBLEM_PROBLEM_HH_ */
