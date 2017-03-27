/* * * * * * *
 * Module for creating and manipulating singly-linked stack of integers
 *
 * created for COMP20007 Design of Algorithms 2017
 * by siyi Guo (Siyig1@student.unimelb.edu.au)
 */

#include <stdbool.h>

typedef struct stack Stack;

//create a new stack and return a pointer to it
 Stack *new_stack();

//destroy a free stack and free it smemory
 void free_stack(Stack *stack);

//push in an element of a stack
 void stack_enstack(Stack *stack, int data);

//pop up an element of a stack
 int stack_destack(Stack *stack);

//find the size of a stack
 int stack_size(Stack *stack);