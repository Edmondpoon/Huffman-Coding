#include "code.h"
#include <fcntl.h>
#include <stdio.h>

//
// code_init aims to create an instance of the Code struct, setting the top and the code array to 0.
//
// code_init takes no arguments.
//
// code_init returns the instance of the Code struct that was created.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
Code code_init(void) {
    Code code = { 0, { 0 } };
    return code;
}

//
// code_size aims to return the size of the given Code.
//
// code_size takes 1 argument: c. The argument c represents a pointer to an instance of a Code struct.
//
// code_size returns the current top of the given Code.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
uint32_t code_size(Code *c) {
    return c->top;
}

//
// code_empty aims to return whether the given Code is empty or not.
//
// code_empty takes 1 argument: c. The argument c represents a pointer to an instance of a Code struct.
//
// code_empty returns whether the given Code is empty or not.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
bool code_empty(Code *c) {
    return c->top == 0;
}

//
// code_full aims to return whether the given Code is full or not.
//
// code_full takes 1 argument: c. The argument c represents a pointer to an instance of a Code struct.
//
// code_full returns whether the given Code is full or not.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
bool code_full(Code *c) {
    return c->top == ALPHABET;
}

//
// code_set_bit aims to set a bit of a Code at a given index position to 1.
//
// code_set_bit takes 2 argument: c and i. The argument c represents a pointer to an instance of a Code struct while i
// represents the index to set.
//
// code_set_bit returns whether it was able to set the bit or not.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
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
// code_clr_bit aims to set a bit of a Code at a given index position to 0.
//
// code_clr_bit takes 2 argument: c and i. The argument c represents a pointer to an instance of a Code struct while i
// represents the index to clear.
//
// code_clr_bit returns whether it was able to clear the bit or not.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
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
// code_get_bit aims to return whether the bit of a Code at a given index is equal to 1.
//
// code_get_bit takes 2 argument: c and i. The argument c represents a pointer to an instance of a Code struct while i
// represents the index to check.
//
// code_get_bit returns whether the bit of the code at the given index is equal to 1 or not.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
//
bool code_get_bit(Code *c, uint32_t i) {
    if (i / 8 < MAX_CODE_SIZE && (c->bits[i / 8] & (1 << (i % 8))) >= 1) {
        return true;
    }
    return false;
}

//
// code_push_bit aims to push a bit onto the given Code.
//
// code_push_bit takes 2 argument: c and bit. The argument c represents a pointer to an instance of a Code struct while
// bit represents the bit to push.
//
// code_push_bit returns whether it was able to push a bit onto the Code or not.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
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
// code_pop_bit aims to pop a bit onto the given Code.
//
// code_pop_bit takes 2 argument: c and bit. The argument c represents a pointer to an instance of a Code struct while
// bit represents the bit to pop.
//
// code_pop_bit returns whether it was able to pop a bit onto the Code or not.
//
// Note that this function is based on and build off of the given ideas by Professor Long in the assignment documentation.
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
// code_print aims to print out the given Code.
//
// code_size takes 1 argument: c. The argument c represents a pointer to an instance of a Code struct.
//
// code_size returns nothing/void.
//
void code_print(Code *c) {
    for (uint32_t i = 0; i < c->top; i++) {
        printf("%d", (c->bits[i / 8] & (1 << (i % 8))) > 0);
    }
    printf("\n");
}
