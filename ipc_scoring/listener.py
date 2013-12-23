import socket

def get_ip_address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915,  # SIOCGIFADDR
        struct.pack('256s', ifname[:15])
    )[20:24])

host = '127.0.0.1'
port = 8888
s = socket.socket()
s.bind((host,port))

while True:
	s.listen(1)
	c, addr = s.accept()
	print 'Connection accepted from ' + str(addr)
	print c.recv(3)
	c.close()

s.close()
