#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "jobs.h"
#include "signal_handler.h"
#include "pipe.h"
#include "redirection.h"

#define MAX_ARGS 100

int main() {

    char ligne[1024];

    init_jobs();
    setup_signal_handler();

    while (1) {

        printf("MiniShell> ");
        fflush(stdout);

        if (fgets(ligne, sizeof(ligne), stdin) == NULL)
            break;

        ligne[strcspn(ligne, "\n")] = '\0';

        // ================= EXIT =================
        if (strcmp(ligne, "exit") == 0)
            break;

        // ================= JOBS =================
        else if (strcmp(ligne, "jobs") == 0)
            print_jobs();

        // ================= COMMANDE EXTERNE =================
        else if (ligne[0] == '!') {

            char *commande = ligne + 1;

            // ===== SI PIPE =====
            if (strchr(commande, '|')) {
                execute_pipe(commande);
                continue;
            }

            // ===== SINON EXECUTION NORMALE =====

            int background = 0;
            int len = strlen(commande);

            if (len > 0 && commande[len - 1] == '&') {
                background = 1;
                commande[len - 1] = '\0';
            }

            while (*commande == ' ')
                commande++;

            char *args[MAX_ARGS];
            int i = 0;

            char *token = strtok(commande, " ");
            while (token != NULL) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            pid_t pid = fork();

            if (pid == 0) {

                handle_redirection(args);

                execvp(args[0], args);
                perror("exec");
                exit(1);
            }

            else if (pid > 0) {

                if (!background)
                    waitpid(pid, NULL, 0);
                else {
                    printf("Processus arrière-plan PID=%d\n", pid);
                    add_job(pid, commande);
                }
            }

            else {
                perror("fork");
            }
        }

        else {
            printf("Commande inconnue\n");
        }
    }

    return 0;
}