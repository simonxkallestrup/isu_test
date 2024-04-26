SRCS=PLCS.cpp
OBJS=$(SRCS:.cpp=.o)
BASEPATH=/home/stud/isu_analogescapers/OSApiStudent/Exercise_4/
CXX=g++
EXE=$(basename $(SRCS) .cpp)

# Determine whether this is a debug build or not. And if platform is Target or host.
ifdef DEBUG
    CXXFLAGS=-ggdb -O0
    LIBPATH=$(BASEPATH)/lib/host/debug
    ifeq ($(TARGET),target)
        LIBPATH=$(BASEPATH)/lib/target/debug
        CXX=arm-rpizw-g++
    endif
else
    CXXFLAGS=-O2
    LIBPATH=$(BASEPATH)/lib/host/release
    ifeq ($(TARGET),target)
        LIBPATH=$(BASEPATH)/lib/target/release
        CXX=arm-rpizw-g++
    endif
endif


# Setup the CFLAGS to ensure that the relevant warnings, includes and defines.
CXXFLAGS+=-Wall -D_REENTRANT -DOS_LINUX -I$(BASEPATH)/inc

# Note how the flags are part of the compilation of .cpp file(s).
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^
# Then again, note how the flags are NOT part of the linking process
$(EXE): $(OBJS)
	$(CXX) -o $(EXE) $(OBJS)  -L$(LIBPATH) -lOSApi -lrt -lpthread

all: $(EXE)

clean:$(OBJS)
	rm -f *.o $(EXE)