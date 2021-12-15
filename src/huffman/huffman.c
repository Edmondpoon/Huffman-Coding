#include "huffman.h"
#include "../utils/stack.h"
#include "../io/io.h"
#include "../utils/pq.h"

static uint16_t tree_size = 0;

//
// build_tree aims to build a Huffman tree based off a given histogram.
//
// build_tree takes 1 argument: hist. The argument hist represents the histogram that contains the frequency of each
// symbol that appears in the file.
//
// build_tree returns the root of the Huffman tree.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
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

//
// build_codes aims to build the codes for each symbol in the file.
//
// build_codes takes 2 arguments: root and table. The argument root represents the root of the Huffman tree while table
// represents an array that should hold the Codes for each symbol.
//
// build_codes returns nothing/void.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
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
            code_push_bit(&code, 0);
            build_codes(root->left, table);
            code_pop_bit(&code, &popped);

            code_push_bit(&code, 1);
            build_codes(root->right, table);
            code_pop_bit(&code, &popped);
        }
    }
    return;
}

//
// dump_tree aims to write out the tree dump of the Huffman tree.
//
// dump_tree takes 2 arguments: outfile and root. The argument root represents the root of the Huffman tree while
// outfile represents the file to write to.
//
// dump_tree returns nothing/void.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
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
        write_bytes(outfile, dump, tree_size);
        tree_size = 0;
    }
    return;
}

//
// rebuild_tree aims to create a Huffman tree given a tree dump.
//
// rebuild_tree takes 2 arguments: nbytes and tree. The argument nbytes represents the number of bytes that comprise of
// the tree dump and tree represent the tree dump.
//
// rebuild_tree returns the root of the rebuilt Huffman tree.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    Stack *stack = stack_create(nbytes);
    Node *node, *right, *left, *parent, *root;
    for (int i = 0; i < nbytes; i++) {
        // Leaf node
        if (tree[i] == 'L') {
            node = node_create(tree[++i], 0);
            stack_push(stack, node);
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

//
// delete_tree aims to free all the memory that was used to create the Huffman tree.
//
// rebuild_tree takes 1 argument: root. The argument root is a pointer to a pointer to a node, which represents
// the root of the Huffman tree.
//
// rebuild_tree returns nothing/void.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
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
