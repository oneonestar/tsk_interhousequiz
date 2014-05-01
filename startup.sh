#!/bin/bash

if [ "$#" -eq 1 ]; then
	cd redis_database_src
	./redis-server & echo "Redis database server PID $!"
	sleep 3
	cd ../html_server
	python server.py & echo "HTML server started PID $!"
	sleep 3
	cd ../main/build
	./quiz 127.0.0.1 "$1"
else
	echo 'usage: sudo ./startup.sh [buzzer addr]'
fi
