import socket
import time
import argparse



#arg setting
parser = argparse.ArgumentParser(description='Web server testing.')
parser.add_argument("-s",'--score', help='Push Score', action="store_true")
parser.add_argument("-s1",'--score1', help='Push Multiple Score', action="store_true")
parser.add_argument("-q",'--question', help='Push Question', action="store_true")
parser.add_argument("-i",'--img', help='Push Img', action="store_true")
parser.add_argument("-i2",'--img2', help='Push Img2', action="store_true")
args = parser.parse_args()


#connect socket
HOST = '127.0.0.1'
PORT=8889
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))

#sending
if args.score:
	s.sendall("score:A:40\n")
if args.score1:
	s.sendall("score:A:40\nscore:L:35\nscore:M:98\n")
if args.question:
	s.sendall("""question:{ "Question": "question", "Options": [ "A", "B", "C", "D" ], "Answer": "correct", "Path": "abc" }\n""")
if args.img:
	s.sendall("img:path:imgs/test.jpg\n")
if args.img2:
	s.sendall("img:path:imgs/test2.jpg\n")

#time.sleep(3)
s.close()

print "received"
