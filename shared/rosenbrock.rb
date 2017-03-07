#!/usr/bin/env ruby

# Please notice that this script is strictly
# problem dependend, since we do not check size and
# dimensions and we already know that f returns only a
# value, while gradient returns a vector.

$mount = "/tmp/mount/"  # base mountpoint
$x     = $mount + "x"   # path for input
$p     = $mount + "p"   # path for parameters
$f     = $mount + "y0"  # path for function
$df    = $mount + "y1"  # path for gradient
$ddf   = $mount + "y2"  # path for hessian

# The next function only writes new parameters
# in parameter file
def write_p(p)
  File.open($p, "w") { |pf|  pf.puts p.map(&:to_s).join("\n") }
end

# The next function only writes new input to the input
# file
def write(x)
  File.open($x, "w") { |xf| xf.puts x.map(&:to_s).join("\n") }
end

# The next function reads the gradient and hessian, thus returns
# an array of array of values. This is what we need to evaluate a
# Newton algorithm
def read()
  return [
    File.open($df, "r").read.chomp.split("\n").map(&:to_f),
    File.open($ddf, "r").read.chomp.split("\n").map(&:to_f)
  ]
end

# This function performs a newton step in the form:
#
# x[+] = x - (hessian(f)(x))^(-1) gradient(f)(x)
def newton_step(x, df, ddf)
  dt = 1.0 / (ddf[0] * ddf[3] - ddf[1] * ddf[2])
  return [
    x[0] - dt * (ddf[3] * df[0] - ddf[2] * df[1]),
    x[1] - dt * (ddf[0] * df[1] - ddf[1] * df[0])
  ]
end

# Evaluates the norm two of 2D vectors
def norm(a, b)
  return ((a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2) ** (0.5)
end

if __FILE__ == $0
  # Maximum iteration
  ITERMAX = 1000
  # Tolerance. If ||x[+] - x|| < tol we stop execution
  # of iterative algorithm.
  TOL = 1e-5

  # Starting Point
  x = z = [0.0, 0.0]
  # Parameter configuration and write configuration
  write_p([1.0, 100.0])

  # Newton Algorithm
  i = 0
  for i in (0..ITERMAX)
    write(z) # write will evaluate again the function
    puts(z.map(&:to_s).join(","))
    df, ddf = read()
    z = newton_step(x, df, ddf)
    break if norm(z, x) < TOL
  end

  # Print on screen results
  puts "Itermax reached" if i == ITERMAX
  puts "Solution (after #{i} iterations):"
  puts "  f(#{z.map(&:to_s).join(", ")}) = #{File.open($f, "r").read.chomp}"
end # __FILE__ == $0
