import smbus
import time
import socket
import fcntl
import struct

#initialization for Arduino
bus = smbus.SMBus(1)
address = 0x04
buttonState = [0, 0, 0, 0, 0, 0]

def get_ip_address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915,  # SIOCGIFADDR
        struct.pack('256s', ifname[:15])
    )[20:24])

def bin(s):
    return str(s) if s<=1 else bin(s>>1) + str(s&1)

def readButton(s):
	for index in range(len(buttonState)):
		if (s&1==1) and buttonState[index]!=1:
			buttonState[index] = 1
			conn.send(str(index))
			print "button ", index+1, " pressed"
		s = s>>1

host = get_ip_address("wlan0")
print host
port = 8888
s = socket.socket()
s.bind((host,port))

while True:
	#start listening from client
	s.listen(1)
	conn, addr = s.accept()
	print 'connection accepted from ' + str(addr) + " at " + str(time.localtime())

	start = time.time()
	end = start + 5

	#start polling
	while True:
		#read signal from arduino
		signal = bus.read_byte(address)
		readButton(signal)

		#check if all buttons were pushed
		if (buttonState[0]==1 and buttonState[1]==1 and buttonState[2]==1 and buttonState[3]==1 and buttonState[4]==1 and buttonState[5]==1) or (time.time()==end or time.time()>end):
			break;

	#end of polling, close connection and reset buttonState
	print "Polling Ends at " + str(time.localtime()) + " !"
	conn.close()
	buttonState = [0, 0, 0, 0, 0, 0]

#close socket at the end
s.close()
