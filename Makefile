CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
OBJS = io.o node.o huffman.o pq.o code.o stack.o
ENCODE = encode.o
DECODE = decode.o

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

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make	
