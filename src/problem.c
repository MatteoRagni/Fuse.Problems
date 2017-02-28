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

#include "problem.h"
#include <string.h>

Problem *Problem_init(size_t xSize, size_t ySize, size_t pSize) {
  Problem *self = (Problem *)malloc(sizeof(Problem));
  if (!ret)
    return NULL;
  self->xSize = xSize;
  self->ySize = ySize;
  self->pSize = pSize;

  self->x = (double *)calloc(sizeof(double) * self->xSize);
  if (!self->x) {
    testProblem_destroy(self);
    return NULL;
  }

  self->y = (double *)calloc(sizeof(double) * self->ySize);
  if (!self->y) {
    testProblem_destroy(self);
    return NULL;
  }

  self->p = (double *)calloc(sizeof(double) * self->pSize);
  if (!self->p) {
    testProblem_destroy(self);
    return NULL;
  }

  return ret;
}

void testProblem_destroy(testProblem *self) {
  if (self->x)
    free(self->x);
  if (self->y)
    free(self->y);
    if(self->y_buf) free(self-y->buf);
  if (self->p)
    free(self->p);
  free(self);
}

int Problem_write__private(Problem *self, const char *input, double *v,
                           size_t size) {
  size_t idx;
  char *input_r = strdup(input);
  char *token;
  char *saveptr;

  idx = 0;
  token = strtok_r(input_r, "\n", saveptr);
  while (token != NULL) {
    if (sscanf(PROBLEM_WRITE_FORMAT, &(v[idx])) != 1) {
      free(input_r);
      return 1;
    }
    idx += 1;
    if (idx >= size)
      break;
    token = strtok_r(NULL, "\n", saveptr);
  }

  if (idx == size) {
    self->f(self);
  }

  free(input_r);
  return 0;
}

int Problem_read_buf(Problem *self) {
  char *token;
  char *output;
  size_t output_size int idx;

  output_size = 0;

  for (size_t idx; idx < self->ySize; idx++) {
    sprintf(token, PROBLEM_READ_FORMAT, self->y[idx]);
    output_size += strlen(token) realloc(output, output_size);
    strcat(output, token);
  }
  free(token);

  if (self->y_buf)
  free(self->y_buf);
  
}

int Problem_read(Problem *self, char *buf, char *size, char *offset) {


  // Copying inside buf
  // size_t output_size = strlen(self->y_buf);
  if (offset > output_size)
    return 0;

  if (offset + size > output_size) {
    memcpy(buf, output + offset, output_size - offset);
    return output_size - offset;
  }

  memcpy(buf, output + offset, size);
  return size;
  // end copy
}
