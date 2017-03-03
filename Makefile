# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# *                                                                           *
# * Copyright (c) 2017 - Matteo Ragni, University of Trento                   *
# *                                                                           *
# * Permission is hereby granted, free of charge, to any person               *
# * obtaining a copy of this software and associated documentation            *
# * files (the "Software"), to deal in the Software without                   *
# * restriction, including without limitation the rights to use,              *
# * copy, modify, merge, publish, distribute, sublicense, and/or sell         *
# * copies of the Software, and to permit persons to whom the                 *
# * Software is furnished to do so, subject to the following                  *
# * conditions:                                                               *
# *                                                                           *
# * The above copyright notice and this permission notice shall be            *
# * included in all copies or substantial portions of the Software.           *
# *                                                                           *
# * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,           *
# * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES           *
# * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                  *
# * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT               *
# * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,              *
# * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING              *
# * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR             *
# * OTHER DEALINGS IN THE SOFTWARE.                                           *
# *                                                                           *
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

GCC     = gcc -g
GXX     = g++ -std=c++11 -g
SRC     = main.c src/fusepr.c src/problem.c
OBJ     = fusepr.o problem.o utils.o
CFLAGS  = -I./include
LIBFUSE = `pkg-config fuse --cflags --libs`
LDFLAGS = -lm -ldl

all: problem
	$(GCC) $(CFLAGS) -c src/fusepr.c $(LDFLAGS) -o fusepr.o
	$(GCC) $(CFLAGS) $(OBJ) main.c $(LDFLAGS) -o fuse_problem

problem-test: problem
	$(GXX) $(CFLAGS) problem.o problem_test.c $(LDFLAGS) -o problem_test.o

shared-test:
	$(GXX) $(CFLAGS) test/libtest.cpp $(LDFLAGS) -fPIC -shared -o test/libtest.so

problem:
	$(GXX) -c src/problem.cpp $(CFLAGS) $(LDFLAGS) -o problem.o

clean:
	rm -f fuse_problem *.o *.so test/*.so
