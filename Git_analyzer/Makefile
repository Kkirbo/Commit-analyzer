CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGET = git_analyzer
SRC = src/main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) output/commits.txt output/stats.txt output/graph.png