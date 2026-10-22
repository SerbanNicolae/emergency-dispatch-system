# Compiler setup.
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g

# Define targets.
TARGETS=tema1

# Manually define all targets.
build: $(TARGETS)

tema1: main.c unit.c incident.c intervention.c
	$(CC) $(CFLAGS) main.c unit.c incident.c intervention.c -o tema1

# Rule to run the program
run: build
	./tema1

# Pack the solution into a zip file.
pack:
	zip -FSr Serban-Nicolae-314CA.zip README Makefile *.c *.h

# Clean the solution.
clean:
	rm -f $(TARGETS)

.PHONY: build run pack clean