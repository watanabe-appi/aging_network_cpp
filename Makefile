TARGET=a.out
all: $(TARGET)
SRC=$(shell ls *.cpp)
OBJ=$(SRC:.cpp=.o)

CC=g++
CPPFLAGS=-O3 -march=native -Wall -Wextra -std=c++11 

-include Makefile.opt

$(TARGET): $(OBJ)
	$(CC) $(CPPFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@


.PHONY: clean dep

dep:
	g++ -MM *.cpp > Makefile.dep

clean:
	rm -f $(TARGET) $(OBJ)

-include Makefile.dep
