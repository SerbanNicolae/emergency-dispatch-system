/*Serban Nicolae-314CA*/
#ifndef INTERVENTION_H
#define INTERVENTION_H

#include "unit.h"
#include "incident.h"

// Intervention structure
typedef struct intervention {
	struct incident *incident;
	struct unit *unit;
} intervention;

// Doubly-linked node for intervention
typedef struct node_intervention {
	intervention info;
	struct node_intervention *next, *prev;
} node_intervention;

// Intervention list structure
typedef struct intervention_list {
	struct node_intervention *head, *tail;
	int size;
} intervention_list;

// Intervention stack node
typedef struct node_stack_intervention node_stack_intervention;
typedef struct node_stack_intervention {
	node_intervention *intervention_in_stack;
	node_stack_intervention *next;
} node_stack_intervention;


// Intervention stack structure
typedef struct stack_intervention {
	node_stack_intervention *head;
	int size;
} stack_intervention;

intervention_list *create_inter_list ();
void add_in_list_inter(intervention_list *interventii, node_incident *inc, node_unit *unit);
void dequee_incident_and_unit (queue_incident *q, queue_available_units *q2, intervention_list *interventii);
stack_intervention *create_stack();
void add_in_stack(stack_intervention *stack, intervention_list *list_intervention);
void undo_last_dispatch(stack_intervention *stack, 								
						intervention_list *list_intervention,
						queue_available_units *q_units,
						queue_incident *q_high, queue_incident*q_medium,
						queue_incident *q_low , FILE *out);
void pop_stack(stack_intervention *stack);
void remove_intervention (intervention_list *list ,int id);
void add_back_queue (queue_incident *q_inc, stack_intervention *stack);
void incident_solved(intervention_list *list_interv, 
					queue_available_units *unitati, int id, FILE *out);
void show_intervention(intervention_list *interventii, FILE *out);
void free_interv_list (intervention_list *interv_list);
void free_stack_interv(stack_intervention *stack_interv);
void cleanup(queue_available_units *q_units, list_unit *unitati, queue_incident *q_high,  
			queue_incident *q_medium,  queue_incident *q_low, incident_list *inc_list, 
			intervention_list *interv_list, stack_intervention *stack_interv);					
#endif