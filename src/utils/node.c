#include "node.h"
#include <stdio.h>
#include <stdlib.h>

//
// Initializes a node.
//
// symbol: the symbol of the node
// frequency: the frequency (number of occurences) of the symbol
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
// Frees the given node.
//
// n: the node to free
//
void node_delete(Node **n) {
    if (*n) {
        free(*n);
        *n = NULL;
    }
    return;
}

//
// Creates a new node that acts as the parent node of two child nodes.
//
// left: the left child node
// right: the right child node
//
Node *node_join(Node *left, Node *right) {
    Node *parent = node_create('$', left->frequency + right->frequency);
    parent->left = left;
    parent->right = right;
    return parent;
}

//
// Prints out every node associated with the given node.
//
// n: the node to start the print from
//
void node_print(Node *n) {
    if (n) {
        printf("Symbol: %d, frequency: %ld\n", n->symbol, n->frequency);
        node_print(n->left);
        node_print(n->right);
    }
    return;
}
