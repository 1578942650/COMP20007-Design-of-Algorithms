/* * * * * * *
 * Module for creating and manipulating queue of integers
 *
 * created for COMP20007 Design of Algorithms 2017
 * by Siyi Guo (siyig1@student.unimelb.edu.au)
 */
 #include <stdlib.h>
 #include <assert.h>
 #include "list.h"
 #include "queue.h"

typedef struct node Node;

struct node
{
	Node *next;
	int data;
};

struct queue 
{
	List *list; 
};


//produce a new queue head
Queue *new_queue()
{
 	Queue *queue = malloc(sizeof *queue);
 	assert(queue);
 	queue->list = new_list();
 	return queue;
}

 //destroy a stack and free it memory
void free_queue(Queue *queue)
{
 	assert(queue != NULL);
 	free_list(queue->list);
}

 //enter into the queue;
void queue_enqueue(Queue *queue, int data)
{
 	assert(queue != NULL);
 	list_add_end(queue->list, data);
}

int queue_dequeue(Queue *queue)
{
 	assert(queue != NULL);
 	return list_remove_start(queue->list);
}

//find the size of a queue
int queue_size(Queue *queue)
{
	assert(queue != NULL);
 	return list_size(queue->list);
}