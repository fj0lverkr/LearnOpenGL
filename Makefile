CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb

BIN     := bin
SRC     := src
INCLUDE := include
RES 	:= res
SHADERS := shaders
TEXTURES:= textures
GLFWDLL := lib/glfw-lib/glfw3.dll

LIBRARIES   := lib/glfw-lib/libglfw3dll.a
EXECUTABLE  := main

.PHONY: copyshaders copytextures copydlls

all: $(BIN)/$(EXECUTABLE) copydlls copyshaders copytextures

copyshaders:
	cp ./$(RES)/$(SHADERS)/* ./$(BIN)/$(RES)/$(SHADERS)/ -r -u

copytextures:
	cp ./$(RES)/$(TEXTURES)/* ./$(BIN)/$(RES)/$(TEXTURES)/ -r -u

copydlls:
	cp ./$(GLFWDLL) ./$(BIN) -u

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/graphics/*.cpp $(SRC)/util/*.cpp $(SRC)/glad/glad.c
	$(CXX) -o $@ -I$(INCLUDE) $^ $(LIBRARIES) -lgdi32

clean:
	-rm $(BIN)/*
