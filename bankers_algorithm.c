#include <stdio.h>

/*int numberInArray(int number, int array[], int size) {
    int i;
    for (i=0; i<size; i++) {
        if (array[i] == )
    }
}*/
int main() {
    int i, j, processCount, resourceCount, allocatedResources[32][32], maximumRequired[32][32], neededResources[32][32], resourceInstances[32], availableResource[32], doneIndex[32];
    int currentlyAllocated, processLeft=0, neededAvailable, processCompletion[32], completedProcess, orderIndex=0;
    char order[32][3];
    printf("Enter number of processes : ");
    scanf("%d", &processCount);
    for (i=0; i<processCount; i++) {
        processCompletion[i] = 0;
    }
    processLeft=processCount;
    printf("Enter number of resources : ");
    scanf("%d", &resourceCount);
    for (i=0; i<resourceCount; i++) {
        printf("Enter the number of instances available for R%d : ", i);
        scanf("%d", &resourceInstances[i]);
    }
    for (i=0; i<processCount; i++) {
        printf("Process %d\n", i);
        for (j=0; j<resourceCount; j++) {
            printf("\tR%d currently allocated for P%d : ", j, i);
            scanf("%d", &allocatedResources[i][j]);
            printf("\tR%d maximum required for P%d : ", j, i);
            scanf("%d", &maximumRequired[i][j]);
            neededResources[i][j] = maximumRequired[i][j] - allocatedResources[i][j];
        }
    }
    for (i=0; i<resourceCount; i++) {
        currentlyAllocated = 0;
        for (j=0; j<processCount; j++) {
            currentlyAllocated += allocatedResources[j][i];
        }
        availableResource[i] = resourceInstances[i] - currentlyAllocated;
    }
    printf("Needed matrix\n");
    for (i=0; i<processCount; i++) {
        for(j=0; j<resourceCount; j++) {
            printf("%d ", neededResources[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    while (processLeft != 0) {
        completedProcess = 0;
        for (i=0; i<processCount; i++) {
            neededAvailable = 1;
            if (processCompletion[i] == 1) {
                continue;
            }
            for (j=0; j<resourceCount; j++) {
                if (neededResources[i][j] > availableResource[j]) {
                    neededAvailable = 0;
                    break;
                }
            }
            if (neededAvailable) {
                completedProcess++;
                for (j=0; j<resourceCount; j++) {
                    availableResource[j] += allocatedResources[i][j];
                }
                processCompletion[i] = 1;
                processLeft--;
                order[orderIndex][0] = 'P';
                order[orderIndex][1] = i+48;
                order[orderIndex++][2] = '\0';
            }
        }
        if (!completedProcess) {
            printf("Could not allocate resources to all the process\n");
            break;
        }
    }
    if (completedProcess) {
        printf("Order of execution\n\t");
        for (i=0; i<processCount; i++) {
            printf("%s ", order[i]);
        }
        printf("\n");
    }
}