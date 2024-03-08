CXX=g++

INCLUDE=/usr/local/include
CXXFLAGS=-I$(INCLUDE)

COMPUTER_SOURCES := $(wildcard computer/*.cpp)
COMPUTER_OBJECTS := $(patsubst computer/%.cpp, build/obj/computer/%.o, $(COMPUTER_SOURCES))

COMPILER_SOURCES := $(wildcard compiler/*.cpp)
COMPILER_OBJECTS := $(patsubst compiler/%.cpp, build/obj/compiler/%.o, $(COMPILER_SOURCES))

all: $(COMPUTER_OBJECTS) $(COMPILER_OBJECTS) build/bin/computer build/bin/compiler

build/obj/computer/%.o: computer/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/obj/compiler/%.o: compiler/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/bin/computer: build/obj/computer/*.o
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^

build/bin/compiler: build/obj/compiler/*.o
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^