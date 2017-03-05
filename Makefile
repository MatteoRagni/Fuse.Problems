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

GCC     = gcc -g -Wall -Wno-comment
GXX     = g++ -std=c++11 -g -Wall -Wno-comment
CFLAGS  = -I./include
CFUSE   = `pkg-config --cflags fuse`
LFUSE   = `pkg-config fuse --libs`
LDFLAGS = $(LFUSE) -lm -ldl

all: fuse.problem
	$(GXX) $(LDFLAGS) -o fuse.problem problem.o fuse.problem.o

fuse.problem: problem
	$(GCC) -c main.c $(CFLAGS) $(CFUSE) -o fuse.problem.o

problem:
	$(GXX) -c src/problem.cpp $(CFLAGS) -o problem.o

rosenbrock:
	$(GXX) $(CFLAGS) shared/librosenbrock.cpp $(LDFLAGS) -fPIC -shared -o shared/librosenbrock.so

clean:
	rm -f fuse.problem *.o *.so test/*.so
