from twisted.internet import protocol, reactor, stdio
from twisted.protocols import basic
from twisted.web import server, resource
from twisted.web.resource import Resource
from twisted.web.server import NOT_DONE_YET




#Global variables
teststr=""
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
#Ctrl server connection port 8889
class CtrlConnection(protocol.Protocol):
	def dataReceived(self,data):
		global teststr
		global connections
		print(data)
		teststr1=data.rstrip("\0")
		for j in teststr1:
			teststr = j	
			for i in connections:
				cycle(i.write)

class CtrlConnectionFactory(protocol.Factory):
	def buildProtocol(self, addr):
		return CtrlConnection()

def cycle(echo):
	global teststr
	echo("event: userlogon\n")
	print("CYCLE"+teststr)
	echo('data: {"username":"' + teststr + '"}')
	echo("\n\n")


#web server 8080
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
reactor.listenTCP(8889,CtrlConnectionFactory())
reactor.listenTCP(8080,site)
reactor.run()
