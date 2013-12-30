from twisted.internet import protocol, reactor, stdio
from twisted.protocols import basic
from twisted.web import server, resource
from twisted.web.resource import Resource
from twisted.web.server import NOT_DONE_YET




#Global variables
ctrlstr=""
connections=[]



#Stdio
class StdinInputHandler(basic.LineReceiver):
	from os import linesep as delimiter

	def connectionMade(self):
		self.transport.write('>>> ')

	def lineReceived(self, line):
		self.sendLine('Echo: ' + line)
		try:
			exec(line)
		except:
			print("Input Error")	
		self.transport.write('>>> ')

#Web
#Ctrl server connection
class CtrlConnection(protocol.Protocol):
	def dataReceived(self,data):
		global ctrlstr
		global connections
		print(data)
		ctrlstr+=data
		if ctrlstr[-1] == '\0':
			for i in connections:
				pushToClient(i.write)

	def connectionLost(self, reason):
		print "LOST:"+ reason.getErrorMessage()

class CtrlConnectionFactory(protocol.Factory):
	def buildProtocol(self, addr):
		return CtrlConnection()

def pushToClient(echo):
	global ctrlstr
	print("pushToClient: "+repr(ctrlstr))
	ctrlList = ctrlstr.split('\0')
	for i in ctrlList:
		if not i:
			continue	#empty command
		print(repr(i))
		arr = i.split(':')
		if arr[0] == "score" or arr[0] == "buzzer":
			#form json string and send
			echo("event: %s\n" % arr[0])
			print(arr)
			echo('data: {"%s":"%s"}'% (arr[1], arr[2]))
			echo("\n\n")
		if arr[0] == "question":
			#already a json string
			print("QUESTION:"+i[i.find(':')+1:])
			echo("event: question\n")
			echo('data:' + i[i.find(':')+1:].replace('\n','<br>'))
			echo("\n\n")
	ctrlstr = ""

#web server
class MyResource(Resource):
	isLeaf = True
	def render_GET(self, request):
		global connections
		if request.postpath.pop(0)!='':
			request.setHeader("Content-Type", "text/event-stream")
			request.setHeader("Cache-Control", "no-cache")
			request.setHeader("Connection", "keep-alive")
			connections.append(request)		
#			lc = twisted.internet.task.LoopingCall(cycle,request.write)
			#lc.start(1)
			#request.notifyFinish().addBoth(lambda _: lc.stop())
			request.write("\n")
			return NOT_DONE_YET
		f = open("html_content.html")
		htmlData=f.read()
		f.close()
		return htmlData


		
#stdin
stdio.StandardIO(StdinInputHandler())
#server
root=MyResource()
site = server.Site(root)


#command line change port support
import argparse
parser = argparse.ArgumentParser(description='Twisted web server.')
parser.add_argument('--ctrlport', nargs='?',default='8889', const='8889', type=int, help='Port to control server')
parser.add_argument('--webport', nargs='?',default='80', const='80', type=int, help='Port to control server')
args = parser.parse_args()

#ctrl connection (8889)
reactor.listenTCP(args.ctrlport,CtrlConnectionFactory())
#web connection (80)
reactor.listenTCP(args.webport,site)


reactor.run()
