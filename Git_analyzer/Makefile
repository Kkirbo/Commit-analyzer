CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGET = git_analyzer
SRC = src/main.c

OUTPUT_DIR = output

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(OUTPUT_DIR)/*.txt
	rm -f $(OUTPUT_DIR)/*.png
	rm -f graph.png