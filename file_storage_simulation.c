#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct File {
    char fileName[32];
};
struct Directory {
    char directoryName[32];
    int fileCount;
    struct File *fileList[32];
    int directoryCount;
    struct Directory *directoryList[32];
    struct Directory *parentDirectory;
} *currentDirectory;
int trim(char *string) {
    int i;
    for (i=0; string[i] != '\0'; i++) {
        if (string[i] == '\n') {
            string[i] = '\0';
        }
    }
}
int sortFiles() {
    int i, j;
    struct File *tempFile;
    for (i=1; i<currentDirectory->fileCount; i++) {
        tempFile = currentDirectory->fileList[i];
        for (j=i-1; j>=0 && strcmp(currentDirectory->fileList[j]->fileName, tempFile->fileName) > 0; j--) {
            currentDirectory->fileList[j+1] = currentDirectory->fileList[j];
        }
        currentDirectory->fileList[j+1] = tempFile;
    }
}
int sortDirectory() {
    int i, j;
    struct Directory *tempDirectory;
    for (i=1; i<currentDirectory->directoryCount; i++) {
        tempDirectory = currentDirectory->directoryList[i];
        for (j=i-1; j>=0 && strcmp(currentDirectory->directoryList[j]->directoryName, tempDirectory->directoryName) > 0; j--) {
            currentDirectory->directoryList[j+1] = currentDirectory->directoryList[j];
        }
        currentDirectory->directoryList[j+1] = tempDirectory;
    }
}
int createNewFile(char *fileName) {
    struct File *newFile;
    int i, j;
    for (i=0; i<currentDirectory->fileCount; i++) {
        if (strcmp(currentDirectory->fileList[i]->fileName, fileName) == 0) {
            return -1;
        }
    }
    newFile = (struct File*)(malloc(sizeof(struct File)));
    if (newFile == NULL) {
        printf("Stack overflow");
        return -1;
    }
    strcpy(newFile->fileName, fileName);
    for (i=0; i<currentDirectory->fileCount; i++) {
        if (strcmp(currentDirectory->fileList[i]->fileName, fileName) > 0) {
            break;
        }
    }
    for (j=currentDirectory->fileCount; j>i; j--) {
        currentDirectory->fileList[j] = currentDirectory->fileList[j-1];
    }
    currentDirectory->fileList[i] = newFile;
    currentDirectory->fileCount++;
    return 0;
}
int createNewDirectory(char *directoryName) {
    struct Directory *newDirectory;
    int i, j;
    for (i=0; i<currentDirectory->directoryCount; i++) {
        if (strcmp(currentDirectory->directoryList[i]->directoryName, directoryName) == 0) {
            return -1;
        }
    }
    newDirectory = (struct Directory*) (malloc(sizeof(struct Directory)));
    if (newDirectory == NULL) {
        printf("Stack overflow");
        return -1;
    }
    strcpy(newDirectory->directoryName, directoryName);
    newDirectory->parentDirectory = currentDirectory;
    newDirectory->fileCount = 0;
    newDirectory->directoryCount = 0;
    for (i=0; i<currentDirectory->directoryCount; i++) {
        if (strcmp(currentDirectory->directoryList[i]->directoryName, directoryName) > 0) {
            break;
        }
    }
    for (j=currentDirectory->directoryCount; j>i; j--) {
        currentDirectory->directoryList[j] = currentDirectory->directoryList[j-1];
    }
    currentDirectory->directoryList[i] = newDirectory;
    currentDirectory->directoryCount++;
    return 0;
}
int changeDirectory(char *directoryName) {
    int i;
    if (strcmp(directoryName, "..") == 0) {
        if (currentDirectory->parentDirectory == NULL) {
            return -1;
        } else {
            currentDirectory = currentDirectory->parentDirectory;
            return 0;
        }
    }
    for(i=0; i<currentDirectory->directoryCount; i++) {
        if (strcmp(currentDirectory->directoryList[i]->directoryName, directoryName) == 0) {
            currentDirectory = currentDirectory->directoryList[i];
            printf("Switched to child '%s'\n", currentDirectory->directoryName);
            printf("dirCount : %d\n", currentDirectory->directoryCount);
            return 0;
        }
    }
    return -1;
}
int listAllDirectories() {
    int i;
    if (currentDirectory->parentDirectory != NULL) {
        printf("..\n");
    }
    for(i=0; i<currentDirectory->directoryCount; i++) {
        printf("%s\n", currentDirectory->directoryList[i]->directoryName);
    }
    return 0;
}
int listAllFiles() {
    int i;
    for (i=0; i<currentDirectory->fileCount; i++) {
        printf("%s\n", currentDirectory->fileList[i]->fileName);
    }
    return 0;
}
int listDirectory() {
    int i;
    printf("\nContents of directory '%s':\n", currentDirectory->directoryName);
    listAllDirectories();
    listAllFiles();
}
int deleteAllNodes(struct Directory *directory) {
    int i;
    for (i=0; i<directory->fileCount; i++) {
        free(directory->fileList[i]);
    }
    for (i=0; i<directory->directoryCount; i++) {
        deleteAllNodes(directory->directoryList[i]);
        free(directory->directoryList[i]);
    }
    return 0;
}
int deleteFile(char *fileName) {
    int i, j;
    for (i=0; i<currentDirectory->fileCount; i++) {
        if (strcmp(currentDirectory->fileList[i]->fileName, fileName) == 0) {
            free(currentDirectory->fileList[i]);
            for (j=i; j<currentDirectory->fileCount-1; j++) {
                currentDirectory->fileList[j] = currentDirectory->fileList[j+1];
            }
            currentDirectory->fileCount--;
            return 0;
        }
    }
    return -1;
}
int deleteDirectory(char *directoryName) {
    int i, j;
    for (i=0; i<currentDirectory->directoryCount; i++) {
        if (strcmp(currentDirectory->directoryList[i]->directoryName, directoryName) == 0) {
            deleteAllNodes(currentDirectory->directoryList[i]);
            free(currentDirectory->directoryList[i]);
            for (j=i; j<currentDirectory->directoryCount-1; j++) {
                currentDirectory->directoryList[j] = currentDirectory->directoryList[j+1];
            }
            currentDirectory->directoryCount--;
            return 0;
        }
    }
    return -1;
}
int main() {
    int i, choice;
    char buffer[64];
    struct Directory rootDirectory;
    strcpy(rootDirectory.directoryName, "/");
    rootDirectory.fileCount = 0;
    rootDirectory.directoryCount = 0;
    currentDirectory = &rootDirectory;
    while (choice != 7) {
        printf("********************************************************************************\n");
        printf("**************************************MENU**************************************\n");
        printf("********************************************************************************\n");
        printf("Current working Directory: '%s'\n", currentDirectory->directoryName);
        printf("1.Create file in current directory\n");
        printf("2.Create directory in current directory\n");
        printf("3.Change directory\n");
        printf("4.Delete file in current directory\n");
        printf("5.Delete directory in current directory\n");
        printf("6.View Structure\n");
        printf("7.Exit\n");
        printf("Enter choice : ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                printf("\n\tCreate new file\n");
                while ((getchar()) != '\n');
                printf("Enter fileName : ");
                fgets(buffer, 32, stdin);
                trim(buffer);
                if (createNewFile(buffer)==0) {
                    printf("INFO: Succesfully created file '%s'", buffer);
                } else {
                    printf("ERROR: Failed to create file '%s'", buffer);
                }
                break;
            case 2:
                printf("\n\tCreate new directory\n");
                printf("Current directory List\n");
                while ((getchar()) != '\n');
                printf("Enter directory name : ");
                fgets(buffer, 32, stdin);
                trim(buffer);
                if (createNewDirectory(buffer) == 0) {
                    printf("INFO: Succesfully created directory '%s'", buffer);
                } else {
                    printf("ERROR: Failed to create directory '%s'", buffer);
                }
                break;
            case 3:
                printf("\n\tChange directory\n");
                printf("Available directories:\n");
                listAllDirectories();
                while ((getchar()) != '\n');
                printf("\nEnter directory name : ");
                fgets(buffer, 32, stdin);
                trim(buffer);
                if (changeDirectory(buffer) == -1) {
                    printf("ERROR: No directory with specified name found\n");
                } else {
                    printf("INFO: Succesfully changed current directory");
                }
                break;
            case 4:
                printf("\n\tDelete file\n");
                printf("Available files:\n");
                listAllFiles();
                printf("\nEnter file name : ");
                while((getchar()) != '\n');
                fgets(buffer, 32, stdin);
                trim(buffer);
                if (deleteFile(buffer) == -1) {
                    printf("ERROR: No file with name '%s' found\n", buffer);
                } else {
                    printf("INFO: Successfully deleted '%s'", buffer);
                }
                break;
            case 5:
                printf("\n\tDelete directory\n");
                printf("Available directories:\n");
                listAllDirectories();
                printf("\nEnter directory name : ");
                while((getchar()) != '\n');
                fgets(buffer, 32, stdin);
                trim(buffer);
                if (deleteDirectory(buffer) == -1) {
                    printf("ERROR: No directory with name '%s' found\n", buffer);
                } else {
                    printf("INFO: Successfully deleted directory '%s'", buffer);
                }
                break;
            case 6:
                printf("\n\tList Current Directory\n");
                listDirectory();
                break;
            case 7:
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
        printf("\n\n");
    }
    deleteAllNodes(&rootDirectory);
    return 0;
}