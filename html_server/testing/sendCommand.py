# -*- coding: utf-8 -*- 
import socket
import time
import argparse



#arg setting
parser = argparse.ArgumentParser(description='Web server testing.')
parser.add_argument("-s",'--score', help='Push Score', action="store_true")
parser.add_argument("-s1",'--score1', help='Push Multiple Score', action="store_true")
parser.add_argument("-s2",'--score2', help='Push Multiple Score2', action="store_true")
parser.add_argument("-q",'--question', help='Push Question', action="store_true")
parser.add_argument("-q2",'--question2', help='Push Question2', action="store_true")
parser.add_argument("-i",'--img', help='Push Img', action="store_true")
parser.add_argument("-i2",'--img2', help='Push Img2', action="store_true")
parser.add_argument("-b",'--buzzer', help='Push buzzer', action="store_true")
parser.add_argument("-b2",'--buzzer2', help='Push buzzer2', action="store_true")
parser.add_argument("-u",'--ui', help='UI', action="store_true")
parser.add_argument("-u2",'--ui2', help='UI2', action="store_true")
parser.add_argument("-a",'--answer', help='answer', action="store_true")
args = parser.parse_args()


#connect socket
HOST = '127.0.0.1'
PORT=8889
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))

#sending
if args.score:
	s.sendall("""score:{"A":"200"}\n""")
if args.score1:
	s.sendall("""score:{"A":"400", "L":"350", "M":"1200"}\n""")
if args.score2:
	s.sendall("""score:{"A":"100", "D":"200", "H":"300", "J":"700", "L":"350", "M":"1200"}\n""")
if args.question:
	s.sendall("""question:{ "Catalog":"必答題", "Set":"題目集1", "ID":"1", "Question": "Which of the following industries belongs to tertiary production?", "Options": [ "A. Agriculture", "B. Construction", "C. Insurance", "D. Energy Supply" ], "Answer": "correct", "Path": "abc" }\n""")
if args.question2:
	s.sendall("""question:{ "Catalog":"附加題", "Set":"科學1", "ID":"1", "Question": "「初生之犢」的「犢」是指什麼?", "Answer": "小牛", "Path": "abc" }\n""")
if args.img:
	s.sendall("""img:{"path":"imgs/test.jpg"}\n""")
if args.img2:
	s.sendall("""img:{"path":"imgs/test2.jpg"}\n""")
if args.buzzer:
	s.sendall("""buzzer:{"A":"20301040"}\n""")
if args.buzzer2:
	s.sendall("""buzzer:{"M":"20301040"}\n""")
if args.ui:
	s.sendall("""ui:{"score":"hide"}\n""")
if args.ui2:
	s.sendall("""ui:{"score":"show"}\n""")
if args.a:
	s.sendall("""answer:{}\n""")

#time.sleep(3)
s.close()

print "received"

