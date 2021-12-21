#include "code.h"
#include <fcntl.h>
#include <stdio.h>

//
// Initializes a code instance.
//
Code code_init(void) {
    Code code = { 0, { 0 } };
    return code;
}

//
// Returns the size of the given Code.
//
// c: the code to check
//
uint32_t code_size(Code *c) {
    return c->top;
}

//
// Returns whether the given Code is empty or not.
//
// c: the code to check
//
bool code_empty(Code *c) {
    return c->top == 0;
}

//
// Returns  whether the given Code is full or not.
//
// c: the code to check
//
bool code_full(Code *c) {
    return c->top == ALPHABET;
}

//
// Sets a bit of a Code at a given index position to 1.
//
// c: the code to alter
// i: the index of the bit to set
//
bool code_set_bit(Code *c, uint32_t i) {
    if (i / 8 > MAX_CODE_SIZE) {
        return false;
    }
    // Got the bit shift example from Eugene's section
    c->bits[i / 8] |= 1 << (i % 8);
    return true;
}

//
// Sets a bit of a Code at a given index position to 1.
//
// c: the code to alter
// i: the index of the bit to clear
//
bool code_clr_bit(Code *c, uint32_t i) {
    if (i / 8 > MAX_CODE_SIZE) {
        return false;
    }
    // Got the bit shift example from Eugene's section
    c->bits[i / 8] &= ~(1 << (i % 8));
    return true;
}

//
// Returns whether the bit of a Code at a given index is equal to 1.
//
// c: the code to check
// i: the index of the bit to check
//
bool code_get_bit(Code *c, uint32_t i) {
    if (i / 8 < MAX_CODE_SIZE && (c->bits[i / 8] & (1 << (i % 8))) >= 1) {
        return true;
    }
    return false;
}

//
// Pushes a bit onto the given Code.
//
// c: the code to push the bit into
// bit: the bit to push
//
bool code_push_bit(Code *c, uint8_t bit) {
    if (code_full(c) || bit > 1) {
        return false;
    }
    switch (bit) {
    case 0: code_clr_bit(c, c->top); break;
    case 1: code_set_bit(c, c->top); break;
    default: break;
    }
    c->top += 1;
    return true;
}

//
// Pops a bit from the given Code.
//
// c: the code to pop a bit from
// bit: an address to store the popped bit into
//
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (code_empty(c)) {
        return false;
    }
    code_clr_bit(c, c->top);
    c->top -= 1;
    *bit = (c->bits[(c->top / 8)] & (1 << (c->top % 8))) > 0;
    return true;
}

//
// Prints out the given Code.
//
// c: the code to print
//
void code_print(Code *c) {
    for (uint32_t i = 0; i < c->top; i++) {
        printf("%d", (c->bits[i / 8] & (1 << (i % 8))) > 0);
    }
    printf("\n");
}
