#include "io.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
static uint8_t code_buffer[BLOCK] = { 0 };
static int32_t code_index = 0;

// Reads a certain number of bytes from a given file / file descriptor.
// Returns the number of bytes read
//
// infile: the file to read the bytes from
// buf   : an array to store the read bytes into
// nbytes: the number of bytes to attempt to read 
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

// Writes a certain number of bytes in to a given file / file descriptor.
// Returns the number of bytes written
//
// outfile: the file to write the bytes in to
// buf    : an array of the bytes to write
// nbytes : the number of bytes to attempt to write
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

// Reads a bit from a given file / file descriptor.
// Returns whether a bit was able to be read
//
// infile: the file to read the bit from
// bit   : the address to store the read bit into
bool read_bit(int infile, uint8_t *bit) {
    static uint8_t buffer[BLOCK];
    static int32_t index = 0, curr_read = 0;
    if (!curr_read || !index) {
        // Refill buffer if possible
        if ((curr_read = read_bytes(infile, buffer, BLOCK)) <= 0) {
            return false;
        }
    }
    *bit = (buffer[index / 8] & (1 << (index % 8))) > 0 ? 1 : 0;
    index += 1;
    if (index == curr_read * 8) {
        // Reset buffer if we have finished reading from the current
        index = 0;
    }
    return true;
}

// Writes out the bits present in a given Code into an outfile.
//
// outfile: the file to write the codes in to
// c      : the code to write 
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

// Writes out the leftover buffered bits into an outfile.
//
// outfile: the file to write the leftover bits in to
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
