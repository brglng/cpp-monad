.PHONY: all test

all:
	@echo "use \"make test\""

monad-test: test.cpp monad.h optional
	@g++ -W -Wall -O2 -std=c++11 -isystem . test.cpp -o monad-test

test: monad-test
	@./monad-test
