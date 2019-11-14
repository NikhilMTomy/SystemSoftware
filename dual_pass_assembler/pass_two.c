#include <stdio.h>
#include <ctype.h>

enum OPTAB {
	START,
	LDA,
	STA,
	LDCH,
	STCH
};
const int OPTAB_VALUES[] = {0x00, 0x01, 0x02, 0x03, 0x04};
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
	if (i==0) {
		return -1;
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
int getOpcodeHex(char *opcode) {
	if (stringEquals(opcode, "START")) {
		return OPTAB_VALUES[START];
	} else if  (stringEquals(opcode, "LDA")) {
		return OPTAB_VALUES[LDA];
	} else if  (stringEquals(opcode, "STA")) {
		return OPTAB_VALUES[STA];
	} else if  (stringEquals(opcode, "LDCH")) {
		return OPTAB_VALUES[LDCH];
	} else if  (stringEquals(opcode, "STCH")) {
		return OPTAB_VALUES[STCH];
	} else {
		return -1;
	}
}
int getLocationFromSymTab(char *operand) {
	char location[64], symbol[64];
	FILE *fileSymtab;
	fileSymtab = fopen("symbolTable.dat", "r");
	while (fscanf(fileSymtab, "%s %s", location, symbol) == 2) {
		if (stringEquals(symbol, operand)) {
			return atoi(location);
		}
	}
	printf("Location of %s not found\n", operand);
	return -1;
	fclose(fileSymtab);
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
void getConstantOperand(char *operand, char *constOperand) {
    int i, j;
    for (i=0; operand[i] != '\''; i++);
    i+=1;
    for (; operand[i] != '\''; i++, j++) {
        constOperand[j] = operand[i];
    }
}
int main () {
	FILE *fileInput, *fileOutput, *fileLength;
	char line[128], fieldLocation[64], fieldLabel[64], fieldOpcode[64], fieldOperand[64], constOperand[64];
	int opcodeHex, length, operandValue, operandLocation, startposition, i, lengthOfOperand;

	fileInput = fopen("intermediate.dat", "r");
	fileOutput = fopen("output.obj", "w");
	fileLength = fopen("lengthFile.dat", "r");

	fscanf(fileLength, "%d", &length);
	printf("Length : %d\n", length);

	fscanf(fileInput, "%s\t%s\t%s\t%s", fieldLocation, fieldLabel, fieldOpcode, fieldOperand);
	if (stringEquals(fieldOpcode, "START")) {
		fprintf(fileOutput, "H%-6s%02X%04X%02X%04X", 
				(stringEquals(fieldLabel, "****")?"":fieldLabel),
				0, atoi(fieldOperand),
				0, length);
		startposition = atoi(fieldOperand);
		fscanf(fileInput, "%s\t%s\t%s\t%s", fieldLocation, fieldLabel, fieldOpcode, fieldOperand);
	} else {
		fprintf(fileOutput, "H%-6s%02X%04X%02X%04X", 
				"",
				0, 0,
				0, length);
		startposition = 0;
	}
	fprintf(fileOutput, "\nT");
	while (!stringEquals(fieldOpcode, "END")) {
		opcodeHex = getOpcodeHex(fieldOpcode);
		if (opcodeHex>=0) {
			operandValue = atoi(fieldOperand);
			if (operandValue == -1) {
				operandLocation = getLocationFromSymTab(fieldOperand);
				//printf("Location of %s is at %d\n", fieldOperand, operandLocation);
				if (operandLocation == -1) {
					printf("Invalid operand %s\n", fieldOperand);
				} else {
					fprintf(fileOutput, "%02X%04X", opcodeHex, operandLocation);
				}
			} else {
				fprintf(fileOutput, "%02X%04X", opcodeHex, operandValue);
			}
		} else if (stringEquals(fieldOpcode, "RESB")) {
			operandValue = atoi(fieldOperand);
			operandValue += operandValue%3;
			for (i=0; i<operandValue/3; i++) {
				fprintf(fileOutput, "%06X", 0);
			}
		} else if (stringEquals(fieldOpcode, "RESW")) {
			operandValue = atoi(fieldOperand);
			for (i=0; i<operandValue; i++) {
				fprintf(fileOutput, "%06X", 0);
			}
		} else if (stringEquals(fieldOpcode, "WORD")) {
			operandValue = atoi(fieldOperand);
			fprintf(fileOutput, "%06X", operandValue);
		} else if (stringEquals(fieldOpcode, "BYTE")) {
			lengthOfOperand = lengthOfConstant(fieldOperand);
			lengthOfOperand += lengthOfOperand%3;
			getConstantOperand(fieldOperand, constOperand);
			fprintf(fileOutput, "%06X", constOperand[0]);
		} else {
			printf("Invalid opcode %s\n", fieldOpcode);
		}
		fscanf(fileInput, "%s\t%s\t%s\t%s", fieldLocation, fieldLabel, fieldOpcode, fieldOperand);
		printf("%s\t%s\t%s\t%s\n", fieldLocation, fieldLabel, fieldOpcode, fieldOperand);
	}
	fprintf(fileOutput, "E00%04X", startposition);
	fclose(fileInput);
	fclose(fileOutput);
	fclose(fileLength);
}