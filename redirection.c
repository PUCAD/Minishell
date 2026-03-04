#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "redirection.h"

void handle_redirection(char **args) {

    for (int i = 0; args[i] != NULL; i++) {

        if (strcmp(args[i], "<") == 0) {

            int fd = open(args[i+1], O_RDONLY);
            if (fd < 0) {
                perror("input");
                exit(1);
            }

            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;
        }

        if (strcmp(args[i], ">") == 0) {

            int fd = open(args[i+1],
                          O_WRONLY | O_CREAT | O_TRUNC,
                          0644);
            if (fd < 0) {
                perror("output");
                exit(1);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
        }
    }
}