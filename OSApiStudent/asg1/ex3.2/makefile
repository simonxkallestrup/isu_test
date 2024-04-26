SOURCES=main.cpp part1.cpp part2.cpp
OBJECTS=$(addprefix $(BUILD_DIR)/, $(SOURCES:.cpp=.o))
DEPS=$(addprefix $(BUILD_DIR)/, $(SOURCES:.cpp=.d))
EXE=prog
CXXFLAGS=-I.

ARCH?=x86-64

# Making for x86 -64 e.g. x86 -64 (the architecture employed)
# > make ARCH=x86 -64
ifeq (${ARCH},x86-64)
CXX=g++
BUILD_DIR=build/x86-64
BIN_DIR=bin/x86-64 #Placerer exe filen i bin/x86-64
endif

# Making for architecture
# > make ARCH=arm
ifeq (${ARCH},arm)
CXX=arm-rpizw-g++
BUILD_DIR=build/arm
BIN_DIR=bin/arm #Placerer exe filen i bin/arm
endif

$(BIN_DIR)/$(EXE): $(DEPS) $(OBJECTS) # << Check the $(DEPS) new dependency
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# %.cpp -> %.o needs to be added! Target is NOT just %.o...
# Rule that describes how a .d (dependency) file is created from a .cpp file
# Similar to the assigment that you just completed %.cpp -> %.o
$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) -c $< -o $@

$(BUILD_DIR)/%.d: %.cpp
	mkdir -p $(dir $@)
	$(CXX) -MT$@ -MM $(CXXFLAGS) $< > $@
	$(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $< >> $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d $(BIN_DIR)/$(EXE)