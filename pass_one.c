#include <stdio.h>
#include <string.h>
#include <ctype.h>

int stricmp(char const *a, char const *b) {
    int i=0;
    for (i=0; a[i] != '\0' && b[i] != '\0'; i++) {
        if (tolower(a[i]) != tolower(b[i])) {
            return 0;
        }
    }
    if (a[i] != b[i]) {
        return 0;
    }
    return 1;
}
int atoi(char *inputString) {
    int i=0, outputInteger=0;
    for (i=0; inputString[i] >= '0' && inputString[i] <= '9'; i++) {
        outputInteger = outputInteger*10 + inputString[i]-'0';
    }
    return outputInteger;
}
void removeNewLine(char *inputString) {
    int i=0;
    for (i=0; inputString[i] != '\0'; i++) {
        if (inputString[i] == '\n' || inputString[i] == '\r') {
            inputString[i] = '\0';
            break;
        }
    }
}
int main() {
    FILE *src, *dest;
    long LOCCTR, startposition;
    char currentLine[128], fieldLabel[64], fieldOperator[64], fieldOperand[64];
    int lineIndex=0, fieldIndex=0;

    src = fopen("source.asm", "r");
    while (fgets(currentLine, 128, src) != NULL) {
        removeNewLine(currentLine);
        printf("|%s|\n", currentLine);

        // Extract fields
        for (lineIndex=0, fieldIndex = 0; currentLine[lineIndex] != '\t' && currentLine[lineIndex] != ' ' && currentLine[lineIndex] != '\0'; lineIndex++, fieldIndex++) {
            fieldLabel[fieldIndex] = currentLine[lineIndex];
        }
        fieldLabel[fieldIndex] = '\0';
        for (; currentLine[lineIndex] == '\t' || currentLine[lineIndex] == ' '; lineIndex++);
        for (fieldIndex = 0; currentLine[lineIndex] != '\t' && currentLine[lineIndex] != ' ' && currentLine[lineIndex] != '\0'; lineIndex++, fieldIndex++) {
            fieldOperator[fieldIndex] = currentLine[lineIndex];
        }
        fieldOperator[fieldIndex] = '\0';
        for (; currentLine[lineIndex] == '\t' || currentLine[lineIndex] == ' '; lineIndex++);
        for (fieldIndex = 0; (currentLine[lineIndex] != '\t' && currentLine[lineIndex] != ' ') && currentLine[lineIndex] != '\0'; lineIndex++, fieldIndex++) {
            fieldOperand[fieldIndex] = currentLine[lineIndex];
        }
        fieldOperand[fieldIndex] = '\0';

        printf("LABEL: [%s]\nOPERATOR: [%s]\nOPERAND: [%s]\n", fieldLabel, fieldOperator, fieldOperand);

        if (stricmp(fieldLabel, "start") != 0) {
            printf("Start line found.\n");
            LOCCTR = atoi(fieldOperand);
            printf("Starting address : %d\n", LOCCTR);
            startposition = LOCCTR;
        }
        break;
    }
    fclose(src);
}