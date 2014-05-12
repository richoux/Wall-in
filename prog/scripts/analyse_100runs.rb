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
building = 0
gap = 0
techtree = 0

# For each line in file
file.each do |line|
  words = line.split(': ')
  if words[0] == "Global cost"
    if words[1].to_i == 0
      count += 1
    end
    lines += 1
    cost += words[1].to_i
  elsif words[0] == "Opt Cost if the objective was building"
    building += words[1].to_i
  elsif words[0] == "Opt Cost if the objective was gap"
    gap += words[1].to_i
  elsif words[0] == "Opt Cost if the objective was techtree"
    techtree += words[1].to_i
  end
end

puts "Number of successes: #{count}"
puts "Number of runs: #{lines}"
rate = count.to_f * 100 / lines.to_f
puts "Success rate: #{rate}"
c = cost / lines.to_f
puts "Average global cost: #{c}"
b = building / count.to_f
puts "Average building cost (over successful runs): #{b}"
g = gap / count.to_f
puts "Average gap cost (over successful runs): #{g}"
t = techtree / count.to_f
puts "Average techtree cost (over successful runs): #{t}"

exit
