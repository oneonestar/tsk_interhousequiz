import socket

HOST = '127.0.0.1'
PORT=8889
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))
s.sendall(raw_input())
s.close()

print "received"
