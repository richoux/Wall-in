#!/usr/bin/ruby

def usage
  puts "#{$0}"
  puts  "Usage: " + $0 + " FILE"
end

# We must have at least a file name
if ARGV.length == 0
  usage
  exit
end

# Open file
file = File.open(ARGV[0])

count = 0
lines = 0

# For each line in file
file.each do |line|
  words = line.split(': ')
  if words[0] == "Optimization cost"
    if words[1].to_i == 0
      count += 1
    end
    lines += 1
  end
end

puts "Number of successes: #{count}"
puts "Number of runs: #{lines}"
rate = count.to_f * 100 / lines.to_f
puts "Success rate: #{rate}"

exit
