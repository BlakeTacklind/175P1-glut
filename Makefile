all:
	g++ -std=c++11 -Wno-write-strings main.cpp types.cpp gline.cpp pline.cpp OpenGLhandler.cpp valueHolder.cpp screen.cpp surface.cpp object3Dsurface.cpp userInterface.cpp line.cpp -lGL -lglut -lncurses

