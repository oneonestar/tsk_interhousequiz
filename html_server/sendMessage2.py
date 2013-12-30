import socket
import time

HOST = '127.0.0.1'
PORT=8889
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))
s.sendall("score:B:40\0score:A:40\0")
#s.sendall("""question:{ "Question": "question", "Options": [ "A", "B", "C", "D" ], "Answer": "correct", "Path": "\/path\/\nabc" }\0""")
#time.sleep(3)
#s.sendall(":10\0")
s.close()

print "received"
