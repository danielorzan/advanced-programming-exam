CXX = g++
EXE = bst
CXXFLAGS = -std=c++14 -Wall -Wextra -g

all: $(EXE)

%.o: %.cc
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(EXE): main.o 
	$(CXX) $^ -o $(EXE) 

main.o: bst.hpp

clean:
	rm -rf src/*.o *.o $(EXE) */*~ *~ a.out*