# Makefile for 2D Paint Program
# Compatible with Wayland Hyprland and X11
#
# Usage:
#   make        - Build the program
#   make run    - Build and run the program
#   make clean  - Remove build artifacts
#   make help   - Show help message
#
# Note: The executable will be created in the root directory.
#       Run it from the root directory so it can find the shaders/ folder.

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Include directories
INCLUDES = -I./include

# Source files directory
SRC_DIR = source

# Source files
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/Application.cpp \
          $(SRC_DIR)/Renderer.cpp \
          $(SRC_DIR)/Shader.cpp \
          $(SRC_DIR)/VertexArray.cpp \
          $(SRC_DIR)/Layer.cpp \
          $(SRC_DIR)/IndexBuffer.cpp \
          $(SRC_DIR)/Rectangle.cpp \
          $(SRC_DIR)/Triangle.cpp \
          $(SRC_DIR)/Circle.cpp \
          $(SRC_DIR)/Polygon.cpp \
          $(SRC_DIR)/glad.c

# Object files directory
OBJ_DIR = build
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJECTS := $(OBJECTS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Libraries for Wayland and X11
LIBS = -lglfw -lGL -lGLU -ldl -lpthread -lwayland-client -lwayland-cursor -lwayland-egl -lX11

# Output executable
TARGET = Madpaint

# Create object directory if it doesn't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJ_DIR) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LIBS)
	@echo ""
	@echo "=========================================="
	@echo "Build complete! Executable: ./$(TARGET)"
	@echo "=========================================="
	@echo ""

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Clean complete! Removed build/ directory and $(TARGET)"

# Rebuild everything
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

# Help target
help:
	@echo "=========================================="
	@echo "2D Paint Program - Makefile Help"
	@echo "=========================================="
	@echo ""
	@echo "Available targets:"
	@echo "  make        - Build the program"
	@echo "  make run    - Build and run the program"
	@echo "  make clean  - Remove build artifacts"
	@echo "  make rebuild - Clean and rebuild"
	@echo "  make help   - Show this help message"
	@echo ""
	@echo "Requirements:"
	@echo "  - g++ compiler"
	@echo "  - GLFW library"
	@echo "  - OpenGL libraries"
	@echo "  - Wayland libraries (for Wayland/Hyprland)"
	@echo "  - X11 libraries (for X11 compatibility)"
	@echo ""
	@echo "Note: Run the executable from the root directory"
	@echo "      so it can find the shaders/ folder."
	@echo ""

.PHONY: all clean rebuild run help

