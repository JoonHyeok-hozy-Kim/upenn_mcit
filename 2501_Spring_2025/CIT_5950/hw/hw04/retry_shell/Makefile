all: roulette counter stdin_echo

counter: counter.cpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -o counter counter.cpp

roulette: roulette.cpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -o roulette roulette.cpp

stdin_echo: stdin_echo.cpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -o stdin_echo stdin_echo.cpp

retry_shell: retry_shell.cpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -o retry_shell retry_shell.cpp

tidy-check: 
	clang-tidy-15 \
        --extra-arg=--std=c++2b \
        -warnings-as-errors=* \
        -header-filter=.* \
        retry_shell.cpp

format:
	clang-format-15 -i --verbose --style=Chromium retry_shell.cpp

clean:
	rm roulette counter stdin_echo

