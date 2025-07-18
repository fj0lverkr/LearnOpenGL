CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   := lib/glfw-lib/libglfw3dll.a
EXECUTABLE  := main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/glad/glad.c
	$(CXX) -o $@ -I$(INCLUDE) $^ $(LIBRARIES) -lgdi32

clean:
	-rm $(BIN)/*
