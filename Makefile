CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

IO = ./src/io/
HUFF = ./src/huffman/
UTILS = ./src/utils/

ENCODE = $(HUFF)encode.o
DECODE = $(HUFF)decode.o
OBJS = $(IO)io.o $(HUFF)huffman.o $(UTILS)code.o $(UTILS)pq.o $(UTILS)stack.o $(UTILS)node.o


.PHONY: all clean format scan-build

all: encode decode

encode: $(OBJS) $(ENCODE)
	$(CC) -o $@ $(OBJS) $(ENCODE)

decode: $(OBJS) $(DECODE)
	$(CC) -o $@ $(OBJS) $(DECODE)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f encode decode *.o

scan-build: clean
	scan-build --use-cc=$(CC) make	
