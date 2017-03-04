#ifndef _FUSE_PROBLEM_DEFINE_HPP_
#define _FUSE_PROBLEM_DEFINE_HPP_

#include <vector>
#include <cmath>
#include <cstddef>

#ifndef PROBLEM_PRECISION
#define PROBLEM_PRECISION double
#endif

typedef void(*Function)(void*, void*, void*);

#define X(i) x->operator[](i)
#define Y(i) y->operator[](i)
#define P(i) p->operator[](i)
#define Y_PUSH(v) y->push_back(v)
#define Y_CLEAR() y->clear()

#define FUNCTION_DEFINE(name, content) \
extern "C" void name(void* _y, void *_x, void *_p) { \
  std::vector<PROBLEM_PRECISION> *y = reinterpret_cast< std::vector<PROBLEM_PRECISION>* >(_y); \
  std::vector<PROBLEM_PRECISION> *p = reinterpret_cast< std::vector<PROBLEM_PRECISION>* >(_p); \
  std::vector<PROBLEM_PRECISION> *x = reinterpret_cast< std::vector<PROBLEM_PRECISION>* >(_x); \
  content; \
}

#define FUNCTION_EXPORT(size_f, size_x, size_p, ...) \
extern "C" { \
 size_t FunctionSizeF() { return size_f; }; \
 size_t FunctionSizeX() { return size_x; }; \
 size_t FunctionSizeP() { return size_p; }; \
 Function FunctionArray[] = { __VA_ARGS__ }; \
}

#endif /* _FUSE_PROBLEM_DEFINE_HPP_ */
