all:
	if [ ! -e build ]; then mkdir build; fi
	gcc -o build/cutest.o -c src/cutest.c
	gcc -o build/linked_list.o -c src/linked_list.c
	ar rcs build/cutest.a build/cutest.o build/linked_list.o

test: all
	gcc -o build/test.o -c src/test.c
	gcc -o build/test build/test.o build/linked_list.o build/cutest.o
	build/test

doc:
	doxygen Doxyfile

install: all doc
	./install.sh

uninstall:
	./uninstall.sh

clean:
	rm -rf ./build
