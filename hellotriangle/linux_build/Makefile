# Makefile
# Chris Schultz
# 30 May 2020
#
# Makefile for Hello Triangle program
CXX=g++
CXXFLAGS=-std=c++11
GLFLAGS=-lglfw3 -lGLEW -lGLU -lGL -lX11  -lpthread -lXrandr -lXi -ldl
RM=/bin/rm -f

hellotriangle: main.cpp
	${CXX} ${CXXFLAGS} main.cpp -o hellotriangle ${GLFLAGS}

clean:
	${RM} hellotriangle
