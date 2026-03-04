#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>

#define MAX_JOBS 100

typedef struct {
    pid_t pid;
    char commande[1024];
    int actif;
} Job;

extern Job jobs[MAX_JOBS];

void init_jobs();
void add_job(pid_t pid, char *commande);
void remove_job(pid_t pid);
void print_jobs();

#endif
