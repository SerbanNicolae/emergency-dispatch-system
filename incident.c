/*Serban Nicolae-314CA*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "incident.h"

// Creates an incident list with a sentinel node
incident_list *create_incidents (){
	incident_list *incidente = malloc(sizeof(incident_list));
	incidente->head = malloc(sizeof(node_incident));
	incidente->head->info.id = 0;
	strcpy(incidente->head->info.priority, "low");
	incidente->head->info.description = malloc(50 * sizeof(char));
	strcpy(incidente->head->info.description , "test incident");
	strcpy(incidente->head->info.status, "solved");
	incidente->head->next = incidente->head;
	incidente->head->prev = incidente->head;
	incidente->size = 0;
	
	return incidente;
}

// Creates an incident queue
queue_incident* create_queue() {
	queue_incident *q = malloc(sizeof(queue_incident));
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	return q;
}

// Reads an incident and dynamically allocates its description
node_incident *incident_command(FILE *f) {

	node_incident *inc = malloc(sizeof(node_incident));
	fscanf(f,"%d",&inc->info.id);
	char c = fgetc(f);
	fscanf(f, "%s", inc->info.priority);
	c = fgetc(f);
	strcpy(inc->info.status , "queued");
	int size = 0, capacity = 10;
	inc->info.description = malloc(10 * sizeof(char));
	inc->info.description[0] = '\0';
	while (c != '\n') {
			c = fgetc(f);
			if(c == '\n') break;
			inc->info.description[size++] = c;
			inc->info.description[size] = '\0';
			if((size + 1) >= capacity ) {
			capacity= capacity * 2;
			char *aux = realloc(inc->info.description, sizeof(char) * capacity);
			inc->info.description = aux;			
		}
	}
	return inc;
}

// Appends an incident to the list
void add_incidents(incident_list *incidente, node_incident *inc) {
	node_incident *curr = incidente->head, *prev;
	while(curr->next != incidente->head) {
		curr = curr->next;
	}
	prev = curr;
	curr->next = inc;
	inc->prev = prev;
	inc->next = incidente->head;
	incidente->head->prev = inc;
	incidente->size++;
	

}

// Enqueues an incident
void add_in_queue(queue_incident *queue, node_incident *inc) {

	node_queue_incident *q_incident = malloc(sizeof(node_queue_incident));
	q_incident ->incident_in_queue = inc;
	q_incident->next = NULL;
	node_queue_incident *curr = queue->head;
	if(curr == NULL) {
		queue->head = q_incident;
		queue->head->next = NULL;
	}	else {
		while(curr->next != NULL) {
		curr = curr->next;
	}
	curr->next = q_incident;
	}
	queue->size++;
}

// Checks dispatch conditions and returns target queue priority
int dispatch_condition (queue_incident *q_high, queue_incident *q_medium, queue_incident *q_low, queue_available_units *q_units) {
	int ok = 0;
	if(q_units->size > 0) {
		ok = 1;
	}
	if (ok == 0) {
		return 0;
	}
	if(q_high->size > 0) {
		ok = 2;
	}
	if (ok < 2) {
		if (q_medium->size > 0) {
			ok = 3;
		}
	}
	if(ok < 2) {
		if (q_low ->size > 0) {
			ok = 4;
		}
	}
	return ok;
}

// Displays an incident by ID or prints an error
void show_incident(incident_list *incidents, int id, FILE *out) {
	if(incidents == NULL || incidents->size == 0) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}

	node_incident *curr = incidents->head->next;
	while(curr != incidents->head) {
		if(curr->info.id == id) {
			break;
		}
		curr = curr->next;
	}
	if(curr == incidents->head) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	fprintf(out, "Incident %d has %s priority, the following description: %s and is %s \n", curr->info.id, curr->info.priority, curr->info.description, curr->info.status );
}

// Frees the incident list
void free_incidents_list (incident_list *inc_list) {

	if(inc_list == NULL || inc_list->size == 0) {
		free(inc_list->head->info.description);
		free(inc_list->head);
		free(inc_list);
		return;
	}

	node_incident *curr = inc_list->head->next, *prev;
	while(curr != inc_list->head) {
		prev = curr;
		curr= curr->next;
		free(prev->info.description);
		free(prev);  
	}
	free(inc_list->head->info.description);
	free(inc_list->head);
	free(inc_list);
}

// Frees the incident queue
void free_incidents_queue(queue_incident *q_inc) {

	if(q_inc == NULL || q_inc->size == 0) {
		free(q_inc);
		return;
	}
	node_queue_incident *curr = q_inc->head, *prev;
	while(curr != NULL) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(q_inc);

}

