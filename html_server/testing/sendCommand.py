import socket
import time
import argparse



#arg setting
parser = argparse.ArgumentParser(description='Web server testing.')
parser.add_argument("-s",'--score', help='Push Score', action="store_true")
parser.add_argument("-s1",'--score1', help='Push Multiple Score', action="store_true")
parser.add_argument("-s2",'--score2', help='Push Multiple Score2', action="store_true")
parser.add_argument("-q",'--question', help='Push Question', action="store_true")
parser.add_argument("-i",'--img', help='Push Img', action="store_true")
parser.add_argument("-i2",'--img2', help='Push Img2', action="store_true")
parser.add_argument("-b",'--buzzer', help='Push buzzer', action="store_true")
parser.add_argument("-b2",'--buzzer2', help='Push buzzer2', action="store_true")
args = parser.parse_args()


#connect socket
HOST = '107.170.4.131'
PORT=8891
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))

#sending
if args.score:
	s.sendall("score:A:200\n")
if args.score1:
	s.sendall("score:A:400\nscore:L:350\nscore:M:1200\n")
if args.score2:
	s.sendall("score:A:100\nscore:D:200\nscore:H:300\nscore:J:700\nscore:L:350\nscore:M:1200\n")
if args.question:
	s.sendall("""question:{ "Question": "question", "Options": [ "A", "B", "C", "D" ], "Answer": "correct", "Path": "abc" }\n""")
if args.img:
	s.sendall("img:path:imgs/test.jpg\n")
if args.img2:
	s.sendall("img:path:imgs/test2.jpg\n")
if args.buzzer:
	s.sendall("buzzer:A:20301040\n")
if args.buzzer2:
	s.sendall("buzzer:M:20301040\n")

#time.sleep(3)
s.close()

print "received"
