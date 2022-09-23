SRCDIR=src
BUILDDIR=obj
TARGET=main

SRCTXT=cc
SOURCES=$(shell find $(SRCDIR) -type f -name *.cc)
#OBJECTS=$(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.cc=.o))

INC=-Iinclude
INCDIR=include


LIBS = -g -m64  $(shell root-config --libs) -lMathMore  -lGenVector -lRooFit -lRooFitCore -lm
CFLAGS = -g -m64 -O2 -Wall $(shell root-config --cflags )

#all: $(TARGET)

OBJS = obj/main.o obj/erecoCalibration.o obj/makeTemplate.o obj/Analyzer.o obj/doFitting.o obj/ReadData.o obj/GeoData.o obj/drawPlot.o 
main: $(OBJS)
	g++ $^ -o $@ $(LIBS)
obj/main.o : src/main.cc include/EQ.h include/DataReader.h
	g++ -c $(CFLAGS) $< -o $@
obj/erecoCalibration.o : src/erecoCalibration.cc include/EQ.h 
	g++ -c $(CFLAGS) $< -o $@
obj/makeTemplate.o : src/makeTemplate.cc include/EQ.h include/rootlogon.h
	g++ -c $(CFLAGS) $< -o $@
obj/Analyzer.o : src/Analyzer.cc include/EQ.h include/rootlogon.h
	g++ -c $(CFLAGS) $< -o $@
obj/doFitting.o : src/doFitting.cc 
	g++ -c $(CFLAGS) $< -o $@
obj/ReadData.o : src/ReadData.cc include/DataReader.h 
	g++ -c $(CFLAGS) $< -o $@
obj/GeoData.o : src/GeoData.cc include/GeoData.h
	g++ -c $(CFLAGS) $< -o $@
obj/drawPlot.o : src/drawPlot.cc
	g++ -c $(CFLAGS) $< -o $@
#$(TARGET):	$(OBJECTS)
#	g++	-c	$^ $@	$<	$(LIBS)	$(OBJECTS) 
#$(BUILDDIR)/main.o:	$(SRCDIR)/main.cc 
#	g++	-c	$(CFLAGS)	$<	-o	$@
#$(BUILDDIR)/%.o:	$(SRCDIR)/%.cc $(INCDIR)/%.h
#	g++ -c $< -o $@ $(CFLAGS) 

clean:
	rm -f $(TARGET) $(BUILDDIR)/*.o

