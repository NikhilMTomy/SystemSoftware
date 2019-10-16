#include <stdio.h>
#include <math.h>

void sort(int array[], int size) {
    int i, j, temp, minpos;
    for (i=0; i<size; i++) {
        minpos = i;
        for (j=i; j<size; j++) {
            if (array[j] < array[minpos]) {
                minpos = j;
            }
        }
        if (minpos !=i ) {
            temp = array[minpos];
            array[minpos] = array[i];
            array[i] = temp;
        }
    }
}
void fcfs(int maxRange, int queue[], int seekCount, int initialHead) {
    int i, headMovements=0;
    printf("FCFS\n");
    printf("%d", initialHead);
    for (i=0; i<seekCount; i++) {
        printf("-->%d", queue[i]);
        if (i==0) {
            headMovements += abs(initialHead-queue[0]);
        } else {
            headMovements += abs(queue[i]-queue[i-1]);
        }
    }
    printf("\nHead movements : %d\n", headMovements);
}
int scan(int maxRange, int queue[], int seekCount, int initialHead) {
    int i, initialHeadIndex, headMovements = 0;
    printf("SCAN\n");
    sort(queue, seekCount);
    for (i=0; i<seekCount; i++) {
        if (queue[i] > initialHead) {
            initialHeadIndex = i;
            break;
        }
    }
    printf("%d", initialHead);
    headMovements = abs(initialHead-queue[initialHeadIndex-1]);
    for (i=initialHeadIndex-1; i >= 0; i--) {
        printf("-->%d", queue[i]);
        if (i != 0) {
            headMovements += abs(queue[i]-queue[i-1]);
        }
    }
    printf("-->0");
    headMovements += abs(queue[0]);
    for (i=initialHeadIndex; i<seekCount; i++) {
        printf("-->%d", queue[i]);
        if (i != initialHeadIndex) {
            headMovements += abs(queue[i]-queue[i-1]);
        } else {
            headMovements += abs(queue[i]-0);
        }
    }
    printf("\nHead movements : %d\n", headMovements);
}
int cscan(int maxRange, int queue[], int seekCount, int initialHead) {
    int i, initialHeadIndex, headMovements=0;
    printf("C-SCAN\n");
    sort(queue, seekCount);
    for (i=0; i<seekCount; i++) {
        if (queue[i] > initialHead) {
            initialHeadIndex = i;
            break;
        }
    }
    printf("%d", initialHead);
    headMovements = abs(initialHead - queue[initialHeadIndex-1]);
    for (i=initialHeadIndex-1; i >= 0; i--) {
        printf("-->%d", queue[i]);
        if (i != initialHeadIndex-1) {
            headMovements += abs(queue[i] - queue[i+1]);
        }
    }
    printf("-->%d-->%d", 0, maxRange-1);
    headMovements += queue[0];
    headMovements += maxRange-1;
    for (i=seekCount-1; i>=initialHeadIndex; i--) {
        printf("-->%d", queue[i]);
        if (i != seekCount-1) {
            headMovements += abs(queue[i] - queue[i+1]);
        } else {
            headMovements += abs(queue[i] - maxRange-1);
        }
    }
    printf("\nHead movements : %d\n", headMovements);
}
int main() {
    int i, maxRange, queue[128], seekCount, initialHead;
    char ch;
    printf("Enter maximum range of disk : ");
    scanf("%d", &maxRange);
    printf("Enter number of locations : ");
    scanf("%d", &seekCount);
    printf("Enter seeks\n");
    for (i=0; i<seekCount; i++) {
        scanf("%d", &queue[i]);
    }
    printf("Enter initial head position : ");
    scanf("%d", &initialHead);
    printf("\n\t1.FCFS\n\t2.SCAN\n\t3.CSCAN\n\t4.Exit\nEnter choice : ");
    while (getchar()!='\n');
    scanf("%c", &ch);
    switch(ch) {
        case '1':
            fcfs(maxRange, queue, seekCount, initialHead);
            break;
        case '2':
            scan(maxRange, queue, seekCount, initialHead);
            break;
        case '3':
            cscan(maxRange, queue, seekCount, initialHead);
            break;
    }
}