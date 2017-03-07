#!/usr/bin/env ruby

require 'benchmark'

# This is a write function
def write(x)
  File.open("/tmp/mount/x", "w") { |xf| xf.puts x.map(&:to_s).join("\n") }
end

# This is a read function
# The read also performs a conversion
def read()
  File.open("/tmp/mount/y0", "r").read.to_f
end

puts Benchmark.measure {
  5_000.times do
    x = [Random.rand((0.0)..(1.0)), Random.rand((0.0)..(1.0))]
    write(x)
    read()
  end
}
