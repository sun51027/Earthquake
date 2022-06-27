LIBS = -g -m64  $(shell root-config --libs) -lMathMore  -lGenVector -lRooFit -lRooFitCore -lm
CFLAGS = -g -m64 -O2 -Wall $(shell root-config --cflags )
OBJS = main.o analyzer.o doFitting.o drawPlot.o readData.o 
main: ${OBJS}
	g++ $^ -o $@ $(LIBS)
main.o: main.cc interface/EQ.h interface/DataReader.h
	g++ -c $(CFLAGS) $< -o $@
analyzer.o : analyzer.cc interface/EQ.h interface/rootlogon.h
	g++ -c $(CFLAGS) $< -o $@
doFitting.o : doFitting.cc 
	g++ -c $(CFLAGS) $< -o $@
drawPlot.o : drawPlot.cc
	g++ -c $(CFLAGS) $< -o $@
readData.o : readData.cc interface/DataReader.h 
	g++ -c $(CFLAGS) $< -o $@
#drawPvalue.o : drawPvalue.cc
#	g++ -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o
