CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g


SRC = dynamic_neurons.c neuron_structure.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = comp

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)