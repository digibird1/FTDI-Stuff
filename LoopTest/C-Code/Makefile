CXXFLAGS=`root-config --cflags` 
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)


SRC=.
INCLUDE=-I$(SRC) -Ilibftdi1-1.2/src

LIB=-Llibftdi1-1.2/build/src

all:  
#	g++   -Xlinker -zmuldefs  -o  run.exe  $(INCLUDE) $(LIB) $(SRC)/stream_test.c -lftdi1
	g++   -Xlinker -zmuldefs  -o  run.exe  $(INCLUDE) $(LIB) $(SRC)/readWrite.cpp -lftdi1  
