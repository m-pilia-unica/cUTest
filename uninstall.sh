#!/usr/bin/env bash

if [ "`whoami`" != "root" ]; then 
	echo "Error: Must be root."
	exit
fi

if [ -e /lib/libcutest.a ]; then 
	echo "rm /lib/libcutest.a"
	rm /lib/libcutest.a
fi

if [ -e /usr/include/cutest.h ]; then 
	echo "rm /usr/include/cutest.h"
	rm /usr/include/cutest.h
fi

if [ -e /usr/include/cutest_private.h ]; then 
	echo "rm /usr/include/cutest_private.h"
	rm /usr/include/cutest_private.h
fi

if [ -e /usr/share/doc/cutest ]; then
	echo "rm -rf /usr/share/doc/cutest"
	rm -rf /usr/share/doc/cutest
fi
