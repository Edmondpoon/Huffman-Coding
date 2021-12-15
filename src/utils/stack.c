#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

//
// stack_create aims to dynamically allocate memory for a stack, set the top of the stack to 0, set the capacity to the
// given parameter, and dynamically allocate memory to store the stack items.
//
// stack_create takes 1 argument: capacity. The argument capacity represents the max number of items that can be stored
//in the stack.
//
// stack_create returns the dynamically allocated stack pointer.
//
// Note that the following code is from Professor Long given in the assignment document.
//
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

//
// stack_delete aims to free the stack and the memory representing the items in the stack.
//
// stack_delete takes 1 argument: s. The argument s represents a pointer to a stack pointer.
//
// stack_delete returns nothing/void.
//
// Note that the following code is from Professor Long given in the assignment document.
//
void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

//
// stack_empty aims to return whether the stack is empty or not.
//
// stack_empty takes 1 argument: s. The argument s is a pointer to a stack that we are checking.
//
// stack_empty returns true or false depending on whether the stack is empty or not.
//
// Note that this function is based on the ideas given by Professor Long in the assignment documentation.
//
bool stack_empty(Stack *s) {
    return s->top == 0;
}

//
// stack_full aims to return whether the stack is full or not.
//
// stack_full takes 1 argument: s. The argument s is a pointer to a stack that we are checking.
//
// stack_full returns true or false depending on whether the stack is full or not.
//
// Note that this function is based on the ideas given by Professor Long in the assignment documentation.
//
bool stack_full(Stack *s) {
    return s->top == s->capacity;
}

//
// stack_size aims to return the current size of the stack (how many elements).
//
// stack_size takes 1 argument: s. The argument s is a pointer to a stack that we are checking.
//
// stack_size returns the number of bytes in the given stack pointer.
//
// Note that this function is based on the ideas given by Professor Long in the assignment documentation.
//
uint32_t stack_size(Stack *s) {
    return s->top;
}

//
// stack_push aims to push a node into the stack.
//
// stack_push takes 2 arguments: s and node. The argument stats is a pointer to a struct and node represents the node to push.
//
// stack_push returns true or false depending on whether it was able to push the node or not.
//
// Note that this function is based on the ideas given by Professor Long in the assignment documentation.
//
bool stack_push(Stack *s, Node *n) {
    // Checks if the stack is already full
    if (stack_full(s)) {
        return false;
    }
    (s->items)[(s->top)++] = n;
    return true;
}

//
// stack_pop aims to pop a node from the stack.
//
// stack_pop takes 2 arguments: s and node. The argument stats is a pointer to a struct and node represents the node that
// was popped from the stack.
//
// stack_pop returns true or false depending on whether it was able to pop a node or not.
//
// Note that this function is based on the ideas given by Professor Long in the assignment documentation.
//
bool stack_pop(Stack *s, Node **n) {
    // Checks if the stack is empty
    if (stack_empty(s)) {
        return false;
    }
    *n = (s->items)[--(s->top)];
    return true;
}

//
// stack_print aims to print the current size of the stack.
//
// stack_print takes 1 argument: s. S represents the stack to print.
//
// stack_print returns nothing/void.
//
void stack_print(Stack *s) {
    printf("%d", stack_size(s));
    return;
}
