#!/usr/bin/env ruby

# Please notice that this script is strictly
# problem dependend, since we do not check size and
# dimensions and we already know that f returns only a
# value, while gradient returns a vector.

$mount = "/tmp/mount/"
$x     = $mount + "x"   # path for input
$p     = $mount + "p"   # path for parameters
$df    = $mount + "y1"  # path for gradient
$ddf   = $mount + "y2"  # path for hessian

# The next function only writes new parameters
# in parameter file
def write_p(p)
  File.open($p, "w") do |pf|
    pf.puts p.map(&:to_s).join("\n")
  end
end

# The next function only writes new input to the input
# file
def write(x)
  File.open($x, "w") do |xf|
    xf.puts x.map(&:to_s).join("\n")
  end
end

# The next function reads the gradient and hessian, thus returns
# an array of array of values
def read()
  df = Files($df, "r").read.chomp.split("\n").map(&:to_f)
  ddf = Files($ddf, "r").read.chomp.split("\n").map(&:to_f)
  return df, ddf
end

def newton_step(x, df, ddf)
  return [
    x[0] - (ddf[0] * df[0] + ddf[1] * df[1]),
    x[1] - (ddf[2] * df[0] + ddf[3] * df[1])
  ]
end
