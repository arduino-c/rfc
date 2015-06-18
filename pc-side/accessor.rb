
require "serialport"

class Accessor
	def initialize(port, baudrate)
		@sp = SerialPort.new(port, baudrate.to_i)
		@sp.read_timeout = 100

		# check if connection established
		while (did = self.get("DID")) == ""
		end
		print "connection established with: id_#{did}\n"
	end

	def finalize(object_id)
		@sp.close
	end

	def set(name, value)
		s = "set #{name} #{value}\r"
		#puts s
		@sp.write s
		return nil
	end

	def get(name)
		g = "get #{name}\r"
		#puts g
		@sp.write g
		return @sp.read
	end

	def call(name, value)
		s = "call #{name} #{value}\r"
		#puts s
		@sp.write s
		return @sp.read
	end
	
	def xx
		@sp.write "call up 10\r"
		while (rval = @sp.read) == ""
		end
		return rval
	end

end
