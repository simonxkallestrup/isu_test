CXX = g++
CXXFLAGS = -Wall -g
TARGET = parts
SOURCE = main.cpp part1.cpp part2.cpp
OBJECTS = ${SOURCE:.cpp=.o}

all: ${TARGET}

${TARGET}:${OBJECTS}
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	@echo "Compiling " $@
	${CXX} -c $< ${CXXFLAGS}

help:
	@echo "available targets to use:"
	@echo all
	@echo help
	@echo clean

clean:
	rm -f ${TARGET} ${OBJECTS}

#dependencies
main.o: main.cpp part1.cpp part1.h part2.cpp part2.h
part1.o: part1.cpp part1.h
part2.o: part2.cpp part2.h