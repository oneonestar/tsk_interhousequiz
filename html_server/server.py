from twisted.internet import protocol, reactor, stdio
from twisted.protocols import basic
from twisted.web import server, resource, static, script
from twisted.web.resource import Resource, NoResource
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
		for i in connections:
			pushToClient(i.write)
		#if ctrlstr[-1] == '\n':
			#for i in connections:
				#pushToClient(i.write)
		ctrlstr = ""
	def connectionLost(self, reason):
		print "Ctrl close:"+ reason.getErrorMessage()

class CtrlConnectionFactory(protocol.Factory):
	def buildProtocol(self, addr):
		return CtrlConnection()

#web server
def pushToClient(echo):
	global ctrlstr
	print("pushToClient: "+repr(ctrlstr))
	ctrlList = ctrlstr.split('\n')
	for i in ctrlList:
		if not i:
			continue	#empty command
		print(repr(i))
		arr = i.split(':')
		if arr[0] == "score" or arr[0] == "buzzer" or arr[0]=="img":
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

def myConnectionLost(reason, request): 
	print("Close HTTP connection:\n"+reason.getErrorMessage())
	print(request.client)
	print(request.getHeader("user-agent"))
	print('\n')
	global connections
	if request in connections:
		connections.remove(request)

class MyResource(Resource):
	isLeaf = True
	def render_GET(self, request):
		global connections
		request.setHeader("Content-Type", "text/event-stream")
		request.setHeader("Cache-Control", "no-cache")
		request.setHeader("Connection", "keep-alive")
		request.notifyFinish().addErrback(myConnectionLost, request)
		connections.append(request)		
		request.write("\n")
		print("New HTTP connection (HTTP GET):")
		print(request.client)
		print(request.getHeader("user-agent"))
		print('\n')
		return NOT_DONE_YET


#disable static.File listing directory function
class FileNoDir(static.File):
	def directoryListing(self):
		return self.childNotFound

		
#stdin
stdio.StandardIO(StdinInputHandler())
#server root directory 
root = FileNoDir(".")
root.childNotFound = NoResource()
root.indexName=["index.html"]
#server send events
root.putChild("events",MyResource())
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
