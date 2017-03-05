#define PROBLEM_PRECISION double

#include "problemShared.hpp"

using namespace std;

FUNCTION_DEFINE(f_rosenbrock, {
  Y_CLEAR();

});

FUNCTION_DEFINE(df0_rosenbrock, {
  Y_CLEAR();

});

FUNCTION_DEFINE(df1_rosenbrock, {
  Y_CLEAR();

});

FUNCTION_DEFINE(ddf00_rosenbrock, {
  Y_CLEAR();

});

FUNCTION_DEFINE(ddf11_rosenbrock, {
  Y_CLEAR();

});

FUNCTION_DEFINE(ddf10_rosenbrock, {
  Y_CLEAR();

});





FUNCTION_EXPORT(2, 2, 2, fA_test, fB_test);
