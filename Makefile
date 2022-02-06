CC=g++
FLAGS = -std=c++17 -lstdc++fs
TARGET= src/main.cpp src/processor.cpp
EXE = isys
all:
	$(CC) $(TARGET) -o $(EXE) $(CFLAGS) $(FLAGS)
