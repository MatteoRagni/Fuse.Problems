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
 * @file problem.h
 * @author Matteo Ragni
 * @date 28 Feb 2017
 * @brief The container for the problem, selected through options
 *
 * This file implements a simple container for the problem.
 * In future it should be written better.
 */

#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define PROBLEM_WRITE_FORMAT "%lf"
#define PROBLEM_READ_FORMAT "%lf\n"

/**
 * @brief Problem container.
 *
 * The problem container contains the last reading for the input values
 * Problem#x,
 * the last output values Problem#y and the parameters value Problem#p.
 */
typedef struct Problem {
  double *x; /**< Placeholder for x values */
  double *p; /**< Placeholder for output values */
  double *y; /**< Placeholder for parameters */
  // double *dy;
  // double *ddy; // I think it is clear the game here...

  char* y_buf;       /**< output buffer for y */
  size_t y_buf_size; /**< output Problem#y_buf buffer size */

  size_t xSize; /**< input placeholder size, it will be used in reading */
  size_t ySize; /**< output placeholder size, it will be used for writing */
  size_t pSize; /**< parameters placeholder size, it will be used for reading
                   parameters */

  int (*f)(struct Problem *); /**< callback for evaluating the function */
  // int (*df)(Problem *);
  // int (*ddf)(Problem *); // ... and also here...
} Problem;

/**
 * @brief New problem initializer
 *
 * Defines a new problem, with the sizes of x, y and p defined. As for now
 * this is more than enough for our tests. If it works, we can use edit it later
 * @param xSize Size for the Problem#x vector
 * @param ySize Size for the Problem#y vector
 * @param zSize Size for the Problem#z vector
 * @return A new instance of Problem
 */
Problem *Problem_init(size_t xSize, size_t ySize, size_t pSize);
/**
 * @brief Problem destroyer
 *
 * Delete an instanced problem, freeing the problem vectors and then
 * freeing the struct. It also deletes the content of the struct if exists
 * @param self the problem to be destroyed
 * @return It returns nothing
 */
void Problem_destroy(Problem *self);

/**
 * @brief private function for updating. Do NOT use it
 *
 * Calls internal function for updating all the values. This function is
 * called when a user perform a writing in the input file.
 * @param self the Problem to be updated
 * @param input the input string from the filesystem
 * @param v vector to update
 * @param size size of the vector. it must be consistent
 * @return Return zero on suceed
 */
int Problem_write__private(Problem *self, const char *input, double *v,
                           size_t size);
/**
 * @brief updates values calling the internal function
 *
 * Calls internal function for updating all the values. This function is
 * called when a user perform a writing in the input file.
 * @param self the Problem to be updated
 * @param input the input string from the filesystem
 * @return Return zero on suceed
 */
int Problem_write_x(Problem *self, const char *input);
/**
 * @brief updates parameters and calls internal function
 *
 * Calls internal function for updating all the values. This function is
 * called when a user perform a writing in the parameters file.
 * @param self the Problem to be updated
 * @param input the input string from the filesystem
 * @return Return zero on suceed
 */
int Problem_write_p(Problem *self, const char *input);
/**
 * @brief return the current representation of the data to be written, with
 * offset
 *
 * Returns the current representation for the output, writing in a buffer that
 * gets in input, storing maximum a prescribed size and a defined offset. This
 * function actually mimic the interface of fuse, with the same arguments. As
 * for now,
 * at each call we make the conversion, but we can also store the conversion in
 * some form in
 * the problem structure and read only the required part
 * @param self the current Problem
 * @param buf the ouput vector of char representation
 * @param size the number of byte to write in buf
 * @param offset the offset for starting the writing
 * @param return byte written to buf
 * @return written size
 */
int Problem_read(Problem *self, char *buf, size_t size, size_t offset);
/**
 * @brief Create a string representation for the data
 *
 * This functions writes only the string representation into the
 * problem struct thus we already know the length of the evaluated string
 * @param self Problem current instance
 * @return buffer size that is saved in Problem#y_buf_size
 */
 int Problem_read_buf(Problem *self);
#endif /* _PROBLEM_H_ */
