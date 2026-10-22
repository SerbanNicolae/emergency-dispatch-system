/*Serban Nicolae-314CA*/
#ifndef UNIT_H
#define UNIT_H

#include <stdio.h>





// Unit structure
typedef struct unit{
	int id;
	char type;
	int availability;
} unit;

// Unit node structure
typedef struct node_unit {
	struct unit info;
	struct node_unit *next;
} node_unit;

// Unit list structure
typedef struct list_unit {
	struct node_unit *head;
	int size;
} list_unit;

// Available units queue node
typedef struct node_queue_units node_queue_units;
typedef struct node_queue_units {

	node_unit *unit_in_queue;
	node_queue_units *next;

}node_queue_units;

// Available units queue structure
typedef struct queue_available_units {
	
	node_queue_units *head;
	int size;        

} queue_available_units;

list_unit* create_unit();
node_unit* read_units(FILE *f, int size, queue_available_units *q_unit);
void add_in_queue_units(queue_available_units *queue, node_unit *unit);
queue_available_units *create_queue_unit();
void num_of_avilable_units(queue_available_units *queue, FILE *out);
void show_unit(list_unit *unitati, int id, FILE *out);
void free_queue_units(queue_available_units *q_units);
void free_list_unit(list_unit *unitati);
#endif