#include "huffman.h"
#include "../io/io.h"
#include "../header.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#define OPTIONS "hvi:o:"
#define STATS   true

enum Files { INFILE, OUTFILE };
enum Paths { LEFT, RIGHT };

void help_message(void);
void close_files(int64_t *files);


int main(int argc, char **argv) {
    int8_t opt = 0;
    bool stats = false;
    int64_t files[2] = { STDIN_FILENO, STDOUT_FILENO };
    // Checks all flags
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v': stats = STATS; break;
        case 'i':
            if (!optarg) {
                close_files(files);
                help_message();
                return 1;
            }
            files[INFILE] = open(optarg, O_RDONLY);
            if (files[INFILE] < 0) {
                perror("Invalid file:");
                close_files(files);
                help_message();
                return 1;
            }
            break;
        case 'o':
            if (!optarg) {
                close_files(files);
                help_message();
                return 1;
            }
            files[OUTFILE] = open(optarg, O_RDWR | O_CREAT | O_TRUNC);
            if (files[OUTFILE] < 0) {
                close_files(files);
                perror("Invalid file");
                help_message();
                return 1;
            }
            break;
        case 'h':
            close_files(files);
            help_message();
            return 0;
        default:
            close_files(files);
            help_message();
            return 1;
        }
    }
    Header header;
    // Ensure the header is valid and the input file is valid
    if (read_bytes(files[INFILE], (uint8_t *) &header, sizeof(header)) < 2) {
        fprintf(stderr, "Unable to read header.\n");
        help_message();
        return 1;
    } else if (header.magic != MAGIC) {
        fprintf(stderr, "Invalid magic number.\n");
        help_message();
        return 1;
    }

    if (files[OUTFILE] != STDOUT_FILENO) {
        fchmod(files[OUTFILE], header.permissions);
    }
    uint8_t tree[header.tree_size];
    read_bytes(files[INFILE], tree, header.tree_size);

    Node *root = rebuild_tree(header.tree_size, tree);
    Node *current = root;
    if (!root) {
        close_files(files);
        delete_tree(&root);
        fprintf(stderr, "Invalid Huffman encoding.\n");
        return 1;
    }

    // Decodes encoded file
    uint8_t bit, buffer[BLOCK];
    uint64_t index = 0, symbols = 0;
    while (read_bit(files[INFILE], &bit) && (symbols < header.file_size)) {
        if (current) {
            switch (bit) {
            case LEFT: current = current->left; break;
            case RIGHT: current = current->right; break;
            default: break;
            }
            if (current && !current->left && !current->right) {
                if (index == BLOCK) {
                    write_bytes(files[OUTFILE], buffer, index);
                    index = 0;
                }
                buffer[index++] = current->symbol;
                symbols += 1;
                current = root;
            }
        }
    }

    write_bytes(files[OUTFILE], buffer, (symbols % BLOCK));
    // Prints stats
    if (stats) {
        fprintf(stderr, "Compressed file size: %ld bytes\n", bytes_read);
        fprintf(stderr, "Uncompressed file size: %ld bytes\n", bytes_written);
        fprintf(
            stderr, "Space saving: %.2lf%%\n", 100 * (1 - (bytes_read / (bytes_written * 1.0))));
    }
    close_files(files);
    delete_tree(&root);
    return 0;
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
}

//
// Prints out the help message that describes how to use the program
//
void help_message(void) {
    printf("SYNOPSIS\n"
           "  A Huffman decoder."
           "  Decompresses a file using the Huffman coding algorithm.\n\n"
           "USAGE\n"
           "  ./decode [-h] [-i infile] [-o outfile]\n\n"
           "OPTIONS\n"
           "  -h             Program usage and help.\n"
           "  -v             Print compression statistics.\n"
           "  -i infile      Input file to decompress.\n"
           "  -o outfile     Output of decompressed data.\n");
    return;
}
