#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_LINE 1024

void analyzeAndPrintCommand(char buffer[MAX_LINE]) {
    char delim[] = " ";
    char* strFind = strtok(buffer, delim);
    // If we reach exit command, just terminate program
    if (strFind != NULL && strcmp(strFind, "exit") == 0) {
        exit(0);
    }

    while(strFind != NULL) {
		printf("%s\n", strFind);
		strFind = strtok(NULL, delim);
	}
}

int main(void) {
    char inputBuffer[MAX_LINE] = {0x00};      /* buffer to hold the command entered */
    
    /* Keep the program running continuously */
    int length = 0;
    
    while (fgets(inputBuffer, MAX_LINE , stdin) != NULL) {        
        
        // Read input from STDIN
        if (length < 0) {
            printf("An error has occured\n");
            exit(1);           /* terminate with error code*/
        }

        inputBuffer[strlen(inputBuffer) - 1] = 0;

        /* When receiving exit command, just say goodbye */
        analyzeAndPrintCommand(inputBuffer);

        /* Reset buffer */
        memset(inputBuffer, 0x00, MAX_LINE);
    }

    return 0;
}

