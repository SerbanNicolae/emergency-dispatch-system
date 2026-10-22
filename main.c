/*Serban Nicolae-314CA*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unit.h"
#include "incident.h"
#include "intervention.h"

int main() {
	int size;
	FILE *f = fopen("tema1.in", "r");
	FILE *out =fopen("tema1.out", "w");
	if (f == NULL || out == NULL) {
		printf("fisierul nu s a deschis");
		return -1;
	}
	fscanf(f, "%d", &size);
	list_unit *unitati = create_unit();
	queue_available_units *q_units = create_queue_unit();	
	queue_incident *queue_low = create_queue();
	queue_incident *queue_medium = create_queue();
	queue_incident *queue_high = create_queue();
	incident_list *incidente = create_incidents();
	intervention_list *interventii = create_inter_list();
	stack_intervention *stack_intervention = create_stack();
	if(unitati == NULL || q_units == NULL || queue_low == NULL || queue_medium == NULL || queue_low == NULL || incidente == NULL || interventii == NULL || stack_intervention == NULL) {
		printf (" locarea a esuat");
		return -1;
	}
	unitati->head = read_units(f, size, q_units);
	unitati->size = size;
	 fscanf(f, "%d", &size);
	while(size) {
	 	char *command = calloc(28, sizeof(char));
	 	fscanf(f, "%s", command);
		if(strstr(command, "ADD_INCIDENT") != NULL) {			
			node_incident *inc = incident_command(f);
	 		add_incidents(incidente, inc);
			if (strstr(inc->info.priority, "low") ) {
				add_in_queue(queue_low, inc);
			} else if (strstr(inc->info.priority, "medium") ) {
						add_in_queue(queue_medium, inc);
			} else if(strstr(inc->info.priority, "high") ) {
					add_in_queue(queue_high, inc);
			}
		} else if(strstr(command, "CHECK_UNITS_AVAILABILITY")) {
			num_of_avilable_units(q_units, out);
		} else if(strcmp(command, "DISPATCH") == 0) {
			int ok = dispatch_condition(queue_high, queue_medium, queue_low, q_units);
			if( ok == 2 ) {
				dequee_incident_and_unit(queue_high, q_units, interventii);
				add_in_stack(stack_intervention, interventii); 
			} else if( ok == 3) {
				dequee_incident_and_unit(queue_medium, q_units, interventii);
				add_in_stack(stack_intervention, interventii);
			} else if( ok == 4) {
				dequee_incident_and_unit(queue_low, q_units, interventii);
				add_in_stack(stack_intervention, interventii);
			} else {
				fprintf(out, "INVALID OPERATION! ERROR 404\n");
			}
		} else if (strstr(command, "UNDO_LAST_DISPATCH")) {
			undo_last_dispatch(stack_intervention, interventii, q_units, queue_high, queue_medium, queue_low, out);
		} else if(strstr(command, "SOLVED_INCIDENT")) {
			int id;
			fscanf(f, "%d", &id);
			incident_solved(interventii, q_units, id, out);
		} else if(strcmp(command,"SHOW_UNIT") == 0) {
			int id;
			fscanf(f, "%d", &id);
			show_unit(unitati, id, out);
		} else if(strcmp(command, "SHOW_INCIDENT") == 0) {
			int id;
			fscanf(f, "%d", &id);
			show_incident(incidente, id, out);
		} else if(strcmp(command, "SHOW_INTERVENTIONS") == 0) {
			show_intervention(interventii, out);
		}
	 	size--;
			free(command);
	 	}
		fclose(f);
		fclose(out);
		 cleanup(q_units, unitati, queue_high, queue_medium, 
				queue_low, incidente, interventii, stack_intervention);

	 return 0;
}