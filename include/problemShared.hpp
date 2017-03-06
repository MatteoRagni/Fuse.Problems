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
 * @file problemShared.hpp
 * @brief This is the only header that is necessary to write shared object
 *
 * This is the only header that user should include to work with problem.fuse and
 * write problem shared object.
 *
 * Please look at librosenbrock.cpp to see how to use it
 */

#ifndef _FUSE_PROBLEM_DEFINE_HPP_
#define _FUSE_PROBLEM_DEFINE_HPP_

#include <vector>
#include <cmath>
#include <cstddef>

#ifndef PROBLEM_PRECISION
/**
 * @def PROBLEM_PRECISION this is teh precision used for the
 * project. For external library, the PROBLEM_PRECISION must be
 * the same as compiled filesystem.
 */
#define PROBLEM_PRECISION double
#endif
/**
 * Typedefinition of the pointer function. Only for internal
 * usage
 */
typedef void(*Function)(void*, void*, void*);
/**
 * @def X() Access x vector (input)
 * @def Y() Access y vector (output)
 * @def P() Access p vector (parameters)
 */
#define X() (*xPtr)
#define Y() (*yPtr)
#define P() (*pPtr)
/**
 * @def FUNCTION_DEFINE(name, content) this definition allows to define
 * a new function that can be added inside the shared object and used
 * in fuse.problem. The name argument allows to define a name for the function
 * while the content is the content of the function.
 */
#define FUNCTION_DEFINE(name, content) \
extern "C" void name(void* _y, void *_x, void *_p) { \
  std::vector<PROBLEM_PRECISION> *yPtr = reinterpret_cast< std::vector<PROBLEM_PRECISION>* >(_y); \
  std::vector<PROBLEM_PRECISION> *pPtr = reinterpret_cast< std::vector<PROBLEM_PRECISION>* >(_p); \
  std::vector<PROBLEM_PRECISION> *xPtr = reinterpret_cast< std::vector<PROBLEM_PRECISION>* >(_x); \
  content; \
}
/**
 * @def FUNCTION_EXPORT(size_f, size_x, size_p, ...) is the macro that allows
 * to export the symbols. The first argument is the number of function
 * that will be defined, size_x is the number of unknown in each function
 * and size_p is the dimension of the parameter vector. All the other
 * arguments is a variable list of function that will be exported. The number
 * of function must be the same of f_size.
 */
#define FUNCTION_EXPORT(size_f, size_x, size_p, ...) \
extern "C" { \
 size_t FunctionSizeF() { return size_f; }; \
 size_t FunctionSizeX() { return size_x; }; \
 size_t FunctionSizeP() { return size_p; }; \
 Function FunctionArray[] = { __VA_ARGS__ }; \
}

#endif /* _FUSE_PROBLEM_DEFINE_HPP_ */
