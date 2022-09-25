SRCDIR=src
BUILDDIR=obj
TARGET=main

SRCTXT=cc
SOURCES=$(shell find $(SRCDIR) -type f -name *.cc)

INC=-Iinclude
INCDIR=include


LIBS = -g -m64  $(shell root-config --libs) -lMathMore  -lGenVector -lRooFit -lRooFitCore -lm
CFLAGS = -g -m64 -O2 -Wall $(shell root-config --cflags )


OBJS = obj/main.o obj/erecoCalibration.o obj/makeTemplate.o obj/Analyzer.o obj/doFitting.o obj/EqdirData.o obj/GeoData.o obj/drawPlot.o 
main: $(OBJS)
	g++ $^ -o $@ $(LIBS)
obj/main.o : src/main.cc include/RadonData.h include/DataReader.h
	g++ -c $(CFLAGS) $< -o $@
obj/erecoCalibration.o : src/erecoCalibration.cc include/RadonData.h 
	g++ -c $(CFLAGS) $< -o $@
obj/makeTemplate.o : src/makeTemplate.cc include/RadonData.h include/rootlogon.h
	g++ -c $(CFLAGS) $< -o $@
obj/Analyzer.o : src/Analyzer.cc include/RadonData.h include/rootlogon.h
	g++ -c $(CFLAGS) $< -o $@
obj/doFitting.o : src/doFitting.cc 
	g++ -c $(CFLAGS) $< -o $@
obj/EqdirData.o : src/EqdirData.cc include/DataReader.h 
	g++ -c $(CFLAGS) $< -o $@
obj/GeoData.o : src/GeoData.cc include/GeoData.h
	g++ -c $(CFLAGS) $< -o $@
obj/drawPlot.o : src/drawPlot.cc
	g++ -c $(CFLAGS) $< -o $@


clean:
	rm -f $(TARGET) $(BUILDDIR)/*.o

