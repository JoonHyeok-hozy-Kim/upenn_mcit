.PHONY = all clean

all: check-time

check-time: check-time.o ctypes.o simple_string.o
	clang++-15 -g3 -gdwarf-4 --std=c++2b -Wall -o check-time check-time.o ctypes.o simple_string.o

simple_string.o: simple_string.cpp simple_string.hpp
	clang++-15 -g3 -gdwarf-4 --std=c++2b -Wall -c simple_string.cpp

check-time.o: check-time.cpp ctypes.hpp simple_string.hpp
	clang++-15 -g3 -gdwarf-4 --std=c++2b -Wall -c check-time.cpp 

ctypes.o: ctypes.cpp ctypes.hpp
	clang++-15 -g3 -gdwarf-4 --std=c++2b -Wall -c ctypes.cpp

clean:
	rm check-time *.o
