#include "node.h"
#include "pq.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint64_t min_child(Node **nodes, uint32_t first, uint32_t last);
void fix_heap(Node **nodes, uint32_t first, uint32_t last);
void swap(Node *node1, Node *node2);

struct PriorityQueue {
    uint32_t head;
    uint32_t capacity;
    Node **nodes;
};

//
// pq_create aims to dynamically allocate memory for a priority queue, set the top of the queue to 0, set the capacity to the
// given parameter, and dynamically allocate memory to store the nodes.
//
// pq_create takes 1 argument: capacity. The argument capacity represents the max number of nodes that can be stored
//in the queue.
//
// pq_create returns the dynamically allocated priority queue pointer.
//
// Note that this function is based on the ideas given by Professor Long in the assignment documentation.
//
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *queue = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (queue) {
        queue->head = 0;
        queue->capacity = capacity;
        queue->nodes = (Node **) calloc(capacity, sizeof(Node *));
        if (!queue->nodes) {
            free(queue);
            queue = NULL;
        }
    }
    return queue;
}

//
// pq_delete aims to free the priority queue and the memory representing the nodes in the queue.
//
// pq_delete takes 1 argument: q. The argument q represents a pointer to a priority queue pointer.
//
// pq_delete returns nothing/void.
//
// Note that this function is based on the ideas given by Professor Long in the assignment documentation.
//
void pq_delete(PriorityQueue **q) {
    if (*q && (*q)->nodes) {
        free((*q)->nodes);
        (*q)->nodes = NULL;
        free(*q);
        *q = NULL;
    }
    return;
}

//
// pq_empty aims to return whether the priority queue is empty or not.
//
// pq_empty takes 1 argument: q. The argument q is a pointer to a priority queue that we are checking.
//
// pq_empty returns true or false depending on whether the priority queue is empty or not.
//
// Note that this function is based on the ideas given by Professor Long in the assignment documentation.
//
bool pq_empty(PriorityQueue *q) {
    return q->head == 0;
}

//
// pq_full aims to return whether the priority queue is full or not.
//
// pq_full takes 1 argument: q. The argument q is a pointer to a priority queue that we are checking.
//
// pq_full returns true or false depending on whether the priority queue is full or not.
//
// Note that this function is based on the ideas given by Professor Long in the assignment documentation.
//
bool pq_full(PriorityQueue *q) {
    return q->head == q->capacity;
}

//
// pq_size aims to return the current size of the priority queue (how many elements).
//
// pq_size takes 1 argument: s. The argument q is a pointer to a priority queue that we are checking.
//
// pq_size returns the number of bytes in the given priority queue pointer.
//
// Note that this function is based on the ideas given by Professor Long in the assignment documentation.
//
uint32_t pq_size(PriorityQueue *q) {
    return q->head;
}

//
// enqueue aims to add a node to the given queue and put it in the correct position in the heap/queue.
//
// enqueue takes 2 arguments: q and n. Q represents the queue to remove from and n represents the node to enqueue.
//
// enqueue returns whether it was able to enqueue a node or not.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q)) {
        return false;
    }
    q->nodes[q->head++] = n;
    uint32_t child = q->head - 1, parent = child / 2;
    // Order the new node
    while (parent > 0) {
        if (q->nodes[child - 1]->frequency < q->nodes[parent - 1]->frequency) {
            swap(q->nodes[parent - 1], q->nodes[child - 1]);
            child = parent;
            parent /= 2;
        } else {
            break;
        }
    }
    return true;
}

//
// dequeue aims to remove the node that had the highest priority from the queue and fix the queue.
//
// dequeue takes 2 arguments: q and n. Q represents the queue to remove from and n represents the dequeued node.
//
// dequeue returns whether it was able to dequeue a node or not.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q)) {
        return false;
    }
    swap(q->nodes[0], q->nodes[--q->head]);
    *n = q->nodes[q->head];
    fix_heap(q->nodes, 1, q->head);
    return true;
}

//
// pq_print aims to print the current nodes of a given queue.
//
// pq_print takes 1 argument: q. The argument q represents the priority queue to check.
//
// pq_print returns nothing/void.
//
void pq_print(PriorityQueue *q) {
    for (uint32_t father = 0; father < q->head; father++) {
        printf("node:%d, freq%ld\n", q->nodes[father]->symbol, q->nodes[father]->frequency);
    }
    printf("-----------\n");
    return;
}

//
// swap aims to swap two nodes.
//
// swap takes 2 arguments: node1 and node2. Node1 and node2 represent the nodes to swap.
//
// swap returns nothing/void.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
void swap(Node *node1, Node *node2) {
    Node temp = *node1;
    *node1 = *node2;
    *node2 = temp;
    return;
}

//
// min_child aims to aim to identify the smaller child node using the idea that the child nodes of a parent node are in
// the 2kth and (2k + 1)th indices.
//
// min_child takes 3 arguments: nodes, first, and last. First and last represent the first and last indices of the heap
// array. Additionally, nodes represents the nodes array that holds the pointers to the nodes.
//
// returns an unsigned 64-bit integer that represents the index of the smaller child node
//
uint64_t min_child(Node **nodes, uint32_t first, uint32_t last) {
    // Calculates the indices of the left and right child nodes
    uint64_t left = 2 * first;
    uint64_t right = left + 1;
    if (right <= last && nodes[right - 1]->frequency < nodes[left - 1]->frequency) {
        return right;
    }
    return left;
}

//
// fix_heap aims to order the array in such a way that it fits the idea of a min heap.
//
// fix_heap takes 3 arguments: nodes, first, and last. First and last represent the first and last indices of the heap
// array. Additionally, nodes represents the nodes array that holds the pointers to the nodes.
//
// fix_heap returns nothing/void
//
void fix_heap(Node **nodes, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t parent = first;
    uint32_t child = min_child(nodes, parent, last);

    // While we have not fully ordered the heap
    while (parent <= last / 2 && !found) {
        // Swap the parent node with the child node if the child is smaller than the parent node
        if (nodes[parent - 1]->frequency > nodes[child - 1]->frequency) {
            swap(nodes[parent - 1], nodes[child - 1]);
            parent = child;
            child = min_child(nodes, parent, last);
        } else {
            // Fully ordered the heap
            found = true;
        }
    }
    return;
}
