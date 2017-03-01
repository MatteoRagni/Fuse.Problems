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

//  ___      _ _   _      _ _
// |_ _|_ _ (_) |_(_)__ _| (_)______
//  | || ' \| |  _| / _` | | |_ / -_)
// |___|_||_|_|\__|_\__,_|_|_/__\___|
Problem* Problem_init(size_t xSize, size_t ySize, size_t pSize) {
  Problem *self = (Problem *)malloc(sizeof(Problem));
  if (!self)
    return NULL;
  self->xSize = xSize;
  self->ySize = ySize;
  self->pSize = pSize;
  self->y_buf_size = 1;

  self->x = (double *)calloc(self->xSize, sizeof(double));
  if (!self->x) {
    Problem_destroy(self);
    return NULL;
  }

  self->y = (double *)calloc(self->ySize, sizeof(double));
  if (!self->y) {
    Problem_destroy(self);
    return NULL;
  }

  self->p = (double *)calloc(self->xSize, sizeof(double));
  if (!self->p) {
    Problem_destroy(self);
    return NULL;
  }

  self->y_buf = (char *)calloc(self->y_buf_size, sizeof(char));
  if(!self->y_buf) {
    Problem_destroy(self);
    return NULL;
  }
  // Problem_update(self);
  return self;
}

//  ___         _
// |   \ ___ __| |_ _ _ ___ _  _
// | |) / -_|_-<  _| '_/ _ \ || |
// |___/\___/__/\__|_| \___/\_, |
//                          |__/
void Problem_destroy(Problem *self) {
  if (self->x)
    free(self->x);
  if (self->y)
    free(self->y);
  if(self->y_buf) {
    free(self->y_buf);
    self->y_buf_size = 0;
  }
  if (self->p)
    free(self->p);
  free(self);
}

// __      __   _ _
// \ \    / / _(_) |_ ___ ___
//  \ \/\/ / '_| |  _/ -_|_-<
//   \_/\_/|_| |_|\__\___/__/
int Problem_write__private(Problem *self, const char *input, double *v,
                           size_t size) {
  size_t read;
  read = ProblemUtil_parser(input, v, size);

  if (read == size) {
    Problem_update(self);
    return size;
  }
  return read;
}


int Problem_write_x(Problem *self, const char *input) {
  return Problem_write__private(self, input, self->x, self->xSize);
}

int Problem_write_p(Problem *self, const char *input) {
  return Problem_write__private(self, input, self->p, self->pSize);
}

//  _   _          _      _
// | | | |_ __  __| |__ _| |_ ___
// | |_| | '_ \/ _` / _` |  _/ -_)
//  \___/| .__/\__,_\__,_|\__\___|
//       |_|
int Problem_update(Problem *self) {
  // Evaluates the function
  self->f(self);
  // Update String Representation
  self->y_buf_size = ProblemUtil_generator(self->y_buf, self->y, self->ySize);
  return self->y_buf_size;
}

//  ___             _
// | _ \___ __ _ __| |
// |   / -_) _` / _` |
// |_|_\___\__,_\__,_|
int Problem_read(Problem *self, char *buf, size_t size, size_t offset) {
  size_t output_size;
  output_size = strlen(self->y_buf);
  if (offset > output_size)
    return 0;

  if ((offset + size) > output_size) {
    memcpy(buf, self->y_buf + offset, output_size - offset);
    return output_size - offset;
  }

  memcpy(buf, (self->y_buf + offset), size);
  return size;
  // end copy
}
