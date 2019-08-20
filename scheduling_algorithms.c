#include <stdio.h>

struct Process {
	int pid;
	int burstTime;
	int waitingTime;
	int turnaroundTime;
	int priority;
};
/***********************************************************************************************/
int fcfs(struct Process processList[], int processCount) {
	int i, currentTime;
	currentTime = 0;
	for (i=0; i<processCount; i++) {
		processList[i].waitingTime = currentTime;
		currentTime += processList[i].burstTime;
		processList[i].turnaroundTime = currentTime;
	}
	return 0;
}
int sjf(struct Process processList[], int processCount) {
	int i, j, currentTime, shortestIdx;
	currentTime = 0;
	for (i=0; i<processCount; i++) {
		shortestIdx = 0;
		for (j=1; j<processCount; j++) {
			if ((processList[j].burstTime>0 && processList[j].burstTime<processList[shortestIdx].burstTime) || processList[shortestIdx].burstTime==0) {
				shortestIdx = j;
			}
		}
		processList[shortestIdx].waitingTime = currentTime;
		currentTime += processList[shortestIdx].burstTime;
		processList[shortestIdx].burstTime = 0;
		processList[shortestIdx].turnaroundTime = currentTime;
	}
}
int roundRobin(struct Process processList[], int processCount) {
	int i, currentTime, quanta=2, executionIdx, pending;
	currentTime = 0;
	executionIdx = 0;
	printf("Enter quanta : ");
	scanf("%d", &quanta);
	while( 1 ) {
		pending = 0;
		for (i=0; i<processCount; i++) {
			if (processList[i].burstTime != 0) {
				pending=1;
			}
		}
		if (!pending) {
			break;
		}
		while (processList[executionIdx].burstTime == 0) {
			if (executionIdx == processCount-1) {
				executionIdx = 0;
			} else {
				executionIdx++;
			}
		}
		for (i=0; i<processCount; i++) {
			if (i!=executionIdx && processList[i].burstTime>0) {
				processList[i].waitingTime += quanta;
			}
		}
		if ( processList[executionIdx].burstTime <= quanta ) {
			currentTime += processList[executionIdx].burstTime;
			processList[executionIdx].burstTime = 0;
		} else {
			currentTime += quanta;
			processList[executionIdx].burstTime -= quanta;
		}
		if (processList[executionIdx].burstTime == 0) {
			processList[executionIdx].turnaroundTime = currentTime;
		}

		if (executionIdx == processCount-1) {
			executionIdx = 0;
		} else {
			executionIdx++;
		}
	}
}
int priority(struct Process processList[], int processCount) {
	int i, currentTime, j, nextIndex, maxPriorityLeft;
	for (i=0, currentTime=0; i<processCount; i++)  {
		nextIndex = -1, maxPriorityLeft=99;
		for (j=0; j<processCount; j++) {
			if (processList[j].burstTime > 0 && maxPriorityLeft > processList[j].priority) {
				nextIndex = j;
				maxPriorityLeft = processList[j].priority;
			}
		}
		if (nextIndex != -1) {
			processList[nextIndex].waitingTime = currentTime;
			currentTime += processList[nextIndex].burstTime;
			processList[nextIndex].turnaroundTime = currentTime;
			processList[nextIndex].burstTime = 0;
		}
	}
	return 0;
}
int displayResults(struct Process processList[], int processCount) {
	int i;
	printf("P\tTAT\tWT\n");
	for (i=0; i<processCount; i++) {
		printf("%d\t%d\t%d\n", i+1, processList[i].turnaroundTime, processList[i].waitingTime);
	}
}
/***********************************************************************************************/
int main() {
	int processCount, i, turnaroundTime, waitingTime, choice;
	struct Process processList[20];
	printf("Enter number of processes : ");
	scanf("%d", &processCount);
	printf("\nScheduling Algorithm\n");
	printf("\t1. FCFS\n");
	printf("\t2. SJF\n");
	printf("\t3. Round Robin\n");
	printf("\t4. Priority\n");
	printf("Enter choice : ");
	while ((choice = getchar()) == '\n');
	for (i=0; i<processCount; i++) {
		processList[i].pid = i;
		printf("Process %d\n", i+1);
		printf("Enter burst time : ");
		scanf("%d", &processList[i].burstTime);
		if (choice == '4') {
			printf("Enter process priority : ");
			scanf("%d", &processList[i].priority);
		}
		processList[i].waitingTime = 0;
		processList[i].turnaroundTime = 0;
	}
	switch(choice) {
		case '1':
			fcfs(processList, processCount);
			displayResults(processList, processCount);
			break;
		case '2':
			sjf(processList, processCount);
			displayResults(processList, processCount);
			break;
		case '3':
			roundRobin(processList, processCount);
			displayResults(processList, processCount);
			break;
		case '4':
			priority(processList, processCount);
			displayResults(processList, processCount);
			break;
		default:
			printf("Invalid choice\n");
	}
	return 0;
}
