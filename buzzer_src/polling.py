import sys
import smbus
import time
import socket
import fcntl
import struct
import subprocess

#initialization for Arduino
bus = smbus.SMBus(1)
address = 0x04
buttonState = [0, 0, 0, 0, 0, 0]
flag = 0

def get_ip_address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915,  # SIOCGIFADDR
        struct.pack('256s', ifname[:15])
    )[20:24])

def bin(s):
    return str(s) if s<=1 else bin(s>>1) + str(s&1)

def readByte(address, byte):
	try:
		data = bus.read_byte(address)
		return data
	except IOError:
		print("IOError in Read")
		subprocess.call(['sudo', 'modprobe', '-r', 'i2c_bcm2708'])
		subprocess.call(['sudo', 'modprobe', 'i2c_bcm2708', 'baudrate=100010'])
		return

def writeByte(address, data):
	try:
		bus.write_byte(address, data)
		flag = 0
	except IOError:
		print("IOError in writeData")
		#subprocess.call(['i2cdetect', '-y', '1'])
		subprocess.call(['sudo', 'modprobe', '-r', 'i2c_bcm2708'])
		subprocess.call(['sudo', 'modprobe', 'i2c_bcm2708', 'baudrate=100010'])
		flag = 1
	return

def readButton(s):
	for index in range(len(buttonState)):
		if (s&1==1) and buttonState[index]!=1:
			buttonState[index] = 1
			conn.send(str(index))
			print "button ", index+1, " pressed"
		s = s>>1

def readByte(address, byte):
	try:
		data = bus.read_byte(address)
		return data
	except IOError:
		print("IOError in Read")
		subprocess.call(['i2cdetect', '-y', '1'])
		return 0

def writeByte(address, data):
	try:
		bus.write_byte(address, data)
		flag = 0
	except IOError:
		print("IOError in writeData")
		subprocess.call(['i2cdetect', '-y', '1'])
		flag = 1
	return

serverAddr = get_ip_address("wlan0")
print "Buzzer Address: ", serverAddr
host = ''
port = 8888
s = socket.socket()
s.bind((host,port))

while True:
	try:
		time.sleep(1);

		#start listening from client
		s.listen(1)
		conn, addr = s.accept()
		time.sleep(0.5)

		start = time.time()
		print 'connection accepted from ' + str(addr) + " at " + time.asctime(time.localtime(start))

		end = start + 15
		writeByte(address, 1)
		time.sleep(0.5)

		#start polling
		while True:
			#read signal from arduino
			signal = readByte(address, 1) 
			readButton(int(signal))

			#check if all buttons were pushed
			if (buttonState[0]==1 and buttonState[1]==1 and buttonState[2]==1 and buttonState[3]==1 and buttonState[4]==1 and buttonState[5]==1) or (time.time()==end or time.time()>end):
				break;

		#end of polling, close connection and reset buttonState
		print "Polling Ends at " + time.asctime(time.localtime(end)) + " !"
		conn.close()
		buttonState = [0, 0, 0, 0, 0, 0]
		writeByte(address, 0)

		flag = 0

	except KeyboardInterrupt:
		#close socket and exit when receiving ctrl+C interrupt
		s.close();
		print "Keyboard Interrupt received, closing socket and exit!"
		sys.exit(0);
