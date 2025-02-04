.PHONY = all clean

all: test_suite #hozy_test

test_suite: catch.o test_suite.o test_string.o simple_string.o 
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -o test_suite catch.o test_suite.o test_string.o simple_string.o

test_suite.o: test_suite.cpp catch.hpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -c test_suite.cpp

test_string.o: test_string.cpp simple_string.o
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -c test_string.cpp

catch.o: catch.cpp catch.hpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -c catch.cpp

#hozy_test: hozy_test.cpp simple_string.o
#	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -o hozy_test hozy_test.cpp simple_string.o

simple_string.o: simple_string.cpp simple_string.hpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -c simple_string.cpp

clean:
	rm test_suite *.o
