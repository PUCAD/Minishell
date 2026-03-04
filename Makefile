CC = gcc
CFLAGS = -Wall -Wextra

OBJ = main.o jobs.o signal_handler.o pipe.o redirection.o

all: minishell

minishell: $(OBJ)
	$(CC) $(CFLAGS) -o minishell $(OBJ)

main.o: main.c jobs.h signal_handler.h pipe.h redirection.h
	$(CC) $(CFLAGS) -c main.c

jobs.o: jobs.c jobs.h
	$(CC) $(CFLAGS) -c jobs.c

signal_handler.o: signal_handler.c signal_handler.h jobs.h
	$(CC) $(CFLAGS) -c signal_handler.c

pipe.o: pipe.c pipe.h
	$(CC) $(CFLAGS) -c pipe.c

redirection.o: redirection.c redirection.h
	$(CC) $(CFLAGS) -c redirection.c

clean:
	rm -f *.o minishell