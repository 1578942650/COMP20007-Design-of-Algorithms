/* * * * * * *
 * Module for creating and manipulating stacks of integers
 *
 * created for COMP20007 Design of Algorithms 2017
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 */

// 'include guards': these lines (and #endif at the bottom) prevent this file
// from being included multiple times. multiple inclusion can cause errors on
// some compilers, especially if the included file contains struct definitions
// how it works: https://en.wikipedia.org/wiki/Include_guard
#ifndef STACK_H
#define STACK_H

typedef struct stack Stack;

// create a new stack and return its pointer
Stack *new_stack();

// destroy a stack and its associated memory
void free_stack(Stack *stack);

// push a new item of data onto the top of a stack. O(1).
void stack_enstack(Stack *stack, int data);

// remove and return the top item of data from a stack. O(1).
// error if the stack is empty (so first ensure stack_size() > 0)
int stack_destack(Stack *stack);

// return the number of items currently in a stack
int stack_size(Stack *stack);

#endif