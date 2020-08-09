CXX		  := clang++
# -g adds debugging info. -Wall includes all warnings. -W controls verbosity. Extra just adds a little more to all warnings.
CXX_FLAGS := -Wall -Wextra -std=c++17 -stdlib=libc++ -g

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= /usr/local/lib
USR_LOCAL_INCLUDE := /usr/local/include
LINKER_FLAGS := -lSDL2 -lSDL2_Image

EXECUTABLE	:= main
CLANG_STYLE ?= google

.PHONY: run

# Each target may have dependencies on the other side of :.
# The next line(s), if any, are the specific commands needed to generate the target. 
run: $(BIN)/$(EXECUTABLE)
	./$(BIN)/$(EXECUTABLE)
	
# Most targets in a Makefile are files. Phony says, "The target has nothing to do with a file by that name."
# Phony targets are always out of date, so the make utility will run them regardless of file system state.
# https://stackoverflow.com/questions/2145590/what-is-the-purpose-of-phony-in-a-makefile
.PHONY: all

# Building "all" in this case is satisfied by building our top-level executable.
# According to GNU docs, target all should compile the entire program.
# https://stackoverflow.com/questions/2514903/what-does-all-stand-for-in-a-makefile
all: $(BIN)/$(EXECUTABLE)

# So here, the top-level executable depends on all *.cpp files are depedencies for compilation.
# And the command to generate the executable compiles those files, including their headers, and pulling in any external libraries.
# The $^ represents what's on the right side of :, and the $@ what's on the left.
$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -I$(USR_LOCAL_INCLUDE) -L$(LIB) $(LINKER_FLAGS) $^ -o $@

.PHONY: clean

clean:
	-rm -Rf $(BIN)/*

set-format:
	clang-format -style=${CLANG_STYLE} -dump-config > .clang-format