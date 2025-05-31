CC = g++
CFLAGS = -Iinclude -std=c++17 -Wall -Wextra
SRC = src/main.cpp src/evaluator.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = evaluator

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run