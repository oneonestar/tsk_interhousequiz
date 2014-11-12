import sqlite3
import json
import redis
conn = sqlite3.connect('test.db')
c = conn.cursor()
r = redis.StrictRedis(host='localhost', port=6379, db=0)

c.execute("CREATE TABLE question (ID text, data text);")
mylist = r.keys("Question:*")
		
result = {}
for item in mylist:
	result[item] = r.get(item)

myData = []
for k, v in result.items():
        t = str(k).split(":")
        myData.append((t[1]+":"+t[2]+":"+t[3], v))

        
c.executemany('INSERT INTO question VALUES (?,?)', myData)

conn.commit()
conn.close()
