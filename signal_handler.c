#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include "jobs.h"

void handler_sigchld(int sig) {

    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        remove_job(pid);
        printf("\nProcessus terminé PID=%d\n", pid);
        fflush(stdout);
    }
}

void setup_signal_handler() {

    struct sigaction sa;
    sa.sa_handler = handler_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
    }
}