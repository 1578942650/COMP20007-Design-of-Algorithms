/* ... */

#include <stdio.h>
#include <stdlib.h>
#include "traverse.h"
#define VISITED 1
#define NOTVISITED 0
#define INFINITY 999999
#define YES 1
#define NOT 0

//Vertex ** vertices: create an array of pointers that point to vertex
//n is the number of nodes in graph. eg 20->
//by unvisited it means the tree of this node has not been completely visited
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
	//initialise the visited array
	int *visited = NULL;
	visited = (int*)calloc(graph->maxn, sizeof(int));

	//DFS search
	Stack *unvisited = new_stack();
	stack_enstack(unvisited, source_id);


	while (stack_size(unvisited) > 0)
	{
		int node = stack_destack(unvisited);

		//if this node has not been visited
		if (visited[node] == NOTVISITED)
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
				stack_enstack(unvisited, node);
			}

			//marked as visited
			visited[node] = VISITED;
		}
	}

	free(visited);
}

void print_bfs(Graph* graph, int source_id) 
{
	//initialise the visited array
	int *visited = NULL;
	visited = (int*)calloc(graph->maxn, sizeof(int));

	//bfs search
	//enqueue the first node
	Queue *unvisited = new_queue();
	queue_enqueue(unvisited, source_id);

	//keep running until visit all node
	while(queue_size(unvisited) > 0)
	{
		//get the current node from queue
		int node = queue_dequeue(unvisited);

		//if this node has not been visited
		if (visited[node] == NOTVISITED)
		{
			//print out the information first
			Vertex *vertex = graph->vertices[node];
			printf("%s\n", vertex->label);
			Edge *edge = vertex->first_edge;

			//visit all edges of this node, put into queue
			//so that the first element from queue is this node's children
			while(edge != NULL)
			{
				queue_enqueue(unvisited, edge->v);
				edge = edge->next_edge;
			}

			//marked as visited
			visited[node] = VISITED;
		}
	}

	free(visited);
}

//unsolved
void detailed_path(Graph* graph, int source_id, int destination_id)
{
	
	//initialise the visited array
	int *visited = NULL;
	visited = (int*)calloc(graph->maxn, sizeof(int));

	//DFS search, push in the star point
	Stack *unvisited = new_stack();
	stack_enstack(unvisited, source_id);
	while (stack_size(unvisited) > 0)
	{

		int node = stack_destack(unvisited);

		//if this node has not been visited, explore this element
		if (visited[node] == NOTVISITED)
		{
			//read node deatail
			Vertex *vertex = graph->vertices[node];
			if (node == destination_id)
			{
				//once we find the destination, stop
				printf("we got the destination\n");
				break;
			}

			//visit all edges of its children
			Edge *edge = vertex->first_edge;

			//read all the children of this node first
			while(edge != NULL)
			{
				stack_enstack(unvisited, edge->v);
				edge = edge->next_edge;
			}

			//marked as visited
			visited[node] = VISITED;
		}
	}

	//once find destination, reverse the stack
	Stack *road = new_stack();
	while (1)
	{
		int cities = stack_destack(unvisited);
		stack_enstack(road, cities);
		if (cities == source_id)
			break;
	}

	//show the road
	while(stack_size(road) > 0)
	{
		int cities = stack_destack(road);
		printf("%s\n", graph->vertices[cities]->label);
	}
}

//unsolved
void all_paths(Graph* graph, int source_id, int destination_id) {
	printf("not yet implemented: put code for part 4 here\n");
}

//part 5
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
	for (int i = 0; i < length; i++)
	{
		if (path[i].distance < shortest_dist && path[i].permant_label == NOT)
		{
			pivot = i;
			shortest_dist = path[i].distance;
		}
	}

	return pivot;
}

void shortest_path(Graph* graph, int source_id, int destination_id) 
{
	//create an array of defined structure that contain(distance, parentnode)
	struct node_dist *dij_path = NULL;
	dij_path = malloc(graph->maxn * sizeof(struct node_dist));
	dij_path[source_id].distance = 0;
	dij_path[source_id].parent_id = 0;
	for (int i = 0; i < graph->maxn; i++ )
	{
		if (i != source_id)
		{
			dij_path[i].distance = INFINITY;
			dij_path[i].parent_id = 0;
		}
		dij_path[i].permant_label = NOT;
	}


	//Dijkstra algorithm
	for(int count = 0; count < graph->maxn; count++)
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
			printf("%s ", graph->vertices[cities]->label);
		else
			printf("%s, ", graph->vertices[cities]->label);
	}

	printf("(%dkm)", dij_path[destination_id].distance);

	free(dij_path);
}
