.PHONY = all clean

all: test_suite #hozy_test_vec

test_suite: catch.o test_suite.o test_vec.o vec.o
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -o test_suite test_suite.o catch.o test_vec.o vec.o

test_suite.o: test_suite.cpp catch.hpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -c test_suite.cpp

#hozy_test_vec: hozy_test_vec2.cpp vec.o
#	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -o hozy_test_vec hozy_test_vec2.cpp vec.o

test_vec.o: test_vec.cpp vec.o catch.o
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -c test_vec.cpp

vec.o: vec.cpp vec.hpp catch.hpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -c vec.cpp

catch.o: catch.cpp catch.hpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -c catch.cpp

clean:
	rm *.o test_suite
