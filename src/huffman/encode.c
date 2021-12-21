#include "huffman.h"
#include "../io/io.h"
#include "../header.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#define OPTIONS "hvi:o:"
#define STATS   true

enum Files { INFILE, OUTFILE, TEMP };

void close_files(int64_t *files);
void encode_file(int64_t *files, uint8_t *buffer, Code *table);
void help_message(char *, int64_t files[3]);
bool check_optarg(char *optarg, int64_t files[3]);


int main(int argc, char **argv) {
    int8_t opt = 0;
    bool stats = false;
    int64_t files[3] = { STDIN_FILENO, STDOUT_FILENO, -1 };
    // Checks all flags
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v': stats = STATS; break;
        case 'i':
            if (!check_optarg(optarg, files)) {
                return EXIT_FAILURE;
            }
            files[INFILE] = open(optarg, O_RDONLY);
            if (files[INFILE] < 0) {
                help_message("Invalid file.\n", files);
                return EXIT_FAILURE;
            }
            break;
        case 'o':
            if (!check_optarg(optarg, files)) {
                return EXIT_FAILURE;
            }
            files[OUTFILE] = open(optarg, O_RDWR | O_CREAT | O_TRUNC);
            if (files[OUTFILE] < 0) {
                help_message("Invalid file.\n", files);
                return EXIT_FAILURE;
            }
            break;
        case 'h': help_message("", files); return EXIT_SUCCESS;
        default: help_message("", files); return EXIT_FAILURE;
        }
    }
    if (files[INFILE] == STDIN_FILENO) {
        files[TEMP] = open("/tmp/read", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG);
    }

    uint16_t unique = 2;
    uint8_t buffer[BLOCK] = { 0 };
    uint64_t curr_read, histogram[ALPHABET] = { 0 };
    histogram[0] += 1;
    histogram[ALPHABET - 1] += 1;
    // creates the histogram for the Huffman tree
    while ((curr_read = read_bytes(files[INFILE], buffer, BLOCK)) > 0) {
        if (files[TEMP] != -1) {
            uint32_t temporary = write_bytes(files[TEMP], buffer, curr_read);
            // Needed to remove the bytes written to the temporary file
            bytes_written -= temporary;
        }
        for (uint64_t i = 0; i < curr_read; i++) {
            if (histogram[buffer[i]] == 0) {
                unique += 1;
            }
            histogram[buffer[i]] += 1;
        }
    }
    Node *root = build_tree(histogram);
    Code table[ALPHABET];
    build_codes(root, table);

    // writes the header
    struct stat sb;
    fstat(files[INFILE], &sb);
    uint64_t file_size = files[TEMP] == -1 ? sb.st_size : bytes_read;
    uint16_t permissions = sb.st_mode, tree_size = (3 * unique) - 1;
    Header header = { MAGIC, permissions, tree_size, file_size };
    if (files[OUTFILE] != STDOUT_FILENO) {
        fchmod(files[OUTFILE], sb.st_mode);
    }
    write_bytes(files[OUTFILE], (uint8_t *) &header, sizeof(header));

    dump_tree(files[OUTFILE], root);
    // writes codes for every symbol
    lseek(files[TEMP] != -1 ? files[TEMP] : files[INFILE], 0, SEEK_SET);
    encode_file(files, buffer, table);
    flush_codes(files[OUTFILE]);

    // Stats print
    if (stats) {
        fprintf(stderr, "Uncompressed file size: %ld bytes\n", bytes_read / 2);
        fprintf(stderr, "Compressed file size: %ld bytes\n", bytes_written);
        fprintf(
            stderr, "Space saving: %.2lf%%\n", 100 * (1 - (bytes_written / (bytes_read / 2.0))));
    }
    delete_tree(&root);
    close_files(files);
    return 0;
}

//
// encode_file simply writes the codes for every symbol in an infile.
//
// encode_file takes 3 arguments: files, buffer, and table. Files is an array of file descriptors (infile and outfile)
// while buffer is a buffer to hold the read bytes. Additionally, table is an array of Codes for every symbol.
//
// encode_file returns nothing/void.
//
void encode_file(int64_t *files, uint8_t *buffer, Code *table) {
    uint64_t curr_read = 0, file = files[INFILE] == STDIN_FILENO ? files[TEMP] : files[INFILE];
    while ((curr_read = read_bytes(file, buffer, BLOCK)) > 0) {
        for (uint64_t i = 0; i < curr_read; i++) {
            write_code(files[OUTFILE], &table[buffer[i]]);
        }
    }
    return;
}

//
// Closes file descriptors.
//
// files: an array of file descriptors
//
void close_files(int64_t *files) {
    if (files[INFILE] != STDIN_FILENO) {
        close(files[INFILE]);
    }
    if (files[OUTFILE] != STDOUT_FILENO) {
        close(files[OUTFILE]);
    }
    if (files[TEMP] != -1) {
        close(files[TEMP]);
        remove("/tmp/read");
    }
}

//
// Prints out the help message that describes how to use the program
//
void help_message(char *error, int64_t files[3]) {
    if (*error != '\0') {
        fprintf(stderr, "%s", error);
    }
    close_files(files);
    fprintf(stderr, "SYNOPSIS\n"
                    "  A word filtering program for the GPRSC.\n"
                    "  Filters out and reports bad words parsed from stdin.\n\n"
                    "USAGE\n"
                    "  ./banhammer [-hs] [-t size] [-f size]\n\n"
                    "OPTIONS\n"
                    "  -h           Program usage and help.\n"
                    "  -s           Print program statistics.\n"
                    "  -t size      Specify hash table size (default: 2^16).\n"
                    "  -f size      Specify Bloom filter size (default: 2^20).\n");
    return;
}

//
// Ensure a flag that needs an argument has an argument.
//
// optarg: represents the input for the flag
// files: an array of file descriptors
//
bool check_optarg(char *optarg, int64_t files[3]) {
    if (!optarg) {
        help_message("", files);
        return false;
    }
    return true;
}
