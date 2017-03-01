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

#include "utils.h"

size_t ProblemUtil_generator(char *buf, const double *v, size_t v_size) {
  char token[PROBLEM_UTIL_MAX_STRING_ALLOC];
  size_t ret;
  size_t idx;

  ret = 1;

  // Wrinting and concatenation of string with specified format
  for (idx = 0; idx < v_size; idx++) {
    snprintf(token, PROBLEM_UTIL_MAX_STRING_ALLOC,
             PROBLEM_UTIL_GENERATOR_FORMAT, v[idx]);
    ret += strlen(token);
    buf = (char *)realloc((void *)buf, ret);
    buf = strncat(buf, token, ret);
  }

  return ret;
}


size_t ProblemUtil_parser(const char *buf, double *v, size_t v_size) {
  size_t idx;
  char * input_copy;
  char * token;
  double read;

  idx = 0;
  input_copy = strdup(buf);

  while ((token = strsep(&input_copy, PROBLEM_UTIL_INPUT_SEPARATOR)) != NULL) {
    if (sscanf(token, PROBLEM_UTIL_PARSER_FORMAT, &read) != 1) {
      return idx;
    }
    v[idx] = read;
    if (++idx >= v_size)
      break;
  }

  return idx;
}


/*
#include <stdio.h>
int main() {
  double x[] = {1.0, 2.0, 3.0, 4.0};
  double y[] = {0, 0, 0, 0};
  size_t size = 4;
  size_t ret = 0;
  char *buf = NULL;
  buf = (char *)malloc(sizeof(char));

  ret = ProblemUtil_generator(buf, x, size);
  printf("%s", buf);

  printf("Length = %d\nReading\n", ret);

  ret = ProblemUtil_parser(buf, y, 4);

  printf("%f\n%f\n%f\n%f\n", y[0], y[1], y[2], y[3]);

  free(buf);

  return 0;
}
*/