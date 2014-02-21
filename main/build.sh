#!/bin/bash
rm build -rf
mkdir -p build
cd build
if [ $# -eq 0 ]
then
	cmake ../src
elif [ $1 = "redis" ]
then
	cmake ../src -DDB:STRING=Redis
elif [ $1 = "mongodb" ] 
then
	cmake ../src -DDB:STRING=Mongodb
fi
make
