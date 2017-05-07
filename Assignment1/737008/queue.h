/* * * * * * *
 * Module for creating and manipulating queues of integers
 *
 * created for COMP20007 Design of Algorithms 2017
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 */

// 'include guards': these lines (and #endif at the bottom) prevent this file
// from being included multiple times. multiple inclusion can cause errors on
// some compilers, especially if the included file contains struct definitions
// how it works: https://en.wikipedia.org/wiki/Include_guard
#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue Queue;

// create a new queue and return its pointer
Queue *new_queue();

// destroy a queue and its associated memory
void free_queue(Queue *queue);

// insert a new item of data at the back of a queue. O(1).
void queue_enqueue(Queue *queue, int data);

// remove and return the item of data at the front of a queue. O(1).
// error if the queue is empty (so first ensure queue_size() > 0)
int queue_dequeue(Queue *queue);

// return the number of items currently in a queue
int queue_size(Queue *queue);

#endif