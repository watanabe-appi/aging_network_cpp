TARGET=aging_simulation
all: $(TARGET)
SRC=$(shell ls *.cpp)
OBJ=$(SRC:.cpp=.o)

CC=g++
CPPFLAGS=-O3 -march=native -Wall -Wextra -std=c++11 
#CPPFLAGS+= -g

-include Makefile.opt

$(TARGET): $(OBJ)
	$(CC) $(CPPFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@


.PHONY: clean dep

dep:
	g++ -MM -std=c++11 *.cpp > Makefile.dep

clean:
	rm -f $(TARGET) $(OBJ)

-include Makefile.dep
