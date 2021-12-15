#include "node.h"
#include <stdio.h>
#include <stdlib.h>

//
// node_create aims to dynamically allocate memory for a node and set the symbol and frequency to their respective
// parameters.
//
// node_create takes 2 argument: symbol and frequency. The argument symbol represents the symbol of the node and
// frequency represents the frequency of the symbol in the file.
//
// node_create returns the dynamically allocated node pointer.
//
// Note that this function is based on the ideas given by Professor Long in the assignment documentation.
//
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *node = (Node *) calloc(1, sizeof(Node));
    if (node) {
        node->symbol = symbol;
        node->frequency = frequency;
    }
    return node;
}

//
// node_delete aims to free the given node.
//
// node_delete takes 1 argument: n. The argument n represents a pointer to a node pointer.
//
// node_delete returns nothing/void.
//
// Note that this function is based on the ideas given by Professor Long in the assignment documentation.
//
void node_delete(Node **n) {
    if (*n) {
        free(*n);
        *n = NULL;
    }
    return;
}

//
// node_join aims to create a new node that acts as the parent node of two child nodes.
//
// node_join takes 2 arguments: left and right. Left and right represent the left and right child nodes.
//
// node_join returns the created parent node.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
Node *node_join(Node *left, Node *right) {
    Node *parent = node_create('$', left->frequency + right->frequency);
    parent->left = left;
    parent->right = right;
    return parent;
}

//
// node_print aims to print out every node associated with the given node.
//
// node_print takes 1 argument: n. The argument n represents the node to check.
//
// node_print returns nothing/void
//
void node_print(Node *n) {
    if (n) {
        printf("Symbol: %d, frequency: %ld\n", n->symbol, n->frequency);
        node_print(n->left);
        node_print(n->right);
    }
    return;
}
