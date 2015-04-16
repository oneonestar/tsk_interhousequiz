import sys
import smbus
import time
import socket,select
import fcntl
import struct
import subprocess

#initialization for Arduino
bus = smbus.SMBus(1)
address = 0x04
buttonState = [0, 0, 0, 0, 0, 0]

def writeByte(address, data):
	try:
		bus.write_byte(address, data)
	except IOError:
		print("IOError in writeData")
		subprocess.call(['sudo', 'modprobe', '-r', 'i2c_bcm2708'])
		subprocess.call(['sudo', 'modprobe', 'i2c_bcm2708', 'baudrate=100010'])

# "12345A" -> "12345A"
# "123A56" -> "56123A"
# "A" is the terminator
def shiftOrder(str):
	i = str.find("A")
	return str[i+1:] + str[0:i]

def readByte(address, byte):
	try:
		data = ""
		for i in range(0,7):
			temp = bus.read_byte(address)
			if temp:
				data += chr(temp)
		return shiftOrder(data)
	except IOError:
		print("IOError in Read")
		subprocess.call(['i2cdetect', '-y', '1'])
		return 0

host = ''
port = 8888
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((host,port))
s.listen(5)
ss = [s]
lastSignal = ""
while True:
	try:
		ready_to_read,_,_ = select.select(ss,[],[],0)
		for i in ready_to_read:
			if s == i:
				client, conn_address = i.accept()
				ss.append(client)
				print 'connection accepted:' + conn_address[0]
				Main_HOST = conn_address[0]
			else:
				data = i.recv(1000)
				if data:
					if data[0]=="0":
						print ("reset();")
						writeByte(address,0)
					elif data[0]=="1":
						print ("enable();")
						writeByte(address,1)
					elif data[0]=="2":
						print ("disable();")
						writeByte(address,2)
				else:
					print 'connection closed:'
					ss.remove(i)
					i.close()
		#start listening from client
#		s.listen(1)
#		conn, addr = s.accept()
#		time.sleep(0.5)

		#start polling
		#read signal from arduino
		time.sleep(0.1)
		signal = readByte(address, 1) 
		if signal != lastSignal and signal != "":
			print("Pressed: " + str(signal));
			print(repr(signal))
			for i in ss:
				if i != s:
					i.send(signal)
					print(i.getpeername())
					print("send: "+signal)
					print(repr(signal))
			print("")
		lastSignal = signal
		time.sleep(0.1)

	except KeyboardInterrupt:
		#close socket and exit when receiving ctrl+C interrupt
		s.close();
		print "Keyboard Interrupt received, closing socket and exit!"
		sys.exit(0);

	finally:
		#end of polling, close connection and reset buttonState
		#print "Polling Ends at " + time.asctime(time.localtime(end)) + " !"
		#conn.close()
		buttonState = [0, 0, 0, 0, 0, 0]
		#writeByte(address, 0)
