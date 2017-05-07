/* 
	Authorship: Siyi Guo
	Student number: 737008
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
#define NONE -1

//Part1
void print_dfs(Graph* graph, int source_id) 
{
	//this function takes a graph and stra point as input,
	//then use depth first search to print out the whore conten of graph
	//this is implemented by using stack.

	//initialise the explored array
	int *explored = NULL;
	explored = (int*)calloc(graph->maxn, sizeof(int));

	//DFS search
	//put the source node into stack
	Stack *unexplored = new_stack();
	stack_enstack(unexplored, source_id);

	//keep running until all nodes has been explored
	while (stack_size(unexplored) > 0)
	{
		//take the node from stack
		int node = stack_destack(unexplored);

		//if this node has not been explored
		if (explored[node] == NOTEXPLORED)
		{
			//print the label of this node
			Vertex *vertex = graph->vertices[node];
			printf("%s\n", vertex->label);

			//visit all edges of its children
			//creating a temproary stack to store children information
			//and reverse the order
			Stack *children = new_stack();
			Edge *edge = vertex->first_edge;

			while(edge != NULL)
			{
				//put the children into stak
				stack_enstack(children, edge->v);
				edge = edge->next_edge;
			}

 
			//reverse the order to match the order from input file
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

//Part2
void print_bfs(Graph* graph, int source_id) 
{
	//this function takes a graph and stra point as input,
	//then use breaadth first search to print out the whore conten of graph
	//this is implemented by using queue
	//if we represent the graph as a tree with multi levels,
	//this function will print out graph level by level.

	//initialise the explored array
	int *explored = NULL;
	explored = (int*)calloc(graph->maxn, sizeof(int));

	//bfs search
	//enqueue the first node
	Queue *unexplored = new_queue();
	queue_enqueue(unexplored, source_id);

	//keep running until all nodes has been explored
	while(queue_size(unexplored) > 0)
	{
		//get the node that first into the queue
		int node = queue_dequeue(unexplored);

		//if this node has not been explored
		if (explored[node] == NOTEXPLORED)
		{
			//print out the information first
			Vertex *vertex = graph->vertices[node];
			printf("%s\n", vertex->label);
			Edge *edge = vertex->first_edge;

			//visit all edges of this node, put into queue
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

//Part3 (use dijkstra algorithm to find out a path)
struct node_dist
{
	//this is created for dijkstra 
	int dist; //this store the distance from source_id
	int parent_id; // store the node to get here
	int permant_label; //lebale whether the distance has been found or not
};

int find_pivot_point(struct node_dist *path, int length)
{
	//this function find the node that
	//has the smallest dist from source id
	//and is not permantlly labelled yet

	int shortest_dist = INFINITY; // shortest dist from source
	int pivot = 0;

	int i;
	for (i = 0; i < length; i++)
	{
		if (path[i].dist < shortest_dist && path[i].permant_label == NOT)
		{
			pivot = i;
			shortest_dist = path[i].dist;
		}
	}

	return pivot;
}

void detailed_path(Graph* graph, int source_id, int destination_id)
{
	//create an array of defined structure that contain(distance, parentnode)
	//where distance is defined by the shorted path from this node
	//to source id

	struct node_dist *dij_path = NULL;
	dij_path = malloc(graph->maxn * sizeof(struct node_dist));
	dij_path[source_id].dist = 0;
	dij_path[source_id].parent_id = NONE;

	//initialse this dij array
	int i;
	for (i = 0; i < graph->maxn; i++ )
	{
		if (i != source_id)
		{
			//set there distance to infinity as we havening find out yet.
			dij_path[i].dist = INFINITY;
			dij_path[i].parent_id = NONE;
		}
		dij_path[i].permant_label = NOT;
	}


	//Dijkstra algorithm
	//count is used to find out how many vertex we have permantely labelled
	int count;
	//go through all node
	for(count = 0; count < graph->maxn; count++)
	{
		//find the node with shortest distance from source id yet not labelled.
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
			int current_dist = dij_path[child].dist;

			//if distance of new path  is smaller than   current node distance
			if(dij_path[pivot].dist + edge->weight < current_dist)
			{
				//replace the new distance, and record path
				dij_path[child].dist = dij_path[pivot].dist + edge->weight;
				dij_path[child].parent_id = pivot;
			}

			//go to next node that connected to current node
			edge = edge->next_edge;
		}
	}

	//show the result
	Stack *shortest_path = new_stack();
	int parent_id = graph->maxn;
	int prev_dis = destination_id;

	//trace back path from destination id
	//since we are tracing back from destination to sourece, 
	//we need a stack to store information
	while(parent_id != NONE)
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
		printf("%s (%dkm)\n", 
				graph->vertices[cities]->label,
				dij_path[cities].dist);
	}
	free(dij_path);	
	free_stack(shortest_path);
}

//Part4
void print_out_path(Graph *graph, int *output, int depth)
{
	//this function is created to print out the path from source to destination
	int i;
	for (i = 0; i < depth - 1; i++)
	{
		printf("%s, ", graph->vertices[output[i]]->label);
	}
	//the case when depth = i as in last one we need to change line
	printf("%s\n", graph->vertices[output[i]]->label);
}

void dfs(Graph* graph, int source_id, int destination_id, 
			int *output, int depth, int *explored)
{
	//this function is recursively called

	//keep a record of path
	//since every time we recursively apply dfs, this means we have not
	//reach destination, so the length of our path must increase by 1
	//and record the new node in our path
	output[depth++] = source_id;
	explored[source_id] = YES;

	//base case that stop exploring and print out the path
	//once we reach the destination
	if (source_id == destination_id)
	{
		print_out_path(graph, output, depth);
	}
	else
	{
		//fethch info about new node
		Vertex *vertex = graph->vertices[source_id];
		Edge *edge= vertex->first_edge;

		//apply dfs to every child of it
		while(edge != NULL)
		{
			//we dont want to go back to source id.
			if (explored[edge->v] == NOT)
			{
				//apply dfs for the child node
				dfs(graph, edge->v, destination_id, output, depth, explored);
			}
			//move to next child
			edge = edge->next_edge;
		}

	}
	//mark this node as unexplored so that this can be used for next path
	explored[source_id] = NOT;
}

void all_paths(Graph* graph, int source_id, int destination_id) 
{
	//initialise the explored array to indicate wheth a node
	//has been recored in the path
	int *explored = NULL;
	explored = (int*)calloc(graph->maxn, sizeof(int));

	//int output array to store the path detail
	int *output = NULL;
	output = malloc(graph->maxn * sizeof(int));

	//initialise the length of path to 0
	int depth = 0;
	
	dfs(graph, source_id, destination_id, output, depth, explored);

	free(explored);
	free(output);
}

//part 5
void shortest_path(Graph* graph, int source_id, int destination_id) 
{
	//create an array of defined structure that contain(dist, parentnode)
	struct node_dist *dij_path = NULL;
	dij_path = malloc(graph->maxn * sizeof(struct node_dist));
	dij_path[source_id].dist = 0;
	dij_path[source_id].parent_id = NONE;

	int i;
	for (i = 0; i < graph->maxn; i++ )
	{
		if (i != source_id)
		{
			dij_path[i].dist = INFINITY;
			dij_path[i].parent_id = NONE;
		}
		dij_path[i].permant_label = NOT;
	}


	//Dijkstra algorithm
	int count;
	for(count = 0; count < graph->maxn; count++)
	{
		//find the node with shortest dist
		int pivot = find_pivot_point(dij_path, graph->maxn);
		//label it as permant label
		dij_path[pivot].permant_label = YES;

		//use it as middle point and calculate dist
		Vertex *vertex = graph->vertices[pivot];
		Edge *edge = vertex->first_edge;

		while(edge != NULL)
		{
			//read the child node 
			int child = edge->v;
			int current_dist = dij_path[child].dist;

			//if dist of new path              < current node dist
			if(dij_path[pivot].dist + edge->weight < current_dist)
			{
				dij_path[child].dist = dij_path[pivot].dist + edge->weight;
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
	//we need stack as we are tracing back from destination to source
	//so we will read destination information first but output it last
	//this is just the property of stack

	while(parent_id != NONE)
	{
		stack_enstack(short_path, prev_dis);
		parent_id = dij_path[prev_dis].parent_id;
		prev_dis = parent_id;
	}
	

	//out put the path
	while (stack_size(short_path) > 0)
	{
		int cities = stack_destack(short_path);

		//if this is the last item in stack
		if (stack_size(short_path) == 0)
			printf("%s ", graph->vertices[cities]->label);
		else
			printf("%s, ", graph->vertices[cities]->label);
	}

	printf("(%dkm)\n", dij_path[destination_id].dist);

	//clean up
	free(dij_path);
	free_stack(short_path);
}
