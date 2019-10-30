#include <stdio.h>

int buffer[16], bufferSize, bufferCount, mutex=1;
int wait() {
    mutex--;
}
int signal() {
    mutex++;
}
int produce() {
    wait(mutex);
    printf("Producing\n");
    bufferCount--;
    signal(mutex);
}
int consume() {
    wait(mutex);
    printf("Consuming\n");
    bufferCount++;
    signal(mutex);
}
int main() {
    int choice;
    printf("Enter buffer size : ");
    scanf("%d", &bufferSize);
    bufferCount = bufferSize;
    do {
        printf("1.Produce\n2.Consume\n3.Exit\n");
        printf("Enter choice : ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                if ((mutex==1)&&(bufferCount>0)) {
                    produce();
                } else if (bufferCount == 0) {
                    printf("Buffer full\n");
                } else {
                    printf("Cannot produce\n");
                }
                break;
            case 2:
                if ((mutex==1) && (bufferCount < bufferSize)) {
                    consume();
                } else if (bufferCount >= bufferSize) {
                    printf("Buffer empty\n");
                } else {
                    printf("Cannot consume");
                }
                break;
            case 3:
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    } while (choice != 3);
}