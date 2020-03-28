// A Multithreaded Program that implements the Banker's algorithm.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

int res, process;
int nProcess = 0;
int **allocated;
int **maxRequired;
int **need;
int *resource;
int *SafeSequence;

pthread_mutex_t lockResource;
pthread_cond_t c;

bool getSafeSeq();

void* CodeOfProcess(void* );

int main(int args, char** argv) {
	srand(time(NULL));

        printf("\n Enter the number of processes? ");
        scanf("%d", &process);

        printf("\nEnter the number of resources? ");
        scanf("%d", &res);

        resource = (int *)malloc(res * sizeof(*resource));
        printf("\nCurrently Available resources (R1 R2 ...)? ");
        for(int i=0; i<res; i++)
                scanf("%d", &resource[i]);

        allocated = (int **)malloc(process * sizeof(*allocated));
        for(int i=0; i<process; i++)
                allocated[i] = (int *)malloc(res * sizeof(**allocated));

        maxRequired = (int **)malloc(process * sizeof(*maxRequired));
        for(int i=0; i<process; i++)
                maxRequired[i] = (int *)malloc(res * sizeof(**maxRequired));

        printf("\n");
        for(int i=0; i<process; i++) {
                printf("\nResource allocated to process %d (R1 R2 ...)? ", i+1);
                for(int j=0; j<res; j++)
                        scanf("%d", &allocated[i][j]);
        }
        printf("\n");

        for(int i=0; i<process; i++) {
                printf("\nMaximum resource required by process %d (R1 R2 ...)? ", i+1);
                for(int j=0; j<res; j++)
                        scanf("%d", &maxRequired[i][j]);
        }
        printf("\n");
        
        need = (int **)malloc(process * sizeof(*need));
        for(int i=0; i<process; i++)
                need[i] = (int *)malloc(res * sizeof(**need));

        for(int i=0; i<process; i++)
                for(int j=0; j<res; j++)
                        need[i][j] = maxRequired[i][j] - allocated[i][j];

	// get safe sequence
	SafeSequence = (int *)malloc(process * sizeof(*SafeSequence));
        for(int i=0; i<process; i++) SafeSequence[i] = -1;

        if(!getSafeSeq()) {
                printf("\nUnsafe State! The processes leads the system to a unsafe state.\n\n");
                exit(-1);
        }

        printf("\n\nSafe Sequence Found : ");
        for(int i=0; i<process; i++) {
                printf("%-3d", SafeSequence[i]+1);
        }

        printf("\nExecuting Processes...\n\n");
        sleep(1);
	
	// run threads
	pthread_t processes[process];
        pthread_attr_t attr;
        pthread_attr_init(&attr);

	int processNumber[process];
	for(int i=0; i<process; i++) processNumber[i] = i;

        for(int i=0; i<process; i++)
                pthread_create(&processes[i], &attr, CodeOfProcess, (void *)(&processNumber[i]));

        for(int i=0; i<process; i++)
                pthread_join(processes[i], NULL);

        printf("\nAll Processes Finished\n");	
	
	// free resource
        free(resource);
        for(int i=0; i<process; i++) {
                free(allocated[i]);
                free(maxRequired[i]);
		free(need[i]);
        }
        free(allocated);
        free(maxRequired);
	free(need);
        free(SafeSequence);
}


bool getSafeSeq() {
	// get safe sequence
        int temp[res];
        for(int i=0; i<res; i++) temp[i] = resource[i];

        bool finished[process];
        for(int i=0; i<process; i++) finished[i] = false;
        int nfinished=0;
        while(nfinished < process) {
                bool safe = false;

                for(int i=0; i<process; i++) {
                        if(!finished[i]) {
                                bool possible = true;

                                for(int j=0; j<res; j++)
                                        if(need[i][j] > temp[j]) {
                                                possible = false;
                                                break;
                                        }

                                if(possible) {
                                        for(int j=0; j<res; j++)
                                                temp[j] += allocated[i][j];
                                        SafeSequence[nfinished] = i;
                                        finished[i] = true;
                                        ++nfinished;
                                        safe = true;
                                }
                        }
                }

                if(!safe) {
                        for(int k=0; k<process; k++) SafeSequence[k] = -1;
                        return false; // no safe sequence found
                }
        }
        return true; // safe sequence found
}

// process code
void* CodeOfProcess(void *arg) {
        int p = *((int *) arg);

	// lock resource
        pthread_mutex_lock(&lockResource);

        // c check
        while(p != SafeSequence[nProcess])
                pthread_cond_wait(&c, &lockResource);

	// process
        printf("\n--> Process %d", p+1);
        printf("\n\tAllocated : ");
        for(int i=0; i<res; i++)
                printf("%3d", allocated[p][i]);

        printf("\n\tNeeded    : ");
        for(int i=0; i<res; i++)
                printf("%3d", need[p][i]);

        printf("\n\tAvailable : ");
        for(int i=0; i<res; i++)
                printf("%3d", resource[i]);

        printf("\n"); sleep(1);

        printf("\tResource Allocated!");
        printf("\n"); sleep(1);
        printf("\tProcess Code Running...");
        printf("\n"); sleep(rand()%3 + 2); // process code
        printf("\tProcess Code Completed...");
        printf("\n"); sleep(1);
        printf("\tProcess Releasing Resource...");
        printf("\n"); sleep(1);
        printf("\tResource Released!");

	for(int i=0; i<res; i++)
                resource[i] += allocated[p][i];

        printf("\n\tNow Available : ");
        for(int i=0; i<res; i++)
                printf("%3d", resource[i]);
        printf("\n\n");

        sleep(1);

	// c broadcast
        nProcess++;
        pthread_cond_broadcast(&c);
        pthread_mutex_unlock(&lockResource);
	pthread_exit(NULL);
}
