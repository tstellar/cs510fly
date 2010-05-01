
.PHONY: linux osx

linux: CPPFLAGS=-DLINUX
linux: INCLUDE=-I/usr/include/OGRE

linux: main

osx: main

main: Main.o World.o InputListener.o
	g++ -o $@ $^ -lOgreMain -lOIS

%.o: %.cpp
	g++ ${CXXFLAGS} ${CPPFLAGS} -c -o $@ $^ ${INCLUDE}

