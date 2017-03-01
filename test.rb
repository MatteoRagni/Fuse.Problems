#!/usr/bin/env ruby

$x = "/tmp/fp/x"
$p = "/tmp/fp/p"
$y = "/tmp/fp/y"


File.open($x, "w") do |f|
  f.puts "1.0\n2.0"
end

File.open($p, "w") do |f|
  f.puts "1.0\n100.0"
end

puts File.open($y, "r").read
