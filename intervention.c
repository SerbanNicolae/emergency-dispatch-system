/*Serban Nicolae-314CA*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intervention.h"


// Creates an intervention list with a sentinel node
intervention_list *create_inter_list () {
	intervention_list *interventii = malloc(sizeof(intervention_list));
	interventii->head = malloc(sizeof(node_intervention));
	interventii->head->info.incident = NULL;
	interventii->head->info.unit = NULL;
	interventii->head->next = interventii->head;
	interventii->head->prev = interventii->head;
	interventii->tail = interventii->head;
	interventii->size = 0;
	return interventii;
}

// Adds an intervention to the list
void add_in_list_inter(intervention_list *interventii, node_incident *inc, node_unit *unit) {

	node_intervention *interventie = malloc(sizeof(node_intervention));
	interventie->info.incident = &(inc->info);
	interventie->info.unit = &(unit->info);
	if(interventii->size == 0) {
		interventii->head->next = interventie;
		interventii->head->prev = interventie;
		interventie->next = interventii->head;
		interventie->prev = interventii->head;
		interventii->tail = interventie;
	} else {
		interventii->tail->next = interventie;
		interventie->prev = interventii->tail;
		interventie->next = interventii->head;
		interventii->head->prev = interventie;
		interventii->tail = interventie;
	}
	interventii->size++;

}

// Dispatches incident and unit to the intervention list
void dequee_incident_and_unit (queue_incident *q, queue_available_units *q2, intervention_list *l_inter ) {
	if (q == NULL || q->size == 0 || q2 == NULL || q2->size == 0) {
        return; 
    }
	node_queue_incident *curr = q->head;//->next
	strcpy(curr->incident_in_queue->info.status, "intervened");
	node_queue_units *curr2 = q2->head;//->next
	curr2->unit_in_queue->info.availability = 0;	
	add_in_list_inter(l_inter, curr->incident_in_queue, curr2->unit_in_queue );
	q->head = q->head->next;
	q2->head = q2->head->next;
	free(curr);
	free(curr2);
	q->size--;
	q2->size--;
}

// Creates an intervention stack
stack_intervention *create_stack() {
	stack_intervention *stack_inter = malloc(sizeof(stack_intervention));
	stack_inter->head = NULL;
	stack_inter->size = 0;
	return stack_inter;

}

// Pushes an intervention onto the stack
void add_in_stack(stack_intervention *stack, intervention_list *list_intervention) {
	
	node_stack_intervention *node_stack = malloc(sizeof(node_stack_intervention));
	node_stack->intervention_in_stack = list_intervention->tail;
	if(stack->size == 0) {
		stack->head = node_stack;
	} else {
		node_stack->next = stack->head;
		stack->head = node_stack;
	}
	stack->size++;
}

// Pops the top element from the stack
void pop_stack (stack_intervention *stack) {

	if(stack->size == 0) {
		return;
	}
	node_stack_intervention *curr = stack->head;
	stack->head = stack->head->next;
	stack->size--;
	free(curr);
}

// Undoes the last dispatched intervention
void undo_last_dispatch(stack_intervention *stack, 								
						intervention_list *list_intervention, 
						queue_available_units *q_units,
						queue_incident *q_high, queue_incident*q_medium,
						queue_incident *q_low, FILE *out) {

	if(stack->size == 0) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	while (1) {
		if(strstr(stack->head->intervention_in_stack->info.incident->status, "intervened")) {
			strcpy(stack->head->intervention_in_stack->info.incident->status, "queued");
			int id_inc = stack->head->intervention_in_stack->info.incident->id;
			stack->head->intervention_in_stack->info.unit->availability = 1;
			if(strstr(stack->head->intervention_in_stack->info.incident->priority, "high")) {
				add_back_queue(q_high, stack);
			} else if (strstr(stack->head->intervention_in_stack->info.incident->priority, "medium")) {
				add_back_queue(q_medium, stack);
			} else if (strstr(stack->head->intervention_in_stack->info.incident->priority, "low")) {
				add_back_queue(q_low, stack);
			}
			
			node_unit *unitate = (node_unit *)stack->head->intervention_in_stack->info.unit;
			remove_intervention(list_intervention, id_inc);
			add_in_queue_units(q_units, unitate);
			pop_stack(stack);			
			break;
		} else {
			pop_stack(stack);
		}
		
	}



}

// Removes an intervention by incident ID
void remove_intervention (intervention_list *list ,int id) {

	node_intervention *curr, *prev;
	curr = list->head->next;
	if(list->size >= 1) {
	while(curr->info.incident->id != id) {
		curr = curr->next;
	}
	prev = curr->prev;
	prev->next = curr->next;
	curr->next->prev = prev;
	list->size--;
	if (curr == list->tail) {
    list->tail = curr->prev; 
}	
	free(curr);
	}
}

// Re-enqueues an incident at the front
void add_back_queue (queue_incident *q_inc, stack_intervention *stack  ) {

	struct incident *inc = stack->head->intervention_in_stack->info.incident; // pointer to incident in stack
	node_incident *inc_node = (node_incident *)inc;								// cast to queue node pointer
	node_queue_incident *new = malloc(sizeof(node_queue_incident));
	new->incident_in_queue = inc_node;
	new->next = q_inc->head;
	q_inc->head = new;
	q_inc->size++;
}



// Marks an incident as solved and frees its unit
void incident_solved(intervention_list *list_interv, 
					queue_available_units *unitati, int id, FILE *out) {
	if(list_interv == NULL || list_interv->size == 0) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
			return;
	}
	node_intervention *curr2 = list_interv->head->next;
	while(curr2 != list_interv->head) {
		if(curr2->info.incident->id == id) {
			break;
		}
		curr2 = curr2->next;
	}
	if(curr2 == list_interv->head) {
			fprintf(out, "INVALID OPERATION! ERROR 404\n");
			return;
		}
	if(strcmp("intervened", curr2->info.incident->status) != 0) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	strcpy(curr2->info.incident->status,"solved");
	curr2->info.unit->availability = 1;
	node_unit *unit = (node_unit*)curr2->info.unit;
	add_in_queue_units(unitati, unit);

}

// Displays all active interventions
void show_intervention(intervention_list *interventii, FILE *out) {
	if(interventii == NULL || interventii->size == 0) {
		fprintf(out, "No intervention has been initiated\n");
		return;
	}
	node_intervention *curr = interventii->head->next;
	while(curr != interventii->head) {
		fprintf(out, "Incident %d was assigned to unit %d, and has the following status: \"%s\"\n", curr->info.incident->id, curr->info.unit->id, curr->info.incident->status);
		curr = curr->next;
	}
}

// Frees the intervention list
void free_interv_list (intervention_list *interv_list) {
	if(interv_list == NULL || interv_list->size == 0) {
		free(interv_list->head);
		free(interv_list);
		return;
	}
	node_intervention *curr = interv_list->head->next, *prev;
	while(curr != interv_list->head) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(interv_list->head);
	free(interv_list);
}

// Frees the intervention stack
void free_stack_interv(stack_intervention *stack_interv) {

	if(stack_interv == NULL || stack_interv->size == 0) {
		free(stack_interv);
		return;
	}
	while (stack_interv->size != 0) {
		pop_stack(stack_interv);
	}
	free(stack_interv);
}


// Cleans up all allocated memory
void cleanup(queue_available_units *q_units, list_unit *unitati, queue_incident *q_high,  
			queue_incident *q_medium,  queue_incident *q_low, incident_list *inc_list, 
			intervention_list *interv_list, stack_intervention *stack_interv) {
		free_queue_units(q_units);
		free_list_unit(unitati);
		free_incidents_queue(q_high);
		free_incidents_queue(q_medium);
		free_incidents_queue(q_low);
		free_incidents_list(inc_list);
		free_interv_list(interv_list);
		free_stack_interv(stack_interv);
}