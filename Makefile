# Compiler and linker
CXX := g++
LINKER := $(CXX)

# Name of the final executable
TARGET := mygame

# Directories
SRCDIR := .
BUILDDIR := build
INCDIR := $(SRCDIR)

# Find all .cpp files in the source directory
SOURCES := $(wildcard $(SRCDIR)/*.cpp)

# Generate object files in the build directory
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

# raylib includes and libraries
INCLUDES := -I$(INCDIR)
LIBDIRS :=

# Compiler flags
CXXFLAGS := -std=c++17 $(INCLUDES)

# Linker flags and libraries for raylib
# These flags may vary depending on your platform
ifeq ($(OS),Windows_NT)
    # Windows settings
    LDFLAGS := -lraylib -lopengl32 -lgdi32 -lwinmm
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        # Linux settings
        LDFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    endif
    ifeq ($(UNAME_S),Darwin)
        # macOS settings
        LDFLAGS := -lraylib -framework OpenGL -framework Cocoa -framework IOKit
    endif
endif

# Build rules
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	$(LINKER) -o $@ $^ $(LIBDIRS) $(LDFLAGS)

# Compile source files into object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

# Clean up build files
clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILDDIR) $(TARGET)

# Phony targets
.PHONY: all clean
