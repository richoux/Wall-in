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
cost = 0
opt = 0
perfect = 0

# For each line in file
file.each do |line|
  words = line.split(': ')
  if words[0] == "Global cost"
    if words[1].to_i == 0
      count += 1
    end
    lines += 1
    cost += words[1].to_i
  end
  if words[0] == "Optimization cost" and words[1] != "2.14748e+09"
    opt += words[1].to_i
    if words[1].to_i == 0
      perfect += 1
    end
  end

end

puts "Number of successes: #{count}"
puts "Number of runs: #{lines}"
rate = count.to_f * 100 / lines.to_f
puts "Success rate: #{rate}"
c = cost / lines.to_f
puts "Average global cost: #{c}"
o = opt / count.to_f
puts "Average optimization cost (over successful runs): #{o}"
puts "Number of perfect walls (no gaps, over successful runs): #{perfect}"

exit
