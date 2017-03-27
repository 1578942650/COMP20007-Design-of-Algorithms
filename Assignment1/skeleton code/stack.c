/* * * * * * *
 * Module for creating and manipulating queue of integers
 *
 * created for COMP20007 Design of Algorithms 2017
 * by Siyi Guo (siyig1@student.unimelb.edu.au)
 */
 #include <stdlib.h>
 #include <assert.h>
 #include "list.h"
 #include "stack.h"

 typedef struct node Node;

 struct node
 {
 	Node *next;
 	int data;
 };

 struct stack
 {
 	List *list;
 };

//produce a new stack head
 Stack *new_stack()
 {
 	Stack *stack = malloc(sizeof *stack);
 	assert(stack);
 	stack->list = new_list();
 	return stack;
 }

 //destroy a stack and free it memory
 void free_stack(Stack *stack)
 {
 	assert(stack != NULL);
 	free_list(stack->list);
 }

//enter into the stack
void stack_enstack(Stack *stack, int data)
{
	assert(stack != NULL);
	list_add_end(stack->list, data);
}

//pop up the stack
int stack_destack(Stack *stack)
{
	assert(stack != NULL);
	return list_remove_end(stack->list);
}

//find the size of a stack
int stack_size(Stack *stack)
{
	assert(stack != NULL);
	return list_size(stack->list);
}


