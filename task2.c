#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>

#define MAX_LINE_LEN 1024


void hex2bin(const char* hexStr, char** binStr);

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("An error has occured\n");
        exit(1);
    }

    FILE* fpIn = fopen(argv[1], "r");

    if (fpIn == NULL) {
        printf("An error has occured: input file does not exist\n");
        exit(1);
    }

    FILE* fpOut = fopen(argv[2], "w+");
    if (fpOut == NULL) {
        printf("An error has occured: input file does not exist\n");
        fclose(fpIn);
        exit(1);
    }

    
    char* lineIn = NULL;

   
    char* lineOut = (char*)malloc((MAX_LINE_LEN + 1) * sizeof(char));
    size_t  len = 0;
    ssize_t readByte = 0;


    while ((readByte = getline(&lineIn, &len, fpIn)) != -1) {
        memset(lineOut, 0x00, (MAX_LINE_LEN + 1) * sizeof(char));        

        lineIn[strcspn(lineIn, "\n")] = 0;
        hex2bin(lineIn, &lineOut);

        fprintf(fpOut, "%s\n", lineOut);
    }

   
    free(lineIn);
    free(lineOut);
    fclose(fpIn);
    fclose(fpOut);

    return 0;
}

void hex2bin(const char* hexStr, char** binStr) {

    const char* digits = "0123456789abcdef";
    const char binary[16][5] = {"0000", "0001", "0010", "0011", "0100",
                                "0101", "0110", "0111", "1000", "1001",
                                "1010", "1011", "1100", "1101", "1110",
                                "1111"};
    char* temp = *binStr;
    int position = 0;

    while(hexStr[position]) {        
        const char *found = strchr(digits, tolower(hexStr[position++]));
        if (found) {
            strcat(temp, binary[found - digits]);
        }
    }

}
