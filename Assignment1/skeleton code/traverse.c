/* 
	Authorship: Siyi Guo
	Created data: 28/3/2017
 */

#include <stdio.h>
#include <stdlib.h>
#include "traverse.h"
#define EXPLORED 1
#define NOTEXPLORED 0
#define INFINITY 999999
#define YES 1
#define NOT 0

//Vertex ** vertices: create an array of pointers that point to vertex
//n is the number of nodes in graph. eg 20->
//by unexplored it means the tree of this node has not been completely explored
void show_input(Graph* graph, int source_id)
{
	//show the input content
	printf("\nn is %d, maxn is %d\n", graph->n, graph->maxn);
	int a = 0;
	while(a < graph->n)
	{
		Vertex *vertex = graph->vertices[a];
		printf("label is %s, index %d\n", vertex->label,a);

		Edge *edge = vertex->first_edge;
		while(edge != NULL)
		{
			printf("  star %s, end %s, length is %d\n", 
					graph->vertices[edge->u]->label,
					graph->vertices[edge->v]->label,
					edge->weight);
			edge = edge->next_edge;
		}
		a++;
	}
}

void print_dfs(Graph* graph, int source_id) 
{
	//initialise the explored array
	int *explored = NULL;
	explored = (int*)calloc(graph->maxn, sizeof(int));

	//DFS search
	Stack *unexplored = new_stack();
	stack_enstack(unexplored, source_id);


	while (stack_size(unexplored) > 0)
	{
		int node = stack_destack(unexplored);

		//if this node has not been explored
		if (explored[node] == NOTEXPLORED)
		{
			//print the label of parent node
			Vertex *vertex = graph->vertices[node];
			printf("%s\n", vertex->label);

			//visit all edges of its children
			Stack *children = new_stack();
			Edge *edge = vertex->first_edge;

			//read all the children of this node first
			while(edge != NULL)
			{
				stack_enstack(children, edge->v);
				edge = edge->next_edge;
			}

 
			//reverse the order 
			while (stack_size(children) > 0)
			{
				int node = stack_destack(children);
				stack_enstack(unexplored, node);
			}

			//marked as explored
			explored[node] = EXPLORED;
			free_stack(children);
		}
	}

	free(explored);
	free_stack(unexplored);
}

void print_bfs(Graph* graph, int source_id) 
{
	//initialise the explored array
	int *explored = NULL;
	explored = (int*)calloc(graph->maxn, sizeof(int));

	//bfs search
	//enqueue the first node
	Queue *unexplored = new_queue();
	queue_enqueue(unexplored, source_id);

	//keep running until visit all node
	while(queue_size(unexplored) > 0)
	{
		//get the current node from queue
		int node = queue_dequeue(unexplored);

		//if this node has not been explored
		if (explored[node] == NOTEXPLORED)
		{
			//print out the information first
			Vertex *vertex = graph->vertices[node];
			printf("%s\n", vertex->label);
			Edge *edge = vertex->first_edge;

			//visit all edges of this node, put into queue
			//so that the first element from queue is this node's children
			while(edge != NULL)
			{
				queue_enqueue(unexplored, edge->v);
				edge = edge->next_edge;
			}

			//marked as explored
			explored[node] = EXPLORED;
		}
	}
	//free(unexplored);
	free(explored);
	free_queue(unexplored);
}

//part3 (use dijkstra)
struct node_dist
{
	//this is created for dijkstra 
	int distance; //this store the distance from source
	int parent_id;
	int permant_label;
};

int find_pivot_point(struct node_dist *path, int length)
{
	//this function find the node that
	//has the smallest distance from source id
	//and is not permantlly labelled yet

	int shortest_dist = INFINITY; // shortest distance from source
	int pivot = 0;

	int i;
	for (i = 0; i < length; i++)
	{
		if (path[i].distance < shortest_dist && path[i].permant_label == NOT)
		{
			pivot = i;
			shortest_dist = path[i].distance;
		}
	}

	return pivot;
}

void detailed_path(Graph* graph, int source_id, int destination_id)
{
	//create an array of defined structure that contain(distance, parentnode)
	struct node_dist *dij_path = NULL;
	dij_path = malloc(graph->maxn * sizeof(struct node_dist));
	dij_path[source_id].distance = 0;
	dij_path[source_id].parent_id = 0;

	int i;
	for (i = 0; i < graph->maxn; i++ )
	{
		if (i != source_id)
		{
			dij_path[i].distance = INFINITY;
			dij_path[i].parent_id = 0;
		}
		dij_path[i].permant_label = NOT;
	}


	//Dijkstra algorithm
	int count;
	for(count = 0; count < graph->maxn; count++)
	{
		//find the node with shortest distance
		int pivot = find_pivot_point(dij_path, graph->maxn);
		//label it as permant label
		dij_path[pivot].permant_label = YES;

		//use it as middle point and calculate distance
		Vertex *vertex = graph->vertices[pivot];
		Edge *edge = vertex->first_edge;

		while(edge != NULL)
		{
			//read the child node 
			int child = edge->v;
			int current_dist = dij_path[child].distance;

			//if distance of new path              < current node distance
			if(dij_path[pivot].distance + edge->weight < current_dist)
			{
				dij_path[child].distance = dij_path[pivot].distance + edge->weight;
				dij_path[child].parent_id = pivot;
			}

			//go to next child
			edge = edge->next_edge;
		}
	}

	//show the result
	Stack *shortest_path = new_stack();
	int parent_id = graph->maxn;
	int prev_dis = destination_id;

	//trace back path from destination id
	while(parent_id != 0)
	{
		stack_enstack(shortest_path, prev_dis);
		parent_id = dij_path[prev_dis].parent_id;
		prev_dis = parent_id;
	}

	//out put the path cities
	while (stack_size(shortest_path) > 0)
	{
		int cities = stack_destack(shortest_path);

		//if this is the last item in stack
		if (stack_size(shortest_path) == 0)
			printf("%s (%dkm)", 
					graph->vertices[cities]->label,
					dij_path[cities].distance);
		else
			printf("%s (%dkm)\n", 
					graph->vertices[cities]->label,
					dij_path[cities].distance);
	}
	free(dij_path);	
	free_stack(shortest_path);
}

//part4
void print_out_path(Graph *graph, int *output, int depth)
{
	int i;
	for (i = 0; i<depth - 1; i++)
	{
		printf("%s, ", graph->vertices[output[i]]->label);
	}
	printf("%s\n", graph->vertices[output[i]]->label);
}

void dfs(Graph* graph, int source_id, int destination_id, 
			int *output, int depth, int *explored)
{
	//keep a record of path
	output[depth++] = source_id;
	explored[source_id] = YES;
	//printf("the depth is %d, from %d to %d\n", depth, source_id, destination_id);

	//base case that stop exploring and print out
	if (source_id == destination_id)
	{
		print_out_path(graph, output, depth);
	}
	else
	{
		if (1)
		{
			//fethch info about source node
			Vertex *vertex = graph->vertices[source_id];
			Edge *edge= vertex->first_edge;

			//apply dfs to every child of it
			while(edge != NULL)
			{
				if (explored[edge->v] == NOT)
				{
					dfs(graph, edge->v, destination_id, output, depth, explored);
				}
				//move to next child
				edge = edge->next_edge;
			}
		}
	}
	explored[source_id] = NOT;
}
void all_paths(Graph* graph, int source_id, int destination_id) 
{
	//initialise the explored array
	int *explored = NULL;
	explored = (int*)calloc(graph->maxn, sizeof(int));

	//int output array
	int *output = NULL;
	output = malloc(graph->maxn * sizeof(int));

	int depth = 0;
	//set up
	dfs(graph, source_id, destination_id, output, depth, explored);

	free(explored);
	free(output);
}

//part 5
void shortest_path(Graph* graph, int source_id, int destination_id) 
{
	//create an array of defined structure that contain(distance, parentnode)
	struct node_dist *dij_path = NULL;
	dij_path = malloc(graph->maxn * sizeof(struct node_dist));
	dij_path[source_id].distance = 0;
	dij_path[source_id].parent_id = 0;

	int i;
	for (i = 0; i < graph->maxn; i++ )
	{
		if (i != source_id)
		{
			dij_path[i].distance = INFINITY;
			dij_path[i].parent_id = 0;
		}
		dij_path[i].permant_label = NOT;
	}


	//Dijkstra algorithm
	int count;
	for(count = 0; count < graph->maxn; count++)
	{
		//find the node with shortest distance
		int pivot = find_pivot_point(dij_path, graph->maxn);
		//label it as permant label
		dij_path[pivot].permant_label = YES;

		//use it as middle point and calculate distance
		Vertex *vertex = graph->vertices[pivot];
		Edge *edge = vertex->first_edge;

		while(edge != NULL)
		{
			//read the child node 
			int child = edge->v;
			int current_dist = dij_path[child].distance;

			//if distance of new path              < current node distance
			if(dij_path[pivot].distance + edge->weight < current_dist)
			{
				dij_path[child].distance = dij_path[pivot].distance + edge->weight;
				dij_path[child].parent_id = pivot;
			}

			//go to next child
			edge = edge->next_edge;
		}
	}

	//show the result
	Stack *short_path = new_stack();
	int parent_id = graph->maxn;
	int prev_dis = destination_id;

	//trace back path from destination id
	while(parent_id != 0)
	{
		stack_enstack(short_path, prev_dis);
		parent_id = dij_path[prev_dis].parent_id;
		prev_dis = parent_id;
	}

	//out put the path cities
	while (stack_size(short_path) > 0)
	{
		int cities = stack_destack(short_path);

		//if this is the last item in stack
		if (stack_size(short_path) == 0)
			printf("%s ", graph->vertices[cities]->label);
		else
			printf("%s, ", graph->vertices[cities]->label);
	}

	printf("(%dkm)", dij_path[destination_id].distance);

	free(dij_path);
	free_stack(short_path);
}
