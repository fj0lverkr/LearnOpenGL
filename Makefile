CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb

BIN     := bin
SRC     := src
INCLUDE := include
RES 	:= res
SHADERS := shaders

LIBRARIES   := lib/glfw-lib/libglfw3dll.a
EXECUTABLE  := main

.PHONY: copyshaders

all: $(BIN)/$(EXECUTABLE) copyshaders

copyshaders:
	cp ./$(RES)/$(SHADERS)/* ./$(BIN)/$(RES)/$(SHADERS)/ -r -u

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/graphics/*.cpp $(SRC)/glad/glad.c
	$(CXX) -o $@ -I$(INCLUDE) $^ $(LIBRARIES) -lgdi32

clean:
	-rm $(BIN)/*
