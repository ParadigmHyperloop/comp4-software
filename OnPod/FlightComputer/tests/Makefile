TESTS = testsMain.o test1.o catch.hpp

runTests: $(TESTS)
	g++ -o runTests $(TESTS) -std=c++11

testsMain.o : testsMain.cpp
	g++ -c testsMain.cpp -std=c++11

test1.o : test1.cpp
	g++ -c test1.cpp  -std=c++11