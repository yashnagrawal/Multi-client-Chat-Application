CC = gcc
CFLAGS = -pthread
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

all: $(BIN_DIR)/main

$(BIN_DIR)/main: $(BIN_DIR)/main.o $(BIN_DIR)/server/server.o $(BIN_DIR)/server/server_utils.o $(BIN_DIR)/client/client.o $(BIN_DIR)/client/client_utils.o $(BIN_DIR)/utils.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BIN_DIR)/server/server.o: $(SRC_DIR)/server/server.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BIN_DIR)/client/client.o: $(SRC_DIR)/client/client.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BIN_DIR)/server/server_utils.o: $(SRC_DIR)/server/server_utils.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BIN_DIR)/client/client_utils.o: $(SRC_DIR)/client/client_utils.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BIN_DIR)/utils.o: $(SRC_DIR)/utils.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	find $(BIN_DIR) -name '*.o' -type f -delete
	rm -f $(BIN_DIR)/main $(BIN_DIR)/log.txt

