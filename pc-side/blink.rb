
require './accessor.rb'


a = Accessor.new("/dev/tty.usbmodem1411", "9600")

puts "start"
print a.xx
puts "exit"