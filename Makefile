
# Simplified Makefile 
# Reference taken from https://gist.github.com/ecnerwala/a3c6332ac626bc448165

#+-------------------------------------+
#|                                     |
#|                                     |
#|    FILE NAME AND COMPILER CONFIG    |
#|                                     |
#|                                     |
#+-------------------------------------+

PROBLEM_NAME := $(wildcard *.cpp *.cc)
LANG := cpp
CXX = g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -Wno-unused-result -Wno-sign-conversion
DEBUG_CXXFLAGS := -fsanitize=address -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize-recover=all -fstack-protector-all -D_FORTIFY_SOURCE=2
DEBUG_CXXFLAGS += -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC
TARGET := $(basename $(PROBLEM_NAME))

EXECUTE := ./$(TARGET)

#+-------------------------------------+
#|                                     |
#|                                     |
#|     Compilation of default file     |
#|                                     |
#|                                     |
#+-------------------------------------+

compile: 
	$(CXX) $(PROBLEM_NAME) $(CXXFLAGS) -o $(TARGET)

%.res: 
	$(EXECUTE) < $*.in > $*.res

# %.out: %

CASES := $(sort $(basename $(wildcard *.in)))
TESTS := $(sort $(basename $(wildcard *.out)))

# if our dir has .out files like sample1.out and sample2.out then TESTS = sample1 sample2
# patsubst replaces everything in TESTS (sample1 sampl2) to __test__<file_name> (__test__sample1 __test__sample2)
# then this pattern rule is executed

__test__%: %.res %.out
	@echo $*.in
	@cat $*.in
	@echo "\n-----"
	@echo $*.out
	@cat $*.out
	@echo "\n-----"
	@echo $*.res
	@cat $*.res
	@echo "Diff: "
	diff $*.res $*.out 
.PHONY: __test__%

# commands test from left to right
test: compile $(patsubst %, __test__%, $(TESTS))
.PHONY: test 
