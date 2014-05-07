from twisted.internet import protocol, reactor, stdio
from twisted.protocols import basic
from twisted.web import server, resource, static, script
from twisted.web.resource import Resource, NoResource
from twisted.web.server import NOT_DONE_YET
from collections import OrderedDict
import urllib 

import json
import redis

r = redis.StrictRedis(host='localhost', port=6379, db=0)

class MyResource(Resource):
	isLeaf = True
	allowedMethods = ('GET','POST')
	def render_GET(self, request):
		return ''

	def render_POST(self, request):
		newdata = request.content.getvalue()
		newdata = urllib.unquote(newdata).decode('utf8')
		newdata = newdata.replace("+", " ")
		print newdata
		# print newdata
		data = {}
		for item in newdata.split('&'):
			if("Option" not in item.split('=')[0]):
				data[item.split('=')[0]] = "".join(item[item.find("=")+1:])
			print item.split('=')
		if request.args["OptionA"][0] != "":
			arr = [request.args["OptionA"][0],request.args["OptionB"][0],request.args["OptionC"][0],request.args["OptionD"][0]]
			data["Options"] = arr
		print data["QuestionID"]
		print data
		print json.dumps(data)
		r.set(data["QuestionID"], json.dumps(data))
		request.redirect(".")
		request.finish()
		return server.NOT_DONE_YET

#disable static.File listing directory function
class FileNoDir(static.File):
	def directoryListing(self):
		return self.childNotFound

class MyLists(Resource):
	isLeaf = True
	def render_GET(self, request):
		request.setHeader('content-type', 'application/json; charset=UTF-8')
		mylist = r.keys("Question:*")
		
		result = {}
		for item in mylist:
			result[item] = r.get(item)
		
		result["new"] = {}	#for Insert Question
		return json.dumps(result, sort_keys=True)

#server root directory 
root = FileNoDir("./www")
root.childNotFound = NoResource()
root.indexName=["index.html"]
#server send events
root.putChild("form",MyResource())
root.putChild("lists",MyLists())
site = server.Site(root)

import argparse
parser = argparse.ArgumentParser(description='Twisted web server.')
parser.add_argument('--webport', nargs='?',default='80', const='80', type=int, help='Port to control server')
args = parser.parse_args()
#web connection (80)
reactor.listenTCP(args.webport,site)
reactor.run()
