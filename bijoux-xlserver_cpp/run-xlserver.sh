#!/bin/bash

if [ ! -e ./src/bijoux_xlserver ]; then
	echo "./bijoux_server does not exist, building..."
	./build.sh
fi


cd src
./bijoux_xlserver
