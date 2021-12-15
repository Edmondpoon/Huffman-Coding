#include "io.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
static uint8_t code_buffer[BLOCK] = { 0 };
static int32_t code_index = 0;

//
// read_bytes aims to read a certain number of bytes from a given file / file descriptor.
//
// read_bytes takes 3 arguments: infile, buf, and nbytes. Infile represents a file descriptor to read from while buf
// represents the buffer to store the read bytes into. Lastly, nbytes represents the number of bytes to read.
//
// read_bytes returns the number of bytes read.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int32_t curr_read = 0;
    // Read nbytes bytes
    while (curr_read < nbytes) {
        int value = read(infile, &buf[curr_read], nbytes - curr_read);
        if (value <= 0) {
            break;
        }
        curr_read += value;
    }
    bytes_read += curr_read;
    return curr_read;
}

//
// write_bytes aims to write a certain number of bytes into a given file / file descriptor.
//
// write_bytes takes 3 arguments: outfile, buf, and nbytes. Outfile represents a file descriptor to write into while buf
// represents the buffer that holds the bytes to write. Lastly, nbytes represents the number of bytes to write.
//
// write_bytes returns the number of bytes written.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int32_t curr_written = 0;
    // Write nbytes bytes
    while (curr_written < nbytes) {
        ssize_t value = write(outfile, &buf[curr_written], nbytes - curr_written);
        if (value <= 0) {
            break;
        }
        curr_written += value;
    }
    bytes_written += curr_written;
    return curr_written;
}

//
// read_bit aims to read a bit from a given file / file descriptor.
//
// read_bit takes 2 arguments: infile and bit. Infile represents a file descriptor to read from while bit represents the
// bit we read.
//
// read_bit returns whether there are any more bits to read or not.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
bool read_bit(int infile, uint8_t *bit) {
    static uint8_t buffer[BLOCK];
    static int32_t index = 0, curr_read = 0;
    if (!curr_read || !index) {
        if ((curr_read = read_bytes(infile, buffer, BLOCK)) <= 0) {
            return false;
        }
    }
    *bit = (buffer[index / 8] & (1 << (index % 8))) > 0 ? 1 : 0;
    index += 1;
    if (index == curr_read * 8) {
        index = 0;
    }
    return true;
}

//
// write_code aims to write out the bits present in a given Code into an outfile.
//
// write_code takes 2 arguments: infile and c. Infile represents a file descriptor to write into while c represents the
// Code to take the bits from.
//
// write_code returns nothing/void.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
void write_code(int outfile, Code *c) {
    // Write full code
    for (uint32_t i = 0; i < code_size(c); i++) {
        if (code_index == BLOCK * 8) {
            flush_codes(outfile);
        }
        if (code_get_bit(c, i)) {
            code_buffer[code_index / 8] |= (1 << (code_index % 8));
        } else {
            code_buffer[code_index / 8] &= ~(1 << (code_index % 8));
        }
        code_index += 1;
    }
    return;
}

//
// flush_code aims to write out the leftover buffered bits into an outfile.
//
// flush_code takes 1 arguments: outfile. Outfile represents a file descriptor to write into.
//
// flush_code returns nothing/void.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
void flush_codes(int outfile) {
    // Zero rest of the byte if we are not at the end of the byte
    if (code_index % 8 > 0) {
        uint32_t next_byte = (8 - (code_index % 8));
        for (uint32_t i = code_index; i < code_index + next_byte; i++) {
            code_buffer[i / 8] &= ~(1 << (i % 8));
        }
        code_index += next_byte;
    }
    write_bytes(outfile, code_buffer, code_index / 8);
    code_index = 0;
    return;
}
