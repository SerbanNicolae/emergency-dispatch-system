/*Serban Nicolae-314CA*/
#include <stdlib.h>
#include "unit.h"


// Creates a unit list
list_unit* create_unit() {
	list_unit *unitati = malloc(sizeof(list_unit));
	unitati->size = 0;
	return unitati;
}

// Reads units from file, builds the list, and enqueues available units
node_unit* read_units(FILE *f, int size, queue_available_units *q_unit) {
	node_unit *prev = NULL;
	node_unit *head = NULL;
	for(int i = 0; i < size; i++) {
		node_unit *unitate = malloc(sizeof(node_unit));
		unitate->next = NULL;
		unitate->info.availability = 1;
		fscanf(f, "%d", &unitate->info.id);
		fscanf(f, " %c", &unitate->info.type);
		if(head == NULL) {
			head = unitate;
		}
		if(prev == NULL) {
			prev = unitate;
		} else {
			prev->next = unitate;
			prev = unitate;
		}
		add_in_queue_units(q_unit, unitate);
		
		
	}
	return head;
}

// Creates an available units queue
queue_available_units *create_queue_unit() {
	queue_available_units *q = malloc(sizeof(queue_available_units));
	q->head = NULL;
	q->size = 0;
	return q;

}

// Enqueues an available unit
void add_in_queue_units(queue_available_units *queue, node_unit *unit) {
	node_queue_units *q_unit = malloc(sizeof(node_queue_units));
	q_unit -> unit_in_queue = unit;
	q_unit->next = NULL;
	node_queue_units *curr = queue->head;
	if(curr == NULL) {
		queue->head = q_unit;
		queue->head->next = NULL;
	} else {
		while(curr->next != NULL) {
			curr = curr->next;
		}
		curr->next = q_unit;
	}
	queue->size++;
}

// Prints the number of available units
void num_of_avilable_units(queue_available_units *queue, FILE *out) {
	fprintf(out, "Number of available units: %d\n", queue->size);
}

// Displays a unit by ID or prints an error
void show_unit(list_unit *unitati, int id, FILE *out) {

if(unitati == NULL || unitati->size == 0) {
	fprintf(out, "INVALID OPERATION! ERROR 404\n");
	return;
}
node_unit *curr = unitati->head;
	while(curr != NULL) {
		if(curr->info.id == id) {
			if(curr->info.availability == 0){
				fprintf(out, "Unit %d is type %c and is unavailable\n", curr->info.id, curr->info.type);
			} else {
				fprintf(out, "Unit %d is type %c and is available\n", curr->info.id, curr->info.type);
			}
			
			break;
		}
		curr = curr->next;
	}
	if(curr == NULL) {
	fprintf(out, "INVALID OPERATION! ERROR 404\n");
	}
	
}

// Frees the units queue
void free_queue_units(queue_available_units *q_units) {

	if(q_units->size == 0) {
		free(q_units);
		return;
	}
	node_queue_units *curr = q_units->head , *prev;
	while (curr != NULL) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(q_units);
}

// Frees the units list
void free_list_unit(list_unit *unitati) {
	if(unitati->size == 0) {
		free(unitati);
		return;
	}
	node_unit *curr = unitati->head, *prev;
	while(curr != NULL) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(unitati);
}