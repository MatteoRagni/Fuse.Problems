# Fuse.Problem

## Introduction

Fuse problem is a file system for numerical problem. A numerical problem is a collection of function that require, to be evaluated, a vector of input and a vector of parameters. The result of the evaluation is stored inside files that can be easily read by all programming languages. This is in fact the main reason why this file system was written: **provide a unique way to test numerical algorithms that can be written in different languages**. This comes to a cost: accessing file system is **extremely slow** with respect to a numerical problem that can be directly accessed by the target language of the numerical algorithm in test, but on the other side it gains the advantages of:

 - creating a test suite that does not need a wrapper in a specific target language
 - creating a test suite that has an evaluation time that is almost independent with respect to different languages, thus a timed test of two algorithms will reflect algorithm time + parsing time + evaluation time, where the last two will be almost identical.

## File system description

The file system can be mounted in a mount point and exposes a series of file that can be written and read by means of normal read/write os operations. The file that are always exposed are:

 - `mountpoint/info`: is a yaml-ish text file that contains some information about the problem that is currently loaded. It is read only.
 - `mountpoint/x`: is the file for the input of variables. It can be read and written. The format for the number is the common float/engineering format, single dimensions are separated by a newline character, and matrix input are suggested to be stored in a row-major format
 - `mountpoint/p`: is the file for the input of parameters. Follows the same rules of `mountpoint/x`

There are other files that contains results of evaluation. The files have name starting with `mountpoint/y` plus a number that is the number of the function evaluated. Functions are declared in shared object file, that are loaded with `dlopen` when file system is mounted, and unloaded when it is unmounted. **It is not possible to change the shared object used, but it is possible to mount several file system, at the same time, by providing different mount point**.

## Shared object description

A shared object implementation sample is contained in `shared/librosenbrock.cpp`. In that file, three functions, that require two variables and two parameters, are defined and added to the list of exported object.

### Shared object implementation

Function definition is performed through the macro:
```c++
FUNCTION_DEFINE( function_name, {
  // function content
});
```
and the function defined are:
 - `f_rosenbrock`: the actual rosenbrock function
 - `df_rosenbrock`: the gradient of the rosenbrock function
 - `ddf_rosenbrock`: the hessian of the rosenbrock function stored in a row-major format
The user defines the problem using the exporter macro that takes three dimensions as input and the list of functions to be exported as remaining arguments:
```c++
FUNCTION_EXPORT(
  number_of_function_to_export,
  dimension_of_the_input_vector,
  dimension_of_the_output_vector,
  comma, seprated list, of, function, pointer);
```
Please notice that the number of function to export must be equal to the number of function pointer provided. Failing this generates an abort on execution, before mounting the filesystem.

### Shared object compilation

The only header required by shared object is [`problemShared.hpp`](http://ragni.me/Fuse.Problems/problemShared_8hpp.html) that contains all necessary macros. It can be compiled with:
```sh
fuse_problems_path=# path to Fuse.Problems source code
shared_source=# name given to the problem that must be compiled

g++ -I$(fuse_problems_path)/include \
  -fPIC -shared -lm \
  lib$(shared_source).cpp -o lib$(shared_source).so
```

## Using filesystem

### Compile

The filesystem can be directly downloaded as a clone of the repository:
```sh
git clone https://github.com/MatteoRagni/Fuse.Problems
cd Fuse.Problems
make
```
It should generate a binary called `fuse.problem`. Towork it requires a C and C++ compiler and linker, and the `libfuse` library installed and configured for `pkg-config` (I think all linux distribution provides this minimal requirements). Even if it is possible to run this code on OSX, it **has not been tested yet**. Windows is not and will not be supported, since `libfuse` for windows does not exist. However, it is possible to run the code in a minimalistic VM with linux, and expose the Fuse.Problems as samba share.

### Mount and umount

To mount the filesystem
```sh
mkdir /tmp/mount
./fuse.problem -s /tmp/mount lib$(shared_source).so
```

The `-s` option is for single-thread filesystem. This will force filesystem operations as blocking. Looking inside  `/tmp/mount`, user will find the discussed files.

To unmount the filesystem:
```
fusermount -u /tmp/mount
```
File system **must** always be unmounted, or it will continue to run in background.

## Example

Let's run `librosenbrock` to test usage, with a ruby script that reads and writes files. The script implements a Newton method on the Rosenbrock function.

The necessary file system is loaded as:
```sh
make all
make rosenbrock

./fuse.problem -s /tmp/mount shared/librosenbrock.so
```

Then `rosenbrock.rb` can be run as test problem. The core elements to interface file system with Ruby language are the three functions:
```ruby
def write(x)
  File.open($x, "w") { |xf| xf.puts x.map(&:to_s).join("\n") }
end
```
for writing a new value (it also triggers a re-evaluation),
```ruby
def write_p(p)
  File.open($p, "w") { |pf|  pf.puts p.map(&:to_s).join("\n") }
end
```
for writing a parameter, and
```ruby
def read()
  return [
    File.open($df, "r").read.chomp.split("\n").map(&:to_f),
    File.open($ddf, "r").read.chomp.split("\n").map(&:to_f)
  ]
end

# [...]

File.open($f, "r").read.chomp
```
for reading gradient, hessian (function `read`) and reading the function value (the last line).
