#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

// Initializes a stack.
//
// capacity: the max capacity of the stack
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (Node **) calloc(capacity, sizeof(Node *));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}

// Frees the stack and the memory representing the items in the stack.
//
// s: the stack to free
void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

// Returns whether the stack is empty or not.
//
// s: the stack to check
bool stack_empty(Stack *s) {
    return s->top == 0;
}

// Returns whether the stack is full or not.
//
// s: the stack to check
bool stack_full(Stack *s) {
    return s->top == s->capacity;
}

// Returns the current size of the stack (how many elements).
//
// s: the stack to check
uint32_t stack_size(Stack *s) {
    return s->top;
}

// Pushes a node into the stack.
//
// s: the stack to push the node into
// n: the node to push
bool stack_push(Stack *s, Node *n) {
    // Checks if the stack is already full
    if (stack_full(s)) {
        return false;
    }
    (s->items)[(s->top)++] = n;
    return true;
}

// Pops a node from the stack.
//
// s: the stack to pop a node from
// n: the address to store the popped node into
bool stack_pop(Stack *s, Node **n) {
    // Checks if the stack is empty
    if (stack_empty(s)) {
        return false;
    }
    *n = (s->items)[--(s->top)];
    return true;
}

// Prints the current size of the stack.
//
// s: the stack to print
void stack_print(Stack *s) {
    printf("%" PRIu32, stack_size(s));
    return;
}
