.PHONY = all clean

all: test_suite 

test_suite: test_suite.o catch.o test_suite.o hash_table.o test_hash_table.o
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -o $@ $^ 

test_suite.o: test_suite.cpp catch.hpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -c test_suite.cpp

test_hash_table.o: test_hash_table.cpp catch.hpp hash_table.hpp 
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -c $< 

hash_table.o: hash_table.hpp hash_table.cpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -c hash_table.cpp


catch.o: catch.cpp catch.hpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -c catch.cpp

#hozy_test: hozy_test.cpp hash_table.o
#	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -o hozy_test hozy_test.cpp hash_table.o



tidy-check: 
	clang-tidy-15 \
        --extra-arg=--std=c++2b \
        -warnings-as-errors=* \
        -header-filter=.* \
        hash_table.cpp

format:
	clang-format-15 -i --verbose --style=Chromium hash_table.cpp

clean:
	rm *.o test_suite
