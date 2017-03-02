#include <vector>
#include <cmath>
#include <cstddef>

#define PRECISION double

#include "problemShared.hpp"
using namespace std;


FUNCTION_DEFINE(fA_test, {
  y->push_back(X(0) + P(0));
  y->push_back(X(1) + P(1));
  y->push_back(Y(0) + Y(1));
});

FUNCTION_DEFINE(fB_test, {
  y->push_back(X(0) - P(0));
  y->push_back(X(1) - P(1));
  y->push_back(Y(0) - Y(1));
});

FUNCTION_EXPORT(2, fA_test, fB_test);
