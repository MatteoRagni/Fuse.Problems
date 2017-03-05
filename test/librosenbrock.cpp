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
 * This is an example for an external library that uses the rosenbrock
 * function. It defines the function rosenbrock, the gradient and the hessian.
 * The rosenbrock function is:
 *
 *   f(x, y) = (a - x)^2 + b (y- x^2)^2
 *
 * that is of dimension 1. The gradient with respecto to x and y:
 *
 *  df[x](x, y) = 4 a x^3 - 4 b x y - 2 a + 2 x
 *  df[y](x, y) = 3 b (y - x^2)
 *
 * that is of dimension 2x1, and than the hessian:
 *
 *  ddf[xx] = 2 + (12 x - 4 y) b
 *  ddf[xy] = -4 b x
 *  ddf[yx] = ddf[xy]
 *  ddf[yy] = 2 b
 *
 * that is of dimension 2x2 and will be stored with row mayor order in
 * the vector in form {ddf[xx], ddf[xy], ddf[yx], ddf[yy]}.
 *
 * For the actual implementation:
 *  - x, y are written as X()[0], X()[1], and X() has dimension 2
 *  - a, b are written as P()[0], P()[1], and P() has dimension 2
 *  - all evaluations are stored in Y(), and all defined functions are 3.
 *
 * A function is defined using FUNCTION_DEFINE(function_name, { function body })
 * While the registration of the function to be exported is performed through
 * the macro FUNCTION_EXPORT(function_size, x_size, p_size, funcA, funcB, ...)
 * Optionally, the precision of the problem can be defined "before" including
 * the required header "problemShared" in the macro PROBLEM_PRECISION.
 *
 * The shared object can be compiled with:
 *
 *  g++ -I../include -fPIC -shared -lm librosenbrock.cpp -o librosenbrock.so
 *
 * The system can be mounted as:
 *
 *  fuse.problem -s /tmp/rosenbrock shared/librosenbrock.so
 */

#define PROBLEM_PRECISION double
#include "problemShared.hpp"

using namespace std;

FUNCTION_DEFINE(f_rosenbrock, {
  Y().clear();
  Y().push_back(
    pow(P()[0] - X()[0], 2.0) + P()[1] * pow(X()[1] - pow(X()[0], 2.0), 2.0)
  );
});

FUNCTION_DEFINE(df_rosenbrock, {
  Y().clear();
  Y().push_back(
    4.0 * P()[1] * pow(X()[0], 3.0) - 4.0 * P()[1] * X()[0] * X()[1] - 2.0 * P()[0] + 2.0 * X()[0]
  );
  Y().push_back(
    2.0 * P()[1] * (X()[1] - pow(X()[0], 2.0))
  );
});

FUNCTION_DEFINE(ddf_rosenbrock, {
  Y().clear();

  PROBLEM_PRECISION ddf01 = -4.0 * P()[1] * X()[0];

  Y().push_back(
    2.0 + (12.0 * pow(X()[0], 2.0) - 4.0 * X()[1]) * P()[1]
  );
  Y().push_back(ddf01);
  Y().push_back(ddf01);
  Y().push_back(
    2.0 * P()[1]
  );
});

FUNCTION_EXPORT(3, 2, 2, f_rosenbrock, df_rosenbrock, ddf_rosenbrock);
