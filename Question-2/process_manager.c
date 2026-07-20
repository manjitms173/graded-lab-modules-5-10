/*
 * Simplified Process Manager for Testing
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define MAX_CHILDREN 5
#define TIMEOUT_SECONDS 5

typedef struct {
    pid_t pid;
    time_t start_time;
    int terminated;
} ChildProcess;

ChildProcess children[MAX_CHILDREN];
int active_children = 0;

void simulate_child_work(int child_id) {
    if (child_id == 2 || child_id == 4) {
        printf("[Child %d - PID %d] Unresponsive (30s sleep)\n", child_id, getpid());
        sleep(30);
    } else {
        int work_time = 1 + (rand() % 3);
        printf("[Child %d - PID %d] Working for %d seconds\n", child_id, getpid(), work_time);
        sleep(work_time);
        printf("[Child %d - PID %d] Completed\n", child_id, getpid());
    }
}

void create_child_process(int child_id) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) {
        srand(time(NULL) ^ getpid());
        printf("[Child %d - PID %d] Started\n", child_id, getpid());
        simulate_child_work(child_id);
        exit(EXIT_SUCCESS);
    } else {
        children[active_children].pid = pid;
        children[active_children].start_time = time(NULL);
        children[active_children].terminated = 0;
        printf("[Parent] Created child %d with PID %d\n", child_id, pid);
        active_children++;
    }
}

void check_and_kill_unresponsive() {
    time_t current_time = time(NULL);
    int status;
    
    for (int i = 0; i < active_children; i++) {
        if (!children[i].terminated) {
            // First check if child already exited
            pid_t result = waitpid(children[i].pid, &status, WNOHANG);
            
            if (result > 0) {
                printf("[Parent] Child PID %d exited (status: %d)\n", 
                       children[i].pid, WEXITSTATUS(status));
                children[i].terminated = 1;
            } else {
                // Check timeout
                time_t elapsed = current_time - children[i].start_time;
                
                if (elapsed > TIMEOUT_SECONDS) {
                    printf("[Parent] Child PID %d unresponsive (%ld sec). Killing...\n",
                           children[i].pid, elapsed);
                    
                    kill(children[i].pid, SIGTERM);
                    sleep(1);
                    
                    if (kill(children[i].pid, 0) == 0) {
                        printf("[Parent] Sending SIGKILL to PID %d\n", children[i].pid);
                        kill(children[i].pid, SIGKILL);
                    }
                    
                    waitpid(children[i].pid, &status, 0);
                    printf("[Parent] Child PID %d terminated\n", children[i].pid);
                    children[i].terminated = 1;
                }
            }
        }
    }
}

int all_children_done() {
    for (int i = 0; i < active_children; i++) {
        if (!children[i].terminated) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    printf("\n=== Process Manager Demo ===\n\n");
    
    memset(children, 0, sizeof(children));
    srand(time(NULL));
    
    printf("Creating %d child processes...\n\n", MAX_CHILDREN);
    for (int i = 0; i < MAX_CHILDREN; i++) {
        create_child_process(i);
        usleep(100000);
    }
    
    printf("\nMonitoring children (timeout: %d seconds)...\n\n", TIMEOUT_SECONDS);
    
    int loop_count = 0;
    while (!all_children_done()) {
        printf("[Parent] Monitoring loop %d - checking children...\n", ++loop_count);
        check_and_kill_unresponsive();
        sleep(1);
        
        if (loop_count > 20) {
            printf("[Parent] Safety timeout reached. Exiting.\n");
            break;
        }
    }
    
    printf("\n=== Summary ===\n");
    for (int i = 0; i < active_children; i++) {
        printf("Child %d (PID %d): %s\n", 
               i, children[i].pid, 
               children[i].terminated ? "Done" : "ERROR");
    }
    
    printf("\nProgram completed!\n\n");
    return EXIT_SUCCESS;
}
