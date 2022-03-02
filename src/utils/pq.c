#include "pq.h"
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

// Initializes a priority queue.
//
// capacity: the capacity of the priority queue
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

// Frees the priority queue and the memory representing the nodes in the queue.
//
// q: the priority queue to free
void pq_delete(PriorityQueue **q) {
    if (*q && (*q)->nodes) {
        free((*q)->nodes);
        (*q)->nodes = NULL;
        free(*q);
        *q = NULL;
    }
    return;
}

// pq_empty aims to return whether the priority queue is empty or not.
//
// q: the queue to check
bool pq_empty(PriorityQueue *q) {
    return q->head == 0;
}

// Returns whether the priority queue is full or not.
//
// q: the priority queue to check
bool pq_full(PriorityQueue *q) {
    return q->head == q->capacity;
}

// Returns the current size of the priority queue (how many elements).
//
// q: the priority queue to check
uint32_t pq_size(PriorityQueue *q) {
    return q->head;
}

// Adds a node to the given queue and put it in the correct position in the heap/queue.
// 
// q: the priority queue to add the node to
// n: the node to add
bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q)) {
        return false;
    }
    q->nodes[q->head++] = n;
    uint32_t child = q->head - 1, parent = child / 2;
    // Order the new node
    while (parent > 0) {
        // Checks parent nodes 
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

// Removes the node that had the highest priority from the queue and fix the queue.
//
// q: the priority queue to dequeue from
// n: the address to store the dequeued node into
bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q)) {
        return false;
    }
    swap(q->nodes[0], q->nodes[--q->head]);
    *n = q->nodes[q->head];
    fix_heap(q->nodes, 1, q->head);
    return true;
}

// Prints the current nodes of a given priority queue.
//
// q: the priority queue to print
void pq_print(PriorityQueue *q) {
    for (uint32_t father = 0; father < q->head; father++) {
        printf("node: %" PRIu8 ", freq%" PRIu64 "\n", q->nodes[father]->symbol, q->nodes[father]->frequency);
    }
    printf("-----------\n");
    return;
}

// Swap two nodes.
//
// node1: the first node to swap
// node2: the second node to swap
void swap(Node *node1, Node *node2) {
    Node temp = *node1;
    *node1 = *node2;
    *node2 = temp;
    return;
}

// Identifies the smaller child node using the idea that the child nodes of a parent node are in the 2kth and (2k + 1)th indices.
//
// nodes: an array of nodes
// first: the first index of the array
// last : the last index of the array
uint64_t min_child(Node **nodes, uint32_t first, uint32_t last) {
    // Calculates the indices of the left and right child nodes
    uint64_t left = 2 * first;
    uint64_t right = left + 1;
    if (right <= last && nodes[right - 1]->frequency < nodes[left - 1]->frequency) {
        return right;
    }
    return left;
}

// Orders the array in such a way that it fits the idea of a min heap.
//
// nodes: an array of nodes
// first: the first index of the array
// last : the last index of the array
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
