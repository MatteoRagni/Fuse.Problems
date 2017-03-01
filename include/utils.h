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
#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/**
 * @brief Maximum string alocation size
 *
 * This define is internally used for the creation of the output string
 */
#define PROBLEM_UTIL_MAX_STRING_ALLOC 1024
#define PROBLEM_UTIL_GENERATOR_FORMAT "%.10E\n"
#define PROBLEM_UTIL_INPUT_SEPARATOR "\n"
#define PROBLEM_UTIL_PARSER_FORMAT "%lf"
/**
 * @brief Double array to string
 *
 * This function performs a conversion of the v array into a string
 * in which each number is a new line in the exported buffer. The
 * return value is the size of the buffer.
 * Buffer is internally reallocated with malloc and should be
 * freed later.
 * At least buffer must be initialize once.
 * @param buf the buffer string
 * @param v the double array
 * @param v_size the double array size
 * @return the size of the array
 */
size_t ProblemUtil_generator(char * buf, const double * v, size_t v_size);

/**
 * @brief parser for simple vector in files
 *
 * This function is a simple parser for the input string. It should be
 * series of number separated by a new line. The parsing is performed
 * through a sscanf standard function.
 * @param buf is the input buffer
 * @param v vector of double to be written
 * @param v_size length of the vector to be saved
 * @return a value with number of elements update
 */
size_t ProblemUtil_parser(const char * buf, double * v, size_t v_size);

#endif /* _UTILS_H_ */
