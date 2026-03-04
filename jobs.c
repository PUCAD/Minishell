#include <stdio.h>
#include <string.h>
#include "jobs.h"

Job jobs[MAX_JOBS];

void init_jobs() {
    for (int i = 0; i < MAX_JOBS; i++)
        jobs[i].actif = 0;
}

void add_job(pid_t pid, char *commande) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (!jobs[i].actif) {
            jobs[i].pid = pid;
            strcpy(jobs[i].commande, commande);
            jobs[i].actif = 1;
            break;
        }
    }
}

void remove_job(pid_t pid) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].actif && jobs[i].pid == pid) {
            jobs[i].actif = 0;
            break;
        }
    }
}

void print_jobs() {
    int vide = 1;
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].actif) {
            printf("[%d] PID: %d - %s\n",
                   i, jobs[i].pid, jobs[i].commande);
            vide = 0;
        }
    }
    if (vide)
        printf("Aucun processus en arrière-plan.\n");
}