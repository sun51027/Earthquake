LIBS = -g -m64  $(shell root-config --libs) -lMathMore  -lGenVector -lRooFit -lRooFitCore -lm
CFLAGS = -g -m64 -O2 -Wall $(shell root-config --cflags )
OBJS = main.o analyzer.o doFitting.o
main: ${OBJS}
	g++ $^ -o $@ $(LIBS)
main.o: main.cc interface/EQ.h
	g++ -c $(CFLAGS) $< -o $@
analyzer.o : analyzer.cc interface/EQ.h
	g++ -c $(CFLAGS) $< -o $@
doFitting.o : doFitting.cc 
	g++ -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o
