TESTS = testsMain.o tests1.o

runTests: $(TESTS)
	g++ -o runTests $(TESTS)

testsMain.o : testsMain.cpp
	g++ -c testsMain.cpp -std=c++11

test1.o : test1.cpp
	g++ -c test1.cpp 