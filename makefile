# Compiler
CXX     := g++

# Flags
SDL_CFLAGS := $(shell pkg-config --cflags sdl3)
SDL_LIBS   := $(shell pkg-config --libs sdl3)

CXXFLAGS := -Wall -Wextra -Werror -std=c++20 -O3 $(SDL_CFLAGS) # -fsanitize=address,undefined -static-libasan -g
LDFLAGS  := $(SDL_LIBS)

# Directories
SRC_DIR   := src
BUILD_DIR := build

PROGS := tinyhouse

# Sources
tinyhouse_SRCS := $(SRC_DIR)/main.cpp $(SRC_DIR)/chessboard.cpp

# Object Files
tinyhouse_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(tinyhouse_SRCS))

# --- Rules ---
all: $(PROGS)

tinyhouse: $(tinyhouse_OBJS)
	@echo "Linking" $@
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling" $@
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD_DIR)

.PHONY: all tinyhouse clean
