import socket
import time
import argparse


#connect socket
HOST = '127.0.0.1'
PORT=8889
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))

while True:
	a = raw_input()
	s.sendall(a+"\n")
