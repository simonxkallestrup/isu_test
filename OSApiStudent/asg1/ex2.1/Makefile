TARGET = hello

all: $(TARGET)

$(TARGET): $(TARGET).o
	$(CXX) -o $(TARGET) $(TARGET).o

$(TARGET).o: $(TARGET).cpp
	$(CXX) -c $(TARGET).cpp

clean:
	rm -f $(TARGET) $(TARGET).o

help:
	@echo "Mulige targets:"
	@echo "  all:		Bygger programmet"
	@echo "  clean:	Sletter program og objektfiler"
	@echo "  help:		Printer denne target-oversigt"