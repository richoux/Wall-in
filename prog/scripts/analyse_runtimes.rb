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

times = []
count = 0

# For each line in file
file.each do |line|
  words = line.split(': ')
  if words[1].to_f < 1000
    times.push( words[1].to_f )
  else
    count += 1
  end
end

puts "Number of successes: #{times.size}."
puts "Number of timeouts: #{count}."

### TIMES ###
# Sort times to compute the median
times.sort!

if times.size.odd?
  median = times[times.size / 2]
else
  median = ( times[times.size / 2 - 1] + times[times.size / 2] ) / 2
end  

mean = times.inject{ |sum, el| sum + el }.to_f / times.size

# Print times results
puts "Times: Min( #{times.min} ) \
| Avg( " + "%.2f" % mean + " ) \
| Med( #{median} ) \
| Max( #{times.max} )"

exit
