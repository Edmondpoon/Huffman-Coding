#include "huffman.h"
#include "../utils/stack.h"
#include "../io/io.h"
#include "../utils/pq.h"

static uint16_t tree_size = 0;

// Builds a Huffman tree based off a given histogram.
// Returns the root of the tree
//
// hist: histogram representing the characters in the input data
Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *queue = pq_create(ALPHABET);
    Node *node, *left, *right, *parent, *root;
    // Create a queue from a histogram
    for (uint16_t key = 0; key < ALPHABET; key++) {
        if (hist[key] > 0) {
            tree_size += 1;
            node = node_create(key, hist[key]);
            enqueue(queue, node);
        }
    }
    tree_size = (3 * tree_size) - 1;
    // Create Huffman tree
    while (pq_size(queue) > 1) {
        dequeue(queue, &left);
        dequeue(queue, &right);
        parent = node_join(left, right);
        enqueue(queue, parent);
    }
    dequeue(queue, &root);
    pq_delete(&queue);
    return root;
}

// Builds the codes for each symbol in the file.
//
// table: an array of codes for each possible character
// root : root of the huffman tree
void build_codes(Node *root, Code table[static ALPHABET]) {
    static Code code;
    if (code_size(&code) == 0) {
        code = code_init();
    }
    uint8_t popped;
    if (root) {
        // Leaf node
        if (!root->left && !root->right) {
            table[root->symbol] = code;
        // Interior node
        } else {
            // Going to the left
            code_push_bit(&code, 0);
            build_codes(root->left, table);
            code_pop_bit(&code, &popped);
            // Going to the right
            code_push_bit(&code, 1);
            build_codes(root->right, table);
            code_pop_bit(&code, &popped);
        }
    }
    return;
}

// Writes out the tree dump of the Huffman tree.
//
// outfile: the file to write the tree dump to
// root   : the root of the huffman tree
void dump_tree(int outfile, Node *root) {
    static uint8_t dump[MAX_TREE_SIZE];
    static uint64_t dump_pointer = 0;
    uint8_t leaf = 'L', interior = 'I';
    if (root) {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);
        // Leaf node
        if (!root->left && !root->right) {
            dump[dump_pointer++] = leaf;
            dump[dump_pointer++] = root->symbol;
        // Interior node
        } else {
            dump[dump_pointer++] = interior;
        }
    }
    if (dump_pointer == tree_size) {
        // Read the entire tree dump
        write_bytes(outfile, dump, tree_size);
        tree_size = 0;
    }
    return;
}

// Creates a Huffman tree given a tree dump.
// Returns the root of the tree
//
// nbytes: the number of bytes that is needed for the entire huffman tree
// tree  : an array of the tree dump
Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    Stack *stack = stack_create(nbytes);
    Node *node, *right, *left, *parent, *root;
    for (int i = 0; i < nbytes; i++) {
        // Leaf node
        if (tree[i] == 'L') {
            node = node_create(tree[++i], 0);
            stack_push(stack, node);
        // Interior node
        } else if (tree[i] == 'I') {
            stack_pop(stack, &right);
            stack_pop(stack, &left);
            parent = node_join(left, right);
            stack_push(stack, parent);
        }
    }
    stack_pop(stack, &root);
    stack_delete(&stack);
    return root;
}

// Frees all the memory that was used to create the Huffman tree via postorder traversal.
//
// root: the root node of the huffman tree
void delete_tree(Node **root) {
    if (*root) {
        delete_tree(&((*root)->left));
        delete_tree(&((*root)->right));
        if (!(*root)->left && !(*root)->right) {
            node_delete(root);
        }
    }
    return;
}
