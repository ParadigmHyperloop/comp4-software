#Object files for tests
TESTS = testsMain.o test1.o PodInternalNetworkTests.o
#Catch Header File
TESTLIB = catch.hpp

COMP = g++

#Catch needs c++11   Add include path for the project
BLDFLAGS = -std=c++11 -I"../include/FlightComputer"

#Link to object files for project. They should be made as the project is compile prior to runing this make file
ALLSRC = $(shell find ../Debug/src -name '*.o')

#Filter out object file for main control loop to avoid multiple definitions of 'main()'
SRC = $(filter-out ../Debug/src/FlightComputer.o,$(ALLSRC))


runTests: $(TESTS) $(SRC)
	$(COMP) -o runTests $(TESTS) $(SRC) $(TSTSRC) $(BLDFLAGS) 

testsMain.o : testsMain.cpp
	$(COMP) -c testsMain.cpp $(BLDFLAGS) 

test1.o : test1.cpp
	$(COMP) -c test1.cpp  $(BLDFLAGS)
	
PodInternalNetworkTests.o : PodInternalNetworkTests.cpp
	$(COMP) -c PodInternalNetworkTests.cpp  $(BLDFLAGS)

#Remove exectuable and object files
clean: 
	-rm *.o runTests
	