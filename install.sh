#!/usr/bin/env bash

if [ "`whoami`" != "root" ]; then
	echo "Error: Must be root."
	exit
else
	echo "cp ./build/cutest.a /lib/libcutest.a"
	cp ./build/cutest.a /lib/libcutest.a
	
	echo "cp ./src/linked_list.h /usr/include/linked_list.h"
	cp ./src/linked_list.h /usr/include/linked_list.h
	
	echo "cp ./src/cutest.h /usr/include/cutest.h"
	cp ./src/cutest.h /usr/include/cutest.h
	
	echo "cp ./src/cutest_private.h /usr/include/cutest_private.h"
	cp ./src/cutest_private.h /usr/include/cutest_private.h

	if [ ! -e /usr/share/doc/cutest ]
	then 
		mkdir /usr/share/doc/cutest
	fi
	echo "cp -R ./build/doc/html /usr/share/doc/cutest/html"
	cp -R ./build/doc/html /usr/share/doc/cutest/html
fi
