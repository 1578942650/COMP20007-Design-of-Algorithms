/* * * * * * *
 * Module for creating and manipulating singly-linked queue of integers
 *
 * created for COMP20007 Design of Algorithms 2017
 * by siyi Guo (Siyig1@student.unimelb.edu.au)
 */

#include <stdbool.h>

typedef struct queue Queue;

//create a new queue and return a pointer to it
 Queue *new_queue();

//destroy of queue and free its memory
 void free_queue(Queue *queue);

//add an element to the queue
 void queue_enqueue(Queue *queue, int data);

//remove an element from the queue
 int queue_dequeue(Queue *queue);

//find the size of a queue
 int queue_size(Queue *queue);