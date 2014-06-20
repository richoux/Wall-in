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
post_time = 0
solution = false

# For each line in file
file.each do |line|
  if line.start_with?("OPTIMIZATION")
    solution = false
  else
    words = line.split(': ')
    if words[0] == "Global cost"
      if words[1].to_i == 0
        count += 1
        solution = true
      end
    end
    if words[0] == "Post-processing time" and solution
      post_time += words[1].to_f
    end
  end
end

puts "Number of successes: #{count}"
time = post_time / count.to_f
puts "Average post processing time: #{time}"

exit
