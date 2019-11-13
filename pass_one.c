#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct structSymtab {
    char label[64];
    long location;
} SYMTAB[10];
char OPTAB[][10] = {"START", "LDA", "STA", "LDCH", "STCH"};
int symtabLength = 0;
int optabLength = 5;


int stringEquals(char const *a, char const *b) {
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
void parseLine(char *currentLine, char *fieldLabel, char *fieldOperator, char *fieldOperand) {
    int lineIndex, fieldIndex;
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
}
int isComment(char *currentLine) {
    int lineIndex;
    for (lineIndex=0; currentLine[lineIndex] == '\t' || currentLine[lineIndex] == ' '; lineIndex++);
    if (currentLine[lineIndex] == ';') {
        return 1;
    }
    return 0;
}
int isInOPTAB(char *operand) {
    int i=0;
    for (i=0; i<optabLength; i++) {
        if (stringEquals(OPTAB[i], operand)) {
            return 1;
        }
    }
    return 0;
}
int isInSYMTAB(char *label) {
    int i=0;
    for (i=0; i<symtabLength; i++) {
        if (stringEquals(SYMTAB[i].label, label)) {
            return 1;
        }
    }
    return 0;
}
int addToSYMTAB(char *label, long location) {
    strcpy(SYMTAB[symtabLength].label, label);
    SYMTAB[symtabLength++].location = location;
}
int lengthOfConstant(char *operand) {
    int i=0, length=0;
    for (i=0; operand[i] != '\''; i++);
    i+=1;
    for (; operand[i] != '\''; i++) {
        length += 1;
    }
    return length;
}
int main() {
    FILE *src, *dest, *symbolTableFile;
    long LOCCTR = 0, startposition = 0, i;
    char currentLine[128], fieldLabel[64], fieldOperator[64], fieldOperand[64];

    src = fopen("source.asm", "r");
    dest = fopen("intermediate.dat", "w");
    while (fgets(currentLine, 128, src) != NULL) {
        removeNewLine(currentLine);
        //printf("|%s|\n", currentLine);

        parseLine(currentLine, fieldLabel, fieldOperator, fieldOperand);

        //printf("LABEL: [%s]\nOPERATOR: [%s]\nOPERAND: [%s]\n", fieldLabel, fieldOperator, fieldOperand);

        if (stringEquals(fieldOperator, "START") != 0) {
            printf("Start line found.\n");
            LOCCTR = atoi(fieldOperand);
            printf("Starting address : %d\n", LOCCTR);
            startposition = LOCCTR;
        }
        fprintf(dest, "%d\t%s\n", LOCCTR, currentLine);
        if (!isComment(currentLine)) {
            if (!stringEquals(fieldLabel, "")) {
                if (isInSYMTAB(fieldLabel)) {
                    printf("Multple uses of label : %s\n", fieldLabel);
                    break;
                } else {
                    addToSYMTAB(fieldLabel, LOCCTR);
                }
            }
            if (isInOPTAB(fieldOperator)) {
                LOCCTR += 3;
            } else if (stringEquals(fieldOperator, "RESW")) {
                LOCCTR += 3*(atoi(fieldOperand));
            } else if (stringEquals(fieldOperator, "RESB")) {
                LOCCTR += 1*(atoi(fieldOperand));
            } else if (stringEquals(fieldOperator, "WORD")) {
                LOCCTR += 3;
            } else if (stringEquals(fieldOperator, "BYTE")) {
                LOCCTR += lengthOfConstant(fieldOperand);
            } else {
                printf("Undefined nmeumonic %s\n", fieldOperator);
                break;
            }
        }
    }
    printf("Program length : %d\n", LOCCTR - startposition);
    symbolTableFile = fopen("symbolTable.dat", "w");
    for (i=0; i<symtabLength; i++) {
        fprintf(symbolTableFile, "%d\t%s\n", SYMTAB[i].location, SYMTAB[i].label);
    }
    fclose(src);
    fclose(dest);
}