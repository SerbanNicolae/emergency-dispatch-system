/*Serban Nicolae-314CA*/
#ifndef INCIDENT_H
#define INCIDENT_H

#include <stdio.h>
#include "unit.h"

// Incident structure
typedef struct incident {
	int id;
	char priority[7];
	char *description;
	char status[13];
} incident;

// Doubly-linked node for incident
typedef struct node_incident {
	incident info;
	struct node_incident *next;
	struct node_incident *prev;
} node_incident;

// Incident list structure
typedef struct incident_list {
	struct node_incident *head;
	int size; 
} incident_list;

// Queue node for incident
typedef struct node_queue_incident node_queue_incident;
typedef struct node_queue_incident {

	node_incident *incident_in_queue;
	node_queue_incident *next;
} node_queue_incident;

// Incident queue structure
typedef struct queue_incident {
	node_queue_incident *head;
	node_queue_incident *tail;
	int size;
}queue_incident;

incident_list *create_incidents ();
queue_incident* create_queue();
node_incident *incident_command(FILE *f);
void add_incidents(incident_list *incidente, node_incident *inc);
void add_in_queue(queue_incident *queue, node_incident *inc);
int dispatch_condition(queue_incident *q_high, queue_incident *q_medium, queue_incident *q_low, queue_available_units *q_units);
void show_incident(incident_list *incidents, int id, FILE *out);
void free_incidents_list (incident_list *inc_list);
void free_incidents_queue(queue_incident *q_inc);

#endif