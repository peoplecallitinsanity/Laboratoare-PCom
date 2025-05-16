CC = gcc
SRC = iperf_client.c
BIN = client_iperf
CFLAGS = -Wall -O2

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

clean:
	rm -f $(BIN)
