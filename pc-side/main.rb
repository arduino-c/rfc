
require './accessor.rb'

$a = Accessor.new("COM1", "9600")

while 1
	puts "channel_a = #{$a.call("channel_a", 0).to_i}"
	puts "channel_b = #{$a.call("channel_b", 0).to_i}"
end






