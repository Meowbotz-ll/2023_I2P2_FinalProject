# Makefile for building a project with Allegro

# Compiler settings
CC = g++
CFLAGS = -g -Wall
INCDIR = C:/msys64/mingw64/include
LIBDIR = C:/msys64/mingw64/lib

# Allegro libraries
LIBS = -lallegro_monolith-debug

# Target executable name
TARGET = game

# Source files
SRCS = $(wildcard *.cpp)

# Object files
OBJS = $(SRCS:.cpp=.o)

# Build target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -L$(LIBDIR) -I$(INCDIR) $(LIBS)

# Compile source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Clean target
clean:
	del $(OBJS) $(TARGET).exe

# Phony targets
.PHONY: clean
