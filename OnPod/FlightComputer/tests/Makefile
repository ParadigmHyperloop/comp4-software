#Object files for tests
TESTS = testsMain.o PodInternalNetworkTests.o

#Catch Header File
TESTLIB = catch.hpp

COMP = g++


#Catch needs c++11   Add include path for the project
BLDFLAGS = -std=c++11 -I../include/FlightComputer -I.././include -lprotoc -lnsl -pthread -lprotobuf -lpthread

#Check Linux vs OSX
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	#No added flags
else
	BLDFLAGS += -stdlib=libc++ -Wl
endif



#Link to object files for project. They should be made as the project is compile prior to runing this make file
ALLSRC = $(shell find ../Debug/src -name '*.o')

#Filter out object file for main control loop to avoid multiple definitions of 'main()'
SRC = $(filter-out ../Debug/src/FlightComputer.o,$(ALLSRC))


runTests: $(TESTS) $(SRC)
	$(COMP) -o runTests $(TESTS) $(SRC) $(TSTSRC) $(BLDFLAGS)
	make test

testsMain.o : testsMain.cpp
	$(COMP) -c testsMain.cpp $(BLDFLAGS) 
	
PodInternalNetworkTests.o : PodInternalNetworkTests.cpp
	$(COMP) -c PodInternalNetworkTests.cpp  $(BLDFLAGS)


# -- ADD A TEST --
# 
# test_File_Name.o : test_File_Name.cpp
#	$(COMP) -c test_File_Name.cpp  $(BLDFLAGS)   -- make sure this line is indented with a tab
#
#   !!!  Add test_File_Name.o to the TESTS macro at the top of this file


#Remove exectuable and object files
clean: 
	-rm *.o runTests

test:
	./runTests
	make clean
