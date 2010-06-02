.PHONY: linux osx

linux: CPPFLAGS=-DLINUX
linux: INCLUDE=-I/usr/include/OGRE

linux: main

osx: main

main: Main.o Game.o InputListener.o Airplane.o Target.o Level.o Enemy.o Display.o World.o PhysicalState.o
	g++ -o $@ $^ -lOgreMain -lOIS -lalure

%.o: %.cpp
	g++ ${CXXFLAGS} ${CPPFLAGS} -c -o $@ $^ ${INCLUDE}

