#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "pipe.h"

#define MAX_ARGS 100

void execute_pipe(char *commande) {

    char *pipe_pos = strchr(commande, '|');
    if (!pipe_pos)
        return;

    *pipe_pos = '\0';

    char *cmd1 = commande;
    char *cmd2 = pipe_pos + 1;

    while (*cmd1 == ' ') cmd1++;
    while (*cmd2 == ' ') cmd2++;

    if (cmd1[0] == '!') cmd1++;
    if (cmd2[0] == '!') cmd2++;

    int pipefd[2];
    pipe(pipefd);

    pid_t pid1 = fork();

    if (pid1 == 0) {

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        char *args1[MAX_ARGS];
        int i = 0;

        char *token = strtok(cmd1, " ");
        while (token) {
            args1[i++] = token;
            token = strtok(NULL, " ");
        }
        args1[i] = NULL;

        execvp(args1[0], args1);
        perror("exec1");
        exit(1);
    }

    pid_t pid2 = fork();

    if (pid2 == 0) {

        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);

        char *args2[MAX_ARGS];
        int i = 0;

        char *token = strtok(cmd2, " ");
        while (token) {
            args2[i++] = token;
            token = strtok(NULL, " ");
        }
        args2[i] = NULL;

        execvp(args2[0], args2);
        perror("exec2");
        exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}