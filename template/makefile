
LIBS = -g -m64  $(shell root-config --libs) -lMathMore  -lGenVector -lRooFit -lRooFitCore -lm
CFLAGS = -g -m64 -O2 -Wall $(shell root-config --cflags )
OBJS =  makeTemplate.o doFitting.o  
main: ${OBJS}
	g++ $^ -o $@ $(LIBS)
makeTemplate.o : makeTemplate.cc ../interface/EQ.h ../rootlogon.h
	g++ -c $(CFLAGS) $< -o $@
doFitting.o : ../doFitting.cc 
	g++ -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o
