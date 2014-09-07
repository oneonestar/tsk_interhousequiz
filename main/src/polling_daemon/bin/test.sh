#!/bin/bash -x 

data=0
while [[ "$data" = "0" ]];
do
	curl -u root:arduino http://192.168.0.100/data/get
	echo ""
done
