#include <vector>
#include <cmath>
#include <cstddef>

#define PROBLEM_PRECISION double

#include "problemShared.hpp"
using namespace std;


FUNCTION_DEFINE(fA_test, {
  Y_PUSH(X(0) + P(0));
  Y_PUSH(X(1) + P(1));
  Y_PUSH(Y(0) + Y(1));
});

FUNCTION_DEFINE(fB_test, {
  Y_PUSH(X(0) - P(0));
  Y_PUSH(X(1) - P(1));
  Y_PUSH(Y(0) - Y(1));
});

FUNCTION_EXPORT(2, 2, 2, fA_test, fB_test);
