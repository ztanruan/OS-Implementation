#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>

#define MAX_LINE_LEN 1024

void bin2hex(const char* binStr, char** hexStr) {
    int hexnum;
    char fourDigitPack[5];
    int binSize = strnlen(binStr, MAX_LINE_LEN);
    char hexChar[2] = {0x00};
    int i;

    // If the len of binary is not a mutiple number by 4, it is not a valid binary string
    if (binSize % 4) {
        printf("---> Wrong binary format \n");
        return;
    }

    // Read and analyse every 4 digit from binary string
    for (i = 0; i < binSize; i += 4) {
        memset(hexChar, 0x00, sizeof(char) *2);
        strncpy(fourDigitPack, binStr + i, 4);
        fourDigitPack[4] = 0;
        
        hexnum = (int)strtol(fourDigitPack, NULL, 2);
        sprintf(hexChar, "%x", hexnum);
        strcat(*hexStr, hexChar);
    }
}

int main(int argc, char** argv) {
    /* Verify input parameter should have at least 1 string */
    if (argc != 3) {
        printf("An error has occured\n");
        exit(1);
    }

    FILE* fpIn = fopen(argv[1], "r");

    // File is not exist, exit program
    if (fpIn == NULL) {
        printf("An error has occured: input file does not exist\n");
        exit(1);
    }

    FILE* fpOut = fopen(argv[2], "w+"); // Open file in write mode, if not exist, create new file
    // File is not exist, exit program
    if (fpOut == NULL) {
        printf("An error has occured: input file does not exist\n");
        fclose(fpIn);
        exit(1);
    }
    
    char* lineIn = NULL;
    char* lineOut = (char*)malloc((MAX_LINE_LEN + 1) * sizeof(char));
    size_t  len = 0;
    ssize_t readByte = 0;

    /* Read input file */
    while ((readByte = getline(&lineIn, &len, fpIn)) != -1) {
        memset(lineOut, 0x00, (MAX_LINE_LEN + 1) * sizeof(char));
        lineIn[strcspn(lineIn, "\n")] = 0; // remove \n

        bin2hex(lineIn, &lineOut);
        fprintf(fpOut, "%s\n", lineOut); // write hex string to out file        
    }

    // Deallocate memory
    free(lineIn);
    free(lineOut);
    fclose(fpIn);
    fclose(fpOut);

    return 0;
}
