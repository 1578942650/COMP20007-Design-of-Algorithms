/* * * * * * * * *
 * Dynamic hash table using cuckoo hashing, resolving collisions by switching
 * keys between two tables with two separate hash functions
 *
 * created for COMP20007 Design of Algorithms - Assignment 2, 2017
 * by 
 * Name: Siyi Guo
 * Student number: 737008
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "cuckoo.h"

// an inner table represents one of the two internal tables for a cuckoo
// hash table. it stores two parallel arrays: 'slots' for storing keys and
// 'inuse' for marking which entries are occupied
typedef struct inner_table 
{
	int64 *slots;	// array of slots holding keys
	bool  *inuse;	// is this slot in use or not?
} InnerTable;

// a cuckoo hash table stores its keys in two inner tables
struct cuckoo_table 
{
	InnerTable *table1; // first table
	InnerTable *table2; // second table
	int size;			// size of each table
};


//helper function for creating and initializing Cuckoo Table table
static void initialise_inner_table(InnerTable *table, int size)
{
	//this function initialise a single hash table
	table->slots = malloc((sizeof *table->slots) * size);
	assert(table->slots);
	table->inuse = malloc((sizeof *table->inuse) * size);
	assert(table->inuse);
	int i;
	for (i = 0; i < size; i++) 
	{
		table->inuse[i] = false;
	}
}

static void initialise_Cuckoo_table(CuckooHashTable *table, int size)
{
	//this function intialise the cuckoo table with size
	assert(size < MAX_TABLE_SIZE && "error: table has grown too large!");

	initialise_inner_table(table->table1, size);
	initialise_inner_table(table->table2, size);

	table->size = size;
}

//helper function to double the size of Cuckoo table
static void double_Cuckoo_table(CuckooHashTable *table)
{
	int64 *oldslots1 = table->table1->slots;
	bool  *oldinuse1 = table->table1->inuse;

	int64 *oldslots2 = table->table2->slots;
	bool  *oldinuse2 = table->table2->inuse;

	int oldsize = table->size;

	initialise_Cuckoo_table(table, oldsize * 2);

	int i;
	for (i = 0; i < oldsize; i++) 
	{
		if (oldinuse[i] == true) 
		{
			cuckoo_hash_table_insert(table, oldslots[i]); /**************This part not done yet**********/
		}
	}

	free(oldslots1);
	free(oldslots2);
	free(oldinuse1);
	free(oldinuse2);
}

/****
 * All Function
 */
// initialise a cuckoo hash table with 'size' slots in each table
CuckooHashTable *new_cuckoo_hash_table(int size) 
{
	CuckooHashTable *table = malloc(sizeof *table);
	assert(table);
	initialise_Cuckoo_table(CuckooHashTable *table, size);
	return table;
}


// free all memory associated with 'table'
void free_cuckoo_hash_table(CuckooHashTable *table) 
{	
	assert(table != NULL);

	//free subtable arrays
	free(table->table1->slots);
	free(table->table1->inuse);

	//free subtable arrays
	free(table->table2->slots);
	free(table->table2->inuse);

	//free table itself
	free(table)
}


// insert 'key' into 'table', if it's not in there already
// returns true if insertion succeeds, false if it was already in there
bool cuckoo_hash_table_insert(CuckooHashTable *table, int64 key) 
{
	assert(table != NULL);


	//recor original position so that when we go back, we know we need to stop
	int original_pos = hA;
	int64 original_key = key;
	int h = -1;

	//count the number of steps as well as which table are we in
	int i = 0

	//in the normal case we assume it is empty
	while (h != original_pos && key != original_key) //or back when we get the same value?
	{
		if (i == 0)
		{
			h == h1(key) % table->size;
			if (cuckoo_hash_table_lookup(table, key) == true)：
			{
				//thiis item already exist in the table
				return false
			}
		}

		if (i % 2 == 0):
		{			
			//this is table 1
			if (table->table1->inuse[h] == false)
			{
				//insert the key, return true for our success
				table->table1->slots[h] == true;
				table->table1->inuse[h] == true;
				return true
			}
			else //if table1 already in use
			{
				//bounce to next table
				int64 temp = table->table1->slots[h];
				table->table1->slots[h] = key;
				h = h2(temp) % table->size;
				key = temp;
			}
		}
		else
		{
			//this is table 2
			if (table->table2->inuse[h] == false)
			{
				//insert the key, return true
				table->table2->slots[h] == true;
				table->table2->inuse[h] == true;
				return true
			}
			else //if table2 already in use
			{
				//bounce to next table
				int64 temp = table->table2->slots[h];
				table->table2->slots[h] = key;
				h = h1(temp) % table->size;
				key = temp;
			}
		}
		//in the end increase i by 1
		i++;
	}

	//in the case it is not empty
	if (h == original_pos && key == original_key)
	{
		//make some space and we try again
		double_Cuckoo_table(table);
		return double_Cuckoo_table(table, key);
	}
}


// lookup whether 'key' is inside 'table'
// returns true if found, false if not
bool cuckoo_hash_table_lookup(CuckooHashTable *table, int64 key) 
{
	int hA = h1(key) % table->size;
	int hB = h2(key) % table->size;

	if (table->table2->slots[hA] == key)
	{
		//the table is already in use
		return true
	}

	if (table->table2->slots[hB] == key)
	{
		//the table is already in use
		return true
	}

	return false;
}


// print the contents of 'table' to stdout
void cuckoo_hash_table_print(CuckooHashTable *table) 
{
	assert(table);
	printf("--- table size: %d\n", table->size);

	// print header
	printf("                    table one         table two\n");
	printf("                  key | address     address | key\n");
	
	// print rows of each table
	int i;
	for (i = 0; i < table->size; i++) {

		// table 1 key
		if (table->table1->inuse[i]) {
			printf(" %20llu ", table->table1->slots[i]);
		} else {
			printf(" %20s ", "-");
		}

		// addresses
		printf("| %-9d %9d |", i, i);

		// table 2 key
		if (table->table2->inuse[i]) {
			printf(" %llu\n", table->table2->slots[i]);
		} else {
			printf(" %s\n",  "-");
		}
	}

	// done!
	printf("--- end table ---\n");
}


// print some statistics about 'table' to stdout
void cuckoo_hash_table_stats(CuckooHashTable *table) 
{
	assert(table != NULL);
	printf("--- table stats ---\n");
	
	// print some information about the table
	printf("current size: %d slots\n", table->size);
	printf("current load: %d items\n", table->load);
	printf(" load factor: %.3f%%\n", table->load * 100.0 / table->size);
	printf("   step size: %d slots\n", STEP_SIZE);
	
	printf("--- end stats ---\n");
}
